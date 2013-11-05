/*
 * Copyright 2013 Canonical Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; version 3.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "ubuntuapplicationapiwrapper.h"

// FIXME: this is hacky way of deciding if running on Touch platform or not
#include <QtCore/qconfig.h>

#ifdef QT_OPENGL_ES_2
#include <ubuntu/ui/ubuntu_ui_session_service.h>
#include <ubuntu/application/ui/window_properties.h>
  #define HAVE_UBUNTU_PLATFORM_API
#endif

#include <QByteArray>
#include <QDir>
#include <QFile>
#include <QtGlobal>

namespace {
    const char gServerName[] = "ubuntu-keyboard-info";
}

UbuntuApplicationApiWrapper::UbuntuApplicationApiWrapper()
    : m_runningOnMir(false)
    , m_clientConnection(0)
    , m_geometry(0)
{
    if (qgetenv("QT_QPA_PLATFORM") == "ubuntumirclient") {
        m_runningOnMir = true;
    }

    m_sharedInfo.reset();

    if (m_runningOnMir) {
        startLocalServer();
    }

    m_geometryUpdateTimer.setInterval(50);
    m_geometryUpdateTimer.setSingleShot(true);
}

void UbuntuApplicationApiWrapper::startLocalServer()
{
    QString socketFilePath = buildSocketFilePath();

    {
        QFile socketFile(socketFilePath);
        if (socketFile.exists()) {
            // It's a leftover from a previous run of maliit-server. let's get rid of it.
            // The other possibility is that another instance of maliit-server is already
            // running, but that's absolutely unsupported.
            if (!socketFile.remove()) {
                qWarning() << "UbuntuApplicationApiWrapper: unable to remove pre-existing"
                           << socketFilePath ;
            }
        }
    }

    connect(&m_localServer, &QLocalServer::newConnection,
            this, &UbuntuApplicationApiWrapper::onNewConnection);
    bool ok = m_localServer.listen(socketFilePath);
    if (!ok) {
        qWarning() << "UbuntuApplicationApiWrapper: failed to listen for connections on"
                   << socketFilePath;
    }
}

void UbuntuApplicationApiWrapper::reportOSKVisible(const int x, const int y, const int width, const int height)
{
#ifdef HAVE_UBUNTU_PLATFORM_API
    if (!m_runningOnMir) { // following method not implemented on Mir
        ubuntu_ui_report_osk_visible(x, y, width, height);
    }
#else
    Q_UNUSED(x)
    Q_UNUSED(y)
    Q_UNUSED(width)
    Q_UNUSED(height)
#endif

    QObject::connect(&m_geometryUpdateTimer, SIGNAL(timeout()),
                     this, SLOT(updateSharedInfo()));
    updateSharedInfo();
}

void UbuntuApplicationApiWrapper::reportOSKInvisible()
{
#ifdef HAVE_UBUNTU_PLATFORM_API
    if (!m_runningOnMir) { // following method not implemented on Mir
        ubuntu_ui_report_osk_invisible();
    }
#endif

    QObject::disconnect(&m_geometryUpdateTimer, SIGNAL(timeout()),
                        this, SLOT(updateSharedInfo()));
}

int UbuntuApplicationApiWrapper::oskWindowRole() const
{
#ifdef HAVE_UBUNTU_PLATFORM_API
    return static_cast<int>(U_ON_SCREEN_KEYBOARD_ROLE);
#else
    return 7;
#endif
}

void UbuntuApplicationApiWrapper::sendInfoToClientConnection()
{
    if (!m_clientConnection
            || m_clientConnection->state() != QLocalSocket::ConnectedState) {
        // can't do it
        return;
    }

    if (m_sharedInfo == m_lastInfoShared) {
        // do not flood our listener with redundant info. This also means that
        // were are getting called unnecessarily
        return;
    }

    const qint64 sharedInfoSize = sizeof(struct SharedInfo);
    qint64 bytesWritten = m_clientConnection->write(reinterpret_cast<char *>(&m_sharedInfo),
                                                    sharedInfoSize);

    if (bytesWritten < 0) {
        qWarning("UbuntuApplicationApiWrapper: Failed to write bytes on client connection");
    } else if (bytesWritten != sharedInfoSize) {
        // Could try sending the remaining bytes until completion but it's really unlikely that
        // this situation will occur
        qWarning() << "UbuntuApplicationApiWrapper: tried to write" << sharedInfoSize << "bytes"
                      "but only" << bytesWritten << "went through";
    }

    m_lastInfoShared = m_sharedInfo;
}

void UbuntuApplicationApiWrapper::onNewConnection()
{
    QLocalSocket *newConnection = m_localServer.nextPendingConnection();

    if (m_clientConnection) {
        qWarning() << "UbuntuApplicationApiWrapper: Refusing incoming connection as we "
                      "already have an active one.";
        delete newConnection;
        return; // ignore it. for simplicity we care to serve only one client (unity8-mir)
    }
    m_clientConnection = newConnection;
    m_lastInfoShared.reset();

    connect(m_clientConnection, &QLocalSocket::disconnected,
            this, &UbuntuApplicationApiWrapper::onClientDisconnected);
}

void UbuntuApplicationApiWrapper::onClientDisconnected()
{
    m_clientConnection->deleteLater();
    m_clientConnection = 0;
}

QString UbuntuApplicationApiWrapper::buildSocketFilePath() const
{
    char *xdgRuntimeDir = getenv("XDG_RUNTIME_DIR");

    if (xdgRuntimeDir) {
        return QDir(xdgRuntimeDir).filePath(gServerName);
    } else {
        return QDir("/tmp").filePath(gServerName);
    }
}

void UbuntuApplicationApiWrapper::updateSharedInfo()
{
    QRectF keyboardSceneRect = m_geometry->visibleRect();
    m_sharedInfo.keyboardX = keyboardSceneRect.x();
    m_sharedInfo.keyboardY = keyboardSceneRect.y();
    m_sharedInfo.keyboardWidth = keyboardSceneRect.width();
    m_sharedInfo.keyboardHeight = keyboardSceneRect.height();

    sendInfoToClientConnection();
}

//! \brief UbuntuApplicationApiWrapper::delayedGeometryUpdate
void UbuntuApplicationApiWrapper::delayedGeometryUpdate()
{
    qDebug() << Q_FUNC_INFO;
    if (m_geometryUpdateTimer.isActive())
        m_geometryUpdateTimer.stop();

    m_geometryUpdateTimer.start();
}

//! \brief UbuntuApplicationApiWrapper::setGeometryItem Set the item that
//! contains the current OKS geometry
//! \param geometry
void UbuntuApplicationApiWrapper::setGeometryItem(KeyboardGeometry *geometry)
{
    if (geometry == m_geometry)
        return;

    if (m_geometry) {
        QObject::disconnect(m_geometry, SIGNAL(visibleRectChanged()),
                            this, SLOT(delayedGeometryUpdate()));
    }

    m_geometry = geometry;
}

// ------------------------------- SharedInfo ----------------------------

bool UbuntuApplicationApiWrapper::SharedInfo::operator ==(const struct SharedInfo &other)
{
    return keyboardX == other.keyboardX
        && keyboardY == other.keyboardY
        && keyboardWidth == other.keyboardWidth
        && keyboardHeight == other.keyboardHeight;
}

void UbuntuApplicationApiWrapper::SharedInfo::reset()
{
    keyboardX = -1;
    keyboardY = -1;
    keyboardWidth = 0;
    keyboardHeight = 0;
}

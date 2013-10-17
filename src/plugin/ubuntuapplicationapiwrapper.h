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

#ifndef UBUNTUAPPLICATIONAPIWRAPPER_H
#define UBUNTUAPPLICATIONAPIWRAPPER_H

#include <QObject>
#include <QLocalServer>
#include <QLocalSocket>
#include <QQuickItem>
#include <QPointer>

#include "scenerectwatcher.h"

/*
 * Class: UbuntuApplicationApiWrapper
 * The OSK-related functions in the ubuntu application api are marked as deprecated.
 * To ensure OSK works on platforms with & without (i.e. Mir), check which platform
 * we are running on: if SurfaceFlinger-based, call the deprecated API, else NOOP.
 *
 * Have added other little methods to help smooth the transition.
 */

class UbuntuApplicationApiWrapper : public QObject
{
    Q_OBJECT
public:
    UbuntuApplicationApiWrapper();

    int oskWindowRole() const;

    void reportOSKVisible(const int, const int, const int, const int);
    void reportOSKInvisible();

    void setRootObject(QQuickItem *rootObject);
private Q_SLOTS:
    void onNewConnection();
    void onClientDisconnected();
    void updateSharedInfo();

private:
    // NB! Must match the definition in unity-mir. Not worth creating a shared header
    // just for that.
    struct SharedInfo {
        qint32 keyboardX;
        qint32 keyboardY;
        qint32 keyboardWidth;
        qint32 keyboardHeight;

        bool operator ==(const struct SharedInfo &other);
        void reset();
    };
    void sendInfoToClientConnection();
    void startLocalServer();
    QString buildSocketFilePath() const;
    void startWatchingExtendedKeysSelector();
    void stopWatchingExtendedKeysSelector();

    bool m_runningOnMir;
    QLocalServer m_localServer;
    QLocalSocket *m_clientConnection;
    struct SharedInfo m_sharedInfo;
    struct SharedInfo m_lastInfoShared;

    /////
    // Items from qml/Keyboard.qml

    // Just the keyboard area
    QPointer<QQuickItem> m_keyboardComp;

    // Keyboard + space for the word ribbon on top
    QPointer<QQuickItem> m_keyboardSurface;

    // The baloon that pops-up when you long press a character key to get
    // its accented versions etc.
    QPointer<QQuickItem> m_extendedKeysSelector;

    SceneRectWatcher m_sceneRectWatcher;
};

#endif // UBUNTUAPPLICATIONAPIWRAPPER_H

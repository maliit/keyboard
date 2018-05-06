/*
 * Copyright (C) 2016 Canonical, Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; version 3.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "mirinputregionupdater.h"

#include <mir_toolkit/mir_window.h>

#include <QGuiApplication>
#include <qpa/qplatformnativeinterface.h>

namespace {

void qRectToMirRectangle(MirRectangle &mirRect, const QRectF &qRect)
{
    mirRect.left = (int)qRect.x();
    mirRect.top = (int)qRect.y();
    mirRect.width = (unsigned int)qRect.width();
    mirRect.height = (unsigned int)qRect.height();
}

bool sameMirRectangle(const MirRectangle &rect1, const MirRectangle &rect2)
{
    return rect1.left == rect2.left
        && rect1.top == rect2.top
        && rect1.width == rect2.width
        && rect1.height == rect2.height;
}

} // anonymous namespace

MirInputRegionUpdater::MirInputRegionUpdater(QWindow *window, KeyboardGeometry *geometry)
    : QObject(0)
    , m_window(window)
    , m_kbdGeometry(geometry)
    , m_mirWindow(0)
    , m_lastRectangle(0)
{
    if (QGuiApplication::platformName() != "ubuntumirclient") {
        qFatal("MirInputRegionUpdater: Platform is not ubuntumirclient.");
    }

    connect(geometry, &KeyboardGeometry::visibleRectChanged, this, &MirInputRegionUpdater::update);

    QPlatformNativeInterface *platform = QGuiApplication::platformNativeInterface();
    m_mirConnection = reinterpret_cast<MirConnection*>(platform->nativeResourceForIntegration("MirConnection"));
}

MirInputRegionUpdater::~MirInputRegionUpdater()
{
    delete m_lastRectangle;
}

void MirInputRegionUpdater::update()
{
    if (m_window.isNull() || m_kbdGeometry.isNull()) {
        return;
    }

    if (!m_mirWindow) {
        QPlatformNativeInterface *platform = QGuiApplication::platformNativeInterface();
        m_mirWindow = reinterpret_cast<MirWindow*>(platform->nativeResourceForWindow("MirWindow", m_window));
    }

    if (!m_mirWindow) {
        // Most likely the backing QPlatformWindow wasn't created yet.
        return;
    }

    MirRectangle rect;
    qRectToMirRectangle(rect, m_kbdGeometry->visibleRect());

    if (m_lastRectangle && sameMirRectangle(rect, *m_lastRectangle)) {
        // Don't repeat yourself
        return;
    }

    MirWindowSpec* spec = mir_create_input_method_window_spec(m_mirConnection, rect.width, rect.height);
    mir_window_spec_set_input_shape(spec, &rect, 1 /* n_rects */);
    mir_window_apply_spec(m_mirWindow, spec);
    mir_window_spec_release(spec);

    if (!m_lastRectangle) {
        m_lastRectangle = new MirRectangle;
    }
    *m_lastRectangle = rect;
}

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

#include <mir_toolkit/mir_surface.h>

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
    , m_surface(0)
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

    if (!m_surface) {
        QPlatformNativeInterface *platform = QGuiApplication::platformNativeInterface();
        m_surface = reinterpret_cast<MirSurface*>(platform->nativeResourceForWindow("MirSurface", m_window));
    }

    if (!m_surface) {
        // Most likely the backing QPlatformSurface wasn't created yet.
        return;
    }

    MirRectangle rect;
    qRectToMirRectangle(rect, m_kbdGeometry->visibleRect());

    if (m_lastRectangle && sameMirRectangle(rect, *m_lastRectangle)) {
        // Don't repeat yourself
        return;
    }

    MirSurfaceSpec* spec = mir_connection_create_spec_for_changes(m_mirConnection);
    mir_surface_spec_set_input_shape(spec, &rect, 1 /* n_rects */);
    mir_surface_apply_spec(m_surface, spec);
    mir_surface_spec_release(spec);

    if (!m_lastRectangle) {
        m_lastRectangle = new MirRectangle;
    }
    *m_lastRectangle = rect;
}

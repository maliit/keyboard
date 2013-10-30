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

#include "mockscreen.h"

MockScreen::MockScreen(QObject* parent)
    : QObject(parent)
    , m_orientation(Qt::PortraitOrientation)
    , m_primaryOrientation(Qt::PortraitOrientation)
    , m_geometry(QRect(0,0,0,0))
{
}

Qt::ScreenOrientation MockScreen::orientation() const
{
    return m_orientation;
}

void MockScreen::setOrientation(Qt::ScreenOrientation orientation)
{
    m_orientation = orientation;
    Q_EMIT orientationChanged(orientation);
}

Qt::ScreenOrientation MockScreen::primaryOrientation() const
{
    return m_primaryOrientation;
}

void MockScreen::setPrimaryOrientation(Qt::ScreenOrientation orientation)
{
    m_primaryOrientation = orientation;
    Q_EMIT primaryOrientationChanged(orientation);
}

QRect MockScreen::geometry() const
{
    return m_geometry;
}

void MockScreen::setGeometry(const QRect& geometry)
{
    m_geometry = geometry;
    Q_EMIT geometryChanged(geometry);
}







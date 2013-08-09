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

#ifndef MOCKSCREEN_H
#define MOCKSCREEN_H

#include <QScreen>


/**
 * @brief The MockScreen class
 * this is not a real QScreen, but it can be used to change properties and fire related signals
 */

class MockScreen : public QObject
{
    Q_OBJECT

    Q_PROPERTY(Qt::ScreenOrientation orientation READ orientation WRITE setOrientation NOTIFY orientationChanged)
    Q_PROPERTY(Qt::ScreenOrientation primaryOrientation READ primaryOrientation WRITE setPrimaryOrientation NOTIFY primaryOrientationChanged)
    Q_PROPERTY(QRect geometry READ geometry WRITE setGeometry NOTIFY geometryChanged)

public:
    MockScreen(QObject* parent = 0);

    Qt::ScreenOrientation orientation() const;
    Q_SLOT void setOrientation(Qt::ScreenOrientation orientation);
    Q_SIGNAL void orientationChanged(Qt::ScreenOrientation orientation);

    Qt::ScreenOrientation primaryOrientation() const;
    Q_SLOT void setPrimaryOrientation(Qt::ScreenOrientation orientation);
    Q_SIGNAL void primaryOrientationChanged(Qt::ScreenOrientation orientation);

    QRect geometry() const;
    Q_SLOT void setGeometry(const QRect& geometry);
    Q_SIGNAL void geometryChanged(const QRect& geometry);

private:
    Qt::ScreenOrientation m_orientation;
    Qt::ScreenOrientation m_primaryOrientation;
    QRect m_geometry;
};

#endif // MOCKSCREEN_H

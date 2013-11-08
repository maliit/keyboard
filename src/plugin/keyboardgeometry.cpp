/*
 * Copyright (C) 2013 Canonical, Ltd.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * Redistributions of source code must retain the above copyright notice, this list
 * of conditions and the following disclaimer.
 * Redistributions in binary form must reproduce the above copyright notice, this list
 * of conditions and the following disclaimer in the documentation and/or other materials
 * provided with the distribution.
 * Neither the name of Nokia Corporation nor the names of its contributors may be
 * used to endorse or promote products derived from this software without specific
 * prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL
 * THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#include "keyboardgeometry.h"

#include <QDebug>

KeyboardGeometry::KeyboardGeometry(QObject *parent) :
    QObject(parent)
  , m_keypadHeight(0)
  , m_canvasHeight(0)
  , m_visibleRect()
  , m_orientation(Qt::PrimaryOrientation)
  , m_shown(false)
{
}

//! \brief KeyboardGeometry::keypadHeight returns the height of the keypad
//! The keypad is the part of the keyboard containing alls the keys and the
//! word ribbon.
//! But it does not include for example the extra height for items like the
//! extended keys
//! \return
int KeyboardGeometry::keypadHeight() const
{
    return m_keypadHeight;
}

//! \brief KeyboardGeometry::setKeypadHeight
//! \param height height of the keypad in pixel
void KeyboardGeometry::setKeypadHeight(int height)
{
    if (height == m_keypadHeight)
        return;

    m_keypadHeight = height;
    Q_EMIT keypadHeightChanged();
}

//! \brief KeyboardGeometry::canvasHeight height for the canvas item
//! FIXME this should not be needed, and calculated in QML directly
//! \return
int KeyboardGeometry::canvasHeight() const
{
    return m_canvasHeight;
}

//! \brief KeyboardGeometry::setCanvasHeight
//! FIXME this should not be needed, and calculated in QML directly
//! \param height
void KeyboardGeometry::setCanvasHeight(int height)
{
    if (height == m_canvasHeight)
        return;

    m_canvasHeight = height;
    Q_EMIT canvasHeightChanged();
}

//! \brief KeyboardGeometry::visibleRect returns the size and position of the total
//! keyboard for the windowing system
//! \return
const QRectF &KeyboardGeometry::visibleRect() const
{
    return m_visibleRect;
}

//! \brief KeyboardGeometry::setVisibleRect
//! \param rect
void KeyboardGeometry::setVisibleRect(const QRectF &rect)
{
    if (rect == m_visibleRect)
        return;

    m_visibleRect = rect;
    Q_EMIT visibleRectChanged();
}

//! \brief KeyboardGeometry::orientation
//! \return
Qt::ScreenOrientation KeyboardGeometry::orientation() const
{
    return m_orientation;
}

//! \brief KeyboardGeometry::setOrientation sets the orientation of the keyboard
//! on the screen
//! \param orient
void KeyboardGeometry::setOrientation(Qt::ScreenOrientation orient)
{
    if (orient == m_orientation)
        return;

    m_orientation = orient;
    Q_EMIT orientationChanged();
}

//! \brief KeyboardGeometry::shown property to indicate if the OSK is visible
//! \return
bool KeyboardGeometry::shown() const
{
    return m_shown;
}

//! \brief KeyboardGeometry::setShown
//! \param show
void KeyboardGeometry::setShown(bool show)
{
    if (show == m_shown)
        return;

    m_shown = show;
    Q_EMIT shownChanged();
}

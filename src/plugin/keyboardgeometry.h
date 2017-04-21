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

#ifndef KEYBOARDGEOMETRY_H
#define KEYBOARDGEOMETRY_H

#include <QObject>
#include <QRectF>

//! \brief The KeyboardGeometry class hold the geometry of the QML keyboard
//! representation
class KeyboardGeometry : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int keypadHeight READ keypadHeight NOTIFY keypadHeightChanged)
    Q_PROPERTY(int canvasHeight READ canvasHeight NOTIFY canvasHeightChanged)
    Q_PROPERTY(QRectF visibleRect READ visibleRect WRITE setVisibleRect NOTIFY visibleRectChanged)
    Q_PROPERTY(Qt::ScreenOrientation orientation READ orientation NOTIFY orientationChanged)
    Q_PROPERTY(bool shown READ shown WRITE setShown NOTIFY shownChanged)

public:
    explicit KeyboardGeometry(QObject *parent = nullptr);

    int keypadHeight() const;
    void setKeypadHeight(int height);

    int canvasHeight() const;
    void setCanvasHeight(int height);

    const QRectF &visibleRect() const;
    Q_SLOT void setVisibleRect(const QRectF &rect);

    Qt::ScreenOrientation orientation() const;
    void setOrientation(Qt::ScreenOrientation orient);

    bool shown() const;
    Q_SLOT void setShown(bool show);

Q_SIGNALS:
    void keypadHeightChanged();
    void canvasHeightChanged();
    void visibleRectChanged();
    void orientationChanged();
    void shownChanged();
    
private:
    int m_keypadHeight;
    int m_canvasHeight;
    QRectF m_visibleRect;
    Qt::ScreenOrientation m_orientation;
    bool m_shown;
};

#endif // KEYBOARDGEOMETRY_H

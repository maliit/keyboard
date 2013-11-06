/*
 * This file is part of Maliit Plugins
 *
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

#include "plugin/keyboardgeometry.h"

#include <QtCore>
#include <QtTest>

namespace MaliitKeyboard {

class TestKeyboardGeometry
    : public QObject
{
    Q_OBJECT

private:
    KeyboardGeometry *m_geometry;

    Q_SLOT void initTestCase()
    {}

    Q_SLOT void init()
    {
        m_geometry = new KeyboardGeometry(this);
    }

    Q_SLOT void cleanup()
    {
        delete m_geometry;
        m_geometry = 0;
    }

    Q_SLOT void testKepdHeight()
    {
        QSignalSpy spy(m_geometry, SIGNAL(keypadHeightChanged()));

        QCOMPARE(m_geometry->keypadHeight(), 0);
        int height = 123;

        // normal set
        m_geometry->setKeypadHeight(height);
        QCOMPARE(m_geometry->keypadHeight(), height);
        QCOMPARE(spy.count(), 1);

        // setting same again does not emit the changed signal
        m_geometry->setKeypadHeight(height);
        QCOMPARE(m_geometry->keypadHeight(), height);
        QCOMPARE(spy.count(), 1);
    }

    Q_SLOT void testCanvasHeight()
    {
        QSignalSpy spy(m_geometry, SIGNAL(canvasHeightChanged()));

        QCOMPARE(m_geometry->canvasHeight(), 0);
        int height = 123;

        // normal set
        m_geometry->setCanvasHeight(height);
        QCOMPARE(m_geometry->canvasHeight(), height);
        QCOMPARE(spy.count(), 1);

        // setting same again does not emit the changed signal
        m_geometry->setCanvasHeight(height);
        QCOMPARE(m_geometry->canvasHeight(), height);
        QCOMPARE(spy.count(), 1);
    }

    Q_SLOT void testVisibleRect()
    {
        QSignalSpy spy(m_geometry, SIGNAL(visibleRectChanged()));

        QRectF rect(0.0, 0.0, 0.0, 0.0);
        QCOMPARE(m_geometry->visibleRect(), rect);

        rect = QRectF(1.0, 2.0, 3.0, 4.0);
        // normal set
        m_geometry->setVisibleRect(rect);
        QCOMPARE(m_geometry->visibleRect(), rect);
        QCOMPARE(spy.count(), 1);

        // setting same again does not emit the changed signal
        m_geometry->setVisibleRect(rect);
        QCOMPARE(m_geometry->visibleRect(), rect);
        QCOMPARE(spy.count(), 1);
    }

    Q_SLOT void testOrientation()
    {
        QSignalSpy spy(m_geometry, SIGNAL(orientationChanged()));

        QCOMPARE(m_geometry->orientation(), Qt::PrimaryOrientation);
        Qt::ScreenOrientation orientation = Qt::LandscapeOrientation;

        // normal set
        m_geometry->setOrientation(orientation);
        QCOMPARE(m_geometry->orientation(), orientation);
        QCOMPARE(spy.count(), 1);

        // setting same again does not emit the changed signal
        m_geometry->setOrientation(orientation);
        QCOMPARE(m_geometry->orientation(), orientation);
        QCOMPARE(spy.count(), 1);
    }

    Q_SLOT void testShown()
    {
        QSignalSpy spy(m_geometry, SIGNAL(shownChanged()));

        QCOMPARE(m_geometry->shown(), false);
        bool show = true;

        // normal set
        m_geometry->setShown(show);
        QCOMPARE(m_geometry->shown(), show);
        QCOMPARE(spy.count(), 1);

        // setting same again does not emit the changed signal
        m_geometry->setShown(show);
        QCOMPARE(m_geometry->shown(), show);
        QCOMPARE(spy.count(), 1);
    }
};

} // namespace

QTEST_MAIN(MaliitKeyboard::TestKeyboardGeometry)
#include "ut_keyboardgeometry.moc"

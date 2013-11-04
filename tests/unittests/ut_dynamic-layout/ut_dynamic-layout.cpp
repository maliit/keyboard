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


#include "plugin/inputmethod.h"
#include "lib/models/wordribbon.h"
#include "lib/models/styleattributes.h"

#include "lib/logic/dynamiclayout.h"
#include "lib/logic/keyboardloader.h"
#include "lib/logic/layouthelper.h"
#include "lib/logic/keyareaconverter.h"
#include "lib/logic/style.h"

#include "common/inputmethodhostprobe.h"
#include "common/mockscreen.h"

#include <maliit/plugins/abstractinputmethodhost.h>

#include <QtTest/QtTest>

using namespace MaliitKeyboard;
using namespace MaliitKeyboard::Logic;

const QRect referenceScreenGeometry(0,0,720,1280);

class TestDynamicLayout: public QObject
{
  Q_OBJECT
private:

    Q_SLOT void initTestCase()
    {
        QVERIFY(qputenv("UBUNTU_KEYBOARD_DATA_DIR", TEST_MALIIT_KEYBOARD_DATADIR));
    }

    void connectUiConstToScreen(const MockScreen& mockScreen)
    {
        const QScreen* screen = qGuiApp->primaryScreen();
        disconnect( screen, SIGNAL(primaryOrientationChanged(Qt::ScreenOrientation)), uiConst, SLOT(onPrimaryOrientationChanged(Qt::ScreenOrientation)) );
        disconnect( screen, SIGNAL(orientationChanged(Qt::ScreenOrientation)), uiConst, SLOT(onPrimaryOrientationChanged(Qt::ScreenOrientation)) );
        disconnect( screen, SIGNAL(geometryChanged(QRect)), uiConst, SLOT(onGeometryChanged(QRect)) );

        connect( &mockScreen, SIGNAL(primaryOrientationChanged(Qt::ScreenOrientation)), uiConst, SLOT(onPrimaryOrientationChanged(Qt::ScreenOrientation)) );
        connect( &mockScreen, SIGNAL(orientationChanged(Qt::ScreenOrientation)), uiConst, SLOT(onPrimaryOrientationChanged(Qt::ScreenOrientation)) );
        connect( &mockScreen, SIGNAL(geometryChanged(QRect)), uiConst, SLOT(onGeometryChanged(QRect)) );
    }

  Q_SLOT void dynamicLayout()
  {
        uiConst->instance();
        QVERIFY(uiConst);

        MockScreen mockScreen;
        connectUiConstToScreen(mockScreen);

        mockScreen.setGeometry(QRect(0,0, 300,400));
        mockScreen.setOrientation(Qt::PortraitOrientation);
        mockScreen.setPrimaryOrientation(Qt::PortraitOrientation);

        uiConst->initDynamicLayout( "test-ui-constants.qml" );
        QCOMPARE( uiConst->windowGeometryRect(Qt::PortraitOrientation).width(), 300 );

        KeyboardLoader loader;
        loader.setActiveId("en_us");    // TODO add a test layout
        Keyboard kb = loader.keyboard();

        QVERIFY(kb.keys.size() > 0);
        QVERIFY2(kb.keys.size() == 33, "test if en_us layout contains 33 keys");

        uiConst->reset();
        connectUiConstToScreen(mockScreen);

        // this is important, so uiConst gets signals and changes internally
        mockScreen.setGeometry( referenceScreenGeometry );
        mockScreen.setOrientation(Qt::PortraitOrientation);
        mockScreen.setPrimaryOrientation(Qt::PortraitOrientation);

        uiConst->initDynamicLayout( "test-ui-constants.qml" );

        QCOMPARE( uiConst->windowGeometryRect(Qt::PortraitOrientation).width(), 720 );

        // this should remain the same even if we rotate the screen, as we depend on contentOrientation
        mockScreen.setOrientation(Qt::LandscapeOrientation);
        QCOMPARE( uiConst->windowGeometryRect(Qt::PortraitOrientation).width(), 720 );

        // virtual width depending on contentOrientation
        QCOMPARE( uiConst->windowWidth(LayoutHelper::Portrait), 720 );
        QCOMPARE( uiConst->windowWidth(LayoutHelper::Landscape), 1280 );

        // not sure if we need this in future
        QVector<int> margins = uiConst->calculateMargins(LayoutHelper::Portrait, kb);
        QVERIFY( margins[0] > 0 );

        // TODO orientation argument not needed, qreal not needed
        QCOMPARE( uiConst->keyWidth(LayoutHelper::Portrait, KeyDescription::Small),     10.0 );
        QCOMPARE( uiConst->keyWidth(LayoutHelper::Portrait, KeyDescription::Medium),    12.0 );
        QCOMPARE( uiConst->keyWidth(LayoutHelper::Portrait, KeyDescription::Large),     15.0 );
        QCOMPARE( uiConst->keyWidth(LayoutHelper::Portrait, KeyDescription::XLarge),    18.0 );
        QCOMPARE( uiConst->keyWidth(LayoutHelper::Portrait, KeyDescription::XXLarge),   24.0 );
        QCOMPARE( uiConst->keyWidth(LayoutHelper::Portrait, KeyDescription::Stretched), 30.0 );

        // keyHeight() not tested, calculated

        QCOMPARE( uiConst->fontSize(LayoutHelper::Portrait), 12.0 );
        QCOMPARE( uiConst->fontSizeSmall(LayoutHelper::Portrait), 10.0 );
  }

    /*
     *  Area
     *      size                    QSize
     *      background              QByteArray
     *      backgroundBorders       QMargins
     *
     *  Key
     *      origin                  QPoint
     *      area                    Area
     *      label                   Label
     *      action                  Action
     *      style                   Key::Style (normal, special, dead)
     *      margins                 QMargins
     *      icon                    QByteArray
     *      has_extended_keys       bool
     *      flags_padding           int
     *      command_sequence        QString
     *
     *  Label
     *      text                    QString
     *      font                    Font
     *      rect                    QRect
     *
     *  Font
     *      name                    QByteArray
     *      size                    int
     *      color                   QByteArray
     *      stretch                 int
     *
     *  KeyDescription
     *      row                     int
     *      flags: left|right spacer, rtl-icon
     *      width                   KeyDescription::Width (Small, Medium, ...)
     *      icon                    KeyDescription::Icon (no, return, backspace, shift, ...)
     *      font_group              KeyDescription::FontGroup (normal, big)
     *
     *  Keyboard
     *      style name              QString
     *      keys                    vector of Key
     *      descriptions            vector of KeyDescription
     *
     *  KeyArea
     *      keys                    vector of Keys
     *      origin                  QPoint
     *      area                    Area
     *      margin                  qreal
     **/


  Q_SLOT void keyAreaConverter()
  {
        KeyboardLoader loader;
        loader.setActiveId("en_us");

        // this really seems overkill
        Style* style = new Style;
        style->setProfile("ubuntu");

        KeyAreaConverter converter(style->attributes(), &loader);
        converter.setLayoutOrientation(MaliitKeyboard::Logic::LayoutHelper::Portrait);

        KeyArea area = converter.keyArea();

        QVERIFY2( area.keys().size() == 33, "Make sure all keys are in keyarea" );
  }
};


QTEST_MAIN(TestDynamicLayout)
#include "ut_dynamic-layout.moc"

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

#ifndef DYNAMICLAYOUT_H
#define DYNAMICLAYOUT_H

#include "logic/layouthelper.h"
#include "models/keydescription.h"
#include "models/keyboard.h"

#include <QObject>

#include <QtCore>
#include <QtGui/QGuiApplication>
#include <QtGui/QScreen>
#include <QtQuick>

namespace MaliitKeyboard {
namespace Logic {

#define uiConst ::MaliitKeyboard::Logic::DynamicLayout::instance()

class DynamicLayoutPrivate;

class DynamicLayout : public QObject
{
    Q_OBJECT

public:
    static DynamicLayout* instance()
    {
        if (!self) {
            self = new DynamicLayout;
        }
        return self;
    }

    static void reset()
    {
        if (self)
            self = new DynamicLayout;
    }

    void initDynamicLayout(QString fileName = UBUNTU_KEYBOARD_DATA_DIR "/maliit-ui-constants.qml");

    int keypadHeight(LayoutHelper::Orientation orientation);
    int windowWidth(LayoutHelper::Orientation orientation);
    QRect windowGeometryRect(Qt::ScreenOrientation orientation);
    qreal keyboardScreenWidthRatio(LayoutHelper::Orientation orientation);

    qreal keyHeight(LayoutHelper::Orientation orientation);
    qreal keyWidth(LayoutHelper::Orientation orientation, KeyDescription::Width width);

    QMargins keyAreaBorders();

    qreal fontSize(LayoutHelper::Orientation orientation);
    qreal fontSizeSmall(LayoutHelper::Orientation orientation);
    QByteArray fontColor();
    QByteArray fontFamily();

    qreal keypadTopMargin(LayoutHelper::Orientation orientation);

    QByteArray keyBackground(Key::Style style, KeyDescription::State state) const;
    Logic::LayoutHelper::Orientation screenToMaliitOrientation(Qt::ScreenOrientation screenOrientation) const;

    qreal spaceBetweenRows(LayoutHelper::Orientation orientation);
    qreal spaceBetweenKeys(LayoutHelper::Orientation orientation);

    int invisibleTouchAreaHeight(LayoutHelper::Orientation orientation);

    void calculateNumberOfRows(Keyboard& kb);
    int numberOfRows() const;

    int wordRibbonHeight(LayoutHelper::Orientation orientation);

    QVector<int> calculateMargins(LayoutHelper::Orientation orientation, Keyboard& kb);

    Q_SLOT void onWordEngineSettingsChanged(bool wordEngineEnabled);

    /* interface to primaryScreen */
    Q_SLOT void onPrimaryOrientationChanged(Qt::ScreenOrientation orientation);
    Q_SLOT void onOrientationChanged(Qt::ScreenOrientation orientation);
    Q_SLOT void onGeometryChanged(const QRect & geometry);

private:
    explicit DynamicLayout(QObject *parent = 0);
    DynamicLayout(DynamicLayout const&) : QObject(0) {}
    DynamicLayout& operator=(DynamicLayout const&){ return *this; }
    static DynamicLayout* self;

    DynamicLayoutPrivate* d;
};

#endif // DYNAMICLAYOUT_H

}
}

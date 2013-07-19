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

#include "dynamiclayout.h"
#include "dynamiclayout_p.h"

namespace MaliitKeyboard {
namespace Logic {

int calculateVerticalMargin(LayoutHelper::Orientation orientation, Keyboard& kb)
{
    Q_UNUSED(kb);

    const int freeSpace = uiConst->keypadHeight(orientation)
                            - (uiConst->keyHeight(orientation) * uiConst->numberOfRows())
                            - uiConst->keypadTopMargin(orientation);

    const int vMargin = (freeSpace / (uiConst->numberOfRows()-1)) / 2;

    return vMargin;
}

QVector<int> DynamicLayout::calculateMargins(LayoutHelper::Orientation orientation,
                              Keyboard& kb)
{
    const qreal keyboardWidth = uiConst->windowGeometryRect(qGuiApp->primaryScreen()->orientation()).size().width();

    QVector<int> margins;

    int spaceTakenByKeys = 0;
    int numberOfKeysInRow = 0;
    for (int index = 0; index < kb.keys.count(); ++index) {
        const KeyDescription &desc(kb.key_descriptions.at(index));
        int width = uiConst->keyWidth(orientation, desc.width);
        spaceTakenByKeys += width;
        numberOfKeysInRow++;

        bool at_row_end((index + 1 == kb.keys.count())
                        || (index + 1 < kb.keys.count()
                            && kb.key_descriptions.at(index + 1).row > desc.row));

        if (at_row_end) {
            int marginThisRow = ( (keyboardWidth - spaceTakenByKeys) / (numberOfKeysInRow-1) ) / 2;
            margins.append(marginThisRow);
            spaceTakenByKeys = 0;
        }
    }

    return margins;
}

void DynamicLayout::calculateNumberOfRows(Keyboard& kb)
{
    if(d->genericStorage->numberOfRows > 0)
        return;

    d->genericStorage->numberOfRows = 0;
    for (int index = 0; index < kb.keys.count(); ++index) {
        const KeyDescription &desc(kb.key_descriptions.at(index));
        bool at_row_end((index + 1 == kb.keys.count())
                        || (index + 1 < kb.keys.count()
                            && kb.key_descriptions.at(index + 1).row > desc.row));

        if (at_row_end)
            d->genericStorage->numberOfRows++;
    }
}

DynamicLayout::DynamicLayout(QObject* parent) : QObject(parent),
    d(new DynamicLayoutPrivate(this))
{}

DynamicLayout* DynamicLayout::self = 0;

void DynamicLayout::initDynamicLayout()
{
    d->initDynamicLayout();
}

QRect DynamicLayout::windowGeometryRect(Qt::ScreenOrientation orientation)
{
    LayoutHelper::Orientation maliitOrientation = LayoutHelper::Landscape;

    if (orientation == Qt::PortraitOrientation || orientation == Qt::InvertedPortraitOrientation)
            maliitOrientation = LayoutHelper::Portrait;

    if (orientation == Qt::InvertedLandscapeOrientation || orientation == Qt::InvertedPortraitOrientation) {
        if (!d->wordRibbonEnabled) {
            return d->storage(maliitOrientation)->windowGeometryRectInverted
                        .adjusted  (0, wordRibbonHeight(maliitOrientation), 0, 0)
                        .translated(0,-wordRibbonHeight(maliitOrientation));
        } else {
            return d->storage(maliitOrientation)->windowGeometryRectInverted;
        }
    }

    if (!d->wordRibbonEnabled)
        return d->storage(maliitOrientation)->windowGeometryRect.adjusted(0, d->storage(maliitOrientation)->wordRibbonHeight, 0, 0);

    return d->storage(maliitOrientation)->windowGeometryRect;
}

qreal DynamicLayout::keyWidth(LayoutHelper::Orientation orientation, KeyDescription::Width requestedSize)
{
    qreal width = 1.0;

    switch (requestedSize) {
    case KeyDescription::Small:
        width = d->genericStorage->keyWidthSmall;
        break;
    case KeyDescription::Medium:
        width = d->genericStorage->keyWidthMedium;
        break;
    case KeyDescription::Large:
        width = d->genericStorage->keyWidthLarge;
        break;
    case KeyDescription::XLarge:
        width = d->genericStorage->keyWidthXLarge;
        break;
    case KeyDescription::XXLarge:
        width = d->genericStorage->keyWidthXXLarge;
        break;
    case KeyDescription::Stretched:
        width = d->genericStorage->keyWidthStretched;
        break;
    default: break;
    }

    width *= d->widthStretchFactor(orientation);

    return width;
}

/**
 *  gaps are defined in dp or gu, while key height stretches
 */

qreal DynamicLayout::keyHeight(LayoutHelper::Orientation orientation)
{
    const qreal numberOfgaps = d->genericStorage->numberOfRows - 1;

    switch (orientation) {
    case LayoutHelper::Landscape:
    {
        const qreal gapSize = d->landscapeStorage->spaceBetweenRows;
        const qreal bottomMargin = d->landscapeStorage->keyboardBottomMargin;
        const qreal topMargin = d->landscapeStorage->keypadTopMargin;
        const qreal keyHeight = (d->landscapeStorage->keypadHeight
                           - (numberOfgaps * gapSize)
                           - bottomMargin
                           - topMargin)
                           / (qreal) d->genericStorage->numberOfRows;
        return keyHeight;
    }
        break;
    case LayoutHelper::Portrait:
    {
        const qreal gapSize = d->portraitStorage->spaceBetweenRows;
        const qreal bottomMargin = d->portraitStorage->keyboardBottomMargin;
        const qreal topMargin = d->portraitStorage->keypadTopMargin;
        const qreal keyHeight = (d->portraitStorage->keypadHeight
                           - (numberOfgaps * gapSize)
                           - bottomMargin
                           - topMargin)
                           / (qreal) d->genericStorage->numberOfRows;
        return keyHeight;
    }
        break;
    default: return 1.0;
    }
}

QByteArray DynamicLayout::keyBackground(Key::Style style, KeyDescription::State state) const
{
    Q_UNUSED(state);

    switch (style) {
    case Key::StyleNormalKey:
        return d->genericStorage->keyBackgroundNormal;
    case Key::StyleSpecialKey:
        return d->genericStorage->keyBackgroundSpecial;
    case Key::StyleDeadKey:
        return d->genericStorage->keyBackgroundDead;
    default: break;
    }

    return "";
}

Logic::LayoutHelper::Orientation DynamicLayout::screenToMaliitOrientation(Qt::ScreenOrientation screenOrientation) const
{
    switch (screenOrientation) {
    case Qt::LandscapeOrientation:
    case Qt::InvertedLandscapeOrientation:
        return Logic::LayoutHelper::Landscape;
        break;
    case Qt::PortraitOrientation:
    case Qt::InvertedPortraitOrientation:
    case Qt::PrimaryOrientation:
    default:
        return Logic::LayoutHelper::Portrait;
    }

    return Logic::LayoutHelper::Portrait;
}

int DynamicLayout::keypadHeight(LayoutHelper::Orientation orientation)
{
    return d->storage(orientation)->keypadHeight;
}

qreal DynamicLayout::keyboardScreenWidthRatio(LayoutHelper::Orientation orientation)
{
    return d->storage(orientation)->keyboardScreenWidthRatio;
}

QMargins DynamicLayout::keyAreaBorders()
{
    return d->genericStorage->keyAreaBorders;
}

qreal DynamicLayout::fontSize(LayoutHelper::Orientation orientation)
{
    Q_UNUSED(orientation);
    return d->genericStorage->fontSize;
}
qreal DynamicLayout::fontSizeSmall(LayoutHelper::Orientation orientation)
{
    Q_UNUSED(orientation);
    return d->genericStorage->fontSizeSmall;
}

QByteArray DynamicLayout::fontColor()
{
    return d->genericStorage->fontColor;
}

QByteArray DynamicLayout::fontFamily()
{
    return d->genericStorage->fontFamily;
}

qreal DynamicLayout::keypadTopMargin(LayoutHelper::Orientation orientation)
{
    return d->storage(orientation)->keypadTopMargin;
}

qreal DynamicLayout::spaceBetweenRows(LayoutHelper::Orientation orientation)
{
    return d->storage(orientation)->spaceBetweenRows;
}

qreal DynamicLayout::spaceBetweenKeys(LayoutHelper::Orientation orientation)
{
    return d->storage(orientation)->spaceBetweenKeys;
}

int DynamicLayout::invisibleTouchAreaHeight(LayoutHelper::Orientation orientation)
{
    return d->storage(orientation)->invisibleTouchAreaHeight;
}

int DynamicLayout::numberOfRows() const
{
    return d->genericStorage->numberOfRows;
}

int DynamicLayout::wordRibbonHeight(LayoutHelper::Orientation orientation)
{
    return d->storage(orientation)->wordRibbonHeight;
}

void DynamicLayout::onWordEngineSettingsChanged(bool wordEngineEnabled)
{
    d->wordRibbonEnabled = wordEngineEnabled;
}

}} // namespaces

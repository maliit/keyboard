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
/*
 * calculates margin per row
 **/

QVector<int> DynamicLayout::calculateMargins(LayoutHelper::Orientation orientation,
                              Keyboard& kb)
{
    const qreal keyboardWidth = windowWidth(orientation);

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
{
    const QScreen* screen = qGuiApp->primaryScreen();

    d->primaryOrientation = screen->primaryOrientation();
    d->orientation = screen->orientation();
    d->geometry = screen->geometry();
}


DynamicLayout* DynamicLayout::self = 0;

void DynamicLayout::initDynamicLayout(QString fileName)
{
    d->initDynamicLayout(fileName);
}

int DynamicLayout::windowWidth(LayoutHelper::Orientation orientation)
{
    return d->storage(orientation)->windowGeometryRect.width();
}

QRect DynamicLayout::windowGeometryRect(Qt::ScreenOrientation orientation)
{
    if (orientation != d->cachedOrientation)
        d->invalidateWindowGeometryCache();

    if ( !d->windowGeometryCacheValid ) {
        LayoutHelper::Orientation maliitOrientation = LayoutHelper::Landscape;

        if (orientation == Qt::PortraitOrientation || orientation == Qt::InvertedPortraitOrientation)
            maliitOrientation = LayoutHelper::Portrait;

        if (orientation == Qt::InvertedLandscapeOrientation || orientation == Qt::InvertedPortraitOrientation) {
            if (!d->wordRibbonEnabled) {
                return d->writeCache( orientation, d->storage(maliitOrientation)->windowGeometryRectInverted
                        .adjusted  (0, wordRibbonHeight(maliitOrientation), 0, 0)
                        .translated(0,-wordRibbonHeight(maliitOrientation)) );
            } else {
                return d->writeCache( orientation, d->storage(maliitOrientation)->windowGeometryRectInverted );
            }
        }

        if (!d->wordRibbonEnabled)
            return d->writeCache( orientation, d->storage(maliitOrientation)->windowGeometryRect.adjusted(0, d->storage(maliitOrientation)->wordRibbonHeight, 0, 0) );

        return d->writeCache( orientation, d->storage(maliitOrientation)->windowGeometryRect );
    }

    return d->windowGeometryRectCached;
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

// used internally
qreal DynamicLayout::keypadTopMargin(LayoutHelper::Orientation orientation)
{
    return d->storage(orientation)->keypadTopMargin;
}
// used internally
qreal DynamicLayout::spaceBetweenRows(LayoutHelper::Orientation orientation)
{
    return d->storage(orientation)->spaceBetweenRows;
}

// used by inputmethod_h
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
    if (d->wordRibbonEnabled != wordEngineEnabled) {
        d->wordRibbonEnabled = wordEngineEnabled;
        d->invalidateWindowGeometryCache();
    }
}
}} // namespaces

/*
 * This file is part of Maliit Plugins
 *
 * Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies). All rights reserved.
 *
 * Contact: Mohammad Anwari <Mohammad.Anwari@nokia.com>
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

#include "keyareaconverter.h"

#include "models/styleattributes.h"
#include "models/area.h"
#include "models/keyarea.h"
#include "models/key.h"
#include "logic/keyboardloader.h"
#include "logic/dynamiclayout.h"

namespace MaliitKeyboard {
namespace Logic {

//! \class KeyAreaConverter
//! Reads keyboard layouts and converts them into key areas. It can apply
//! styling attributes, making the key areas ready for rendering.
//! Explicit functions are used for the different key areas, instead of a
//! generic lookup. This makes the class easier to use and better documents
//! the valid combinations.

namespace {
//! \brief Creates a key area from a keyboard.
//! \param attributes The styling attributes that get applied to the key area.
//! \param source The keyboard layout used for the key area.
//! \param orientation The layout orientation.
//! \param is_extended_keyarea Whether the resulting key area is used for
//!        extended keys (optional).
KeyArea createFromKeyboard(StyleAttributes *attributes,
                           const Keyboard &source,
                           LayoutHelper::Orientation orientation,
                           bool is_extended_keyarea = false)
{
    DynamicLayout::instance()->initDynamicLayout();

    // An ad-hoc geometry updater that also uses styling information.
    KeyArea ka;
    Keyboard kb(source);

    uiConst->calculateNumberOfRows(kb);

    if (not attributes) {
        qCritical() << __PRETTY_FUNCTION__
                    << "No style given, aborting.";
        return ka;
    }

    attributes->setStyleName(kb.style_name);

    Font font;
    font.setName("");
    font.setSize(1.0);
    font.setColor("");

    Font small_font(font);
    small_font.setSize(1.0);

    static const QMargins bg_margins( QMargin() );

    const qreal max_width( uiConst->windowWidth(orientation) );
    const qreal key_height(uiConst->keyHeight(orientation));
    const qreal keypadHeight = uiConst->keypadHeight(orientation);

    const qreal padding = attributes->keyAreaPadding(orientation);



    QPoint pos(0, 0);
    QVector<int> row_indices;
    int spacer_count = 0;
    qreal consumed_width = 0;

    QVector<int> margins = uiConst->calculateMargins(orientation, kb);

    int row = 0;
    for (int index = 0; index < kb.keys.count(); ++index) {
        const qreal margin = margins[row];
        row_indices.append(index);
        Key &key(kb.keys[index]);
        const KeyDescription &desc(kb.key_descriptions.at(index));

        int width = 0;
        pos.setY((key_height + uiConst->spaceBetweenRows(orientation)) * desc.row + uiConst->keypadTopMargin(orientation));
        bool at_row_start((index == 0)
                          || (kb.key_descriptions.at(index - 1).row < desc.row));
        bool at_row_end((index + 1 == kb.keys.count())
                        || (index + 1 < kb.keys.count()
                            && kb.key_descriptions.at(index + 1).row > desc.row));

        if (at_row_end)
            row++;

        if (desc.left_spacer || desc.right_spacer) {
            ++spacer_count;
        }

        width = uiConst->keyWidth(orientation, desc.width);

        const qreal key_margin((at_row_start || at_row_end) ? margin + padding : margin * 2);

        Area area;
        area.setBackground(uiConst->keyBackground(key.style(), KeyDescription::NormalState));
        area.setBackgroundBorders(bg_margins);
        area.setSize(QSize(width + key_margin, key_height));
        key.setArea(area);

        key.setOrigin(pos);
        key.setMargins(QMargins(at_row_start ? padding : margin, margin,
                                at_row_end   ? padding : margin, margin));

        const QString &text(key.label().text());
        key.rLabel().setFont(text.count() > 1 ? small_font : font);

        if (key.icon().isEmpty()) {
            key.setIcon(attributes->icon(desc.icon,
                                         KeyDescription::NormalState));
        } else {
            key.setIcon(attributes->customIcon(key.icon()));
        }

        pos.rx() += key.rect().width();

        if (at_row_end) {
            if (not is_extended_keyarea
                && spacer_count > 0 && pos.x() < max_width + 1) {
                const int spacer_width = qMax<int>(0, max_width - pos.x()) / spacer_count;
                pos.setX(0);
                int right_x = 0;

                Q_FOREACH (int row_index, row_indices) {
                    Key &k(kb.keys[row_index]);
                    const KeyDescription &d(kb.key_descriptions.at(row_index));

                    QRect r(k.rect());
                    QMargins m(k.margins());
                    int extra_width = 0;

                    if (d.left_spacer) {
                        m.setLeft(m.left() + spacer_width);
                        extra_width += spacer_width;
                    }

                    if (d.right_spacer) {
                        m.setRight(m.right() + spacer_width);
                        extra_width += spacer_width;
                    }

                    k.setMargins(m);

                    r.translate(right_x - r.left(), 0);
                    r.setWidth(r.width() + extra_width);
                    k.setOrigin(r.topLeft());
                    k.rArea().setSize(r.size());

                    right_x = r.x() + r.width();
                }
            }

            consumed_width = qMax<qreal>(consumed_width, key.rect().x()
                                                         + key.rect().width()
                                                         + padding);

            row_indices.clear();
            pos.setX(0);
            spacer_count = 0;
        }
    }

    Area area;
    area.setBackground(attributes->keyAreaBackground());
    area.setBackgroundBorders( uiConst->keyAreaBorders() );

    area.setSize(QSize((is_extended_keyarea ? consumed_width : max_width),
                       keypadHeight
                       ));

    ka.setArea(area);
    ka.setOrigin(is_extended_keyarea ? QPoint(0, -attributes->verticalOffset(orientation))
                                     : QPoint(0, uiConst->wordRibbonHeight(orientation)));
    ka.setKeys(kb.keys);

    return ka;
}
}


//! \param attributes The styling attributes that should be applied to the
//!                   created key areas.
//! \param loader The keyboard layout loader.
KeyAreaConverter::KeyAreaConverter(StyleAttributes *attributes,
                                   KeyboardLoader *loader)
    : m_attributes(attributes)
    , m_loader(loader)
    , m_orientation(LayoutHelper::Landscape)
{
    if (not attributes || not loader) {
        qFatal("Neither attributes nor loader can be null.");
    }
}


KeyAreaConverter::~KeyAreaConverter()
{}


//! \brief Sets the layout orientation used for creating key areas.
//! \param orientation The layout orientation. Default: landscape.
void KeyAreaConverter::setLayoutOrientation(LayoutHelper::Orientation orientation)
{
    m_orientation = orientation;
}

//! \brief Returns the main key area.
KeyArea KeyAreaConverter::keyArea() const
{
    return createFromKeyboard(m_attributes, m_loader->keyboard(), m_orientation);
}


//! \brief Returns the next key area (right of main key area).
KeyArea KeyAreaConverter::nextKeyArea() const
{
    return createFromKeyboard(m_attributes, m_loader->nextKeyboard(), m_orientation);
}


//! \brief Returns the previous key area (left of main key area).
KeyArea KeyAreaConverter::previousKeyArea() const
{
    return createFromKeyboard(m_attributes, m_loader->previousKeyboard(), m_orientation);
}


//! \brief Returns the main key area with shift bindings activated.
KeyArea KeyAreaConverter::shiftedKeyArea() const
{
    return createFromKeyboard(m_attributes, m_loader->shiftedKeyboard(), m_orientation);
}


//! \brief Returns the symbols key area.
//! \param page The symbols page to return (optional).
KeyArea KeyAreaConverter::symbolsKeyArea(int page) const
{
    return createFromKeyboard(m_attributes, m_loader->symbolsKeyboard(page), m_orientation);
}


//! \brief Returns the main key area with dead key bindings activated.
//! \param dead The key used to look up the dead keys.
KeyArea KeyAreaConverter::deadKeyArea(const Key &dead) const
{
    return createFromKeyboard(m_attributes, m_loader->deadKeyboard(dead), m_orientation);
}


//! \brief Returns the main key area with dead key bindings activated.
//! \param dead The key used to look up the dead keys.
KeyArea KeyAreaConverter::shiftedDeadKeyArea(const Key &dead) const
{
    return createFromKeyboard(m_attributes, m_loader->shiftedDeadKeyboard(dead), m_orientation);
}


//! Returns an extended key area.
//! \param key The key used to look up the extended key binding.
KeyArea KeyAreaConverter::extendedKeyArea(const Key &key) const
{
    return createFromKeyboard(m_attributes, m_loader->extendedKeyboard(key), m_orientation, true);
}


//! Returns the number key area.
KeyArea KeyAreaConverter::numberKeyArea() const
{
    return createFromKeyboard(m_attributes, m_loader->numberKeyboard(), m_orientation);
}


//! Returns the phone number key area.
KeyArea KeyAreaConverter::phoneNumberKeyArea() const
{
    return createFromKeyboard(m_attributes, m_loader->phoneNumberKeyboard(), m_orientation);
}

}} // namespace Logic, MaliitKeyboard

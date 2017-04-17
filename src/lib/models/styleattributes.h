/*
 * This file is part of Maliit Plugins
 *
 * Copyright (C) 2012 One Laptop per Child Association
 *
 * Contact: maliit-discuss@lists.maliit.org
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

#ifndef MALIIT_KEYBOARD_STYLEATTRIBUTES_H
#define MALIIT_KEYBOARD_STYLEATTRIBUTES_H

#include "models/key.h"
#include "models/keydescription.h"

#include <QtCore>

namespace MaliitKeyboard {

class StyleAttributes
{
private:
    const QScopedPointer<const QSettings> m_store;
    QString m_style_name;

public:
    explicit StyleAttributes(const QSettings *store);
    virtual ~StyleAttributes();

    virtual void setStyleName(const QString &name);
    QByteArray wordRibbonBackground() const;
    QByteArray keyAreaBackground() const;
    QByteArray magnifierKeyBackground() const;
    QByteArray keyBackground(Key::Style style,
                             KeyDescription::State state) const;

    QMargins wordRibbonBackgroundBorders() const;
    QMargins keyAreaBackgroundBorders() const;
    QMargins magnifierKeyBackgroundBorders() const;
    QMargins keyBackgroundBorders() const;

    QByteArray icon(KeyDescription::Icon icon,
                    KeyDescription::State state) const;

    QByteArray customIcon(const QString &icon_name) const;

    QStringList fontFiles() const;

    QByteArray fontName(Qt::ScreenOrientation orientation) const;
    QByteArray fontColor(Qt::ScreenOrientation orientation) const;
    qreal fontSize(Qt::ScreenOrientation orientation) const;
    qreal smallFontSize(Qt::ScreenOrientation orientation) const;
    qreal candidateFontSize(Qt::ScreenOrientation orientation) const;
    qreal magnifierFontSize(Qt::ScreenOrientation orientation) const;
    qreal candidateFontStretch(Qt::ScreenOrientation orientation) const;

    qreal wordRibbonHeight(Qt::ScreenOrientation orientation) const;
    qreal magnifierKeyHeight(Qt::ScreenOrientation orientation) const;
    qreal keyHeight(Qt::ScreenOrientation orientation) const;

    qreal magnifierKeyWidth(Qt::ScreenOrientation orientation) const;
    qreal keyWidth(Qt::ScreenOrientation orientation,
                   KeyDescription::Width width) const;
    qreal keyAreaWidth(Qt::ScreenOrientation orientation) const;

    qreal keyMargin(Qt::ScreenOrientation orientation) const;
    qreal keyAreaPadding(Qt::ScreenOrientation orienation) const;

    qreal verticalOffset(Qt::ScreenOrientation orientation) const;
    qreal magnifierKeyLabelVerticalOffset(Qt::ScreenOrientation orientation) const;
    qreal safetyMargin(Qt::ScreenOrientation orientation) const;

    QByteArray keyPressSound() const;
    QByteArray keyReleaseSound() const;
    QByteArray layoutChangeSound() const;
    QByteArray keyboardHideSound() const;

    qreal keyboardTotalHeight(Qt::ScreenOrientation orientation) const;
    qreal keyboardVisibleHeight(Qt::ScreenOrientation orientation) const;
    qreal topMargin(Qt::ScreenOrientation orientation) const;
};

} // namespace MaliitKeyboard

#endif // MALIIT_KEYBOARD_STYLEATTRIBUTES_H

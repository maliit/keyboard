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

#include "utils.h"
#include "models/area.h"
#include "models/label.h"
#include "models/key.h"
#include "models/wordcandidate.h"
#include "logic/style.h"
#include "coreutils.h"

#include <QtCore>
#include <QPixmap>
#include <QFont>
#include <QPainter>
#include <qdrawutil.h>

namespace {

QString g_images_directory_path;
QHash<QByteArray, QPixmap> g_pixmap_cache;

} // unnamed namespace

namespace MaliitKeyboard {
namespace Utils {

void setImagesDirectoryPath(const QString &path)
{
    g_images_directory_path = path;
}

QPixmap loadPixmap(const QByteArray &id)
{
    if (id.isEmpty()) {
        return QPixmap();
    }

    const QPixmap &result(g_pixmap_cache.value(id));

    if (not result.isNull()) {
        return result;
    }

    QString filename(g_images_directory_path + "/" + id);
    QPixmap new_pixmap(filename);
    g_pixmap_cache.insert(id, new_pixmap);

    return new_pixmap;
}

void renderKey(QPainter *painter,
               const Key &key,
               const QPoint &origin)
{
    const QMargins &m(key.margins());
    const QRect &key_rect(key.rect().translated(origin).adjusted(m.left(), m.top(), -m.right(), -m.bottom()));
    const Area &area(key.area());

    qDrawBorderPixmap(painter, key_rect, area.backgroundBorders(),
                      Utils::loadPixmap(area.background()));

    const Label &key_label(key.label());
    const Font &key_font(key_label.font());
    QFont painter_font(key_font.name());
    painter_font.setBold(true);
    painter_font.setPixelSize(key_font.size());
    painter_font.setStretch(key_font.stretch());

    painter->setFont(painter_font);
    painter->setPen(QColor(key_font.color().data()));

    const QString &text(key_label.text());
    const QRect &text_rect(key_label.rect().isEmpty() ? key_rect : key_label.rect());
    const QPixmap &icon(Utils::loadPixmap(key.icon()));

    if (not icon.isNull()) {
        const QPoint &c(key_rect.center());
        const QPoint tl(c.x() - icon.width() / 2, c.y() - icon.height() / 2);
        painter->drawPixmap(tl, icon);
    } else if (not text.isEmpty()) {
        painter->drawText(text_rect, Qt::AlignCenter, text);
    }
}

void renderWordCandidate(QPainter *painter,
                         const WordCandidate &candidate,
                         const QPoint &origin)
{
    const QRect &candidate_rect(candidate.rect().translated(origin));
    const Label &label(candidate.label());
    const Font &candidate_font(label.font());
    QFont painter_font(candidate_font.name());
    painter_font.setBold(true);
    painter_font.setPixelSize(candidate_font.size());

    painter->setFont(painter_font);
    painter->setPen(QColor(candidate_font.color().data()));

    const QString &text(label.text());

    if (not text.isEmpty()) {
        painter->drawText(candidate_rect, Qt::AlignCenter, text);
    }
}

Key applyOverride(const Key &original_key,
                  const Logic::KeyOverrides &overrides)
{
    Key overriden_key(original_key);
    Logic::KeyOverrides::const_iterator override_iter(overrides.find(CoreUtils::idFromKey(original_key)));

    if (override_iter != overrides.end()) {
        const Key &override(override_iter.value());
        const QString &text_override(override.label().text());
        const QByteArray &icon_override(override.icon());

        if (not text_override.isEmpty()) {
            overriden_key.rLabel().setText(text_override);
        }
        if (not icon_override.isEmpty()) {
            overriden_key.setIcon(icon_override);
        }
    }

    return overriden_key;
}

}} // namespace Utils, MaliitKeyboard

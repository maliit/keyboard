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

#include "keyareaitem.h"
#include "utils.h"
#include "models/key.h"

#include <qdrawutil.h>

namespace MaliitKeyboard {

KeyAreaItem::KeyAreaItem(QGraphicsItem *parent)
    : QGraphicsItem(parent)
    , m_key_area()
{
    setCacheMode(QGraphicsItem::DeviceCoordinateCache);
}

KeyAreaItem::~KeyAreaItem()
{}

KeyArea KeyAreaItem::keyArea() const
{
    return m_key_area;
}

void KeyAreaItem::setKeyArea(const KeyArea &ka)
{
    if (m_key_area != ka) {
        prepareGeometryChange();
        m_key_area = ka;
        update();
    }
}

Logic::KeyOverrides KeyAreaItem::keyOverrides() const
{
    return m_key_overrides;
}

void KeyAreaItem::setKeyOverrides(const Logic::KeyOverrides &overrides)
{
    if (m_key_overrides != overrides) {
        m_key_overrides = overrides;
        update();
    }
}

QRectF KeyAreaItem::boundingRect() const
{
    return m_key_area.rect();
}

void KeyAreaItem::paint(QPainter *painter,
                        const QStyleOptionGraphicsItem *,
                        QWidget *)
{
    const KeyArea &ka(m_key_area);
    const Area &area(ka.area());

    qDrawBorderPixmap(painter, boundingRect().toRect(),
                      area.backgroundBorders(), Utils::loadPixmap(area.background()));

    Q_FOREACH (const Key &k, ka.keys()) {
        Key key(Utils::applyOverride(k, m_key_overrides));

        Utils::renderKey(painter, key, boundingRect().topLeft().toPoint());
    }
}

} // namespace MaliitKeyboard

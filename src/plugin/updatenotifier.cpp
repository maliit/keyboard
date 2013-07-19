/*
 * This file is part of Maliit Plugins
 *
 * Copyright (C) 2012 Openismus GmbH. All rights reserved.
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

#include "updatenotifier.h"

#include <maliit/plugins/updateevent.h>

namespace MaliitKeyboard {

namespace
{

const char* const g_surrounding_text_property("surroundingText");
const char* const g_cursor_position_property("cursorPosition");
const char* const g_anchor_position_property("anchorPosition");
const char* const g_has_selection("hasSelection");

} // unnamed namespace

class UpdateNotifierPrivate
{
public:
    UpdateNotifierPrivate();

    bool has_selection;
};

UpdateNotifierPrivate::UpdateNotifierPrivate()
    : has_selection(false)
{}

UpdateNotifier::UpdateNotifier(QObject *parent)
    : QObject(parent)
    , d_ptr(new UpdateNotifierPrivate)
{}

UpdateNotifier::~UpdateNotifier()
{}

void UpdateNotifier::notify(MImUpdateEvent* event)
{
    Q_D(UpdateNotifier);

    const QStringList properties_changed(event->propertiesChanged());

    if (properties_changed.contains(g_has_selection)) {
        const bool has_selection(event->value(g_has_selection).toBool());

        d->has_selection = has_selection;
    }

    if (not d->has_selection and properties_changed.contains(g_cursor_position_property)) {
        const int cursor_position(event->value(g_cursor_position_property).toInt());
        const QString surrounding_text(event->value(g_surrounding_text_property).toString());
        bool emit_a_signal(true);

        if (emit_a_signal and properties_changed.contains(g_anchor_position_property)) {
            const int anchor_position(event->value(g_anchor_position_property).toInt());

            emit_a_signal = (anchor_position == cursor_position);
        }

        if (emit_a_signal) {
            Q_EMIT cursorPositionChanged(cursor_position, surrounding_text);
        }
    }
}

void UpdateNotifier::notifyOverride(const Logic::KeyOverrides &overriden_keys,
                                    bool update /* = false */)
{
    Q_EMIT keysOverriden(overriden_keys, update);
}

} // namespace MaliitKeyboard

/*
 * Copyright (c) 2013 Thomas Moenicke
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

#include "coreutils.h"

#include "models/key.h"

namespace MaliitKeyboard {
namespace CoreUtils {
namespace {

const char *const g_action_key_id = "actionKey";

} // unnamed namespace

const QString &pluginLanguageDirectory()
{
    static const QByteArray env_data_directory = qgetenv("MALIIT_KEYBOARD_QML_DIR");
    static QString language_directory = QString::fromUtf8(env_data_directory.isEmpty()
                                                            ? MALIIT_KEYBOARD_QML_DIR
                                                            : env_data_directory);

    return language_directory;
}

const QString &pluginDataDirectory()
{
    static const QByteArray env_data_directory = qgetenv("MALIIT_PLUGINS_DATADIR");
    static const QString data_directory = QString::fromUtf8(env_data_directory.isEmpty()
                                                            ? MALIIT_PLUGINS_DATA_DIR
                                                            : env_data_directory);

    return data_directory;
}

const QString &maliitKeyboardDataDirectory()
{
    static const QByteArray env_data_directory = qgetenv("MALIIT_KEYBOARD_DATADIR");
    static const QString data_directory = QString::fromUtf8(env_data_directory.isEmpty()
                                                            ? MALIIT_KEYBOARD_QML_DIR
                                                            : env_data_directory);

    return data_directory;
}

QString idFromKey(const Key &key)
{
    switch (key.action()) {
    case Key::ActionReturn:
        return g_action_key_id;

    case Key::ActionInsert:
        return key.label();

    default:
        // TODO: handle more key actions if needed.
        return QString();
    }
}

}} // namespace CoreUtils, MaliitKeyboard

// -*- mode: C++; c-basic-offset: 4; indent-tabs-mode: nil; c-file-offsets: ((innamespace . 0)); -*-
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

#include <QDir>
#include <QFile>
#include <QRegExp>

#include "parser/layoutparser.h"
#include "coreutils.h"

#include "keyboardloader.h"

namespace {

using namespace MaliitKeyboard;

// For getting languages directory we use a function instead of global constant
// because global constants are initialized before main runs. In this case that
// would mean that CoreUtils::pluginDataDirectory is ran before any code in main
// and it sets its static variable once. We want to be able to set the
// environment variable altering behaviour of pluginDataDirectory for testing
// purposes.
QString getLanguagesDir()
{
    static QString languages_dir;

    if (languages_dir.isEmpty()) {
        // From http://doc.qt.nokia.com/4.7/qdir.html#separator: If you always
        // use "/", Qt will translate your paths to conform to the underlying
        // operating system.
        languages_dir = CoreUtils::pluginLanguageDirectory() + "/languages";
    }

    return languages_dir;
}

typedef const QStringList (LayoutParser::*ParserFunc)() const;

TagKeyboardPtr getTagKeyboard(const QString &id)
{
    if (id.isEmpty()) {
        return TagKeyboardPtr();
    }

    const QString path(getLanguagesDir() + "/" + id + ".xml");
    QFile file(path);

    if (file.exists()) {
        file.open(QIODevice::ReadOnly);

        LayoutParser parser(&file);
        const bool result(parser.parse());

        file.close();
        if (result) {
            return parser.keyboard();
        } else {
            qWarning() << __PRETTY_FUNCTION__ << "Could not parse file:" << path << ", error:" << parser.errorString();
        }
    } else {
        qWarning() << __PRETTY_FUNCTION__ << "File not found:" << path;
    }

    return TagKeyboardPtr();
}

QPair<Key, KeyDescription> keyAndDescFromTags(const TagKeyPtr &key,
                                              const TagBindingPtr &binding,
                                              int row)
{
    Key skey;
    KeyDescription skey_description;

    skey.setExtendedKeysEnabled(key->extended());
    skey.rLabel().setText(binding->label());

    if (binding->dead()) {
        // TODO: document it.
        skey.setAction(Key::ActionDead);
    } else {
        skey.setAction(static_cast<Key::Action>(binding->action()));
    }

    skey.setCommandSequence(binding->sequence());
    skey.setIcon(binding->icon().toUtf8());
    skey.setStyle(static_cast<Key::Style>(key->style()));

    skey_description.row = row;
    skey_description.use_rtl_icon = key->rtl();
    skey_description.left_spacer = false;
    skey_description.right_spacer = false;
    skey_description.width = static_cast<KeyDescription::Width>(key->width());

    switch (skey.action()) {
    case Key::ActionBackspace:
        skey_description.icon = KeyDescription::BackspaceIcon;
        break;
    case Key::ActionReturn:
        skey_description.icon = KeyDescription::ReturnIcon;
        break;
    case Key::ActionShift:
        skey_description.icon = KeyDescription::ShiftIcon;
        break;
    case Key::ActionClose:
        skey_description.icon = KeyDescription::CloseIcon;
        break;
    case Key::ActionLeftLayout:
        skey_description.icon = KeyDescription::LeftLayoutIcon;
        break;
    case Key::ActionRightLayout:
        skey_description.icon = KeyDescription::RightLayoutIcon;
        break;
    default:
        if (skey.icon().isEmpty()) {
            skey_description.icon = KeyDescription::NoIcon;
        } else {
            skey_description.icon = KeyDescription::CustomIcon;
        }
        break;
    }

    skey_description.font_group = KeyDescription::NormalFontGroup;

    return qMakePair(skey, skey_description);
}

Keyboard getKeyboard(const TagKeyboardPtr &keyboard,
                     bool shifted = false,
                     int page = 0,
                     const QString &dead_label = "")
{
    Keyboard skeyboard;
    const QChar dead_key((dead_label.size() == 1) ? dead_label[0] : QChar::Null);

    if (keyboard) {
        TagLayoutPtrs layouts(keyboard->layouts());

        if (not layouts.isEmpty()) {
            const TagSectionPtrs sections(layouts.first()->sections());
            // sections cannot be empty - parser does not allow that.
            const TagSectionPtr section(sections[page % sections.size()]);
            const TagRowPtrs rows(section->rows());
            int row_num(0);
            QString section_style(section->style());
            int key_count(0);

            Q_FOREACH (const TagRowPtr &row, rows) {
                const TagRowElementPtrs elements(row->elements());
                bool spacer_met(false);

                Q_FOREACH (const TagRowElementPtr &element, elements) {
                    if (element->element_type() == TagRowElement::Key) {
                        const TagKeyPtr key(element.staticCast<TagKey>());
                        const TagBindingPtr binding(key->binding());
                        TagBindingPtr the_binding;
                        const TagModifiersPtrs all_modifiers(binding->modifiers());

                        ++key_count;

                        if (not shifted or all_modifiers.isEmpty()) {
                            the_binding = binding;
                        } else {
                            Q_FOREACH (const TagModifiersPtr &modifiers, all_modifiers) {
                                if (modifiers->keys() == TagModifiers::Shift) {
                                    the_binding = modifiers->binding();
                                }
                            }
                            if (not the_binding) {
                                the_binding = binding;
                            }
                        }

                        const int index(dead_key.isNull() ? -1 : the_binding->accents().indexOf(dead_key));
                        QPair<Key, KeyDescription> key_and_desc(keyAndDescFromTags(key, the_binding, row_num));

                        key_and_desc.first.rLabel().setText(index < 0 ? the_binding->label()
                                                                      : the_binding->accented_labels().at(index));
                        key_and_desc.second.left_spacer = spacer_met;
                        key_and_desc.second.right_spacer = false;

                        skeyboard.keys.append(key_and_desc.first);
                        skeyboard.key_descriptions.append(key_and_desc.second);
                        spacer_met = false;
                    } else { // spacer
                        if (not skeyboard.key_descriptions.isEmpty()) {
                            KeyDescription &previous_skey_description(skeyboard.key_descriptions.last());

                            if (previous_skey_description.row == row_num) {
                                previous_skey_description.right_spacer = true;
                            }
                        }
                        spacer_met = true;
                    }
                }
                ++row_num;
            }
            if (section_style.isEmpty()) {
                section_style = "keys" + QString::number(key_count);
            }
            skeyboard.style_name = section_style;
        }
    }
    return skeyboard;
}

QPair<TagKeyPtr, TagBindingPtr> getTagKeyAndBinding(const TagKeyboardPtr &keyboard,
                                                    const QString &label,
                                                    bool *shifted)
{
    QPair<TagKeyPtr, TagBindingPtr> pair;

    if (keyboard) {
        TagLayoutPtrs layouts(keyboard->layouts());

        if (not layouts.isEmpty()) {
            // sections cannot be empty - parser does not allow that.
            TagRowPtrs rows(layouts.first()->sections().first()->rows());

            Q_FOREACH (const TagRowPtr &row, rows) {
                TagRowElementPtrs elements(row->elements());

                Q_FOREACH (const TagRowElementPtr &element, elements) {
                    if (element->element_type() == TagRowElement::Key) {
                        TagKeyPtr key(element.staticCast<TagKey>());
                        TagBindingPtr the_binding;
                        TagBindingPtr binding(key->binding());

                        // Hotfix for suppressing long-press on space bringing
                        // up extended keys if another key has empty label, in
                        // given layout.
                        // FIXME: Make extended keyboard/keyarea part of key
                        // model instead, to avoid wrong lookups.
                        if (binding->action() == TagBinding::Space) {
                            continue;
                        }
                        if (binding->label() == label) {
                            the_binding = binding;
                            *shifted = false;
                        } else {
                            const TagModifiersPtrs all_modifiers(binding->modifiers());

                            Q_FOREACH (const TagModifiersPtr &modifiers, all_modifiers) {
                                const TagBindingPtr mod_binding(modifiers->binding());

                                if (mod_binding->label() == label) {
                                    the_binding = mod_binding;
                                    *shifted = (modifiers->keys() == TagModifiers::Shift);
                                    break;
                                }
                            }
                        }

                        if (the_binding) {
                            pair.first = key;
                            pair.second = the_binding;
                            return pair;
                        }
                    }
                }
            }
        }
    }
    return pair;
}

Keyboard getImportedKeyboard(const QString &id,
                             ParserFunc func,
                             const QString &file_prefix,
                             const QString &default_file,
                             int page = 0)
{
    QString path(getLanguagesDir() + "/" + id + ".xml");
    QFile file(path);

    if (file.exists()) {
        file.open(QIODevice::ReadOnly);

        LayoutParser parser(&file);
        const bool result(parser.parse());

        file.close();
        if (result) {
            const QStringList f_results((parser.*func)());

            Q_FOREACH (const QString &f_result, f_results) {
                const QFileInfo file_info(getLanguagesDir() + "/" + f_result);

                if (file_info.exists() and file_info.isFile()) {
                    const TagKeyboardPtr keyboard(getTagKeyboard(file_info.baseName()));
                    return getKeyboard(keyboard, false, page);
                }
            }

            // If we got there then it means that we got xml layout file that does not use
            // new <import> syntax or just does not specify explicitly which file to import.
            // In this case we have to search imports list for entry with filename beginning
            // with file_prefix.
            const QStringList imports(parser.imports());
            const QRegExp file_regexp("^(" + file_prefix + ".*).xml$");

            Q_FOREACH (const QString &import, imports) {
                if (file_regexp.exactMatch(import)) {
                    QFileInfo file_info(getLanguagesDir() + "/" + import);

                    if (file_info.exists() and file_info.isFile()) {
                        const TagKeyboardPtr keyboard(getTagKeyboard(file_regexp.cap(1)));
                        return getKeyboard(keyboard, false, page);
                    }
                }
            }

            // If we got there then we try to just load a file with name in default_file.
            QFileInfo file_info(getLanguagesDir() + "/" + default_file);

            if (file_info.exists() and file_info.isFile()) {
                const TagKeyboardPtr keyboard(getTagKeyboard(file_info.baseName()));
                return getKeyboard(keyboard, false);
            }
        } else {
            qWarning() << __PRETTY_FUNCTION__ << "Could not parse file:" << path << ", error:" << parser.errorString();
        }
    } else {
        qWarning() << __PRETTY_FUNCTION__ << "File not found:" << path;
    }
    return Keyboard();
}

} // anonymous namespace

namespace MaliitKeyboard {

class KeyboardLoaderPrivate
{
public:

    QString active_id;
};

KeyboardLoader::KeyboardLoader(QObject *parent)
    : QObject(parent)
    , d_ptr(new KeyboardLoaderPrivate)
{}

KeyboardLoader::~KeyboardLoader()
{}

QStringList KeyboardLoader::ids() const
{
    QStringList ids;
    QDir dir(getLanguagesDir(),
             "*.xml",
             QDir::Name | QDir::IgnoreCase,
             QDir::Files | QDir::NoSymLinks | QDir::Readable);

    if (dir.exists()) {
        QFileInfoList file_infos(dir.entryInfoList());

        Q_FOREACH (const QFileInfo &file_info, file_infos) {
            QFile file(file_info.filePath());
            file.open(QIODevice::ReadOnly);
            LayoutParser parser(&file);

            if (parser.isLanguageFile()) {
                ids.append(file_info.baseName());
            }
        }
    }
    return ids;
}

QString KeyboardLoader::activeId() const
{
    Q_D(const KeyboardLoader);
    return d->active_id;
}

void KeyboardLoader::setActiveId(const QString &id)
{
    Q_D(KeyboardLoader);

    if (d->active_id != id) {
        d->active_id = id;

        // FIXME: Emit only after parsing new keyboard.
        Q_EMIT keyboardsChanged();
    }
}

QString KeyboardLoader::title(const QString &id) const
{
    const TagKeyboardPtr keyboard(getTagKeyboard(id));

    if (keyboard) {
        return keyboard->title();
    }

    return QString();
}

Keyboard KeyboardLoader::keyboard() const
{
    Q_D(const KeyboardLoader);
    TagKeyboardPtr keyboard(getTagKeyboard(d->active_id));

    return getKeyboard(keyboard);
}

Keyboard KeyboardLoader::nextKeyboard() const
{
    Q_D(const KeyboardLoader);

    const QStringList all_ids(ids());

    if (all_ids.isEmpty()) {
        return Keyboard();
    }

    int next_index(all_ids.indexOf(d->active_id) + 1);

    if (next_index >= all_ids.size()) {
        next_index = 0;
    }

    TagKeyboardPtr keyboard(getTagKeyboard(all_ids[next_index]));

    return getKeyboard(keyboard);
}

Keyboard KeyboardLoader::previousKeyboard() const
{
    Q_D(const KeyboardLoader);

    const QStringList all_ids(ids());

    if (all_ids.isEmpty()) {
        return Keyboard();
    }

    int previous_index(all_ids.indexOf(d->active_id) - 1);

    if (previous_index < 0) {
        previous_index = 0;
    }

    TagKeyboardPtr keyboard(getTagKeyboard(all_ids[previous_index]));

    return getKeyboard(keyboard);
}

Keyboard KeyboardLoader::shiftedKeyboard() const
{
    Q_D(const KeyboardLoader);
    TagKeyboardPtr keyboard(getTagKeyboard(d->active_id));

    return getKeyboard(keyboard, true);
}

Keyboard KeyboardLoader::symbolsKeyboard(int page) const
{
    Q_D(const KeyboardLoader);

    return getImportedKeyboard(d->active_id, &LayoutParser::symviews, "symbols", "symbols_en.xml", page);
}

Keyboard KeyboardLoader::deadKeyboard(const Key &dead) const
{
    Q_D(const KeyboardLoader);
    TagKeyboardPtr keyboard(getTagKeyboard(d->active_id));

    return getKeyboard(keyboard, false, 0, dead.label().text());
}

Keyboard KeyboardLoader::shiftedDeadKeyboard(const Key &dead) const
{
    Q_D(const KeyboardLoader);
    TagKeyboardPtr keyboard(getTagKeyboard(d->active_id));

    return getKeyboard(keyboard, true, 0, dead.label().text());
}

Keyboard KeyboardLoader::extendedKeyboard(const Key &key) const
{
    // Hotfix for suppressing long-press on space bringing up extended keys if
    // another key has empty label, in given layout.
    // FIXME: Make extended keyboard/keyarea part of key model instead, to
    // avoid wrong lookups.
    if (key.action() == Key::ActionSpace) {
        return Keyboard();
    }

    Q_D(const KeyboardLoader);
    const TagKeyboardPtr keyboard(getTagKeyboard(d->active_id));
    bool shifted(false);
    const QPair<TagKeyPtr, TagBindingPtr> pair(getTagKeyAndBinding(keyboard, key.label().text(), &shifted));
    Keyboard skeyboard;

    if (pair.first and pair.second) {
        const TagExtendedPtr extended(pair.first->extended());

        if (extended) {
            const TagRowPtrs rows(extended->rows());
            int row_index(0);

            Q_FOREACH (const TagRowPtr &row, rows) {
                const TagRowElementPtrs elements(row->elements());

                Q_FOREACH (const TagRowElementPtr &element, elements) {
                    switch (element->element_type()) {
                    case TagRowElement::Key: {
                        const TagKeyPtr key(element.staticCast<TagKey>());
                        const TagBindingPtr binding(key->binding());
                        TagBindingPtr the_binding;

                        if (shifted) {
                            const TagModifiersPtrs all_modifiers(binding->modifiers());

                            Q_FOREACH(const TagModifiersPtr &modifiers, all_modifiers) {
                                if (modifiers->keys() == TagModifiers::Shift) {
                                    the_binding = modifiers->binding();
                                }
                            }
                        }
                        if (not the_binding) {
                            the_binding = binding;
                        }

                        QPair<Key, KeyDescription> key_and_desc(keyAndDescFromTags(key, the_binding, row_index));

                        skeyboard.keys.append(key_and_desc.first);
                        skeyboard.key_descriptions.append(key_and_desc.second);
                    } break;

                    case TagRowElement::Spacer:
                        break;
                    }
                }
                ++row_index;
            }
            // I don't like this prepending source key idea - it should be done
            // in language layout file.
            if (row_index == 1
                and not key.label().text().isEmpty()
                and key.action() == Key::ActionInsert) {
                Key first_key(skeyboard.keys.first());
                KeyDescription first_desc(skeyboard.key_descriptions.first());

                first_key.rLabel().setText(key.label().text());
                first_key.setIcon(key.icon());
                skeyboard.keys.prepend(first_key);
                skeyboard.key_descriptions.prepend(first_desc);
            }
        }
    }
    return skeyboard;
}

Keyboard KeyboardLoader::numberKeyboard() const
{
    Q_D(const KeyboardLoader);

    return getImportedKeyboard(d->active_id, &LayoutParser::numbers, "number", "number.xml");
}

Keyboard KeyboardLoader::phoneNumberKeyboard() const
{
    Q_D(const KeyboardLoader);

    return getImportedKeyboard(d->active_id, &LayoutParser::phonenumbers, "phonenumber", "phonenumber.xml");
}

} // namespace MaliitKeyboard

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

#include "utils.h"
#include "coreutils.h"

#include "models/key.h"
#include "models/keydescription.h"
#include "models/keyboard.h"
#include "models/styleattributes.h"
#include "logic/keyboardloader.h"
#include "logic/keyareaconverter.h"
#include "logic/style.h"
#include "logic/layouthelper.h"

#include <QtCore>
#include <QtTest>
#include <QDebug>

using namespace MaliitKeyboard;

typedef QSharedPointer<KeyboardLoader> SharedKeyboardLoader;

typedef QPair<QString, QString> DictionaryValue;
typedef QMap<QString, QString> Dictionary;

typedef QPair<Key, KeyDescription> KeyDescriptionPair;

Q_DECLARE_METATYPE(Dictionary)
Q_DECLARE_METATYPE(Keyboard)

namespace {

SharedKeyboardLoader getLoader(const QString &id)
{
    SharedKeyboardLoader loader(new KeyboardLoader);

    loader->setActiveId(id);
    return loader;
}

Keyboard stringToKeyboard(const QString &str)
{
    enum
    {
        ExpectFirstSeparator,
        ExpectKey,
        ExpectSeparator
    } expected_char = ExpectFirstSeparator;

    Keyboard kb;
    int row(0);
    bool left_spacer(false);

    Q_FOREACH(const QChar &c, str) {
        switch (expected_char) {
        case ExpectKey:
            expected_char = ExpectSeparator;

            if (c == '\n') {
                ++row;
            } else {
                Label label;
                Key key;
                KeyDescription key_desc;

                label.setText(c);
                key.setLabel(label);
                key_desc.row = row;
                key_desc.left_spacer = left_spacer;
                key_desc.right_spacer = false;
                kb.keys.append(key);
                kb.key_descriptions.append(key_desc);
            }
            left_spacer = false;
            break;

        case ExpectFirstSeparator:
        case ExpectSeparator:
            expected_char = ExpectKey;

            switch (c.toLatin1()) {
            case '|':
                break;

            case ' ':
                if (not kb.key_descriptions.isEmpty()) {
                    KeyDescription& last(kb.key_descriptions.last());

                    if (last.row == row) {
                        last.right_spacer = true;
                    }
                }
                left_spacer = true;
                break;

            default:
                qFatal("A bug in test - wrong separator (expected either | or space)");
                break;
            }
            break;

        default:
            qFatal("A bug in test - wrong expected char type.");
            break;
        }
    }

    if (expected_char == ExpectSeparator) {
        qFatal("A bug in test - last char should be a separator");
    }

    return kb;
}


Dictionary &operator<<(Dictionary &dict, const DictionaryValue &value)
{
    dict.insert(value.first, value.second);
    return dict;
}

Keyboard &operator<<(Keyboard &keyboard, const KeyDescriptionPair &value)
{
    keyboard.keys.append(value.first);
    keyboard.key_descriptions.append(value.second);
    return keyboard;
}

KeyDescriptionPair createActionPair(Key::Action action = Key::ActionInsert,
                                    KeyDescription::Icon icon_type = KeyDescription::NoIcon,
                                    const QByteArray &icon = "")
{
    Key key;
    KeyDescription desc;

    key.setAction(action);
    key.setIcon(icon);
    desc.icon = icon_type;
    return KeyDescriptionPair(key, desc);
}

KeyDescriptionPair createPair(const QString &text,
                              int row,
                              bool left_spacer,
                              bool right_spacer)
{
    Label label;
    Key key;
    KeyDescription desc;

    label.setText(text);
    key.setLabel(label);
    desc.row = row;
    desc.left_spacer = left_spacer;
    desc.right_spacer = right_spacer;

    return KeyDescriptionPair(key, desc);
}

void clearKeyboard(Keyboard &kb)
{
    kb.keys.clear();
    kb.key_descriptions.clear();
    kb.style_name.clear();
}

Key getKey(const QString &text,
           Key::Action action = Key::ActionInsert)
{
    Label label;
    Key key;

    label.setText(text);
    key.setLabel(label);
    key.setAction(action);

    return key;
}

} // unnamed namespace

class TestLanguageLayoutLoading
    : public QObject
{
    Q_OBJECT

private:
    void compareKeyboards(const Keyboard &kb1, const Keyboard &kb2)
    {
        QCOMPARE(kb1.keys.size(), kb2.keys.size());
        QCOMPARE(kb1.key_descriptions.size(), kb2.key_descriptions.size());
        QCOMPARE(kb1.keys.size(), kb1.key_descriptions.size());

        for (int iter(0); iter < kb1.keys.size(); ++iter) {
            QCOMPARE(kb1.keys[iter].label().text(), kb2.keys[iter].label().text());

            const KeyDescription& kd1(kb1.key_descriptions[iter]);
            const KeyDescription& kd2(kb2.key_descriptions[iter]);

            QCOMPARE(kd1.row, kd2.row);
            QCOMPARE(kd1.left_spacer, kd2.left_spacer);
            QCOMPARE(kd1.right_spacer, kd2.right_spacer);
        }
    }

    // QCOMPARE is a macro with "return;" statement in it. So it does not quit
    // the test immediately on failure when it is used in a function called by
    // our test routine. So we are adding another ugly macro. Wheee...
#define COMPARE_KEYBOARDS(gotten_keyboard, expected_keyboard) \
    compareKeyboards(gotten_keyboard, expected_keyboard); \
    if (QTest::currentTestFailed()) { \
        return; \
    }

    Q_SLOT void initTestCase()
    {
        QVERIFY(qputenv("MALIIT_PLUGINS_DATADIR", TEST_DATADIR));
        QVERIFY(qputenv("MALIIT_KEYBOARD_DATADIR", TEST_MALIIT_KEYBOARD_DATADIR));
    }

    Q_SLOT void testSanity_data()
    {
        QTest::addColumn<QString>("string_desc");
        QTest::addColumn<Keyboard>("expected_keyboard");

        Keyboard kb;
        QTest::newRow("Empty string - empty keyboard")
            << ""
            << kb;
        clearKeyboard(kb);

        QTest::newRow("Complicated string")
            << " a b \n c|d|"
            << (kb
                << createPair("a", 0, true, true)
                << createPair("b", 0, true, true)
                << createPair("c", 1, true, false)
                << createPair("d", 1, false, false));
        clearKeyboard(kb);
    }

    Q_SLOT void testSanity()
    {
        QFETCH(QString, string_desc);
        QFETCH(Keyboard, expected_keyboard);

        COMPARE_KEYBOARDS(stringToKeyboard(string_desc), expected_keyboard);
    }

//    Q_SLOT void testGeneral_data()
//    {
//        QTest::addColumn<QString>("keyboard_id");
//        QTest::addColumn<QString>("expected_keyboard");
//        QTest::addColumn<QString>("expected_shifted_keyboard");
//        QTest::addColumn<QString>("expected_number_keyboard");
//        QTest::addColumn<QString>("expected_phone_number_keyboard");
//        QTest::addColumn<Dictionary>("expected_dead_keyboards");
//        QTest::addColumn<Dictionary>("expected_shifted_dead_keyboards");
//        QTest::addColumn<Dictionary>("expected_extended_keyboards");
//        QTest::addColumn<QStringList>("expected_symbols_keyboards");

//        Dictionary dead;
//        Dictionary sdead;
//        Dictionary ext;

//        QTest::newRow("General test")
//            << "general_test1"
//            << "|q|w|\n p a "
//            << "|Q|W|\n p a "
//            << "|0|1|\n 2 3 "
//            << " 9 8 \n 7|6 "
//            << (dead
//                << DictionaryValue(QString::fromUtf8("´"), "|q|e|\n p a ")
//                << DictionaryValue(";", "|q|r|\n p a ")
//                << DictionaryValue("'", "|q|t|\n p a "))
//            << (sdead
//                << DictionaryValue(QString::fromUtf8("´"), "|Q|E|\n p a ")
//                << DictionaryValue(";", "|Q|r|\n p a ")
//                << DictionaryValue("'", "|Q|T|\n p a "))
//            << (ext
//                << DictionaryValue("q", "|y|u|\n|i|o|")
//                << DictionaryValue("Q", "|Y|U|\n|I|O|"))
//            << (QStringList()
//                << "|1|\n|2|"
//                << "|3|\n|4|");
//        dead.clear();
//        sdead.clear();
//        ext.clear();
//    }

//    Q_SLOT void testGeneral()
//    {
//        QFETCH(QString, keyboard_id);
//        QFETCH(QString, expected_keyboard);
//        QFETCH(QString, expected_shifted_keyboard);
//        QFETCH(QString, expected_number_keyboard);
//        QFETCH(QString, expected_phone_number_keyboard);
//        QFETCH(Dictionary, expected_dead_keyboards);
//        QFETCH(Dictionary, expected_shifted_dead_keyboards);
//        QFETCH(Dictionary, expected_extended_keyboards);
//        QFETCH(QStringList, expected_symbols_keyboards);

//        SharedKeyboardLoader loader(getLoader(keyboard_id));

//        QVERIFY(loader->ids().indexOf(keyboard_id) != -1);
//        qDebug() << "Keyboard";
//        COMPARE_KEYBOARDS(loader->keyboard(), stringToKeyboard(expected_keyboard));
//        qDebug() << "Shifted eyboard";
//        COMPARE_KEYBOARDS(loader->shiftedKeyboard(), stringToKeyboard(expected_shifted_keyboard));
//        qDebug() << "Number keyboard";
//        COMPARE_KEYBOARDS(loader->numberKeyboard(), stringToKeyboard(expected_number_keyboard));
//        qDebug() << "Phonenumber keyboard";
//        COMPARE_KEYBOARDS(loader->phoneNumberKeyboard(), stringToKeyboard(expected_phone_number_keyboard));

//        Q_FOREACH(const QString &dead_text, expected_dead_keyboards.keys()) {
//            Key dead_key;
//            Label dead_label;

//            dead_label.setText(dead_text);
//            dead_key.setLabel(dead_label);
//            qDebug() << "Dead keyboard for:" << dead_text;
//            COMPARE_KEYBOARDS(loader->deadKeyboard(dead_key), stringToKeyboard(expected_dead_keyboards[dead_text]));
//        }

//        Q_FOREACH(const QString &dead_text, expected_shifted_dead_keyboards.keys()) {
//            Key dead_key;
//            Label dead_label;

//            dead_label.setText(dead_text);
//            dead_key.setLabel(dead_label);
//            qDebug() << "Shifted dead keyboard for:" << dead_text;
//            COMPARE_KEYBOARDS(loader->shiftedDeadKeyboard(dead_key), stringToKeyboard(expected_shifted_dead_keyboards[dead_text]));
//        }

//        for (int iter(0); iter <= expected_symbols_keyboards.size(); ++iter) {
//            int index(iter % expected_symbols_keyboards.size());

//            qDebug() << "Symbols keyboard, page:" << iter << "(expected page:" << index << ")";
//            COMPARE_KEYBOARDS(loader->symbolsKeyboard(iter), stringToKeyboard(expected_symbols_keyboards[index]));
//        }
//    }

//    Q_SLOT void testStyle_data()
//    {
//        QTest::addColumn<QString>("keyboard_id");
//        QTest::addColumn<QString>("expected_style");

//        QTest::newRow("No style given in xml")
//            << "style_test1"
//            << "keys4";

//        QTest::newRow("Style given in xml")
//            << "style_test2"
//            << "four_symbols";
//    }

//    Q_SLOT void testStyle()
//    {
//        QFETCH(QString, keyboard_id);
//        QFETCH(QString, expected_style);

//        SharedKeyboardLoader loader(getLoader(keyboard_id));
//        QCOMPARE(loader->keyboard().style_name, expected_style);
//    }

//    Q_SLOT void testAction_data()
//    {
//        QTest::addColumn<QString>("keyboard_id");
//        QTest::addColumn<Keyboard>("expected_keyboard");

//        Keyboard kb;

//        QTest::newRow("Implicit insert action")
//            << "action_test1"
//            << (kb
//                << createActionPair());
//        clearKeyboard(kb);

//        QTest::newRow("Explicit actions")
//            << "action_test2"
//            << (kb
//                << createActionPair()
//                << createActionPair(Key::ActionShift)
//                << createActionPair(Key::ActionBackspace)
//                << createActionPair(Key::ActionReturn));
//        clearKeyboard(kb);

//        QTest::newRow("Dead action")
//            << "action_test3"
//            << (kb
//                << createActionPair(Key::ActionDead));
//        clearKeyboard(kb);
//    }

//    Q_SLOT void testAction()
//    {
//        QFETCH(QString, keyboard_id);
//        QFETCH(Keyboard, expected_keyboard);

//        SharedKeyboardLoader loader(getLoader(keyboard_id));
//        Keyboard gotten_keyboard(loader->keyboard());

//        QCOMPARE(gotten_keyboard.keys.size(), expected_keyboard.keys.size());

//        for (int iter(0); iter < expected_keyboard.keys.size(); ++iter) {
//            const Key &gotten_key(gotten_keyboard.keys[iter]);
//            const Key &expected_key(expected_keyboard.keys[iter]);

//            QCOMPARE(gotten_key.action(), expected_key.action());
//        }
//    }

//    Q_SLOT void testIcon_data()
//    {
//        QTest::addColumn<QString>("keyboard_id");
//        QTest::addColumn<Keyboard>("expected_keyboard");

//        Keyboard kb;
//        QTest::newRow("Default icons")
//            << "icon_test1"
//            << (kb
//                << createActionPair(Key::ActionBackspace, KeyDescription::BackspaceIcon)
//                << createActionPair(Key::ActionReturn, KeyDescription::ReturnIcon)
//                << createActionPair(Key::ActionShift, KeyDescription::ShiftIcon)
//                << createActionPair(Key::ActionClose, KeyDescription::CloseIcon));
//        clearKeyboard(kb);

//        QTest::newRow("Custom or empty icons")
//            << "icon_test2"
//            << (kb
//                << createActionPair()
//                << createActionPair(Key::ActionInsert, KeyDescription::CustomIcon, "overriden_icon"));
//        clearKeyboard(kb);

//        /* TODO: Commented out for now, as it is not not possible to override
//           icons for some action keys and I don't know if we want to allow
//           this. I would say that we indeed do want.

//        QTest::newRow("Overriden icons")
//            << "icon_test3"
//            << (kb
//                << actionPair(Key::ActionClose, KeyDescription::CustomIcon, "overriden_icon"));
//        clearKeyboard(kb);
//        */
//    }

//    Q_SLOT void testIcon()
//    {
//        QFETCH(QString, keyboard_id);
//        QFETCH(Keyboard, expected_keyboard);

//        SharedKeyboardLoader loader(getLoader(keyboard_id));
//        Keyboard gotten_keyboard(loader->keyboard());

//        QCOMPARE(gotten_keyboard.keys.size(), expected_keyboard.keys.size());

//        for (int iter(0); iter < expected_keyboard.keys.size(); ++iter) {
//            const Key& gotten_key(gotten_keyboard.keys[iter]);
//            const Key& expected_key(expected_keyboard.keys[iter]);
//            const KeyDescription& gotten_desc(gotten_keyboard.key_descriptions[iter]);
//            const KeyDescription& expected_desc(expected_keyboard.key_descriptions[iter]);

//            QCOMPARE(gotten_key.action(), expected_key.action());
//            QCOMPARE(gotten_desc.icon, expected_desc.icon);
//            QCOMPARE(gotten_key.icon(), expected_key.icon());
//        }
//    }

    Q_SLOT void testExtended_data()
    {
        QTest::addColumn<QString>("keyboard_id");
        QTest::addColumn<Key>("pressed_key");
        QTest::addColumn<QString>("expected_keyboard");

        // TODO: we should get rid of this prepending stuff and fix the language
        // layouts.
        /*
        QTest::newRow("Extended keyboard for unshifted key (also notice prepending)")
            << "extended_test"
            << getKey("a")
            << "|a|b|c|";
        */

//        QTest::newRow("Extended keyboard for shifted key (also notice prepending)")
//            << "extended_test"
//            << getKey("A")
//            << "|A|B|C|";

//        QTest::newRow("Ignore spacers in extended keyboard")
//            << "extended_test"
//            << getKey("d")
//            << "|d|e|f|";

//        QTest::newRow("Extended keyboard with multiple rows (also notice lack of prepending)")
//            << "extended_test"
//            << getKey("g")
//            << "|h|i|\n|j|k|";

//        QTest::newRow("Extended keyboard for empty label (also notice lack of prepending the key with empty label)")
//            << "extended_test"
//            << getKey("")
//            << "|x|";

        QTest::newRow("No extended keyboard for spacebars.")
            << "extended_test"
            << getKey("", Key::ActionSpace)
            << "";

//        QTest::newRow("No non-action-insert prepending")
//            << "extended_test"
//            << getKey("close", Key::ActionClose)
//            << "|X|";
    }

    Q_SLOT void testExtended()
    {
        QFETCH(QString, keyboard_id);
        QFETCH(Key, pressed_key);
        QFETCH(QString, expected_keyboard);

        SharedKeyboardLoader loader(getLoader(keyboard_id));

        COMPARE_KEYBOARDS(loader->extendedKeyboard(pressed_key), stringToKeyboard(expected_keyboard));
    }

    Q_SLOT void testStylingProfile()
    {
        const Logic::LayoutHelper::Orientation orientation(Logic::LayoutHelper::Landscape);
        Style style;
        QCOMPARE(style.availableProfiles().size(), 1);
        QCOMPARE(style.availableProfiles().first(), QString("test-profile"));
        QCOMPARE(style.profile(), QString());
        QCOMPARE(style.attributes()->fontSize(orientation), 0.0);
        QCOMPARE(style.extendedKeysAttributes()->fontSize(orientation), 0.0);
        QCOMPARE(style.directory(Style::Images), QString());
        QCOMPARE(style.directory(Style::Fonts), QString());
        QCOMPARE(style.directory(Style::Sounds), QString());

        QSignalSpy profile_changed_spy(&style, SIGNAL(profileChanged()));
        style.setProfile("test-profile");
        QCOMPARE(profile_changed_spy.count(), 1);
        QCOMPARE(style.profile(), QString("test-profile"));
        QCOMPARE(style.attributes()->fontSize(orientation), 10.0);
        QCOMPARE(style.extendedKeysAttributes()->fontSize(orientation), 0.0);

        const QString test_profile_dir(QString::fromLatin1(TEST_MALIIT_KEYBOARD_DATADIR)
                                       + "/styles/test-profile");
        QCOMPARE(style.directory(Style::Images), test_profile_dir + "/images");
        QCOMPARE(style.directory(Style::Fonts), test_profile_dir + "/fonts");
        QCOMPARE(style.directory(Style::Sounds), test_profile_dir + "/sounds");
    }

//    Q_SLOT void testKeyGeometryStyling_data()
//    {
//        QTest::addColumn<int>("key_index");
//        QTest::addColumn<QString>("expected_label");
//        QTest::addColumn<int>("expected_left_distance");
//        QTest::addColumn<int>("expected_right_distance");
//        QTest::addColumn<int>("expected_left_edge");
//        QTest::addColumn<int>("expected_right_edge");

//        QTest::newRow("1st row: ' a|b ', testing geometry of 'a'.")
//            << 0 << QString("a") << 25 << 5 << 0 << 50;

//        QTest::newRow("1st row: ' a|b ', testing geometry of 'b'.")
//            << 1 << QString("b") << 5 << 25 << 50 << 100;
//    }

//    Q_SLOT void testKeyGeometryStyling()
//    {
//        QFETCH(int, key_index);
//        QFETCH(QString, expected_label);
//        QFETCH(int, expected_left_distance);
//        QFETCH(int, expected_right_distance);
//        QFETCH(int, expected_left_edge);
//        QFETCH(int, expected_right_edge);

//        Style style;
//        style.setProfile("test-profile");
//        StyleAttributes *attributes = style.attributes();
//        SharedKeyboardLoader loader(getLoader("styling_profile_test"));
//        Logic::KeyAreaConverter converter(attributes, loader.data());

//        const KeyArea key_area(converter.keyArea());
//        const Key key(key_area.keys().at(key_index));

//        QCOMPARE(key.label().text(), expected_label);
//        QCOMPARE(key.margins().left(), expected_left_distance);
//        QCOMPARE(key.margins().right(), expected_right_distance);
//        QCOMPARE(key.rect().x(), expected_left_edge);
//        QCOMPARE(key.rect().x() + key.rect().width(), expected_right_edge);
//    }
};

QTEST_MAIN(TestLanguageLayoutLoading)
#include "ut_language-layout-loading.moc"

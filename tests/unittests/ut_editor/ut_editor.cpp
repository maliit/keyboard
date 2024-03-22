/*
 * This file is part of Maliit Plugins
 *
 * Copyright (C) 2012 Openismus GmbH. All rights reserved.
 *
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

#include "wordengineprobe.h"
#include "common/inputmethodhostprobe.h"

#include "plugin/editor.h"
#include "models/key.h"
#include "models/text.h"

#include <QtTest>

using namespace MaliitKeyboard;

Q_DECLARE_METATYPE(WordCandidateList)

namespace {
    void initializeWordEngine(Logic::WordEngineProbe *word_engine)
    {
        word_engine->addSpellingCandidate(QString("He"), QString("Hello"));
        word_engine->addSpellingCandidate(QString("Hel"), QString("Hello"));
        word_engine->addSpellingCandidate(QString("Helo"), QString("Hello"));
        word_engine->addSpellingCandidate(QString("Helol"), QString("Hello"));
        word_engine->addSpellingCandidate(QString("Hello"), QString("Hello"));
        word_engine->addSpellingCandidate(QString("Wo"), QString("World"));
        word_engine->addSpellingCandidate(QString("Wor"), QString("World"));
        word_engine->addSpellingCandidate(QString("Word"), QString("World"));
        word_engine->addSpellingCandidate(QString("Wordl"), QString("World"));
        word_engine->addSpellingCandidate(QString("World"), QString("World"));
    }

    void appendInput(Editor *editor, const QString &input)
    {
        Q_FOREACH(const QChar &c, input) {
            Key key;
            if (c.isSpace()) {
                key.setAction(Key::ActionSpace);
            } else if (c == '\r') {
                key.setAction(Key::ActionReturn);
            } else {
                key.setAction(Key::ActionInsert);
                key.rLabel() = QString(c);
            }
            editor->onKeyPressed(key);
            editor->onKeyReleased(key);
        }
    }

} // namespace

class TestEditor
    : public QObject
{
    Q_OBJECT

private:
    Q_SLOT void initTestCase()
    {
        qRegisterMetaType<WordCandidateList>("WordCandidateList");
    }

    Q_SLOT void testAutoCorrect_data()
    {
        QTest::addColumn<bool>("enable_auto_correct");
        QTest::addColumn<QString>("input");
        QTest::addColumn<QString>("expected_commit_history");

        QTest::newRow("auto-correct disabled")
                << false << "Helol Wordl! " << "Helol Wordl! ";
        QTest::newRow("auto-correct disabled, multiple punctation")
                << false << "Helol Wordl!! " << "Helol Wordl!! ";
        QTest::newRow("auto-correct disabled, digits")
                << false << "Helol Wordl12 " << "Helol Wordl12 ";
//        QTest::newRow("auto-correct enabled")
//                << true << "Helol Wordl! " << "Hello World! ";
//        QTest::newRow("auto-correct enabled, multiple punctation")
//                << true << "Helol Wordl!! " << "Hello World!! ";

        // Tests for the auto-correct and separator-at-end behavior
        // FIXME: In the current testing infra, we cannot really test this properly, as we are using the 'backspace' character
        //  a lot during auto-correction, which is currently not handled too well here.
        QTest::newRow("auto-correct enabled, commit with space, check separators")
                << true << "Hel , Wor ." << "Hello , World .";
        QTest::newRow("auto-correct enabled, commit with separators, check separators")
                << true << "Hel. Wor." << "Hello. World.";
        QTest::newRow("auto-correct enabled, check if two spaces are full-stop")
                << true << "Hel  " << "Hello . ";
        QTest::newRow("auto-correct enabled, check two spaces don't insert full-stop after .")
                << true << "Hello.  " << "Hello.  ";
        QTest::newRow("auto-correct enabled, check two spaces don't insert full-stop after !")
                << true << "Hello!  " << "Hello!  ";
        QTest::newRow("auto-correct enabled, check two spaces don't insert full-stop after ?")
                << true << "Hello?  " << "Hello?  ";
        QTest::newRow("auto-correct enabled, check two spaces don't insert full-stop after ...")
                << true << "Hello...  " << "Hello...  ";
        QTest::newRow("auto-correct enabled, check two spaces does insert full-stop after )")
                << true << "(Hello)  " << "(Hello) . ";
        //QTest::newRow("auto-correct enabled, check removal of unnecessary whitespaces")
        //        << true << "Hello.       . " << "Hello.. ";
    }

    Q_SLOT void testAutoCorrect()
    {
        QFETCH(bool, enable_auto_correct);
        QFETCH(QString, input);
        QFETCH(QString, expected_commit_history);

        Logic::WordEngineProbe *word_engine = new Logic::WordEngineProbe;
        Editor editor(EditorOptions(), new Model::Text, word_engine);

        InputMethodHostProbe host;
        editor.wordEngine()->setEnabled(true);
        editor.setHost(&host);

        initializeWordEngine(word_engine);

        editor.wordEngine()->setWordPredictionEnabled(true);
        editor.setAutoCorrectEnabled(enable_auto_correct);
        editor.setPreeditEnabled(true);
        editor.setAutoCapsEnabled(true);
        editor.setDoubleSpaceFullStopEnabled(true);

        appendInput(&editor, input);

        QCOMPARE(host.commitStringHistory(), expected_commit_history);
    }

    Q_SLOT void testAutoCaps_data()
    {
        QTest::addColumn<bool>("enable_auto_correct");
        QTest::addColumn<bool>("enable_auto_caps");
        QTest::addColumn<QString>("input");
        QTest::addColumn<QString>("expected_commit_history");
        QTest::addColumn<int>("expected_auto_caps_activated_count");

        QTest::newRow("auto-correct and autocaps disabled, no punctation")
                << false << false << "Helol Wordl " << "Helol Wordl " << 0;
//        QTest::newRow("auto-correct enabled, no punctation")
//                << true << "Helol Wordl " << "Hello World " << 0;
        QTest::newRow("auto-correct and autocaps disabled, dot")
                << false << false << "Helol Wordl. " << "Helol Wordl. " << 0;
//        QTest::newRow("auto-correct enabled, dot")
//                << true << "Helol Wordl. " << "Hello World. " << 1;
        QTest::newRow("auto-correct and autocaps disabled, excalamation mark")
                << false << false << "Helol Wordl! " << "Helol Wordl! " << 0;
//        QTest::newRow("auto-correct enabled, excalamation mark")
//                << true << "Helol Wordl! " << "Hello World! " << 1;
        QTest::newRow("auto-correct and autocaps disabled, multiple dots")
                << false << false << "Helol Wordl... " << "Helol Wordl... " << 0;
//        QTest::newRow("auto-correct enabled, multiple dots")
//                << true << "Helol Wordl... " << "Hello World... " << 1;
        QTest::newRow("auto-correct and autocaps disabled, comma")
                << false << false << "Helol Wordl, " << "Helol Wordl, " << 0;
//        QTest::newRow("auto-correct enabled, comma")
//                << true << "Helol Wordl, " << "Hello World, " << 0;
        QTest::newRow("auto-correct and autocaps disabled, quotation mark")
                << false << false << "Helol Wordl\" " << "Helol Wordl\" " << 0;
//        QTest::newRow("auto-correct enabled, quotation mark")
//                << true << "Helol Wordl\" " << "Hello World\" " << 0;
        QTest::newRow("auto-correct and autocaps disabled, multiple sentences with mixed punctation")
                << false << false << "This is a \"first sentence\". And a second, one! "
                << "This is a \"first sentence\". And a second, one! " << 0;
        QTest::newRow("auto-correct and autocaps disabled, multiple sentences with dots")
                << false << false << "First sentence. Second one. And Third. "
                << "First sentence. Second one. And Third. " << 0;

        QTest::newRow("auto-correct disabled, autocaps, no punctation")
                << false << true << "Helol Wordl " << "Helol Wordl " << 0;
        QTest::newRow("auto-correct disabled, autocaps, dot")
                << false << true << "Helol Wordl. " << "Helol Wordl. " << 1;
        QTest::newRow("auto-correct disabled, autocaps, excalamation mark")
                << false << true << "Helol Wordl! " << "Helol Wordl! " << 1;
        QTest::newRow("auto-correct disabled, autocaps, multiple dots")
                << false << true << "Helol Wordl... " << "Helol Wordl... " << 1;
        QTest::newRow("auto-correct disabled, autocaps, comma")
                << false << true << "Helol Wordl, " << "Helol Wordl, " << 0;
        QTest::newRow("auto-correct disabled, autocaps, quotation mark")
                << false << true << "Helol Wordl\" " << "Helol Wordl\" " << 0;
        QTest::newRow("auto-correct disabled, autocaps, multiple sentences with mixed punctation")
                << false << true << "This is a \"first sentence\". And a second, one! "
                << "This is a \"first sentence\". And a second, one! " << 2;
        QTest::newRow("auto-correct disabled, autocaps, multiple sentences with dots")
                << false << true << "First sentence. Second one. And Third. "
                << "First sentence. Second one. And Third. " << 3;

        // Tests for the auto-correct and autocaps separator functionality
        // FIXME: In the current testing infra, we cannot really test this properly, as we are using the 'backspace' character
        //  a lot during auto-correction, which is currently not handled too well here.
        QTest::newRow("auto-correct enabled, autocaps disabled, after separator auto-correction")
                << true << false << "Hello Wor . This should be autocapsed "
                << "Hello World . This should be autocapsed " << 0;
        QTest::newRow("auto-correct and autocaps enabled, autocaps after separator auto-correction")
                << true << true << "Hello Wor . This should be autocapsed "
                << "Hello World . This should be autocapsed " << 2;
    }

    Q_SLOT void testAutoCaps()
    {
        QFETCH(bool, enable_auto_correct);
        QFETCH(bool, enable_auto_caps);
        QFETCH(QString, input);
        QFETCH(QString, expected_commit_history);
        QFETCH(int, expected_auto_caps_activated_count);

        Logic::WordEngineProbe *word_engine = new Logic::WordEngineProbe;
        Editor editor(EditorOptions(), new Model::Text, word_engine);
        QSignalSpy auto_caps_activated_spy(&editor, SIGNAL(autoCapsActivated()));

        InputMethodHostProbe host;
        editor.setHost(&host);

        initializeWordEngine(word_engine);

        editor.wordEngine()->setWordPredictionEnabled(true);
        editor.wordEngine()->setEnabled(true);
        editor.setAutoCorrectEnabled(enable_auto_correct);
        editor.setPreeditEnabled(true);
        editor.setAutoCapsEnabled(enable_auto_caps);

        appendInput(&editor, input);

        QCOMPARE(host.commitStringHistory(), expected_commit_history);
        Q_UNUSED(expected_auto_caps_activated_count)
        QCOMPARE(auto_caps_activated_spy.count(), expected_auto_caps_activated_count);
    }

    Q_SLOT void testRegressionIssue2()
    {
        QSKIP("Test failing, but working correctly in live keyboard…");
        Logic::WordEngineProbe *word_engine = new Logic::WordEngineProbe;
        Editor editor(EditorOptions(), new Model::Text, word_engine);
        QSignalSpy auto_caps_activated_spy(&editor, SIGNAL(autoCapsActivated()));

        InputMethodHostProbe host;
        editor.setHost(&host);

        initializeWordEngine(word_engine);

        editor.wordEngine()->setWordPredictionEnabled(true);
        editor.wordEngine()->setEnabled(true);
        editor.setAutoCorrectEnabled(false);
        editor.setPreeditEnabled(true);
        editor.setAutoCapsEnabled(true);

        appendInput(&editor, "Yesterday (th");
        editor.replaceAndCommitPreedit("the");

        QCOMPARE(host.commitStringHistory(), QString("Yesterday (the"));
    }
};

QTEST_MAIN(TestEditor)
#include "ut_editor.moc"

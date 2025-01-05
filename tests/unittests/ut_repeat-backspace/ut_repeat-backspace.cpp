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
#include "models/key.h"
#include "models/text.h"
#include "logic/wordengine.h"
#include "plugin/editor.h"
#include "common/wordengineprobe.h"

#include <inputmethodhostprobe.h>

#include <QtCore>
#include <QtTest>

using namespace MaliitKeyboard;

typedef void (Editor::*Method)(const Key &key);

Q_DECLARE_METATYPE(Method);

// This test suite verifies how Editor implements auto-repeat for backspace key
class TestRepeatBackspace
    : public QObject
{
    Q_OBJECT

private:
    QScopedPointer<Editor> editor;
    QScopedPointer<InputMethodHostProbe> host;
    EditorOptions options;
    int delay;

    Q_SLOT void initTestCase()
    {
        options.backspace_auto_repeat_delay = 50;
        options.backspace_auto_repeat_interval = 20;

        delay = qMax(options.backspace_auto_repeat_delay, options.backspace_auto_repeat_interval) + 10;
    }

    Q_SLOT void init()
    {
        editor.reset(new Editor(options, new Model::Text, new Logic::WordEngineProbe));
        host.reset(new InputMethodHostProbe);
        editor->setHost(host.data());
    }

    Q_SLOT void cleanup()
    {
        editor.reset();
        host.reset();
    }

    /*
     * testClick verifies following scenario:
     * 1) press (or move finger into) backspace key
     * 2) release key without delay
     * At this point we should send exactly one key event
     * and auto-repeat should not be triggered
     */
    Q_SLOT void testClick_data()
    {
        QTest::addColumn<Method>("initiate");

        Method enter = &Editor::onKeyEntered;
        Method press = &Editor::onKeyPressed;

        QTest::newRow("enter") << enter;
        QTest::newRow("press") << press;
    }

    Q_SLOT void testClick()
    {
        QFETCH(Method, initiate);

        Key backspace;
        backspace.setAction(Key::ActionBackspace);

        (editor.data()->*initiate)(backspace);
        editor->onKeyReleased(backspace);

        QCOMPARE(host->keyEventCount(), 2);
        QCOMPARE(host->lastKeyEvent().type(), QEvent::KeyRelease);
        QCOMPARE(host->lastKeyEvent().key(), int(Qt::Key_Backspace));

        QTest::qWait(delay);

        QCOMPARE(host->keyEventCount(), 2);
    }

    /*
     * testFastMove verifies following scenario:
     * 1) press (or move finger into) backspace key
     * 2) move finger to another key without delay
     * At this point we should not send key events at all.
     */
    Q_SLOT void testFastMove_data()
    {
        QTest::addColumn<Method>("initiate");

        Method enter = &Editor::onKeyEntered;
        Method press = &Editor::onKeyPressed;

        QTest::newRow("enter") << enter;
        QTest::newRow("press") << press;
    }

    Q_SLOT void testFastMove()
    {
        QFETCH(Method, initiate);

        Key backspace;
        backspace.setAction(Key::ActionBackspace);

        (editor.data()->*initiate)(backspace);
        editor->onKeyExited(backspace);

        QCOMPARE(host->keyEventCount(), 0);

        QTest::qWait(delay);

        QCOMPARE(host->keyEventCount(), 0);
    }

    /*
     * testRepeat verifies main scenario for auto repeat:
     * 1) press (or move finger into) backspace key
     * 2) wait a bit, so autorepeat starts
     * 3) we should send first key event now
     * 4) wait again
     * 5) auto-repeat send second event with different delay
     * 3) release (or move finger away from) backspace key
     * Auto-repeat should be stopped now and no new events will be sent
     * even after delay.
     */
    Q_SLOT void testRepeat_data()
    {
        QTest::addColumn<Method>("initiate");
        QTest::addColumn<Method>("finalize");

        Method enter = &Editor::onKeyEntered;
        Method exit = &Editor::onKeyExited;
        Method press = &Editor::onKeyPressed;
        Method release = &Editor::onKeyReleased;

        QTest::newRow("enter-exit")    << enter << exit;
        QTest::newRow("enter-release") << enter << release;
        QTest::newRow("press-release") << press << release;
        QTest::newRow("press-exit")    << press << exit;
    }

    Q_SLOT void testRepeat()
    {
        QFETCH(Method, initiate);
        QFETCH(Method, finalize);

        Key backspace;
        backspace.setAction(Key::ActionBackspace);

        (editor.data()->*initiate)(backspace);

        QCOMPARE(host->keyEventCount(), 0);

        {
            QSignalSpy spy(host.data(), &InputMethodHostProbe::keyEventSent);
            QTRY_VERIFY(spy.count() > 0);
            QCOMPARE(host->keyEventCount(), 2);
            QCOMPARE(host->lastKeyEvent().type(), QEvent::KeyRelease);
            QCOMPARE(host->lastKeyEvent().key(), int(Qt::Key_Backspace));
        }

        {
            QSignalSpy spy(host.data(), &InputMethodHostProbe::keyEventSent);
            QTRY_VERIFY(spy.count() > 0);
            QCOMPARE(host->keyEventCount(), 4);
            QCOMPARE(host->lastKeyEvent().type(), QEvent::KeyRelease);
            QCOMPARE(host->lastKeyEvent().key(), int(Qt::Key_Backspace));
        }

        (editor.data()->*finalize)(backspace);

        QTest::qWait(delay);

        QCOMPARE(host->keyEventCount(), 4);
    }

    Q_SLOT void testInvalidSurroundingText_data()
    {   
        QTest::addColumn<Method>("initiate");
        Method release = &Editor::onKeyReleased;

        QTest::newRow("release") << release;
    }

    /*
     * Verifies that the keyboard doesn't crash when pressing backspace with
     * invalid surrounding text data.
     */
    Q_SLOT void testInvalidSurroundingText()
    {
        QFETCH(Method, initiate);

        editor->wordEngine()->setEnabled(true);
        editor->setPreeditEnabled(true);
        editor->onHasSelectionChanged(false);
        editor->text()->setSurrounding("123");
        editor->text()->setSurroundingOffset(9);

        Key backspace;
        backspace.setAction(Key::ActionBackspace);

        (editor.data()->*initiate)(backspace);
        editor->onKeyReleased(backspace);
        editor->checkPreeditReentry(true);
        QCOMPARE(host->keyEventCount(), 2);
    }


};

QTEST_MAIN(TestRepeatBackspace)

#include "ut_repeat-backspace.moc"

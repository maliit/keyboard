/*
 * Copyright (C) 2013 Canonical, Ltd.
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

#include "models/text.h"

#include <QtCore>
#include <QtTest>

namespace MaliitKeyboard {

class TestText : public QObject
{
    Q_OBJECT

private:

    Q_SLOT void initTestCase()
    {}

    Q_SLOT void init()
    {}

    Q_SLOT void cleanup()
    {}

    Q_SLOT void testRemoveFromPreedit_data()
    {
        QTest::addColumn<QString>("preedit");
        QTest::addColumn<QString>("surrounding");
        QTest::addColumn<int>("cursorPosition");
        QTest::addColumn<int>("deleteLength");
        QTest::addColumn<QString>("newPreedit");
        QTest::addColumn<bool>("returnValue");

        QTest::newRow("delete 0 of 2 at end") << QString("ab") << QString("s")
                                              << 2 << 0 << QString("ab") << false;
        QTest::newRow("delete 1 of 2 at end") << QString("ab") << QString("s")
                                              << 2 << 1 << QString("a") << true;
        QTest::newRow("delete 2 of 2 at end") << QString("ab") << QString("s")
                                              << 2 << 2 << QString("") << true;
        QTest::newRow("delete 3 of 2 at end") << QString("ab") << QString("s")
                                              << 2 << 3 << QString("ab") << false;
        QTest::newRow("delete 1 of 2 in middle") << QString("ab") << QString("s")
                                                 << 1 << 1 << QString("b") << true;
        QTest::newRow("delete 2 of 2 in middle") << QString("ab") << QString("s")
                                                 << 1 << 2 << QString("ab") << false;
    }

    Q_SLOT void testRemoveFromPreedit()
    {
        QFETCH(QString, preedit);
        QFETCH(QString, surrounding);
        QFETCH(int, cursorPosition);
        QFETCH(int, deleteLength);
        QFETCH(QString, newPreedit);
        QFETCH(bool, returnValue);

        Model::Text text;
        text.setPreedit(preedit, cursorPosition);
        text.setSurrounding(surrounding);

        bool ok;
        ok = text.removeFromPreedit(deleteLength);

        QCOMPARE(text.preedit(), newPreedit);
        QCOMPARE(text.surrounding(), surrounding);
        QCOMPARE(ok, returnValue);
    }
};

} // namespace

QTEST_MAIN(MaliitKeyboard::TestText)
#include "ut_text.moc"

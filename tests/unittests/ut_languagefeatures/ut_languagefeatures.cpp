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

#include "logic/languagefeatures.h"

#include <QtCore>
#include <QtTest>

namespace MaliitKeyboard {

class TestLanguageFeatures : public QObject
{
    Q_OBJECT

private:
    Logic::LanguageFeatures m_languageFeatures;

    Q_SLOT void initTestCase()
    {}

    Q_SLOT void init()
    {}

    Q_SLOT void cleanup()
    {}

    Q_SLOT void testAppendixForReplacedPreedit_data()
    {
        QTest::addColumn<QString>("preedit");
        QTest::addColumn<QString>("expectedResult");

        QTest::newRow("usual case") << QString("hello") << QString(" ");
        QTest::newRow("empty preedit") << QString("") << QString("");
    }

    Q_SLOT void testAppendixForReplacedPreedit()
    {
        QFETCH(QString, preedit);
        QFETCH(QString, expectedResult);

        QString result = m_languageFeatures.appendixForReplacedPreedit(preedit);
        QCOMPARE(result, expectedResult);
    }
};

} // namespace

QTEST_MAIN(MaliitKeyboard::TestLanguageFeatures)
#include "ut_languagefeatures.moc"

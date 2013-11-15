/*
 * This file is part of Maliit Plugins
 *
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

#include "plugin/keyboardsettings.h"

#include <QtCore>
#include <QtTest>

namespace MaliitKeyboard {

class TestKeyboardSettings
    : public QObject
{
    Q_OBJECT

private:
    KeyboardSettings *m_settings;

    Q_SLOT void initTestCase()
    {}

    Q_SLOT void init()
    {
        m_settings = new KeyboardSettings(this);
    }

    Q_SLOT void cleanup()
    {
        delete m_settings;
        m_settings = 0;
    }

    Q_SLOT void testSettingUpdated_data()
    {
        QTest::addColumn<QString>("key");
        QTest::addColumn<int>("languagesSpyCount");
        QTest::addColumn<int>("capitalSpyCount");
        QTest::addColumn<int>("completionSpyCount");
        QTest::addColumn<int>("predictSpyCount");
        QTest::addColumn<int>("spellSpyCount");
        QTest::addColumn<int>("feedbackSpyCount");

        QTest::newRow("languages changed") << QString("enabledLanguages")
                                           << 1 << 0 << 0 << 0 << 0 << 0;
        QTest::newRow("capitalization changed") << QString("autoCapitalization")
                                                << 0 << 1 << 0 << 0 << 0 << 0;
        QTest::newRow("completion changed") << QString("autoCompletion")
                                            << 0 << 0 << 1 << 0 << 0 << 0;
        QTest::newRow("predict changed") << QString("predictiveText")
                                         << 0 << 0 << 0 << 1 << 0 << 0;
        QTest::newRow("spellcheck changed") << QString("spellChecking")
                                         << 0 << 0 << 0 << 0 << 1 << 0;
        QTest::newRow("feedback changed") << QString("keyPressFeedback")
                                          << 0 << 0 << 0 << 0 << 0 << 1;
        QTest::newRow("unknown changed") << QString("unknownKey")
                                         << 0 << 0 << 0 << 0 << 0 << 0;
    }

    Q_SLOT void testSettingUpdated()
    {
        QFETCH(QString, key);
        QFETCH(int, languagesSpyCount);
        QFETCH(int, capitalSpyCount);
        QFETCH(int, completionSpyCount);
        QFETCH(int, predictSpyCount);
        QFETCH(int, spellSpyCount);
        QFETCH(int, feedbackSpyCount);

        QSignalSpy languagesSpy(m_settings, SIGNAL(enabledLanguagesChanged(QStringList)));
        QSignalSpy capitalSpy(m_settings, SIGNAL(autoCapitalizationChanged(bool)));
        QSignalSpy completionSpy(m_settings, SIGNAL(autoCompletionChanged(bool)));
        QSignalSpy predictSpy(m_settings, SIGNAL(predictiveTextChanged(bool)));
        QSignalSpy spellSpy(m_settings, SIGNAL(spellCheckingChanged(bool)));
        QSignalSpy feedbackSpy(m_settings, SIGNAL(keyPressFeedbackChanged(bool)));

        m_settings->settingUpdated(key);

        QCOMPARE(languagesSpy.count(), languagesSpyCount);
        QCOMPARE(capitalSpy.count(), capitalSpyCount);
        QCOMPARE(completionSpy.count(), completionSpyCount);
        QCOMPARE(predictSpy.count(), predictSpyCount);
        QCOMPARE(spellSpy.count(), spellSpyCount);
        QCOMPARE(feedbackSpy.count(), feedbackSpyCount);
    }
};

} // namespace

QTEST_MAIN(MaliitKeyboard::TestKeyboardSettings)
#include "ut_keyboardsettings.moc"

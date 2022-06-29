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

#ifndef INPUTMETHODHOSTPROBE_H
#define INPUTMETHODHOSTPROBE_H

#include <maliit/plugins/abstractinputmethodhost.h>
#include <maliit/plugins/plugindescription.h>

#include <QKeyEvent>

class InputMethodHostProbe
    : public MAbstractInputMethodHost
{
    Q_OBJECT

private:
    QString m_commit_string_history;
    QString m_last_preedit_string;
    QKeyEvent m_last_key_event;
    int m_key_event_count;
    QList<Maliit::PreeditTextFormat> m_last_preedit_text_format_list;
    int m_last_replace_start;
    int m_last_replace_length;
    int m_last_cursor_pos;
    bool m_preedit_string_sent;

public:
    InputMethodHostProbe();

    QString commitStringHistory() const;
    void sendCommitString(const QString &string,
                          int replace_start,
                          int replace_length,
                          int cursor_pos);

    QString lastPreeditString() const;
    int lastReplaceStart() const;
    int lastReplaceLength() const;
    int lastCursorPos() const;
    bool preeditStringSent() const;
    void sendPreeditString(const QString &string,
                           const QList<Maliit::PreeditTextFormat> &format,
                           int replace_start, 
                           int replace_length,
                           int cursor_pos);

    QKeyEvent lastKeyEvent() const;
    int keyEventCount() const;
    void sendKeyEvent(const QKeyEvent& event, Maliit::EventRequestType);
    QList<Maliit::PreeditTextFormat> lastPreeditTextFormatList() const;

    Q_SIGNAL void keyEventSent(QKeyEvent ev);

    // unused reimpl
    int contentType(bool&) override {return 0;}
    bool correctionEnabled(bool&) override {return false;}
    bool predictionEnabled(bool&) override {return false;}
    bool autoCapitalizationEnabled(bool&) override {return false;}
    bool surroundingText(QString&, int&) override {return false;}
    bool hasSelection(bool&) override {return false;}
    int inputMethodMode(bool&) override {return 0;}
    QRect preeditRectangle(bool&) override {return QRect();}
    QRect cursorRectangle(bool&) override {return QRect();}
    int anchorPosition(bool&) override {return 0;}
    QString selection(bool&) override {return QString();}
    void notifyImInitiatedHiding() override {}
    void setRedirectKeys(bool) override {}
    void setDetectableAutoRepeat(bool) override {}
    void setGlobalCorrectionEnabled(bool) override {}
    void switchPlugin(Maliit::SwitchDirection) override {}
    void switchPlugin(const QString&) override {}
    void setSelection(int, int) override {}
    void setOrientationAngleLocked(bool) override {}
    QList<MImPluginDescription> pluginDescriptions(Maliit::HandlerState) const override {return QList<MImPluginDescription>();}
    Maliit::Plugins::AbstractPluginSetting* registerPluginSetting(const QString &,
                                                                  const QString &,
                                                                  Maliit::SettingEntryType ,
                                                                  const QVariantMap &) override { return 0; }
    void invokeAction(const QString &,
                      const QKeySequence &) override {}

    void registerWindow(QWindow*, Maliit::Position) override {}
    void setScreenRegion(const QRegion&, QWindow*) override {}
    void setInputMethodArea(const QRegion&, QWindow*) override {}

    QVariant inputMethodQuery(Qt::InputMethodQuery, const QVariant&) const override { return 0; }
};

#endif // INPUTMETHODHOSTPROBE_H

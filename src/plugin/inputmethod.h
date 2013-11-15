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

#ifndef MALIIT_KEYBOARD_INPUTMETHOD_H
#define MALIIT_KEYBOARD_INPUTMETHOD_H

#include <maliit/plugins/abstractinputmethod.h>
#include <maliit/plugins/abstractinputmethodhost.h>
#include <maliit/plugins/keyoverride.h>

#include <QtGui>
#include <QtQuick/QQuickView>

class InputMethodPrivate;

class InputMethod
    : public MAbstractInputMethod
{
    Q_OBJECT
    Q_DISABLE_COPY(InputMethod)
    Q_DECLARE_PRIVATE(InputMethod)

    Q_PROPERTY(bool predictionEnabled READ predictionEnabled NOTIFY predictionEnabledChanged)
    Q_PROPERTY(bool showWordRibbon READ showWordRibbon NOTIFY showWordRibbonChanged)
    Q_PROPERTY(TextContentType contentType READ contentType WRITE setContentType NOTIFY contentTypeChanged)
    Q_PROPERTY(QStringList enabledLanguages READ enabledLanguages NOTIFY enabledLanguagesChanged)
    Q_PROPERTY(QString activeLanguage READ activeLanguage WRITE setActiveLanguage NOTIFY activeLanguageChanged)
    Q_PROPERTY(QString systemLanguage READ systemLanguage NOTIFY systemLanguageChanged)
    Q_PROPERTY(bool useAudioFeedback READ useAudioFeedback NOTIFY useAudioFeedbackChanged)

    Q_ENUMS(TextContentType)

public:
    /// Same as Maliit::TextContentType but usable in QML
    enum TextContentType {
        FreeTextContentType = Maliit::FreeTextContentType,
        NumberContentType = Maliit::NumberContentType,
        PhoneNumberContentType = Maliit::PhoneNumberContentType,
        EmailContentType = Maliit::EmailContentType,
        UrlContentType = Maliit::UrlContentType,
        CustomContentType = Maliit::CustomContentType
    };

    explicit InputMethod(MAbstractInputMethodHost *host);
    virtual ~InputMethod();

    //! \reimp
    virtual void show();
    Q_SLOT virtual void hide();
    virtual void setPreedit(const QString &preedit,
                            int cursor_position);
    virtual void switchContext(Maliit::SwitchDirection direction,
                               bool animated);
    virtual QList<MAbstractInputMethod::MInputMethodSubView>
    subViews(Maliit::HandlerState state = Maliit::OnScreen) const;
    virtual void setActiveSubView(const QString &id,
                                  Maliit::HandlerState state = Maliit::OnScreen);
    virtual QString activeSubView(Maliit::HandlerState state = Maliit::OnScreen) const;
    virtual void handleFocusChange(bool focusIn);
    virtual void handleAppOrientationChanged(int angle);
    virtual bool imExtensionEvent(MImExtensionEvent *event);
    virtual void setKeyOverrides(const QMap<QString, QSharedPointer<MKeyOverride> > &overrides);
    //! \reimp_end

    Q_SLOT void deviceOrientationChanged(Qt::ScreenOrientation orientation);

    Q_SLOT void updateWordEngine();

    bool predictionEnabled();
    bool showWordRibbon();

    TextContentType contentType();
    Q_SLOT void setContentType(TextContentType contentType);

    void update();

    const QStringList &enabledLanguages() const;

    const QString &activeLanguage() const;
    Q_SLOT void setActiveLanguage(const QString& newLanguage);

    const QString &systemLanguage() const;

    Q_SLOT void onVisibleRectChanged();
    bool useAudioFeedback() const;

Q_SIGNALS:
    void predictionEnabledChanged();
    void showWordRibbonChanged(bool show);
    void contentTypeChanged(TextContentType contentType);
    void activateAutocaps();
    void enabledLanguagesChanged(QStringList languages);
    void activeLanguageChanged(QString language);
    void systemLanguageChanged(QString language);
    void useAudioFeedbackChanged();
    void wordEngineEnabledChanged(bool wordEngineEnabled);
    void wordRibbonEnabledChanged(bool wordRibbonEnabled);
    void windowGeometryRectChanged(QRect rect);

private:
    Q_SLOT void onStyleSettingChanged();
    Q_SLOT void onAutoCorrectSettingChanged();
    Q_SLOT void onEnabledLanguageSettingsChanged();
    Q_SLOT void updateAutoCaps();

    Q_SLOT void updateKey(const QString &key_id,
                          const MKeyOverride::KeyOverrideAttributes changed_attributes);
    Q_SLOT void onKeyboardClosed();

    Q_SLOT void onLayoutWidthChanged(int width);
    Q_SLOT void onLayoutHeightChanged(int height);

    void checkInitialAutocaps();

    const QScopedPointer<InputMethodPrivate> d_ptr;
};

#endif // MALIIT_KEYBOARD_INPUTMETHOD_H

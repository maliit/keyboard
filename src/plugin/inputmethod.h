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

    Q_PROPERTY(TextContentType contentType READ contentType WRITE setContentType NOTIFY contentTypeChanged)
    Q_PROPERTY(QStringList enabledLanguages READ enabledLanguages NOTIFY enabledLanguagesChanged)
    Q_PROPERTY(QString activeLanguage READ activeLanguage WRITE setActiveLanguage NOTIFY activeLanguageChanged)
    Q_PROPERTY(QString previousLanguage READ previousLanguage WRITE setPreviousLanguage NOTIFY previousLanguageChanged)
    Q_PROPERTY(bool useAudioFeedback READ useAudioFeedback NOTIFY useAudioFeedbackChanged)
    Q_PROPERTY(QString audioFeedbackSound READ audioFeedbackSound NOTIFY audioFeedbackSoundChanged)
    Q_PROPERTY(QObject* actionKeyOverride READ actionKeyOverride NOTIFY actionKeyOverrideChanged)
    Q_PROPERTY(bool useHapticFeedback READ useHapticFeedback NOTIFY useHapticFeedbackChanged)
    Q_PROPERTY(QString keyboardState READ keyboardState WRITE setKeyboardState NOTIFY keyboardStateChanged)
    Q_PROPERTY(bool hasSelection READ hasSelection NOTIFY hasSelectionChanged)
    Q_PROPERTY(QString currentPluginPath READ currentPluginPath NOTIFY currentPluginPathChanged)
    Q_PROPERTY(QString preedit READ preedit WRITE replacePreedit NOTIFY preeditChanged)
    Q_PROPERTY(int cursorPosition READ cursorPosition WRITE setCursorPosition NOTIFY cursorPositionChanged)
    Q_PROPERTY(QString surroundingLeft READ surroundingLeft)
    Q_PROPERTY(QString surroundingRight READ surroundingRight)

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
    virtual void handleClientChange();
    virtual void reset();
    virtual bool imExtensionEvent(MImExtensionEvent *event);
    virtual void setKeyOverrides(const QMap<QString, QSharedPointer<MKeyOverride> > &overrides);
    //! \reimp_end

    Q_SLOT void deviceOrientationChanged(Qt::ScreenOrientation orientation);

    Q_SLOT void updateWordEngine();

    TextContentType contentType();
    Q_SLOT void setContentType(TextContentType contentType);

    void update();

    const QStringList &enabledLanguages() const;

    const QString &activeLanguage() const;
    Q_SLOT void setActiveLanguage(const QString& newLanguage);

    const QString &previousLanguage() const;
    Q_SLOT void setPreviousLanguage(const QString& prevLanguage);

    Q_SLOT void onVisibleRectChanged();

    bool useAudioFeedback() const;
    const QString audioFeedbackSound() const;
    bool useHapticFeedback() const;

    const QString keyboardState() const;
    Q_SLOT void setKeyboardState(const QString& state);

    bool hasSelection() const;

    const QString currentPluginPath() const;

    const QString &preedit();
    void replacePreedit(const QString &preedit);
    int cursorPosition() const;
    void setCursorPosition(const int pos);

    QObject* actionKeyOverride() const;

    QString surroundingLeft();
    QString surroundingRight();

    Q_SLOT void close();

    Q_INVOKABLE bool languageIsSupported(const QString plugin);
    Q_SLOT void onLanguageChanged(const QString& language);

    Q_SLOT void onPluginPathsChanged(const QStringList& pluginPaths);

Q_SIGNALS:
    void contentTypeChanged(TextContentType contentType);
    void activateAutocaps();
    void deactivateAutocaps();
    void enabledLanguagesChanged(QStringList languages);
    void activeLanguageChanged(QString language);
    void previousLanguageChanged(QString language);
    void useAudioFeedbackChanged();
    void audioFeedbackSoundChanged(QString sound);
    void useHapticFeedbackChanged();
    void wordEngineEnabledChanged(bool wordEngineEnabled);
    void wordRibbonEnabledChanged(bool wordRibbonEnabled);
    void windowGeometryRectChanged(QRect rect);
    void actionKeyOverrideChanged();
    void keyboardStateChanged(QString state);
    void keyboardReset();
    void hasSelectionChanged(bool hasSelection);
    void currentPluginPathChanged(QString currentPluginPath);
    void languagePluginChanged(QString pluginPath, QString languageId);
    void preeditChanged(QString preedit);
    void cursorPositionChanged(int cursor_position);

private:
    Q_SLOT void onAutoCorrectSettingChanged();
    Q_SLOT void onEnabledLanguageSettingsChanged();
    Q_SLOT void onDoubleSpaceSettingChanged();
    Q_SLOT void updateAutoCaps();

    Q_SLOT void onKeyboardClosed();

    Q_SLOT void onLayoutWidthChanged(int width);
    Q_SLOT void onLayoutHeightChanged(int height);

    Q_SLOT void onWordEnginePluginChanged();

    void checkAutocaps();

    const QScopedPointer<InputMethodPrivate> d_ptr;
};

#endif // MALIIT_KEYBOARD_INPUTMETHOD_H

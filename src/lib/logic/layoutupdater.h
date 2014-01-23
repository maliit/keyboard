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

#ifndef MALIIT_KEYBOARD_LAYOUTUPDATER_H
#define MALIIT_KEYBOARD_LAYOUTUPDATER_H

#include "keyboardloader.h"

#include "models/key.h"
#include "models/wordcandidate.h"
#include "logic/layouthelper.h"
#include "logic/style.h"

#include <QtCore>

namespace MaliitKeyboard {
namespace Logic {

class LayoutUpdaterPrivate;

class LayoutUpdater
    : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(LayoutUpdater)
    Q_DECLARE_PRIVATE(LayoutUpdater)

public:
    explicit LayoutUpdater(QObject *parent = 0);
    virtual ~LayoutUpdater();

    void init();

    QStringList keyboardIds() const;
    QString activeKeyboardId() const;
    void setActiveKeyboardId(const QString &id);
    QString keyboardTitle(const QString &id) const;

    void setLayout(LayoutHelper *layout);
    Q_SLOT void setOrientation(LayoutHelper::Orientation orientation);

    void setStyle(const SharedStyle &style);
    Key modifyKey(const Key &key,
                  KeyDescription::State state) const;

    // Key signal handlers:
    Q_SLOT void onKeyPressed(const Key &key);
    Q_SLOT void onKeyLongPressed(const Key &key);
    Q_SLOT void onKeyReleased(const Key &key);
    Q_SLOT void onKeyAreaPressed(Logic::LayoutHelper::Panel panel);
    Q_SLOT void onKeyAreaReleased(Logic::LayoutHelper::Panel panel);
    Q_SLOT void onKeyEntered(const Key &key);
    Q_SLOT void onKeyExited(const Key &key);
    Q_SLOT void clearActiveKeysAndMagnifier();
    Q_SLOT void resetOnKeyboardClosed();

    // ExtendedKeyArea signal handlers:
    Q_SLOT void onExtendedKeysShown(const Key &main_key);
    Q_SIGNAL void addToUserDictionary();

    Q_SIGNAL void keyboardTitleChanged(const QString &title);
    Q_SIGNAL void languageChanged(const QString& languageId);

private:
    Q_SIGNAL void shiftPressed();
    Q_SIGNAL void shiftReleased();
    Q_SIGNAL void autoCapsActivated();
    Q_SIGNAL void shiftCancelled();

    Q_SLOT void syncLayoutToView();
    Q_SLOT void onKeyboardsChanged();

    Q_SIGNAL void symKeyReleased();
    Q_SIGNAL void symSwitcherReleased();

    Q_SLOT void switchToMainView();
    Q_SLOT void switchToPrimarySymView();
    Q_SLOT void switchToSecondarySymView();

    Q_SIGNAL void deadkeyPressed();
    Q_SIGNAL void deadkeyReleased();
    Q_SIGNAL void deadkeyCancelled();

    Q_SLOT void switchToAccentedView();

    const QScopedPointer<LayoutUpdaterPrivate> d_ptr;
};

}} // namespace Logic, MaliitKeyboard

#endif // MALIIT_KEYBOARD_LAYOUTUPDATER_H

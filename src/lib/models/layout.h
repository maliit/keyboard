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

#ifndef MALIIT_KEYBOARD_LAYOUT_H
#define MALIIT_KEYBOARD_LAYOUT_H

#include "models/key.h"
#include <QtCore>

namespace MaliitKeyboard {

class KeyArea;


namespace Logic {
class LayoutHelper;
class LayoutUpdater;
}

namespace Model {

class LayoutPrivate;

// TODO: Move the important/remaining layout handling features from
// Logic::LayoutHelper into this, effectively merging the two classes.
// TODO2: Have an ExtendedLayout subclass, instead of checking active panel flags.
class Layout
    : public QAbstractListModel
{
    Q_OBJECT
    Q_DISABLE_COPY(Layout)
    Q_DECLARE_PRIVATE(Layout)

    Q_PROPERTY(QString title READ title
                             NOTIFY titleChanged)
    Q_PROPERTY(bool visible READ isVisible
                            NOTIFY visibleChanged)
    Q_PROPERTY(int width READ width
                         NOTIFY widthChanged)
    Q_PROPERTY(int height READ height
                          NOTIFY heightChanged)
    Q_PROPERTY(QPoint origin READ origin
                             NOTIFY originChanged)
    Q_PROPERTY(QUrl background READ background
                               NOTIFY backgroundChanged)
    Q_PROPERTY(QRectF background_borders READ backgroundBorders
                                         NOTIFY backgroundBordersChanged)

    Q_PROPERTY(State keyboard_state READ state WRITE setState
                            NOTIFY stateChanged)
    Q_PROPERTY(QString activeView READ activeView WRITE setActiveView
                                         NOTIFY activeViewChanged)

    Q_ENUMS(State)

public:
    enum State {
        DefaultState,
        ShiftedState,
        PrimarySymbolState,
        SecondarySymbolState,
        DeadkeyState
    };

    enum Roles {
        RoleKeyRectangle = Qt::UserRole + 1,
        RoleKeyReactiveArea,
        RoleKeyBackground,
        RoleKeyBackgroundBorders,
        RoleKeyText,
        RoleKeyFont,
        RoleKeyFontColor,
        RoleKeyFontSize,
        RoleKeyFontStretch,
        RoleKeyIcon,
        RoleKeyActionInsert,
        RoleKeyAction   // Extra introspection detail for testing.
    };

    explicit Layout(QObject *parent = 0);
    virtual ~Layout();

    Q_SLOT void setTitle(const QString &title);
    Q_SLOT QString title() const;
    Q_SIGNAL void titleChanged(const QString &changed);

    Q_SLOT void setKeyArea(const KeyArea &area);
    KeyArea keyArea() const;

    void replaceKey(int index,
                    const Key &key);

    Q_SLOT bool isVisible() const;
    Q_SIGNAL void visibleChanged(bool changed);

    Q_SLOT int width() const;
    Q_SIGNAL void widthChanged(int changed);

    Q_SLOT int height() const;
    Q_SIGNAL void heightChanged(int changed);

    Q_SLOT QPoint origin() const;
    Q_SIGNAL void originChanged(const QPoint &changed);

    Q_SLOT QUrl background() const;
    Q_SIGNAL void backgroundChanged(const QUrl &changed);

    Q_SLOT QRectF backgroundBorders() const;
    Q_SIGNAL void backgroundBordersChanged(const QRectF &changed);

    // FIXME: Turn into class variable?
    Q_SLOT void setImageDirectory(const QString &directory);

    Q_SLOT State state() const;
    Q_SLOT void setState(Model::Layout::State state);
    Q_SIGNAL void stateChanged(Model::Layout::State state);

    Q_SLOT QString activeView() const;
    Q_SLOT void setActiveView(const QString& activeViewId);
    Q_SIGNAL void activeViewChanged(const QString &activeViewId);

    virtual QHash<int, QByteArray> roleNames() const;
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
    virtual QVariant data(const QModelIndex &index,
                          int role) const;

    Q_INVOKABLE QVariant data(int index,
                              const QString &role) const;

private:
    const QScopedPointer<LayoutPrivate> d_ptr;
};

}} // namespace Model, MaliitKeyboard

#endif // MALIIT_KEYBOARD_KEYAREACONTAINER_H

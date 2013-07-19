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

#ifndef MALIIT_KEYBOARD_LAYOUT_H
#define MALIIT_KEYBOARD_LAYOUT_H

#include "models/key.h"
#include "models/keyarea.h"
#include "models/wordribbon.h"

#include <QtCore>

namespace MaliitKeyboard {
namespace Logic {

typedef QMap<QString, Key> KeyOverrides;

class LayoutPrivate;

// TODO: Implement hit test on Layout, one to check whether key was hit, one to check whether word candidate was hit.
// Should return invalid key/wc, or found key/wc.
// Would be used by Glass.
class Layout
    : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(Layout)
    Q_DECLARE_PRIVATE(Layout)

public:
    enum Orientation {
        Landscape,
        Portrait
    };

    enum Alignment {
        Left,
        Top,
        Right,
        Bottom
    };

    enum Panel {
        LeftPanel,
        RightPanel,
        CenterPanel,
        ExtendedPanel,
        NumPanels
    };

    Q_ENUMS(Orientation)
    Q_ENUMS(Panel)

    explicit Layout(QObject *parent = 0);
    virtual ~Layout();

    QSize screenSize() const;
    void setScreenSize(const QSize &size);
    Q_SIGNAL void screenSizeChanged(const QSize &size);

    Orientation orientation() const;
    void setOrientation(Orientation orientation);
    Q_SIGNAL void orientationChanged(Orientation orientation);

    Alignment alignment() const;
    void setAlignment(Alignment alignment);
    Q_SIGNAL void alignmentChanged(Alignment alignment);

    Panel activePanel() const;
    void setActivePanel(Panel panel);
    Q_SIGNAL void activePanelChanged(Panel panel);

    KeyArea activeKeyArea() const;
    QRect activeKeyAreaGeometry() const;

    KeyArea leftPanel() const;
    void setLeftPanel(const KeyArea &left);
    Q_SIGNAL void leftPanelChanged(const KeyArea &left,
                                   const Logic::KeyOverrides &overrides);

    KeyArea rightPanel() const;
    void setRightPanel(const KeyArea &right);
    Q_SIGNAL void rightPanelChanged(const KeyArea &right,
                                    const Logic::KeyOverrides &overrides);

    KeyArea centerPanel() const;
    void setCenterPanel(const KeyArea &center);
    Q_SIGNAL void centerPanelChanged(const KeyArea &center,
                                     const Logic::KeyOverrides &overrides);

    KeyArea extendedPanel() const;
    void setExtendedPanel(const KeyArea &extended);
    Q_SIGNAL void extendedPanelChanged(const KeyArea &extended,
                                       const Logic::KeyOverrides &overrides);

    WordRibbon wordRibbon() const;
    void setWordRibbon(const WordRibbon &ribbon);
    Q_SIGNAL void wordRibbonChanged(const WordRibbon &ribbon);

    QVector<Key> activeKeys() const;
    void clearActiveKeys();
    void appendActiveKey(const Key &key);
    void removeActiveKey(const Key &key);
    Q_SIGNAL void activeKeysChanged(const QVector<Key> &keys,
                                    const Logic::KeyOverrides &overrides);
    Q_SIGNAL void activeExtendedKeysChanged(const QVector<Key> &keys,
                                            const Logic::KeyOverrides &overrides);

    Key magnifierKey() const;
    void setMagnifierKey(const Key &key);
    void clearMagnifierKey();
    Q_SIGNAL void magnifierKeyChanged(const Key &key,
                                      const Logic::KeyOverrides &overrides);

    Q_SLOT void onKeysOverriden(const Logic::KeyOverrides &overriden_keys,
                                bool update);

private:
    const QScopedPointer<LayoutPrivate> d_ptr;
};

}} // namespace Logic, MaliitKeyboard

#endif // MALIIT_KEYBOARD_LAYOUT_H

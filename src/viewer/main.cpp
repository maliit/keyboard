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

#include "dashboard.h"

#include "models/keyarea.h"
#include "models/key.h"
#include "models/font.h"
#include "models/wordcandidate.h"
#include "models/wordribbon.h"

#include "logic/layout.h"
#include "logic/layoutupdater.h"
#include "logic/wordengine.h"

#include "view/renderer.h"
#include "view/glass.h"
#include "view/setup.h"

#ifdef HAVE_QT_MOBILITY
#include "view/soundfeedback.h"
typedef MaliitKeyboard::SoundFeedback DefaultFeedback;
#else
#include "view/nullfeedback.h"
typedef MaliitKeyboard::NullFeedback DefaultFeedback;
#endif

#include <QApplication>
#include <QWidget>

namespace {
MaliitKeyboard::Key createKey(const QByteArray &id,
                              const MaliitKeyboard::Font &f,
                              const QRect &kr,
                              const QByteArray &t,
                              const QMargins &m,
                              MaliitKeyboard::Key::Action a = MaliitKeyboard::Key::ActionInsert)
{
    MaliitKeyboard::Key k;
    k.rLabel().setText(t);
    k.setOrigin(kr.topLeft());
    k.rArea().setSize(kr.size());
    k.rArea().setBackground(id);
    k.rLabel().setFont(f);
    k.setMargins(m);
    k.setAction(a);
    k.rArea().setBackgroundBorders(QMargins(6, 6, 6, 6));

    return k;
}

}

int main(int argc,
         char ** argv)
{
    QApplication app(argc, argv);

    MaliitKeyboard::Dashboard *dashboard = new MaliitKeyboard::Dashboard;
    dashboard->show();

    MaliitKeyboard::Renderer renderer;
//    renderer.setSurfaceFactory(dashboard);
    dashboard->setRenderer(&renderer);

    MaliitKeyboard::Glass glass;
//    glass.setWindow(renderer.viewport());

    // One layout updater can only manage one layout. If more layouts need to
    // be managed, then more layout updaters are required.
    MaliitKeyboard::Logic::LayoutUpdater lu0;

    MaliitKeyboard::Logic::Layout l0;
    l0.setAlignment(MaliitKeyboard::Logic::Layout::Bottom);
    l0.setScreenSize(dashboard->size());

    MaliitKeyboard::Font font;
    font.setColor(QByteArray("#ddd"));
    font.setSize(20);

    MaliitKeyboard::WordRibbon ribbon;
    MaliitKeyboard::Area area;
    area.setBackground(QByteArray("key-background.png"));
    area.setBackgroundBorders(QMargins(10, 10, 10, 10));
    area.setSize(QSize(856, 40));
    ribbon.setArea(area);

    l0.setWordRibbon(ribbon);

    glass.addLayout(&l0);
    lu0.setLayout(&l0);

    MaliitKeyboard::Logic::Layout l1;
    l1.setAlignment(MaliitKeyboard::Logic::Layout::Top);
    l1.setScreenSize(dashboard->size());
    glass.addLayout(&l1);

    MaliitKeyboard::Logic::LayoutUpdater lu1;
    lu1.setLayout(&l1);

    DefaultFeedback feedback;
    MaliitKeyboard::Setup::connectAll(&glass, &l0, &lu0, &renderer,
                                      dashboard->editor(), &feedback);

    MaliitKeyboard::Setup::connectGlassToLayoutUpdater(&glass, &lu1);
    MaliitKeyboard::Setup::connectLayoutToRenderer(&l1, &renderer);


    QObject::connect(&glass,    SIGNAL(keyboardClosed()),
                     dashboard, SLOT(onHide()));

    QObject::connect(dashboard, SIGNAL(orientationChanged(Layout::Orientation)),
                     &lu0,      SLOT(setOrientation(Layout::Orientation)));

    QObject::connect(dashboard, SIGNAL(orientationChanged(Layout::Orientation)),
                     &lu1,      SLOT(setOrientation(Layout::Orientation)));

    QObject::connect(&glass,    SIGNAL(keyboardClosed()),
                     dashboard, SIGNAL(keyboardClosed()));

    QObject::connect(dashboard, SIGNAL(keyboardClosed()),
                     &lu0,      SLOT(resetOnKeyboardClosed()));

    QObject::connect(dashboard, SIGNAL(keyboardClosed()),
                     &lu1,      SLOT(resetOnKeyboardClosed()));

    QObject::connect(dashboard, SIGNAL(keyboardClosed()),
                     &renderer, SLOT(hide()));

    QObject::connect(dashboard, SIGNAL(keyboardClosed()),
                     dashboard, SLOT(onHide()));

    // Allow to specify keyboard id via command line:
    QString keyboard_id("en_gb");
    bool found_keyboard_id = false;

    Q_FOREACH (const QString &arg, QApplication::arguments()) {
        if (found_keyboard_id && not arg.isEmpty()) {
            keyboard_id = arg;
        }

        if (arg == "--id" || arg == "-id") {
            found_keyboard_id = true;
        }
    }

    lu0.setActiveKeyboardId(keyboard_id);
    lu1.setActiveKeyboardId("toolbar");
    renderer.show();

    return app.exec();
}

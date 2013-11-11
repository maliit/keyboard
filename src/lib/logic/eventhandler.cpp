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

#include "eventhandler.h"
#include "layoutupdater.h"
#include "models/layout.h"

namespace MaliitKeyboard {
namespace Logic {

class EventHandlerPrivate
{
public:
    Model::Layout * const layout;
    LayoutUpdater * const updater;

    explicit EventHandlerPrivate(Model::Layout * const new_layout,
                                 LayoutUpdater * const new_updater);
};


EventHandlerPrivate::EventHandlerPrivate(Model::Layout *const new_layout,
                                         LayoutUpdater *const new_updater)
    : layout(new_layout)
    , updater(new_updater)
{
    Q_ASSERT(new_layout != 0);
    Q_ASSERT(new_updater != 0);
}


//! \brief Performs event handling for Model::Layout instance, using a LayoutUpdater instance.
//!
//! Does not take ownership of either layout or updater.
EventHandler::EventHandler(Model::Layout * const layout,
                           LayoutUpdater * const updater,
                           QObject *parent)
    : QObject(parent)
    , d_ptr(new EventHandlerPrivate(layout, updater))
{}


EventHandler::~EventHandler()
{}


void EventHandler::onExtendedKeysShown(const Key &key)
{
    Q_D(EventHandler);
    d->updater->onExtendedKeysShown(key);
}


void EventHandler::onEntered(int index)
{
    Q_D(EventHandler);

    const QVector<Key> &keys(d->layout->keyArea().keys());

    if (index >= keys.count()) {
        qWarning() << __PRETTY_FUNCTION__
                   << "Invalid index:" << index
                   << "Keys available:" << keys.count();
        return;
    }

    const Key &key(keys.at(index));

    const Key pressed_key(d->updater->modifyKey(key, KeyDescription::PressedState));
    d->layout->replaceKey(index, pressed_key);
    d->updater->onKeyEntered(key);

    Q_EMIT keyEntered(key);
}


void EventHandler::onExited(int index)
{
    Q_D(EventHandler);

    const QVector<Key> &keys(d->layout->keyArea().keys());

    if (index >= keys.count()) {
        qWarning() << __PRETTY_FUNCTION__
                   << "Invalid index:" << index
                   << "Keys available:" << keys.count();
        return;
    }

    const Key &key(keys.at(index));

    const Key normal_key(d->updater->modifyKey(key, KeyDescription::NormalState));
    d->layout->replaceKey(index, normal_key);
    d->updater->onKeyExited(normal_key);

    Q_EMIT keyExited(key);
}


void EventHandler::onPressed(int index)
{
    Q_D(EventHandler);

    const QVector<Key> &keys(d->layout->keyArea().keys());

    if (index >= keys.count()) {
        qWarning() << __PRETTY_FUNCTION__
                   << "Invalid index:" << index
                   << "Keys available:" << keys.count();
        return;
    }

    const Key &key(keys.at(index));

    const Key pressed_key(d->updater->modifyKey(key, KeyDescription::PressedState));
    d->layout->replaceKey(index, pressed_key);
    d->updater->onKeyPressed(pressed_key);

    Q_EMIT keyPressed(pressed_key);
}


void EventHandler::onReleased(int index)
{
    Q_D(EventHandler);

    const QVector<Key> &keys(d->layout->keyArea().keys());

    if (index >= keys.count()) {
        qWarning() << __PRETTY_FUNCTION__
                   << "Invalid index:" << index
                   << "Keys available:" << keys.count();
        return;
    }

    const Key &key(keys.at(index));

    const Key normal_key(d->updater->modifyKey(key, KeyDescription::NormalState));
    d->layout->replaceKey(index, normal_key);
    d->updater->onKeyReleased(normal_key);

    Q_EMIT keyReleased(normal_key);
}


void EventHandler::onPressAndHold(int index)
{
    Q_D(EventHandler);

    const QVector<Key> &keys(d->layout->keyArea().keys());

    if (index >= keys.count()) {
        qWarning() << __PRETTY_FUNCTION__
                   << "Invalid index:" << index
                   << "Keys available:" << keys.count();
        return;
    }

    const Key &key(keys.at(index));

    // FIXME: long-press on space needs to work again to save words to dictionary!
    if (key.hasExtendedKeys()) {
        Q_EMIT extendedKeysShown(key);
    }

    Q_EMIT keyLongPressed(key);
}

void EventHandler::onWordCandidatePressed(QString word)
{
    WordCandidate candidate(WordCandidate::SourcePrediction, word);
    Q_EMIT wordCandidatePressed(candidate);
}

void EventHandler::onWordCandidateReleased(QString word)
{
    WordCandidate candidate(WordCandidate::SourcePrediction, word);
    Q_EMIT wordCandidateReleased(candidate);
}

void EventHandler::onLanguageChangeRequested(QString languageId)
{
    Q_D(EventHandler);

    d->updater->setActiveKeyboardId(languageId);
}

void EventHandler::onKeyPressed(QString label, QString action)
{
    Key key;
    key.setLabel(label);

    if (action == "backspace")
        key.setAction(Key::ActionBackspace);

    Q_EMIT keyPressed(key);
}

void EventHandler::onKeyReleased(QString label, QString action)
{
    Key key;
    key.setLabel(label);

    if (action == "return")
        key.setAction(Key::ActionReturn);
    else if (action == "backspace")
        key.setAction(Key::ActionBackspace);
    else if (action == "space")
        key.setAction(Key::ActionSpace);
    else if (action == "shift")
        key.setAction(Key::ActionShift);
    else
        key.setAction(Key::ActionInsert);

    Q_EMIT keyReleased(key);
}

void EventHandler::onKeyEntered(QString label)
{
    Key key;
    key.setLabel(label);

    Q_EMIT keyEntered(key);
}


void EventHandler::onKeyExited(QString label)
{
    Key key;
    key.setLabel(label);

    Q_EMIT keyExited(key);
}



}} // namespace Logic, MaliitKeyboard

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

#include <algorithm>
#include <tr1/functional>

#include "layout.h"
#include "coreutils.h"

namespace MaliitKeyboard {
namespace Logic {

namespace {

bool removeKey(QVector<Key> *keys,
               const Key &key)
{
    if (not keys) {
        return false;
    }

    for (int index = 0; index < keys->count(); ++index) {
        const Key &current(keys->at(index));
        if (current.origin() == key.origin()
                && current.label() == key.label()) {
            keys->remove(index);
            return true;
        }
    }

    return false;
}

struct KeyPredicate
{
    KeyPredicate(const QSet<QString> &changed_ids)
        : m_changed_ids(changed_ids)
    {}

    bool operator()(Key &key)
    {
        return (m_changed_ids.constFind(CoreUtils::idFromKey(key)) != m_changed_ids.constEnd());
    }

    const QSet<QString> &m_changed_ids;
};

} // namespace

typedef std::tr1::function<void(const KeyArea &, const KeyOverrides &)> EmitFunc;

class LayoutPrivate
{
public:
    QSize screen_size;
    QPoint origin;
    Layout::Orientation orientation;
    Layout::Alignment alignment;
    Layout::Panel active_panel;
    KeyArea left;
    KeyArea right;
    KeyArea center;
    KeyArea extended;

    // TODO: Make WordCandidates part of KeyArea
    WordRibbon ribbon;

    // TODO: Store active keys in KeyArea
    struct {
        QVector<Key> left;
        QVector<Key> right;
        QVector<Key> center;
        QVector<Key> extended;
    } active_keys;

    Key magnifier_key;
    KeyOverrides overriden_keys;

    explicit LayoutPrivate();

    KeyArea lookup(Layout::Panel panel) const;
    QPoint panelOrigin() const;
    void overrideCheck(const QSet<QString> &changed_ids,
                       KeyArea &key_area,
                       const EmitFunc& func);
};

LayoutPrivate::LayoutPrivate()
    : screen_size()
    , orientation(Layout::Landscape)
    , alignment(Layout::Bottom)
    , active_panel(Layout::CenterPanel)
    , left()
    , right()
    , center()
    , extended()
    , ribbon()
    , active_keys()
    , magnifier_key()
    , overriden_keys()
{}

KeyArea LayoutPrivate::lookup(Layout::Panel panel) const
{
    switch(panel) {
    case Layout::LeftPanel: return left;
    case Layout::RightPanel: return right;
    case Layout::CenterPanel: return center;
    case Layout::ExtendedPanel: return extended;
    case Layout::NumPanels: break;
    }

    qCritical() << __PRETTY_FUNCTION__
                << "Should not be reached, invalid panel:" << panel;
    return KeyArea();
}

QPoint LayoutPrivate::panelOrigin() const
{
    return QPoint(0, ribbon.area().size().height());
}

void LayoutPrivate::overrideCheck(const QSet<QString> &changed_ids,
                                  KeyArea &key_area,
                                  const EmitFunc &func)
{
    QVector<Key> &keys(key_area.rKeys());
    KeyPredicate predicate(changed_ids);

    if (std::find_if(keys.begin(), keys.end(), predicate) != keys.end()) {
        func(key_area, overriden_keys);
    }
}

Layout::Layout(QObject *parent)
    : QObject(parent)
    , d_ptr(new LayoutPrivate)
{}

Layout::~Layout()
{}

QSize Layout::screenSize() const
{
    Q_D(const Layout);
    return d->screen_size;
}

void Layout::setScreenSize(const QSize &size)
{
    Q_D(Layout);

    if (d->screen_size != size) {
        d->screen_size = size;
        Q_EMIT screenSizeChanged(d->screen_size);
    }
}

Layout::Orientation Layout::orientation() const
{
    Q_D(const Layout);
    return d->orientation;
}

void Layout::setOrientation(Orientation orientation)
{
    Q_D(Layout);

    if (d->orientation != orientation) {
        d->orientation = orientation;
        Q_EMIT orientationChanged(d->orientation);
    }
}

Layout::Alignment Layout::alignment() const
{
    Q_D(const Layout);
    return d->alignment;
}

void Layout::setAlignment(Alignment alignment)
{
    Q_D(Layout);

    if (d->alignment != alignment) {
        d->alignment = alignment;
        Q_EMIT alignmentChanged(d->alignment);
    }
}

Layout::Panel Layout::activePanel() const
{
    Q_D(const Layout);
    return d->active_panel;
}

void Layout::setActivePanel(Panel panel)
{
    Q_D(Layout);

    if (panel != NumPanels && d->active_panel != panel) {
        d->active_panel = panel;
        Q_EMIT activePanelChanged(d->active_panel);
    }
}

KeyArea Layout::activeKeyArea() const
{
    Q_D(const Layout);
    return d->lookup(activePanel());
}

QRect Layout::activeKeyAreaGeometry() const
{
    Q_D(const Layout);

    switch(d->active_panel) {
    case LeftPanel: return d->left.rect();
    case RightPanel: return d->right.rect();
    case CenterPanel: return d->center.rect();
    // FIXME: Dirty hack, because we don't want the glass to take extended key origin into account:
    case ExtendedPanel: return QRect(QPoint(), d->extended.area().size());

    default:
        break;
    }

    qCritical() << __PRETTY_FUNCTION__
                << "Should not be reached, invalid panel:" << d->active_panel;
    return QRect();
}

KeyArea Layout::leftPanel() const
{
    Q_D(const Layout);
    return d->left;
}

void Layout::setLeftPanel(const KeyArea &left)
{
    Q_D(Layout);

    if (d->left != left) {
        d->left = left;
        Q_EMIT leftPanelChanged(d->left, d->overriden_keys);
    }
}

KeyArea Layout::rightPanel() const
{
    Q_D(const Layout);
    return d->right;
}

void Layout::setRightPanel(const KeyArea &right)
{
    Q_D(Layout);

    if (d->right != right) {
        d->right = right;
        Q_EMIT rightPanelChanged(d->right, d->overriden_keys);
    }
}

KeyArea Layout::centerPanel() const
{
    Q_D(const Layout);
    return d->center;
}

void Layout::setCenterPanel(const KeyArea &center)
{
    Q_D(Layout);

    if (d->center != center) {
        d->center = center;
        Q_EMIT centerPanelChanged(d->center, d->overriden_keys);
    }
}

KeyArea Layout::extendedPanel() const
{
    Q_D(const Layout);
    return d->extended;
}

void Layout::setExtendedPanel(const KeyArea &extended)
{
    Q_D(Layout);

    if (d->extended != extended) {
        d->extended = extended;
        Q_EMIT extendedPanelChanged(d->extended, d->overriden_keys);
    }
}

WordRibbon Layout::wordRibbon() const
{
    Q_D(const Layout);
    return d->ribbon;
}

void Layout::setWordRibbon(const WordRibbon &ribbon)
{
    Q_D(Layout);

    if (d->ribbon != ribbon) {
        d->ribbon = ribbon;
        Q_EMIT wordRibbonChanged(d->ribbon);
    }
}

QVector<Key> Layout::activeKeys() const
{
    Q_D(const Layout);

    switch (d->active_panel) {
    case LeftPanel: return d->active_keys.left;
    case RightPanel: return d->active_keys.right;
    case CenterPanel: return d->active_keys.center;
    case ExtendedPanel: return d->active_keys.extended;
    case NumPanels: break;
    }

    return QVector<Key>();
}

void Layout::clearActiveKeys()
{
    Q_D(Layout);

    d->active_keys.left.clear();
    d->active_keys.right.clear();
    d->active_keys.center.clear();
    d->active_keys.extended.clear();

    QVector<Key> empty;
    KeyOverrides empty_overrides;
    Q_EMIT activeKeysChanged(empty, empty_overrides);
    Q_EMIT activeExtendedKeysChanged(empty, empty_overrides);
}

void Layout::appendActiveKey(const Key &key)
{
    Q_D(Layout);

    switch (d->active_panel) {
    case LeftPanel:
    case RightPanel:
    case NumPanels: break;

    case CenterPanel:
        d->active_keys.center.append(key);
        Q_EMIT activeKeysChanged(d->active_keys.center, d->overriden_keys);
        break;

    case ExtendedPanel:
        d->active_keys.extended.append(key);
        Q_EMIT activeExtendedKeysChanged(d->active_keys.extended, d->overriden_keys);
        break;
    }
}

void Layout::removeActiveKey(const Key &key)
{
    Q_D(Layout);

    switch (d->active_panel) {
    case LeftPanel:
    case RightPanel:
    case NumPanels: break;

    case CenterPanel:
        if (removeKey(&d->active_keys.center, key)) {
            Q_EMIT activeKeysChanged(d->active_keys.center, d->overriden_keys);
        }
        break;

    case ExtendedPanel:
        if (removeKey(&d->active_keys.extended, key)) {
            Q_EMIT activeExtendedKeysChanged(d->active_keys.extended, d->overriden_keys);
        }
        break;
    }
}

Key Layout::magnifierKey() const
{
    Q_D(const Layout);
    return d->magnifier_key;
}

void Layout::setMagnifierKey(const Key &key)
{
    Q_D(Layout);

    if (d->magnifier_key != key) {
        d->magnifier_key = key;
        d->magnifier_key.setOrigin(d->magnifier_key.origin() + d->panelOrigin());
        Q_EMIT magnifierKeyChanged(d->magnifier_key, d->overriden_keys);
    }
}

void Layout::clearMagnifierKey()
{
    setMagnifierKey(Key());
}

void Layout::onKeysOverriden(const KeyOverrides &overriden_keys,
                             bool update)
{
    Q_D(Layout);
    QSet<QString> changed_ids;

    if (update) {
        for (KeyOverrides::const_iterator i(overriden_keys.begin()), e(overriden_keys.end()); i != e; ++i) {
            KeyOverrides::iterator override(d->overriden_keys.find(i.key()));

            if (override != d->overriden_keys.end()
                && override.value() != i.value()) {
                override.value() = i.value();
                changed_ids.insert(i.key());
            }
        }
    } else if (d->overriden_keys != overriden_keys) {
        changed_ids = QSet<QString>::fromList(d->overriden_keys.keys()).unite(QSet<QString>::fromList(overriden_keys.keys()));
        d->overriden_keys = overriden_keys;
    }

    using std::tr1::placeholders::_1;
    using std::tr1::placeholders::_2;

    d->overrideCheck(changed_ids, d->left, std::tr1::bind(&Layout::leftPanelChanged, this, _1, _2));
    d->overrideCheck(changed_ids, d->right, std::tr1::bind(&Layout::rightPanelChanged, this, _1, _2));
    d->overrideCheck(changed_ids, d->center, std::tr1::bind(&Layout::centerPanelChanged, this, _1, _2));
    d->overrideCheck(changed_ids, d->extended, std::tr1::bind(&Layout::extendedPanelChanged, this, _1, _2));
}

}} // namespace Logic, MaliitKeyboard

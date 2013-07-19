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

#include "layouthelper.h"
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

class LayoutHelperPrivate
{
public:
    QSize screen_size;
    QPoint origin;
    LayoutHelper::Orientation orientation;
    LayoutHelper::Alignment alignment;
    LayoutHelper::Panel active_panel;
    KeyArea left;
    KeyArea right;
    KeyArea center;
    KeyArea extended;

    // TODO: Make WordCandidates part of KeyArea
    WordRibbon* ribbon;

    // TODO: Store active keys in KeyArea
    struct {
        QVector<Key> left;
        QVector<Key> right;
        QVector<Key> center;
        QVector<Key> extended;
    } active_keys;

    Key magnifier_key;
    KeyOverrides overriden_keys;

    explicit LayoutHelperPrivate();

    KeyArea lookup(LayoutHelper::Panel panel) const;
    QPoint panelOrigin() const;
    void overrideCheck(const QSet<QString> &changed_ids,
                       KeyArea &key_area,
                       const EmitFunc& func);
};

LayoutHelperPrivate::LayoutHelperPrivate()
    : screen_size()
    , orientation(LayoutHelper::Landscape)
    , alignment(LayoutHelper::Bottom)
    , active_panel(LayoutHelper::CenterPanel)
    , left()
    , right()
    , center()
    , extended()
    , ribbon(new WordRibbon)
    , active_keys()
    , magnifier_key()
    , overriden_keys()
{}

KeyArea LayoutHelperPrivate::lookup(LayoutHelper::Panel panel) const
{
    switch(panel) {
    case LayoutHelper::LeftPanel: return left;
    case LayoutHelper::RightPanel: return right;
    case LayoutHelper::CenterPanel: return center;
    case LayoutHelper::ExtendedPanel: return extended;
    case LayoutHelper::NumPanels: break;
    }

    qCritical() << __PRETTY_FUNCTION__
                << "Should not be reached, invalid panel:" << panel;
    return KeyArea();
}

QPoint LayoutHelperPrivate::panelOrigin() const
{
    return QPoint(0, ribbon->area().size().height());
}

void LayoutHelperPrivate::overrideCheck(const QSet<QString> &changed_ids,
                                  KeyArea &key_area,
                                  const EmitFunc &func)
{
    QVector<Key> &keys(key_area.rKeys());
    KeyPredicate predicate(changed_ids);

    if (std::find_if(keys.begin(), keys.end(), predicate) != keys.end()) {
        func(key_area, overriden_keys);
    }
}

LayoutHelper::LayoutHelper(QObject *parent)
    : QObject(parent)
    , d_ptr(new LayoutHelperPrivate)
{}

LayoutHelper::~LayoutHelper()
{}

QSize LayoutHelper::screenSize() const
{
    Q_D(const LayoutHelper);
    return d->screen_size;
}

void LayoutHelper::setScreenSize(const QSize &size)
{
    Q_D(LayoutHelper);

    if (d->screen_size != size) {
        d->screen_size = size;
        Q_EMIT screenSizeChanged(d->screen_size);
    }
}

LayoutHelper::Orientation LayoutHelper::orientation() const
{
    Q_D(const LayoutHelper);
    return d->orientation;
}

void LayoutHelper::setOrientation(Orientation orientation)
{
    Q_D(LayoutHelper);

    if (d->orientation != orientation) {
        d->orientation = orientation;
        Q_EMIT orientationChanged(d->orientation);
    }
}

LayoutHelper::Alignment LayoutHelper::alignment() const
{
    Q_D(const LayoutHelper);
    return d->alignment;
}

void LayoutHelper::setAlignment(Alignment alignment)
{
    Q_D(LayoutHelper);

    if (d->alignment != alignment) {
        d->alignment = alignment;
        Q_EMIT alignmentChanged(d->alignment);
    }
}

LayoutHelper::Panel LayoutHelper::activePanel() const
{
    Q_D(const LayoutHelper);
    return d->active_panel;
}

void LayoutHelper::setActivePanel(Panel panel)
{
    Q_D(LayoutHelper);

    if (panel != NumPanels && d->active_panel != panel) {
        d->active_panel = panel;
        Q_EMIT activePanelChanged(d->active_panel);
    }
}

KeyArea LayoutHelper::activeKeyArea() const
{
    Q_D(const LayoutHelper);
    return d->lookup(activePanel());
}

QRect LayoutHelper::activeKeyAreaGeometry() const
{
    Q_D(const LayoutHelper);

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

KeyArea LayoutHelper::leftPanel() const
{
    Q_D(const LayoutHelper);
    return d->left;
}

void LayoutHelper::setLeftPanel(const KeyArea &left)
{
    Q_D(LayoutHelper);

    if (d->left != left) {
        d->left = left;
        Q_EMIT leftPanelChanged(d->left, d->overriden_keys);
    }
}

KeyArea LayoutHelper::rightPanel() const
{
    Q_D(const LayoutHelper);
    return d->right;
}

void LayoutHelper::setRightPanel(const KeyArea &right)
{
    Q_D(LayoutHelper);

    if (d->right != right) {
        d->right = right;
        Q_EMIT rightPanelChanged(d->right, d->overriden_keys);
    }
}

KeyArea LayoutHelper::centerPanel() const
{
    Q_D(const LayoutHelper);
    return d->center;
}

void LayoutHelper::setCenterPanel(const KeyArea &center)
{
    Q_D(LayoutHelper);

    if (d->center != center) {
        d->center = center;
        Q_EMIT centerPanelChanged(d->center, d->overriden_keys);
    }
}

KeyArea LayoutHelper::extendedPanel() const
{
    Q_D(const LayoutHelper);
    return d->extended;
}

void LayoutHelper::setExtendedPanel(const KeyArea &extended)
{
    Q_D(LayoutHelper);

    if (d->extended != extended) {
        d->extended = extended;
        Q_EMIT extendedPanelChanged(d->extended, d->overriden_keys);
    }
}

WordRibbon* LayoutHelper::wordRibbon() const
{
    Q_D(const LayoutHelper);
    return d->ribbon;
}

void LayoutHelper::setWordRibbon(WordRibbon* ribbon)
{
    Q_D(LayoutHelper);

    if (d->ribbon != ribbon) {
        d->ribbon = ribbon;
        Q_EMIT wordRibbonChanged(d->ribbon);
    }
}

QVector<Key> LayoutHelper::activeKeys() const
{
    Q_D(const LayoutHelper);

    switch (d->active_panel) {
    case LeftPanel: return d->active_keys.left;
    case RightPanel: return d->active_keys.right;
    case CenterPanel: return d->active_keys.center;
    case ExtendedPanel: return d->active_keys.extended;
    case NumPanels: break;
    }

    return QVector<Key>();
}

void LayoutHelper::clearActiveKeys()
{
    Q_D(LayoutHelper);

    d->active_keys.left.clear();
    d->active_keys.right.clear();
    d->active_keys.center.clear();
    d->active_keys.extended.clear();

    QVector<Key> empty;
    KeyOverrides empty_overrides;
    Q_EMIT activeKeysChanged(empty, empty_overrides);
    Q_EMIT activeExtendedKeysChanged(empty, empty_overrides);
}

void LayoutHelper::appendActiveKey(const Key &key)
{
    Q_D(LayoutHelper);

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

void LayoutHelper::removeActiveKey(const Key &key)
{
    Q_D(LayoutHelper);

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

Key LayoutHelper::magnifierKey() const
{
    Q_D(const LayoutHelper);
    return d->magnifier_key;
}

void LayoutHelper::setMagnifierKey(const Key &key)
{
    Q_D(LayoutHelper);

    if (d->magnifier_key != key) {
        d->magnifier_key = key;

        Key magnifier(d->magnifier_key);
        magnifier.setOrigin(d->magnifier_key.origin());

        // FIXME: Avoid this silly conversion by changing the API (also in LayoutUpdater) to take a KeyArea instead.
        KeyArea area;
        area.setOrigin(magnifier.origin());
        magnifier.setOrigin(QPoint());

        area.setArea(magnifier.area());
        magnifier.rArea().setBackground(QByteArray());
        area.rKeys().append(magnifier);

        Q_EMIT magnifierChanged(area);
    }
}

void LayoutHelper::clearMagnifierKey()
{
    setMagnifierKey(Key());
}

void LayoutHelper::onKeysOverriden(const KeyOverrides &overriden_keys,
                             bool update)
{
    Q_D(LayoutHelper);
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

    d->overrideCheck(changed_ids, d->left, std::tr1::bind(&LayoutHelper::leftPanelChanged, this, _1, _2));
    d->overrideCheck(changed_ids, d->right, std::tr1::bind(&LayoutHelper::rightPanelChanged, this, _1, _2));
    d->overrideCheck(changed_ids, d->center, std::tr1::bind(&LayoutHelper::centerPanelChanged, this, _1, _2));
    d->overrideCheck(changed_ids, d->extended, std::tr1::bind(&LayoutHelper::extendedPanelChanged, this, _1, _2));
}

}} // namespace Logic, MaliitKeyboard

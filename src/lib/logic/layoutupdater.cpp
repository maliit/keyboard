/*
 * This file is part of Maliit Plugins
 *
 * Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies). All rights reserved.
 * Copyright (C) 2012 Openismus GmbH
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

#include "layoutupdater.h"
#include "style.h"

#include "models/area.h"
#include "models/keydescription.h"
#include "models/wordribbon.h"
#include "models/wordcandidate.h"
#include "models/text.h"
#include "models/styleattributes.h"

#include "logic/keyareaconverter.h"

namespace MaliitKeyboard {
namespace Logic {

enum ActivationPolicy {
    ActivateElement,
    DeactivateElement
};

Key modifyKey(const Key &key,
              KeyDescription::State state,
              const StyleAttributes *attributes)
{
    if (not attributes) {
        return key;
    }

    Key k(key);

    k.rArea().setBackground(attributes->keyBackground(key.style(), state));
    k.rArea().setBackgroundBorders(attributes->keyBackgroundBorders());

    return k;
}

void applyStyleToCandidate(WordCandidate *candidate,
                           const StyleAttributes *attributes,
                           LayoutHelper::Orientation orientation,
                           ActivationPolicy policy)
{
    if (not candidate || not attributes) {
        return;
    }

    Label &label(candidate->rLabel());
    Font f(label.font());
    f.setSize(attributes->candidateFontSize(orientation));
    f.setStretch(attributes->candidateFontStretch(orientation));

    QByteArray color;
    switch(policy) {
    case ActivateElement:
        color = QByteArray("#fff");
        break;

    case DeactivateElement:
        color = QByteArray("#ddd");
        break;
    }

    f.setColor(color);
    label.setFont(f);
}

// FIXME: Make word candidates fit word ribbon also after orientation change.
void applyStyleToWordRibbon(WordRibbon *ribbon,
                            const SharedStyle &style,
                            LayoutHelper::Orientation orientation)
{
    if (not ribbon || style.isNull()) {
        return;
    }

    Area area;
    StyleAttributes *const a(style->attributes());

    area.setBackground(a->wordRibbonBackground());
    area.setBackgroundBorders(a->wordRibbonBackgroundBorders());
    area.setSize(QSize(a->keyAreaWidth(orientation), a->wordRibbonHeight(orientation)));
    ribbon->setArea(area);
}

bool updateWordRibbon(LayoutHelper *layout,
                      const WordCandidate &candidate,
                      const StyleAttributes *attributes,
                      ActivationPolicy policy)
{
    if (not layout || not attributes) {
        return false;
    }

    QVector<WordCandidate> &candidates(layout->wordRibbon()->rCandidates());

    for (int index = 0; index < candidates.count(); ++index) {
        WordCandidate &current(candidates[index]);

        if (current.label().text() == candidate.label().text()) {
            // in qml we don´t care about ( current.rect() == candidate.rect() )
            applyStyleToCandidate(&current, attributes, layout->orientation(), policy);
            layout->setWordRibbon(layout->wordRibbon());

            return true;
        }
    }

    return false;
}

QRect adjustedRect(const QRect &rect, const QMargins &margins)
{
    return rect.adjusted(margins.left(), margins.top(), -margins.right(), -margins.bottom());
}

Key magnifyKey(const Key &key,
               const StyleAttributes *attributes,
               LayoutHelper::Orientation orientation,
               const QRectF &key_area_rect)
{
    Font magnifier_font;
    magnifier_font.setName(attributes->fontName(orientation));
    magnifier_font.setColor(attributes->fontColor(orientation));
    magnifier_font.setSize(attributes->magnifierFontSize(orientation));

    if (key.action() != Key::ActionInsert) {
        return Key();
    }

    const QRect adjusted_key_rect(adjustedRect(key.rect(), key.margins()));
    QRect magnifier_rect(adjusted_key_rect.topLeft(),
                         QSize(attributes->magnifierKeyWidth(orientation),
                               attributes->magnifierKeyHeight(orientation)));
    magnifier_rect.translate((adjusted_key_rect.width() - magnifier_rect.width()) / 2,
                             -1 * attributes->verticalOffset(orientation));

    const QRect &mapped(magnifier_rect.translated(key_area_rect.topLeft().toPoint()));

    const int delta_left(mapped.left()
                         - (key_area_rect.left()
                         + attributes->safetyMargin(orientation)));
    const int delta_right((key_area_rect.x()
                           + key_area_rect.width()
                           - attributes->safetyMargin(orientation))
                          - (mapped.x() + mapped.width()));

    if (delta_left < 0) {
        magnifier_rect.translate(qAbs<int>(delta_left), 0);
    } else if (delta_right < 0) {
        magnifier_rect.translate(delta_right, 0);
    }

    Key magnifier(key);
    magnifier.setOrigin(magnifier_rect.topLeft());
    magnifier.rArea().setBackground(attributes->magnifierKeyBackground());
    magnifier.rArea().setSize(magnifier_rect.size());
    magnifier.rArea().setBackgroundBorders(attributes->magnifierKeyBackgroundBorders());
    magnifier.rLabel().setFont(magnifier_font);

    // Compute label rectangle, contains the text:
    const qreal label_offset(attributes->magnifierKeyLabelVerticalOffset(orientation));
    const QSize &magnifier_size(magnifier.area().size());
    const QRect label_rect(0, 0,
                           magnifier_size.width(),
                           magnifier_size.height() - label_offset);
    magnifier.rLabel().setRect(label_rect);
    magnifier.setMargins(QMargins());

    return magnifier;
}

class LayoutUpdaterPrivate
{
public:
    bool initialized;
    LayoutHelper *layout;
    KeyboardLoader loader;
    SharedStyle style;
    bool word_ribbon_visible;
    LayoutHelper::Panel close_extended_on_release;

    explicit LayoutUpdaterPrivate()
        : initialized(false)
        , layout(0)
        , loader()
        , style()
        , word_ribbon_visible(false)
        , close_extended_on_release(LayoutHelper::NumPanels) // NumPanels counts as invalid panel.
    {}

    bool inShiftedState() const
    {
        return false;
    }

    bool arePrimarySymbolsShown() const
    {
        return false;
    }

    bool areSecondarySymbolsShown() const
    {
        return false;
    }

    bool areSymbolsShown() const
    {
        return arePrimarySymbolsShown() or areSecondarySymbolsShown();
    }

    bool inDeadkeyState() const
    {
        return false;
    }

    const StyleAttributes * activeStyleAttributes() const
    {
        return (layout->activePanel() == LayoutHelper::ExtendedPanel
                ? style->extendedKeysAttributes() : style->attributes());
    }
};

LayoutUpdater::LayoutUpdater(QObject *parent)
    : QObject(parent)
    , d_ptr(new LayoutUpdaterPrivate)
{
    connect(&d_ptr->loader, SIGNAL(keyboardsChanged()),
            this,           SLOT(onKeyboardsChanged()),
            Qt::UniqueConnection);
}

LayoutUpdater::~LayoutUpdater()
{}

void LayoutUpdater::init()
{
}

QStringList LayoutUpdater::keyboardIds() const
{
    Q_D(const LayoutUpdater);
    return d->loader.ids();
}

QString LayoutUpdater::activeKeyboardId() const
{
    Q_D(const LayoutUpdater);
    return d->loader.activeId();
}

void LayoutUpdater::setActiveKeyboardId(const QString &id)
{
    Q_D(LayoutUpdater);
    d->loader.setActiveId(id);
    Q_EMIT languageChanged(id);
}

QString LayoutUpdater::keyboardTitle(const QString &id) const
{
    Q_D(const LayoutUpdater);
    return d->loader.title(id);
}

void LayoutUpdater::setLayout(LayoutHelper *layout)
{
    Q_D(LayoutUpdater);
    d->layout = layout;

    if (not d->initialized) {
        init();
        d->initialized = true;
    }
}

void LayoutUpdater::setOrientation(LayoutHelper::Orientation orientation)
{
    Q_D(LayoutUpdater);

    if (d->layout && d->style && d->layout->orientation() != orientation) {
        d->layout->setOrientation(orientation);

        KeyAreaConverter converter(d->style->attributes(), &d->loader);
        converter.setLayoutOrientation(orientation);
        d->layout->setCenterPanel(d->inShiftedState() ? converter.shiftedKeyArea()
                                                      : converter.keyArea());

        if (isWordRibbonVisible())
            applyStyleToWordRibbon(d->layout->wordRibbon(), d->style, orientation);

        clearActiveKeysAndMagnifier();
    }
}

void LayoutUpdater::setStyle(const SharedStyle &style)
{
    Q_D(LayoutUpdater);
    d->style = style;
}

bool LayoutUpdater::isWordRibbonVisible() const
{
    Q_D(const LayoutUpdater);
    return d->word_ribbon_visible;
}

void LayoutUpdater::setWordRibbonVisible(bool visible)
{
    Q_D(LayoutUpdater);

    if (d->word_ribbon_visible != visible) {
        d->word_ribbon_visible = visible;

        d->layout->wordRibbon()->clearCandidates();

        Q_EMIT wordRibbonVisibleChanged(visible);
    }
}

//! \brief Modify visual appearance of a key, depending on state.
//!
//! Uses the currently active style and the key action to decide the visual
//! appearance. Can be used to make a key appear pressed or a shift key appear
//! locked.
//!
//! \param key The key to modify.
//! \param state The key state (normal, pressed, disabled, highlighted).
Key LayoutUpdater::modifyKey(const Key &key,
                             KeyDescription::State state) const
{
    Q_D(const LayoutUpdater);
    return MaliitKeyboard::Logic::modifyKey(key, state, d->activeStyleAttributes());
}

void LayoutUpdater::onKeyPressed(const Key &key)
{
    Q_D(LayoutUpdater);

    if (not d->layout) {
        return;
    }

    d->layout->appendActiveKey(MaliitKeyboard::Logic::modifyKey(key, KeyDescription::PressedState,
                                                                d->activeStyleAttributes()));

    if (d->layout->activePanel() == LayoutHelper::CenterPanel) {
        d->layout->setMagnifierKey(magnifyKey(key, d->activeStyleAttributes(), d->layout->orientation(),
                                              d->layout->centerPanel().rect()));
    }

    switch (key.action()) {
    case Key::ActionShift:
        Q_EMIT shiftPressed();
        break;

    case Key::ActionDead:
        Q_EMIT deadkeyPressed();
        break;

    default:
        break;
    }
}

void LayoutUpdater::onKeyLongPressed(const Key &key)
{
    Q_UNUSED(key);
    Q_D(LayoutUpdater);

    if (not d->layout || d->style.isNull()) {
        return;
    }

    clearActiveKeysAndMagnifier();

    const LayoutHelper::Orientation orientation(d->layout->orientation());
    StyleAttributes * const extended_attributes(d->style->extendedKeysAttributes());
    const qreal vertical_offset(d->style->attributes()->verticalOffset(orientation));
    KeyAreaConverter converter(extended_attributes, &d->loader);
    converter.setLayoutOrientation(orientation);
    KeyArea ext_ka(converter.extendedKeyArea(key));

    if (not ext_ka.hasKeys()) {
        if (key.action() == Key::ActionSpace) {
            Q_EMIT addToUserDictionary();
        }
        return;
    }

    const QSize &ext_panel_size(ext_ka.area().size());
    const QSize &center_panel_size(d->layout->centerPanel().area().size());
    const QPointF &key_center(key.rect().center());
    const qreal safety_margin(extended_attributes->safetyMargin(orientation));

    QPoint offset(qMax<int>(safety_margin, key_center.x() - ext_panel_size.width() / 2),
                  key.rect().top() - vertical_offset);

    if (offset.x() + ext_panel_size.width() > center_panel_size.width()) {
        offset.rx() = center_panel_size.width() - ext_panel_size.width() - safety_margin;
    }

    ext_ka.setOrigin(offset);
    d->layout->setExtendedPanel(ext_ka);
    d->layout->setActivePanel(LayoutHelper::ExtendedPanel);
}

void LayoutUpdater::onKeyReleased(const Key &key)
{
    Q_D(const LayoutUpdater);

    if (not d->layout) {
        return;
    }

    d->layout->removeActiveKey(key);
    d->layout->clearMagnifierKey();

    if (d->layout->activePanel() == LayoutHelper::ExtendedPanel) {
        d->layout->clearActiveKeys();
        d->layout->setExtendedPanel(KeyArea());
        d->layout->setActivePanel(LayoutHelper::CenterPanel);
        return;
    }

    switch (key.action()) {
    case Key::ActionShift:
        Q_EMIT shiftReleased();
        break;

    case Key::ActionInsert:
        break;

    case Key::ActionSym:
        Q_EMIT symKeyReleased();
        break;

    case Key::ActionSwitch:
        Q_EMIT symSwitcherReleased();
        break;

    case Key::ActionDead:
        Q_EMIT deadkeyReleased();
        break;

    default:
        break;
    }
}

void LayoutUpdater::onKeyAreaPressed(LayoutHelper::Panel panel)
{
    Q_D(LayoutUpdater);

    if (not d->layout) {
        return;
    }

    if (d->layout->activePanel() == LayoutHelper::ExtendedPanel && panel != LayoutHelper::ExtendedPanel) {
        d->close_extended_on_release = panel;
    }
}

void LayoutUpdater::onKeyAreaReleased(LayoutHelper::Panel panel)
{
    Q_D(LayoutUpdater);

    if (not d->layout) {
        return;
    }

    if (d->close_extended_on_release == panel) {
        d->layout->setExtendedPanel(KeyArea());
        d->layout->setActivePanel(LayoutHelper::CenterPanel);
    }

    d->close_extended_on_release = LayoutHelper::NumPanels;
}

void LayoutUpdater::onKeyEntered(const Key &key)
{
    Q_D(const LayoutUpdater);

    if (not d->layout) {
        return;
    }

    d->layout->appendActiveKey(MaliitKeyboard::Logic::modifyKey(key, KeyDescription::PressedState,
                                                                d->activeStyleAttributes()));

    if (d->layout->activePanel() == LayoutHelper::CenterPanel) {
        d->layout->setMagnifierKey(magnifyKey(key, d->activeStyleAttributes(), d->layout->orientation(),
                                              d->layout->centerPanel().rect()));
    }
}

void LayoutUpdater::onKeyExited(const Key &key)
{
    Q_D(const LayoutUpdater);

    if (not d->layout) {
        return;
    }

    d->layout->removeActiveKey(key);
    d->layout->clearMagnifierKey(); // FIXME: This is in a race with onKeyEntered.
}

void LayoutUpdater::clearActiveKeysAndMagnifier()
{
    Q_D(const LayoutUpdater);

    if (not d->layout) {
        qCritical() << __PRETTY_FUNCTION__
                    << "No layout specified.";
        return;
    }

    d->layout->clearActiveKeys();
    d->layout->clearMagnifierKey();
}

void LayoutUpdater::resetOnKeyboardClosed()
{
    Q_D(const LayoutUpdater);

    clearActiveKeysAndMagnifier();
    d->layout->setExtendedPanel(KeyArea());
    d->layout->setActivePanel(LayoutHelper::CenterPanel);
}

void LayoutUpdater::onWordCandidatesChanged(const WordCandidateList &candidates)
{
    Q_D(LayoutUpdater);

    if (not d->layout || not isWordRibbonVisible()) {
        qWarning() << __PRETTY_FUNCTION__
                   << "No layout specified or word ribbon not visible.";
        return;
    }

    // Copy WordRibbon instance in order to preserve geometry and styling:
    d->layout->wordRibbon()->clearCandidates();

    const StyleAttributes * const attributes(d->activeStyleAttributes());
    const LayoutHelper::Orientation orientation(d->layout->orientation());
    const int candidate_width(attributes->keyAreaWidth(orientation) / (orientation == LayoutHelper::Landscape ? 6 : 4));

    for (int index = 0; index < candidates.count(); ++index) {
        WordCandidate word_candidate(candidates.at(index));
        // FIXME candidate height needs to come from word ribbon height
        word_candidate.rArea().setSize(QSize(word_candidate.source() == WordCandidate::SourceUser
                                             ? attributes->keyAreaWidth(orientation) : candidate_width, 56));
        word_candidate.setOrigin(QPoint(index * candidate_width, 0));
        applyStyleToCandidate(&word_candidate, d->activeStyleAttributes(), orientation, DeactivateElement);
        d->layout->wordRibbon()->appendCandidate(word_candidate);
    }
}

void LayoutUpdater::onExtendedKeysShown(const Key &main_key)
{
    Q_D(LayoutUpdater);

    if (not d->layout || d->style.isNull()) {
        return;
    }

    clearActiveKeysAndMagnifier();

    const LayoutHelper::Orientation orientation(d->layout->orientation());
    StyleAttributes * const extended_attributes(d->style->extendedKeysAttributes());
    const qreal vertical_offset(d->style->attributes()->verticalOffset(orientation));
    KeyAreaConverter converter(extended_attributes, &d->loader);
    converter.setLayoutOrientation(orientation);
    KeyArea ext_ka(converter.extendedKeyArea(main_key));

    if (not ext_ka.hasKeys()) {
        if (main_key.action() == Key::ActionSpace) {
            Q_EMIT addToUserDictionary();
        }
        return;
    }

    const QSize &ext_panel_size(ext_ka.area().size());
    const QSize &center_panel_size(d->layout->centerPanel().area().size());
    const QPointF &key_center(main_key.rect().center());
    const qreal safety_margin(extended_attributes->safetyMargin(orientation));

    QPoint offset(qMax<int>(safety_margin, key_center.x() - ext_panel_size.width() / 2),
                  main_key.rect().top() - vertical_offset);

    if (offset.x() + ext_panel_size.width() > center_panel_size.width()) {
        offset.rx() = center_panel_size.width() - ext_panel_size.width() - safety_margin;
    }

    ext_ka.setOrigin(offset);
    d->layout->setExtendedPanel(ext_ka);
    d->layout->setActivePanel(LayoutHelper::ExtendedPanel);
}

void LayoutUpdater::onWordCandidatePressed(const WordCandidate &candidate)
{
    Q_D(LayoutUpdater);

    if (d->layout
        && isWordRibbonVisible()
        && updateWordRibbon(d->layout, candidate, d->activeStyleAttributes(), ActivateElement)) {
    }
}

void LayoutUpdater::onWordCandidateReleased(const WordCandidate &candidate)
{
    Q_D(LayoutUpdater);

    if (d->layout
        && isWordRibbonVisible()
        && updateWordRibbon(d->layout, candidate, d->activeStyleAttributes(), DeactivateElement)) {
        if (candidate.source() == WordCandidate::SourcePrediction
            || candidate.source() == WordCandidate::SourceSpellChecking) {
            Q_EMIT wordCandidateSelected(candidate.word());
        } else if (candidate.source() == WordCandidate::SourceUser) {
            Q_EMIT userCandidateSelected(candidate.word());
        }
    }
}

void LayoutUpdater::syncLayoutToView()
{
    Q_D(const LayoutUpdater);

    if (not d->layout) {
        return;
    }

    // Symbols do not care about shift state.
    if (d->areSymbolsShown()) {
        return;
    }

    if (d->inDeadkeyState()) {
        switchToAccentedView();
    } else {
        switchToMainView();
    }
}

void LayoutUpdater::onKeyboardsChanged()
{
    Q_D(LayoutUpdater);

    Q_EMIT keyboardTitleChanged(d->loader.title(d->loader.activeId()));
}

void LayoutUpdater::switchToMainView()
{
    Q_D(LayoutUpdater);

    if (not d->layout || d->style.isNull()) {
        return;
    }

    d->layout->clearActiveKeys();
    d->layout->clearMagnifierKey();

    const LayoutHelper::Orientation orientation(d->layout->orientation());

    if (d->word_ribbon_visible)
        applyStyleToWordRibbon(d->layout->wordRibbon(), d->style, orientation);

    KeyAreaConverter converter(d->style->attributes(), &d->loader);
    converter.setLayoutOrientation(orientation);
    d->layout->setCenterPanel(d->inShiftedState() ? converter.shiftedKeyArea()
                                                  : converter.keyArea());

    if (d->inShiftedState())
        Q_EMIT d->layout->stateChanged(Model::Layout::ShiftedState);
    else if (d->inDeadkeyState())
        Q_EMIT d->layout->stateChanged(Model::Layout::DeadkeyState);
    else
        Q_EMIT d->layout->stateChanged(Model::Layout::DefaultState);
}

void LayoutUpdater::switchToPrimarySymView()
{
    Q_D(LayoutUpdater);

    if (not d->layout || d->style.isNull()) {
        return;
    }

    const LayoutHelper::Orientation orientation(d->layout->orientation());
    KeyAreaConverter converter(d->style->attributes(), &d->loader);
    converter.setLayoutOrientation(orientation);
    d->layout->setCenterPanel(converter.symbolsKeyArea(0));

    Q_EMIT d->layout->stateChanged(Model::Layout::PrimarySymbolState);
}

void LayoutUpdater::switchToSecondarySymView()
{
    Q_D(LayoutUpdater);

    if (not d->layout || d->style.isNull()) {
        return;
    }

    const LayoutHelper::Orientation orientation(d->layout->orientation());
    KeyAreaConverter converter(d->style->attributes(), &d->loader);
    converter.setLayoutOrientation(orientation);
    d->layout->setCenterPanel(converter.symbolsKeyArea(1));

    Q_EMIT d->layout->stateChanged(Model::Layout::SecondarySymbolState);
}

void LayoutUpdater::switchToAccentedView()
{
    Q_D(LayoutUpdater);

    if (not d->layout || d->style.isNull()) {
        return;
    }


    const LayoutHelper::Orientation orientation(d->layout->orientation());
    KeyAreaConverter converter(d->style->attributes(), &d->loader);
    converter.setLayoutOrientation(orientation);
}

}} // namespace Logic, MaliitKeyboard

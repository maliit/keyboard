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

#include "abstracttexteditor.h"
#include "models/wordribbon.h"
#include "logic/abstractlanguagefeatures.h"

#include <QElapsedTimer>
#include <QRegularExpression>

namespace MaliitKeyboard {

//! \class EditorOptions
//! \brief Plain struct implementing editor options.

//! \fn EditorOptions::EditorOptions()
//! \brief Constructor.
//!
//! Sets backspace_auto_repeat_delay to 500 miliseconds and backspace_auto_repeat_interval to 300 miliseconds.

//! \var EditorOptions::backspace_auto_repeat_delay
//! \brief Delay before first automatically repeated key in miliseconds.

//! \var EditorOptions::backspace_auto_repeat_interval
//! \brief Interval between automatically repeated key in miliseconds.

//! \class AbstractTextEditor
//! \brief Class implementing preedit edition.
//!
//! It owns a text model (which can be gotten by text() method) and a
//! word engine (word_engine()). The class has to be subclassed and
//! subclass has to provide sendPreeditString(), sendCommitString(),
//! sendKeyEvent(), invokeAction() and destructor implementations.

// The function declaration has to be in one line, because \fn is a
// single line parameter.
//! \fn void AbstractTextEditor::sendPreeditString(const QString &preedit, Model::Text::PreeditFace face, const Replacement &replacement)
//! \brief Sends preedit to application.
//! \param preedit Preedit to send.
//! \param face Face of the preedit.
//! \param replacement Struct describing replacement of the text.
//!
//! Implementations of this pure virtual method have to convert \a
//! face into specific attributes used by backend they
//! support. Preedit text has to be sent as-is. Replacement members
//! should be understood as follows: if either start or length are
//! lesser than zero or both of them are zeros then those parameters
//! should be ignored. Otherwise they mark the beginning and length of
//! a surrounding text's substring that is going to be replaced by
//! given \a preedit. Cursor position member of replacement should be
//! ignored if its value is lesser than zero. Otherwise it describes a
//! position of cursor relatively to the beginning of preedit.

//! \fn void AbstractTextEditor::sendCommitString(const QString &commit)
//! \brief Commits a string to application.
//! \param commit String to be commited in place of preedit.
//!
//! Implementations of this method should discard current preedit and
//! commit given \a commit in its place.

//! \fn void AbstractTextEditor::sendKeyEvent(const QKeyEvent &ev)
//! \brief Sends a key event to application.
//! \param ev Key event to send.
//!
//! The implementation should translate passed \a ev to values their
//! backend understands and pass this to application.

//! \fn void AbstractTextEditor::invokeAction(const QString &action, const QKeySequence &sequence)
//! \brief Invokes an action in the application
//! \param action Action to invoke
//! \param sequence Key sequence to emit when action cannot be called directly
//!
//! Application first tries to invoke a signal/slot \a action and when not available
//! it will emit the key sequence \a sequence. One would call this method for
//! example with "copy", "CTRL+C" arguments.

//! \property AbstractTextEditor::preeditEnabled
//! \brief Describes whether preedit is enabled.
//!
//! When it is \c false then everything typed by virtual keyboard is
//! immediately commited.

//! \property AbstractTextEditor::autoCorrectEnabled
//! \brief Describes whether auto correction on space is enabled.
//!
//! When it is \c true then pressing space will commit corrected word
//! if it was misspelled. Otherwise it will just commit what was in
//! preedit.

//! \fn void AbstractTextEditor::autoCapsActivated()
//! \brief Emitted when auto capitalization mode is enabled for
//! following input.

//! \fn void AbstractTextEditor::autoCorrectEnabledChanged(bool enabled)
//! \brief Emitted when auto correction setting changes.
//! \param enabled New setting.

//! \fn void AbstractTextEditor::preeditEnabledChanged(bool enabled)
//! \brief Emitted when preedit setting changes.
//! \param enabled New setting.

//! \fn void AbstractTextEditor::preeditChanged(const QString &preedit)
//! \brief Emitted when preedit string changes.
//! \param enabled New string.

//! \fn void AbstractTextEditor::cursorPositionChanged(int pos)
//! \brief Emitted when preedit cusor position changes.
//! \param enabled New position.

//! \fn void AbstractTextEditor::wordCandidatesChanged(const WordCandidateList &word_candidates)
//! \brief Emitted when new word candidates are generated.
//! \param word_candidates New word candidates.
//!
//! Note that the list might be empty as well to indicate that there
//! should be no word candidates.

//! \fn void AbstractTextEditor::keyboardClosed()
//! \brief Emitted when keyboard close is requested.

//! \class AbstractTextEditor::Replacement
//! \brief Plain struct containing beginning and length of replacement and
//! desired cursor position.
//!
//! Mostly used by sendPreeditString() implementations.

//! \fn AbstractTextEditor::Replacement::Replacement()
//! \brief Constructor.
//!
//! Constructs an instance with no replacement and no cursor position
//! change.

//! \fn AbstractTextEditor::Replacement::Replacement(int position)
//! \brief Constructor.
//! \param position New cursor position.
//!
//! Constructs an instance with no replacement and new cursor position.

//! \fn AbstractTextEditor::Replacement::Replacement(int r_start, int r_length, int position)
//! \brief Constructor.
//! \param r_start Replacement start.
//! \param r_length Replacement length.
//! \param position New cursor position.
//!
//! Constructs an instance with a replacement and new cursor position.

//! \var AbstractTextEditor::Replacement::start
//! \brief Beginning of replacement.

//! \var AbstractTextEditor::Replacement::length
//! \brief Length of replacement.

//! \var AbstractTextEditor::Replacement::cursor_position
//! \brief New cursor position relative to the beginning of preedit.

EditorOptions::EditorOptions()
    : backspace_auto_repeat_delay(500)
    , backspace_auto_repeat_interval(200)
    , backspace_auto_repeat_acceleration_rate(10)
    , backspace_auto_repeat_min_interval(50)
    , backspace_word_switch_threshold(3)
    , backspace_word_interval(400)
    , backspace_word_acceleration_rate(10)
    , backspace_word_min_interval(50)
{}

class AbstractTextEditorPrivate
{
public:
    QTimer auto_repeat_backspace_timer;
    bool backspace_sent;
    bool repeating_backspace;
    EditorOptions options;
    QScopedPointer<Model::Text> text;
    QScopedPointer<Logic::AbstractWordEngine> word_engine;
    bool preedit_enabled;
    bool auto_correct_enabled;
    bool auto_caps_enabled;
    int ignore_next_cursor_position;
    QString ignore_next_surrounding_text;
    bool look_for_a_double_space;
    bool look_for_a_triple_space;
    bool double_space_full_stop_enabled;
    bool editing_middle_of_text;
    QString appendix_for_previous_preedit;
    int backspace_acceleration;
    int backspace_word_acceleration;
    int deleted_words;
    QString keyboardState;
    QString previous_preedit;
    int previous_preedit_position;

    explicit AbstractTextEditorPrivate(const EditorOptions &new_options,
                                       Model::Text *new_text,
                                       Logic::AbstractWordEngine *new_word_engine);
    bool valid() const;
};

AbstractTextEditorPrivate::AbstractTextEditorPrivate(const EditorOptions &new_options,
                                                     Model::Text *new_text,
                                                     Logic::AbstractWordEngine *new_word_engine)
    : auto_repeat_backspace_timer()
    , backspace_sent(false)
    , repeating_backspace(false)
    , options(new_options)
    , text(new_text)
    , word_engine(new_word_engine)
    , preedit_enabled(false)
    , auto_correct_enabled(false)
    , auto_caps_enabled(false)
    , ignore_next_cursor_position(-1)
    , ignore_next_surrounding_text()
    , look_for_a_double_space(false)
    , look_for_a_triple_space(false)
    , double_space_full_stop_enabled(false)
    , editing_middle_of_text(false)
    , appendix_for_previous_preedit()
    , backspace_acceleration(0)
    , backspace_word_acceleration(0)
    , deleted_words(0)
    , keyboardState(QStringLiteral("CHARACTERS"))
    , previous_preedit()
    , previous_preedit_position(0)
{
    auto_repeat_backspace_timer.setSingleShot(true);
    (void) valid();
}

bool AbstractTextEditorPrivate::valid() const
{
                                                         // TODO language_fetaures
    const bool is_invalid(text.isNull() || word_engine.isNull());

    if (is_invalid) {
        qCritical() << __PRETTY_FUNCTION__
                    << "Invalid text model, or no word engine given! The text editor will not function properly.";
    }

    return (not is_invalid);
}

//! \brief Constructor.
//! \param options Editor options.
//! \param text Text model.
//! \param word_engine Word engine.
//! \param parent Parent of this instance or \c NULL if none is needed.
//!
//! Takes ownership of \a text, \a word_engine and \a language_features.
AbstractTextEditor::AbstractTextEditor(const EditorOptions &options,
                                       Model::Text *text,
                                       Logic::AbstractWordEngine *word_engine,
                                       QObject *parent)
    : QObject(parent)
    , d_ptr(new AbstractTextEditorPrivate(options, text, word_engine))
{
    connect(&d_ptr->auto_repeat_backspace_timer, &QTimer::timeout,
            this,                                &AbstractTextEditor::autoRepeatBackspace);

    connect(word_engine, &Logic::AbstractWordEngine::enabledChanged,
            this,        &AbstractTextEditor::setPreeditEnabled);

    connect(word_engine, &Logic::AbstractWordEngine::candidatesChanged,
            this,        &AbstractTextEditor::wordCandidatesChanged);

    connect(word_engine, &Logic::AbstractWordEngine::preeditFaceChanged,
            this,        &AbstractTextEditor::setPreeditFace);

    connect(word_engine, &Logic::AbstractWordEngine::primaryCandidateChanged,
            this,        &AbstractTextEditor::setPrimaryCandidate);

    connect(this,        &AbstractTextEditor::autoCorrectEnabledChanged,
            word_engine, &Logic::AbstractWordEngine::setAutoCorrectEnabled);

    setPreeditEnabled(word_engine->isEnabled());
}

//! \brief Destructor.
AbstractTextEditor::~AbstractTextEditor() = default;

//! \brief Gets editor's text model.
Model::Text * AbstractTextEditor::text() const
{
    Q_D(const AbstractTextEditor);
    return d->text.data();
}

//! \brief Gets editor's word engine.
Logic::AbstractWordEngine * AbstractTextEditor::wordEngine() const
{
    Q_D(const AbstractTextEditor);
    return d->word_engine.data();
}

//! \brief Reacts to key press.
//! \param key Pressed key.
//!
//! For now it only checks whether backspace was pressed. In such case
//! preedit is commited and primary candidate is cleared.
void AbstractTextEditor::onKeyPressed(const Key &key)
{
    Q_D(AbstractTextEditor);

    if (not d->valid()) {
        return;
    }

    if (key.action() == Key::ActionBackspace) {
        d->backspace_sent = false;
        d->auto_repeat_backspace_timer.start(d->options.backspace_auto_repeat_delay);
        d->deleted_words = 0;
    }
}

//! \brief Reacts to key release.
//! \param key Released key.
//!
//! If common key is pressed then it is appended to preedit.  If
//! backspace was pressed then preedit is commited and a character
//! before cursor is removed. If space is pressed then primary
//! candidate is applied if enabled. In other cases standard behaviour
//! applies.
void AbstractTextEditor::onKeyReleased(const Key &key)
{
    Q_D(AbstractTextEditor);

    if (not d->valid()) {
        return;
    }

    const QString text = key.label();
    QString keyText = QString();
    Qt::Key event_key = Qt::Key_unknown;
    bool look_for_a_double_space = d->look_for_a_double_space;
    bool look_for_a_triple_space = d->look_for_a_triple_space;
    bool email_detected = false;

    // Detect if the user is entering an email address and avoid spacing, autocaps and autocomplete changes
    QString textOnLeft = d->text->surroundingLeft() + d->text->preedit();
    if (key.action() == Key::ActionBackspace) {
        textOnLeft.chop(1);
    }
    QStringList leftHandWords = textOnLeft.split(QStringLiteral(" "));
    if (!d->word_engine->languageFeature()->alwaysShowSuggestions() && !leftHandWords.isEmpty() && leftHandWords.last().contains(QLatin1String("@"))) {
        email_detected = true;
    }

    // we reset the flags here so that we won't have to add boilerplate code later
    if (d->look_for_a_triple_space) {
        d->look_for_a_triple_space = false;
    }
    if (look_for_a_double_space) {
        d->look_for_a_double_space = false;
    }

    switch(key.action()) {
    case Key::ActionInsert: {
        bool alreadyAppended = false;
        bool auto_caps_activated = false;
        const bool isSeparator = d->word_engine->languageFeature()->isSeparator(text);
        const bool isSymbol = d->word_engine->languageFeature()->isSymbol(text);
        const bool replace_preedit = d->auto_correct_enabled && not d->text->primaryCandidate().isEmpty() &&
                    not d->text->preedit().isEmpty() && isSeparator;
        const bool enablePreeditAtInsertion = d->word_engine->languageFeature()->enablePreeditAtInsertion();

        d->previous_preedit = QString();

        if (d->preedit_enabled) {
            if (!enablePreeditAtInsertion &&
                    (d->text->surroundingRight().left(1).contains(QRegularExpression(R"([\w])")) || email_detected)) {
                // We're editing in the middle of a word or entering an email address, so just insert characters directly
                d->text->appendToPreedit(text);
                commitPreedit();
                alreadyAppended = true;
            } else if (replace_preedit) {
                // this means we should commit the candidate, add the separator and whitespace
                d->text->setPreedit(d->text->primaryCandidate());
                d->text->appendToPreedit(text);
                commitPreedit();
                if (!email_detected) {
                    auto_caps_activated = d->word_engine->languageFeature()->activateAutoCaps(d->text->surroundingLeft() + d->text->preedit() + text);
                }
                alreadyAppended = true;
            }
            else if (d->auto_correct_enabled && (isSeparator || isSymbol)) {
                if(isSeparator && d->keyboardState == QLatin1String("CHARACTERS") && !email_detected) {
                    // remove all whitespaces before the separator, then add a whitespace after it
                    removeTrailingWhitespaces();
                }

                d->text->appendToPreedit(text);
                if (!email_detected) {
                    auto_caps_activated = d->word_engine->languageFeature()->activateAutoCaps(d->text->surroundingLeft() + d->text->preedit());
                }
                commitPreedit();
                alreadyAppended = true;
            }
            else if (d->keyboardState == QLatin1String("EMOJI")) {
                d->text->appendToPreedit(text);
                commitPreedit();
                alreadyAppended = true;
            }
        }

        // if we had modified the preedit already because of a separator entry, there is no need to perform all the
        // steps like appending the input or computing candidates - as all we needed was already done in the previous part
        if (not alreadyAppended) {
            d->text->appendToPreedit(text);

            // computeCandidates can change preedit face, so needs to happen
            // before sending preedit:
            if (d->preedit_enabled) {
                d->word_engine->computeCandidates(d->text.data());
            }

            if (!d->word_engine->languageFeature()->showPrimaryInPreedit() && d->preedit_enabled) {
                sendPreeditString(d->text->preedit(), d->text->preeditFace(),
                                  Replacement(d->text->cursorPosition()));
            }
        }

        if (not d->preedit_enabled) {
            commitPreedit();
        }
        else if (auto_caps_activated && d->auto_caps_enabled) {
            // standard input (like certain separators, e.g. '.' in western languages) can also trigger autoCaps
            Q_EMIT autoCapsActivated();
        }

        Q_EMIT preeditChanged(d->text->preedit());
        Q_EMIT cursorPositionChanged(d->text->cursorPosition());
    } break;

    case Key::ActionBackspace: {
        if (not d->backspace_sent) {
            bool uncommittedDelete = d->text->preedit().isEmpty();
            singleBackspace();
            if (!email_detected) {
                checkPreeditReentry(uncommittedDelete);
            }
        } else if (!email_detected) {
            checkPreeditReentry(false);
        }

        d->auto_repeat_backspace_timer.stop();
        d->repeating_backspace = false;
        d->backspace_acceleration = 0;
    } break;

    case Key::ActionSpace: {
        QString space = QStringLiteral(" ");
        QString textOnLeft = d->text->surroundingLeft() + d->text->preedit();
        QString textOnLeftTrimmed = textOnLeft.trimmed();
        QStringList textOnRightList = d->text->surroundingRight().split(QStringLiteral("\n"));
        QString textOnRight;
        if (!textOnRightList.isEmpty()) {
            textOnRight = textOnRightList.first().trimmed();
        }
        bool auto_caps_activated = d->word_engine->languageFeature()->activateAutoCaps(textOnLeft);
        const bool replace_preedit = d->auto_correct_enabled
                                     && not d->text->primaryCandidate().isEmpty()
                                     && not d->text->preedit().isEmpty()
                                     && d->word_engine->languageFeature()->commitOnSpace();
        const QString stopSequence = d->word_engine->languageFeature()->fullStopSequence();
        bool full_stop_inserted = false;

        // every double-space character inputs one-after-another force a full-stop, so trigger it if needed
        if (d->double_space_full_stop_enabled && not look_for_a_double_space) {
            // Only enable if the language plugin inserts spaces after word completion (e.g. pinyin doesn't)
            if (d->text->preedit().isEmpty() || d->word_engine->languageFeature()->appendixForReplacedPreedit(d->text->preedit()) == QLatin1String(" ")) {
                d->look_for_a_double_space = true;
            }
        }

        // Delete automatically inserted full stop if the user continues
        // pressing space after a double space.
        if (look_for_a_triple_space) {
            singleBackspace();
            singleBackspace();
            d->text->appendToPreedit(QStringLiteral("  "));
        }

        if (replace_preedit) {
            if (!textOnRight.isEmpty() && d->editing_middle_of_text) {
                // Don't insert a space if we are correcting a word in the middle of a sentence
                space = QString();
                d->look_for_a_double_space = false;
                d->editing_middle_of_text = false;
            } else {
                space = d->appendix_for_previous_preedit = d->word_engine->languageFeature()->appendixForReplacedPreedit(d->text->preedit());
            }
            d->previous_preedit = d->text->preedit();
            d->previous_preedit_position = d->text->surroundingOffset();
            d->text->setPreedit(d->text->primaryCandidate());
            if (d->previous_preedit.right(1) == QLatin1String("'") && d->text->preedit().right(1) != QLatin1String("'")) {
                // If the user has added an apostrophe to the end of the word
                // we should preserve this, as it may be user as a single quote
                // or the plural form of certain names (but would otherwise be
                // replaced by auto-correct as it's treated as a normal
                // character for use inside words).
                d->text->setPreedit(d->text->preedit() + "'");
                d->previous_preedit_position -= 1;
            }
            if (d->previous_preedit.left(1) == QLatin1String("'") &&  d->text->preedit().left(1) != QLatin1String("'")) {
                // Same for apostrophes at the beginning of the word
                d->text->setPreedit("'" + d->text->preedit());
                d->previous_preedit_position -= 1;
            }
        }
        // Only insert a full stop after double space if there isn't already
        // a separator, and there isn't a separator immediately prior to a ')'
        else if (look_for_a_double_space
                 && not stopSequence.isEmpty()
                 && textOnLeft.size() >= 2
                 && textOnLeft.at(textOnLeft.size() - 1).isSpace()
                 && !textOnLeft.at(textOnLeft.size() - 2).isSpace()
                 && textOnLeftTrimmed.size() > 0
                 && !d->word_engine->languageFeature()->isSeparator(textOnLeftTrimmed.at(textOnLeftTrimmed.size() - 1))
                 && !(textOnLeftTrimmed.endsWith(QLatin1String(")"))
                      && textOnLeftTrimmed.size() > 1
                      && d->word_engine->languageFeature()->isSeparator(textOnLeftTrimmed.at(textOnLeftTrimmed.size() - 2)))) {
            removeTrailingWhitespaces();
            if (!d->word_engine->languageFeature()->commitOnSpace()) {
                // Commit when inserting a fullstop if we don't insert on spaces
                d->previous_preedit = d->text->preedit();
                d->previous_preedit_position = d->text->surroundingOffset();
                d->text->setPreedit(d->text->primaryCandidate().trimmed());
                d->text->appendToPreedit(stopSequence);
                commitPreedit();
            } else {
                d->text->appendToPreedit(stopSequence);
            }

            // we need to re-evaluate autocaps after our changes to the preedit
            textOnLeft = d->text->surroundingLeft() + d->text->preedit();
            auto_caps_activated = d->word_engine->languageFeature()->activateAutoCaps(textOnLeft);
            full_stop_inserted = true;
            d->look_for_a_triple_space = true;
        }

        d->text->appendToPreedit(space);

        if (d->word_engine->languageFeature()->commitOnSpace() || full_stop_inserted) {
            commitPreedit();
        } else if (d->preedit_enabled) {
            d->word_engine->computeCandidates(d->text.data());

            sendPreeditString(d->text->preedit(), d->text->preeditFace(),
                              Replacement(d->text->cursorPosition()));
        }

        Q_EMIT preeditChanged(d->text->preedit());
        Q_EMIT cursorPositionChanged(d->text->cursorPosition());

        if (auto_caps_activated && d->auto_caps_enabled) {
            Q_EMIT autoCapsActivated();
        }
    } break;

    case Key::ActionReturn: {
        event_key = Qt::Key_Return;
        keyText = QStringLiteral("\r");
        if (d->word_engine->languageFeature()->showPrimaryInPreedit()) {
            d->text->setPreedit(d->text->primaryCandidate());
            commitPreedit();
        }
    } break;

    case Key::ActionCommit: {
        commitPreedit();

        Q_EMIT preeditChanged(d->text->preedit());
        Q_EMIT cursorPositionChanged(d->text->cursorPosition());
    } break;

    case Key::ActionClose:
        Q_EMIT keyboardClosed();
        break;

    case Key::ActionLeft:
        event_key = Qt::Key_Left;
        break;

    case Key::ActionUp:
        event_key = Qt::Key_Up;
        break;

    case Key::ActionRight:
        event_key = Qt::Key_Right;
        break;

    case Key::ActionDown:
        event_key = Qt::Key_Down;
        break;
        
    case Key::ActionKeySequence:
        sendKeySequence(text, QKeySequence::fromString(key.commandSequence()));
        break;
        
    case Key::ActionCommand:
        invokeAction(text, QKeySequence::fromString(key.commandSequence()));
        break;

    case Key::ActionLeftLayout:
        Q_EMIT leftLayoutSelected();
        break;

    case Key::ActionRightLayout:
        Q_EMIT rightLayoutSelected();
        break;

    case Key::ActionHome:
        event_key = Qt::Key_Home;
        break;

    case Key::ActionEnd:
        event_key = Qt::Key_End;
        break;

    default:
        break;
    }

    if (event_key != Qt::Key_unknown) {
        commitPreedit();
        sendKeyPressAndReleaseEvents(event_key, Qt::NoModifier, keyText);

        Q_EMIT preeditChanged(d->text->preedit());
        Q_EMIT cursorPositionChanged(d->text->cursorPosition());
    }
}

//! \brief Reacts to sliding into a key.
//! \param key Slid in key.
//!
//! For now it only set backspace repeat timer if we slide into
//! backspace.
void AbstractTextEditor::onKeyEntered(const Key &key)
{
    Q_D(AbstractTextEditor);

    if (key.action() == Key::ActionBackspace) {
        d->backspace_sent = false;
        d->auto_repeat_backspace_timer.start(d->options.backspace_auto_repeat_delay);
    }
}

//! \brief Reacts to sliding out of a key.
//! \param key Slid out key.
//!
//! For now it only stops backspace repeat timer if we slide out
//! from backspace.
void AbstractTextEditor::onKeyExited(const Key &key)
{
    Q_D(AbstractTextEditor);

    if (key.action() == Key::ActionBackspace) {
        d->auto_repeat_backspace_timer.stop();
        d->repeating_backspace = false;
        d->word_engine->computeCandidates(d->text.data());
    }
}

//! \brief Replaces current preedit with given replacement
//! \param replacement New preedit.
void AbstractTextEditor::replacePreedit(const QString &replacement)
{
    Q_D(AbstractTextEditor);

    if (not d->valid()) {
        return;
    }

    if (replacement == d->text->preedit()) {
        return;
    }

    d->text->setPreedit(replacement);
    // computeCandidates can change preedit face, so needs to happen
    // before sending preedit:
    d->word_engine->computeCandidates(d->text.data());
    sendPreeditString(d->text->preedit(), d->text->preeditFace());

    Q_EMIT preeditChanged(d->text->preedit());
    Q_EMIT cursorPositionChanged(d->text->cursorPosition());
}

void AbstractTextEditor::replaceTextWithPreedit(const QString &replacement, int start, int len, int pos)
{
    Q_D(AbstractTextEditor);

    if (not d->valid()) {
        return;
    }

    d->text->setPreedit(replacement);
    replacePreedit(replacement);
    Replacement word_r(start, len, pos);
    sendPreeditString(d->text->preedit(), d->text->preeditFace(),
                      word_r);

    Q_EMIT preeditChanged(d->text->preedit());
    Q_EMIT cursorPositionChanged(d->text->cursorPosition());
}

void AbstractTextEditor::onWordCandidateSelected(const QString &word)
{
    Q_D(AbstractTextEditor);

    if (not d->valid()) {
        return;
    }

    if (d->word_engine->languageFeature()->shouldDelayCandidateCommit()) {
        return;
    }

    replaceAndCommitPreedit(word);
}

//! \brief Replaces current preedit with given replacement and then
//! commits it.
//! \param replacement New preedit string to commit.
void AbstractTextEditor::replaceAndCommitPreedit(const QString &replacement)
{
    Q_D(AbstractTextEditor);

    if (not d->valid()) {
        return;
    }

    d->text->setPreedit(replacement);
    const bool auto_caps_activated = d->word_engine->languageFeature()->activateAutoCaps(d->text->preedit());
    d->appendix_for_previous_preedit = d->word_engine->languageFeature()->appendixForReplacedPreedit(d->text->preedit());
    if (d->auto_correct_enabled) {
        if ((!d->text->surroundingRight().trimmed().isEmpty() && d->editing_middle_of_text) || d->word_engine->languageFeature()->contentType() == Maliit::UrlContentType) {
            // Don't insert a space if we are correcting a word in the middle of a sentence or if we're in a Url field
            d->appendix_for_previous_preedit = QString();
            d->editing_middle_of_text = false;
        }
        d->text->appendToPreedit(d->appendix_for_previous_preedit);
    }
    commitPreedit();

    if (d->auto_caps_enabled) {
        if (auto_caps_activated) {
            Q_EMIT autoCapsActivated();
        } else {
            Q_EMIT autoCapsDeactivated();
        }
    }

    Q_EMIT preeditChanged(d->text->preedit());
    Q_EMIT cursorPositionChanged(d->text->cursorPosition());
}

//! \brief Clears preedit.
void AbstractTextEditor::clearPreedit()
{
    Q_D(AbstractTextEditor);

    replacePreedit(QString());
    text()->setSurrounding(QString());
    text()->setSurroundingOffset(0);

    if (not d->valid()) {
        return;
    }

    qDebug() << "in clear preedit.. clearing word engine";
    d->word_engine->clearCandidates();
}

//! \brief Returns whether preedit functionality is enabled.
//! \sa preeditEnabled
bool AbstractTextEditor::isPreeditEnabled() const
{
    Q_D(const AbstractTextEditor);
    return d->preedit_enabled;
}

//! \brief Sets whether enable preedit functionality.
//! \param enabled \c true to enable preedit functionality.
//! \sa preeditEnabled
void AbstractTextEditor::setPreeditEnabled(bool enabled)
{
    Q_D(AbstractTextEditor);

    if (d->preedit_enabled != enabled) {
        d->preedit_enabled = enabled;
        Q_EMIT preeditEnabledChanged(d->preedit_enabled);
    }
}

//! \brief Set cursor position of preedit.
void AbstractTextEditor::setCursorPosition(int pos)
{
    Q_D(AbstractTextEditor);

    if (not d->valid()) {
        return;
    }

    if (pos == d->text->cursorPosition()) {
        return;
    }

    d->text->setCursorPosition(pos);

    sendPreeditString(d->text->preedit(), d->text->preeditFace(),
                      Replacement(d->text->cursorPosition()));
    Q_EMIT preeditChanged(d->text->preedit());
    Q_EMIT cursorPositionChanged(d->text->cursorPosition());
}

//! \brief Returns whether auto-correct functionality is enabled.
//! \sa autoCorrectEnabled
bool AbstractTextEditor::isAutoCorrectEnabled() const
{
    Q_D(const AbstractTextEditor);
    return d->auto_correct_enabled;
}

//! \brief Sets whether enable the auto-correct functionality.
//! \param enabled \c true to enable auto-correct functionality.
//! \sa autoCorrectEnabled
void AbstractTextEditor::setAutoCorrectEnabled(bool enabled)
{
    Q_D(AbstractTextEditor);

    if (d->auto_correct_enabled != enabled) {
        d->auto_correct_enabled = enabled;
        Q_EMIT autoCorrectEnabledChanged(d->auto_correct_enabled);
    }
}

bool AbstractTextEditor::isAutoCapsEnabled() const
{
    Q_D(const AbstractTextEditor);
    return d->auto_caps_enabled;
}

void AbstractTextEditor::setAutoCapsEnabled(bool enabled)
{
    Q_D(AbstractTextEditor);

    if (d->auto_caps_enabled != enabled) {
        d->auto_caps_enabled = enabled;
        Q_EMIT autoCapsEnabledChanged(d->auto_caps_enabled);
    }
}

//! \brief Returns whether double space full-stop is enabled
//! \sa doubleSpaceFullStopEnabled
bool AbstractTextEditor::isDoubleSpaceFullStopEnabled() const
{
    Q_D(const AbstractTextEditor);
    return d->double_space_full_stop_enabled;
}

//! \brief Sets whether the double space full-stop functionality is enabled.
//! \param enabled \c true to enable double space full-stop functionality.
//! \sa doubleSpaceFullStopEnabled
void AbstractTextEditor::setDoubleSpaceFullStopEnabled(bool enabled)
{
    Q_D(AbstractTextEditor);

    if (d->double_space_full_stop_enabled != enabled) {
        d->double_space_full_stop_enabled = enabled;
        Q_EMIT doubleSpaceFullStopEnabledChanged(d->double_space_full_stop_enabled);
    }
}

//! \brief Commits current preedit.
void AbstractTextEditor::commitPreedit()
{
    Q_D(AbstractTextEditor);

    if (not d->valid() || d->text->preedit().isEmpty()) {
        return;
    }

    sendCommitString(d->text->preedit());
    d->text->commitPreedit();
    d->word_engine->clearCandidates();
}

void AbstractTextEditor::removeTrailingWhitespaces()
{
    Q_D(AbstractTextEditor);

    const QString textOnLeft = d->text->surroundingLeft() + d->text->preedit();

    QString::const_iterator begin = textOnLeft.cbegin();
    QString::const_iterator i = textOnLeft.cend();
    while (i != begin) {
        --i;
        if (!i->isSpace()) break;
        singleBackspace();
    }
}

// TODO: this implementation does not take into account following features:
// 1) preedit string
//      if there is preedit then first call to autoRepeatBackspace should clean it completely
//      and following calls should remove remaining text character by character
// 2) multitouch
//      it is not completely clean how to handle multitouch for backspace,
//      but we can follow the strategy from meego-keyboard - release pressed
//      key when user press another one at the same time. Then we do not need to
//      change anything in this method
//! \brief Sends backspace and sets backspace repeat timer.
void AbstractTextEditor::autoRepeatBackspace()
{
    Q_D(AbstractTextEditor);

    d->repeating_backspace = true;

    if (d->deleted_words < d->options.backspace_word_switch_threshold) {
        singleBackspace();

        // Gradually speed up single letter deletion
        if(d->options.backspace_auto_repeat_interval - d->backspace_acceleration > d->options.backspace_auto_repeat_min_interval) {
            d->backspace_acceleration += d->options.backspace_auto_repeat_acceleration_rate;
        }
        d->auto_repeat_backspace_timer.start(d->options.backspace_auto_repeat_interval - d->backspace_acceleration);
        d->backspace_word_acceleration = 0;
    } else {
        autoRepeatWordBackspace();
        d->backspace_acceleration = 0;
    }
}

/*!
 * \brief AbstractTextEditor::autoRepeatWordBackspace same as autoRepeatBackspace()
 * but deletes whole words
 */
void AbstractTextEditor::autoRepeatWordBackspace()
{
    Q_D(AbstractTextEditor);

    if (d->text->surroundingOffset() > 0) {
        QString word = wordLeftOfCursor();
        for (int i=0; i<word.length(); ++i)
            singleBackspace();
    } else {
        singleBackspace();
    }

    // Gradually speed up deletion to allow for deleting large blocks of text
    if (d->options.backspace_word_interval - d->backspace_word_acceleration > d->options.backspace_word_min_interval) {
        d->backspace_word_acceleration += d->options.backspace_word_acceleration_rate;
    }

    d->auto_repeat_backspace_timer.start(d->options.backspace_word_interval - d->backspace_word_acceleration);
}

/*!
 * \brief AbstractTextEditor::wordLeftOfCursor returns the word that is left to
 * to the cursor
 * \return
 */
QString AbstractTextEditor::wordLeftOfCursor() const
{
    Q_D(const AbstractTextEditor);

    const QString leftSurrounding = d->text->surroundingLeft();
    int idx = leftSurrounding.length() - 1;
    while (idx >= 0 && !d->word_engine->languageFeature()->isSeparator(leftSurrounding.at(idx))) {
        --idx;
    }
    int length = d->text->surroundingOffset() - idx;

    return leftSurrounding.right(length);
}

//! \brief Adds \a word to user dictionary.
//! \param word Word to be added.
void AbstractTextEditor::addToUserDictionary(const QString &word)
{
    Q_D(AbstractTextEditor);

    d->word_engine->addToUserDictionary(word);
    d->text->setPrimaryCandidate(word);

    Q_EMIT wordCandidatesChanged(WordCandidateList());
}

//! \brief Sends preedit string to application with no replacement.
//! \param preedit Preedit to send.
//! \param face Face of the preedit.
void AbstractTextEditor::sendPreeditString(const QString &preedit,
                                           Model::Text::PreeditFace face)
{
    sendPreeditString(preedit, face, Replacement());
}

//! \brief AbstractTextEditor::singleBackspace deletes one charater at he current
//! cursor position.
void AbstractTextEditor::singleBackspace()
{
    Q_D(AbstractTextEditor);
    bool in_word = false;
    QString textOnLeft = d->text->surroundingLeft();

    if (d->text->preedit().isEmpty()) {
        in_word = textOnLeft.right(1) != QLatin1String(" ");
        sendKeyPressAndReleaseEvents(Qt::Key_Backspace, Qt::NoModifier);
        // Deletion of surrounding text isn't updated in the model until later
        // Update it locally here for autocaps detection
        textOnLeft.chop(1);
    } else {
        in_word = true;
        d->text->removeFromPreedit(1);
        textOnLeft += d->text->preedit();

        // Clear previous word candidates
        Q_EMIT wordCandidatesChanged(WordCandidateList());
        sendPreeditString(d->text->preedit(), d->text->preeditFace(),
                          Replacement(d->text->cursorPosition()));

        Q_EMIT preeditChanged(d->text->preedit());
        Q_EMIT cursorPositionChanged(d->text->cursorPosition());

        if (d->text->preedit().isEmpty()) {
            d->word_engine->clearCandidates();
            d->text->commitPreedit();
            // XXX: This is something like a workaround for maliit reporting an invalid state to Qt.
            //  When preedit is cleared, for Qt not reporting it as inputMethodComposing all the time we need
            //  to flush out all the TextFormat attributes - so we actually need to commit anything for that
            //  to happen
            sendCommitString(QString());
        }
    }

    if (in_word && textOnLeft.right(1) == QLatin1String(" ")) {
        // We were in a word, but now we're not, so we've just finished deleting a word
        d->deleted_words++;
    }

    textOnLeft = textOnLeft.trimmed();

    const bool auto_caps_activated = d->word_engine->languageFeature()->activateAutoCaps(textOnLeft);
    if (d->auto_caps_enabled) {
        if (auto_caps_activated) {
            Q_EMIT autoCapsActivated();
        } else if(!textOnLeft.isEmpty()) {
            Q_EMIT autoCapsDeactivated();
        }
    }

    if(!d->text->surroundingRight().trimmed().isEmpty()) {
        d->editing_middle_of_text = true;
    }
    d->backspace_sent = true;
}

void AbstractTextEditor::onKeyboardStateChanged(QString state) {
    Q_D(AbstractTextEditor);

    d->keyboardState = state;
}

void AbstractTextEditor::sendKeySequence(const QString &action, const QKeySequence &sequence) {

    static const Qt::KeyboardModifiers AllModifiers = Qt::ShiftModifier | Qt::ControlModifier | Qt::AltModifier
            | Qt::MetaModifier | Qt::KeypadModifier;

    QKeySequence actionSequence;

    if (action == "Copy") {
        actionSequence = QKeySequence::Copy;
    }
    else if (action == "Paste") {
        actionSequence = QKeySequence::Paste;
    }
    else if (action == "Cut") {
        actionSequence = QKeySequence::Cut;
    }
    else if (action == "Tab") {
        actionSequence = QKeySequence::AddTab;
    }
    else if (action == "Redo") {
        actionSequence = QKeySequence::Redo;
    }
    else if (action == "Undo") {
        actionSequence = QKeySequence::Undo;
    }
    else if (action == "SelectAll") {
        actionSequence = QKeySequence::SelectAll;
    }
    else if (action == "SelectNextChar") {
        actionSequence = QKeySequence::SelectNextChar;
    }
    else if (action == "SelectPreviousChar") {
        actionSequence = QKeySequence::SelectPreviousChar;
    }
    else if (action == "SelectNextLine") {
        actionSequence = QKeySequence::SelectNextLine;
    }
    else if (action == "SelectPreviousLine") {
        actionSequence = QKeySequence::SelectPreviousLine;
    }
    else if (action == "SelectPreviousWord") {
        actionSequence = QKeySequence::SelectPreviousWord;
    }
    else if (action == "SelectNextWord") {
        actionSequence = QKeySequence::SelectNextWord;
    }
    else if (action == "SelectStartOfLine") {
        actionSequence = QKeySequence::SelectStartOfLine;
    }
    else if (action == "SelectEndOfLine") {
        actionSequence = QKeySequence::SelectEndOfLine;
    }
    else if (action == "SelectStartOfDocument") {
        actionSequence = QKeySequence::SelectStartOfDocument;
    }
    else if (action == "SelectEndOfDocument") {
        actionSequence = QKeySequence::SelectEndOfDocument;
    }
    else if (action == "MoveToNextChar") {
        actionSequence = QKeySequence::MoveToNextChar;
    }
    else if (action == "MoveToPreviousChar") {
        actionSequence = QKeySequence::MoveToPreviousChar;
    }
    else if (action == "MoveToPreviousWord") {
        actionSequence = QKeySequence::MoveToPreviousWord;
    }
    else if (action == "MoveToNextWord") {
        actionSequence = QKeySequence::MoveToNextWord;
    }
    else if (action == "MoveToStartOfLine") {
        actionSequence = QKeySequence::MoveToStartOfLine;
    }
    else if (action == "MoveToEndOfLine") {
        actionSequence = QKeySequence::MoveToEndOfLine;
    }
    else if (action == "MoveToStartOfDocument") {
        actionSequence = QKeySequence::MoveToStartOfDocument;
    }
    else if (action == "MoveToEndOfDocument") {
        actionSequence = QKeySequence::MoveToEndOfDocument;
    }else{
        actionSequence = QKeySequence::UnknownKey;
    }

    if (actionSequence == QKeySequence::UnknownKey) {
        actionSequence = sequence;
    }

    for (int i = 0; i < actionSequence.count(); i++) {
        const int key = actionSequence[i] & ~AllModifiers;
        const int modifiers = actionSequence[i] & AllModifiers;
        QString text("");
        if (modifiers == Qt::NoModifier || modifiers == Qt::ShiftModifier) {
            text = QString::number(key);
        }
        sendKeyPressAndReleaseEvents(key, static_cast<Qt::KeyboardModifiers>(modifiers), text);
    }
}

void AbstractTextEditor::sendKeyPressAndReleaseEvents(
    int key, Qt::KeyboardModifiers modifiers, const QString& text) {
    QKeyEvent press(QEvent::KeyPress, key, modifiers, text);
    sendKeyEvent(press);
    QKeyEvent release(QEvent::KeyRelease, key, modifiers, text);
    sendKeyEvent(release);
}

void AbstractTextEditor::setPreeditFace(Model::Text::PreeditFace face)
{
    Q_D(AbstractTextEditor);

    text()->setPreeditFace(face);
    sendPreeditString(d->text->preedit(), face);
}

void AbstractTextEditor::setPrimaryCandidate(QString candidate)
{
    Q_D(AbstractTextEditor);
    text()->setPrimaryCandidate(candidate);

    if (d->word_engine->languageFeature()->showPrimaryInPreedit()) {
        sendPreeditString(candidate, d->text->preeditFace());
    }
}

//! \brief AbstractTextEditor::checkPreeditReentry  Checks to see whether we should
//! place a word back in to pre-edit after a character has been deleted
void AbstractTextEditor::checkPreeditReentry(bool uncommittedDelete)
{
    Q_D(AbstractTextEditor);

    if(!isPreeditEnabled() || m_hasSelection) {
        return;
    }

    if(!text()->preedit().isEmpty()) {
        d->word_engine->computeCandidates(d->text.data());
        return;
    }

    if(!d->word_engine->languageFeature()->restorePreedit()) {
        return;
    }

    int currentOffset = text()->surroundingOffset();
    if(currentOffset > 1 && currentOffset <= text()->surrounding().size()) {
        QString lastChar;
        if(uncommittedDelete) {
            // -2 for just deleted character that hasn't been committed and to reach character before cursor
            lastChar = text()->surrounding().at(currentOffset-2);
        } else {
            lastChar = text()->surrounding().at(currentOffset-1);
        }
        if (!QRegularExpression(R"(\W+)").match(lastChar).hasMatch() &&
            !d->word_engine->languageFeature()->isSymbol(lastChar)) {
            QStringList leftWords = text()->surroundingLeft().trimmed().split(QRegularExpression (R"([\s\d]+)"));
            int trimDiff = text()->surroundingLeft().size() - text()->surroundingLeft().trimmed().size();
            if(leftWords.last().isEmpty()) {
                // If removed char was punctuation trimming will result in an empty entry
                leftWords.removeLast();
                trimDiff += 1;
            }
            if(d->text->surroundingRight().left(1).contains(QRegularExpression (R"([\w])"))) {
                // Don't enter pre-edit in the middle of a word
                return;
            }
            QString recreatedPreedit = leftWords.last();
            if(trimDiff == 0 && uncommittedDelete) {
                // Remove the last character from the word if we weren't just deleting a space
                // as the last backspace hasn't been committed yet.
                recreatedPreedit.chop(1);
            }

            for(int i = 0; i < recreatedPreedit.size(); i++) {
                singleBackspace();
            }

            if (!d->previous_preedit.isEmpty()) {
                int deletePos = d->text->surroundingOffset() - d->previous_preedit_position - recreatedPreedit.size();
                if (deletePos == 0 || deletePos == 1) {
                    recreatedPreedit = d->previous_preedit;
                    text()->setRestoredPreedit(true);
                }
                d->previous_preedit = QString();
            }
            replaceTextWithPreedit(recreatedPreedit, 0, 0, recreatedPreedit.size());
        }
    }

    d->word_engine->computeCandidates(d->text.data());
}

void AbstractTextEditor::onHasSelectionChanged(bool hasSelection) {
    m_hasSelection = hasSelection;
}

} // namespace MaliitKeyboard

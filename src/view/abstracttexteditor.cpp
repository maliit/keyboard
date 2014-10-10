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

namespace {

//! \brief Checks whether given \a c is a word separator.
//! \param c Char to test.
//!
//! Other way to do checks would be using isLetterOrNumber() + some
//! other methods. But UTF is so crazy that I am not sure whether
//! other strange categories are parts of the word or not. It is
//! easier to specify punctuations and whitespaces.
inline bool isSeparator(const QChar &c)
{
    return (c.isPunct() or c.isSpace());
}

//! \brief Extracts a word boundaries at cursor position.
//! \param surrounding_text Text from which extraction will happen.
//! \param cursor_position Position of cursor within \a surrounding_text.
//! \param replacement Place where replacement data will be stored.
//!
//! \return whether surrounding text was valid (not empty).
//!
//! If cursor is placed right after the word, boundaries of this word
//! are extracted.  Otherwise if cursor is placed right before the
//! word, then no word boundaries are stored - instead invalid
//! replacement is stored. It might happen that cursor position is
//! outside the string, so \a replacement will have fixed position.
bool extractWordBoundariesAtCursor(const QString& surrounding_text,
                                   int cursor_position,
                                   AbstractTextEditor::Replacement *replacement)
{
    const int text_length(surrounding_text.length());

    if (text_length == 0) {
        return false;
    }

    // just in case - if cursor is far after last char in surrounding
    // text we place it right after last char.
    cursor_position = qBound(0, cursor_position, text_length);

    // cursor might be placed in after last char (that is to say - its
    // index might be the one of string terminator) - for simplifying
    // the algorithm below we fake it as cursor is put on delimiter:
    // "abc" - surrounding text
    //     | - cursor placement
    // "abc " - fake surrounding text
    const QString fake_surrounding_text(surrounding_text + " ");
    const QChar *const fake_data(fake_surrounding_text.constData());
    // begin is index of first char in a word
    int begin(-1);
    // end is index of a char after last char in a word.
    // -2, because -2 - (-1) = -1 and we would like to
    // have -1 as invalid length.
    int end(-2);

    for (int iter(cursor_position); iter >= 0; --iter) {
        const QChar &c(fake_data[iter]);

        if (isSeparator(c)) {
            if (iter != cursor_position) {
                break;
            }
        } else {
            begin = iter;
        }
    }

    if (begin >= 0) {
        // take note that fake_data's last QChar is always a space.
        for (int iter(cursor_position); iter <= text_length; ++iter) {
            const QChar &c(fake_data[iter]);

            end = iter;
            if (isSeparator(c)) {
                break;
            }
        }
    }

    if (replacement) {
        replacement->start = begin;
        replacement->length = end - begin;
        replacement->cursor_position = cursor_position;
    }

    return true;
}

} // unnamed namespace

EditorOptions::EditorOptions()
    : backspace_auto_repeat_delay(500)
    , backspace_auto_repeat_interval(200)
    , backspace_word_delay(3000)
    , backspace_word_interval(400)
    , backspace_word_acceleration_rate(10)
    , backspace_word_min_interval(50)
{}

class AbstractTextEditorPrivate
{
public:
    QTimer auto_repeat_backspace_timer;
    QElapsedTimer backspace_hold_timer;
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
    bool double_space_full_stop_enabled;
    bool editing_middle_of_text;
    QString appendix_for_previous_preedit;
    int backspace_word_acceleration;
    QString keyboardState;

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
    , double_space_full_stop_enabled(false)
    , editing_middle_of_text(false)
    , appendix_for_previous_preedit()
    , backspace_word_acceleration(0)
    , keyboardState("CHARACTERS")
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
    connect(&d_ptr->auto_repeat_backspace_timer, SIGNAL(timeout()),
            this,                                SLOT(autoRepeatBackspace()));

    connect(word_engine, SIGNAL(enabledChanged(bool)),
            this,        SLOT(setPreeditEnabled(bool)));

    connect(word_engine, SIGNAL(candidatesChanged(WordCandidateList)),
            this,        SIGNAL(wordCandidatesChanged(WordCandidateList)));

    connect(word_engine, SIGNAL(preeditFaceChanged(Model::Text::PreeditFace)),
            this,        SLOT(setPreeditFace(Model::Text::PreeditFace)));

    connect(word_engine, SIGNAL(primaryCandidateChanged(QString)),
            this,        SLOT(setPrimaryCandidate(QString)));
    
    connect(this,        SIGNAL(autoCorrectEnabledChanged(bool)),
            word_engine, SLOT(setAutoCorrectEnabled(bool)));

    setPreeditEnabled(word_engine->isEnabled());
}

//! \brief Destructor.
AbstractTextEditor::~AbstractTextEditor()
{}

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
        d->backspace_hold_timer.restart();
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
    QString keyText = QString("");
    Qt::Key event_key = Qt::Key_unknown;
    bool look_for_a_double_space = d->look_for_a_double_space;

    if (look_for_a_double_space) {
        // we reset the flag here so that we won't have to add boilerplate code later
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

        if (d->preedit_enabled) {
            if (d->text->surroundingRight().left(1).contains(QRegExp("[\\w]"))) {
                // We're editing in the middle of a word, so just insert characters directly
                d->text->appendToPreedit(text);
                commitPreedit();
                alreadyAppended = true;
            } else if (replace_preedit) {
                // this means we should commit the candidate, add the separator and whitespace
                d->text->setPreedit(d->text->primaryCandidate());
                d->text->appendToPreedit(text);
                if (d->keyboardState == "CHARACTERS") {
                    d->appendix_for_previous_preedit = d->word_engine->languageFeature()->appendixForReplacedPreedit(d->text->preedit());
                    d->text->appendToPreedit(d->appendix_for_previous_preedit);
                }
                commitPreedit();
                auto_caps_activated = d->word_engine->languageFeature()->activateAutoCaps(d->text->surroundingLeft() + d->text->preedit() + text);
                alreadyAppended = true;
            }
            else if (d->auto_correct_enabled && (isSeparator || isSymbol)) {
                if(isSeparator && d->keyboardState == "CHARACTERS") {
                    // remove all whitespaces before the separator, then add a whitespace after it
                    removeTrailingWhitespaces();
                }

                d->text->appendToPreedit(text);
                auto_caps_activated = d->word_engine->languageFeature()->activateAutoCaps(d->text->surroundingLeft() + d->text->preedit());
                if(isSeparator && d->keyboardState == "CHARACTERS") {
                    d->text->appendToPreedit(d->word_engine->languageFeature()->appendixForReplacedPreedit(d->text->preedit()));
                }
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

            sendPreeditString(d->text->preedit(), d->text->preeditFace(),
                              Replacement(d->text->cursorPosition()));
        }

        if (not d->preedit_enabled) {
            commitPreedit();
        }
        else if (auto_caps_activated && d->auto_caps_enabled) {
            // standard input (like certain separators, e.g. '.' in western languages) can also trigger autoCaps
            Q_EMIT autoCapsActivated();
        }
    } break;

    case Key::ActionBackspace: {
        if (not d->backspace_sent) {
            singleBackspace();
            checkPreeditReentry(true);
        } else {
            checkPreeditReentry(false);
        }

        d->auto_repeat_backspace_timer.stop();
        d->repeating_backspace = false;
    } break;

    case Key::ActionSpace: {
        QString space = " ";
        QString textOnLeft = d->text->surroundingLeft() + d->text->preedit();
        QString textOnRight = d->text->surroundingRight().trimmed();
        bool auto_caps_activated = d->word_engine->languageFeature()->activateAutoCaps(textOnLeft);
        const bool replace_preedit = d->auto_correct_enabled && not d->text->primaryCandidate().isEmpty() && not d->text->preedit().isEmpty();
        const QString stopSequence = d->word_engine->languageFeature()->fullStopSequence();

        // every double-space character inputs one-after-another force a full-stop, so trigger it if needed
        if (d->double_space_full_stop_enabled && not look_for_a_double_space) {
            d->look_for_a_double_space = true;
        }

        if (replace_preedit) {
            if (!textOnRight.isEmpty() && d->editing_middle_of_text) {
                // Don't insert a space if we are correcting a word in the middle of a sentence
                space = "";
                d->look_for_a_double_space = false;
                d->editing_middle_of_text = false;
            } else {
                space = d->appendix_for_previous_preedit = d->word_engine->languageFeature()->appendixForReplacedPreedit(d->text->preedit());
            }
            d->text->setPreedit(d->text->primaryCandidate());
        }
        else if (look_for_a_double_space && not stopSequence.isEmpty()) {
            removeTrailingWhitespaces();
            d->text->appendToPreedit(d->word_engine->languageFeature()->fullStopSequence());

            // we need to re-evaluate autocaps after our changes to the preedit
            textOnLeft = d->text->surroundingLeft() + d->text->preedit();
            auto_caps_activated = d->word_engine->languageFeature()->activateAutoCaps(textOnLeft);
        }

        d->text->appendToPreedit(space);
        commitPreedit();

        if (auto_caps_activated && d->auto_caps_enabled) {
            Q_EMIT autoCapsActivated();
        }
    } break;

    case Key::ActionReturn: {
        event_key = Qt::Key_Return;
        keyText = QString("\r");
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

    case Key::ActionCommand:
        invokeAction(text, QKeySequence::fromString(key.commandSequence()));

    case Key::ActionLeftLayout:
        Q_EMIT leftLayoutSelected();
        break;

    case Key::ActionRightLayout:
        Q_EMIT rightLayoutSelected();
        break;

    default:
        break;
    }

    if (event_key != Qt::Key_unknown) {
        commitPreedit();
        sendKeyPressAndReleaseEvents(event_key, Qt::NoModifier, keyText);
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
}

//! \brief Clears preedit.
void AbstractTextEditor::clearPreedit()
{
    Q_D(AbstractTextEditor);

    replacePreedit("");
    text()->setSurrounding("");
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
        if (*i != ' ') break;
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

    if (d->backspace_hold_timer.elapsed() < d->options.backspace_word_delay) {
        singleBackspace();
        d->auto_repeat_backspace_timer.start(d->options.backspace_auto_repeat_interval);
        d->backspace_word_acceleration = 0;
    } else {
        autoRepeatWordBackspace();
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
    while (idx >= 0 && !isSeparator(leftSurrounding.at(idx))) {
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

    QString textOnLeft = d->text->surroundingLeft();

    if (d->text->preedit().isEmpty()) {
        sendKeyPressAndReleaseEvents(Qt::Key_Backspace, Qt::NoModifier);
        // Deletion of surrounding text isn't updated in the model until later
        // Update it locally here for autocaps detection
        textOnLeft.chop(1);
    } else {
        d->text->removeFromPreedit(1);
        textOnLeft += d->text->preedit();
        
        // Clear previous word candidates
        Q_EMIT wordCandidatesChanged(WordCandidateList());
        sendPreeditString(d->text->preedit(), d->text->preeditFace(),
                          Replacement());

        if (d->text->preedit().isEmpty()) {
            d->word_engine->clearCandidates();
            d->text->commitPreedit();
            // XXX: This is something like a workaround for maliit reporting an invalid state to Qt.
            //  When preedit is cleared, for Qt not reporting it as inputMethodComposing all the time we need
            //  to flush out all the TextFormat attributes - so we actually need to commit anything for that
            //  to happen
            sendCommitString("");
        }
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

//! \brief Reacts to cursor position change in application's text
//! field.
//! \param cursor_position new cursor position
//! \param surrounding_text surrounding text of a preedit
//!
//! Extract words with the cursor inside and replaces it with a preedit.
//! This is called preedit activation.
void AbstractTextEditor::onCursorPositionChanged(int cursor_position,
                                                 const QString &surrounding_text)
{
    Q_D(AbstractTextEditor);
    Replacement r;

    if (not extractWordBoundariesAtCursor(surrounding_text, cursor_position, &r)) {
        return;
    }

    if (r.start < 0 or r.length < 0) {
        if (d->ignore_next_surrounding_text == surrounding_text and
            d->ignore_next_cursor_position == cursor_position) {
            d->ignore_next_surrounding_text.clear();
            d->ignore_next_cursor_position = -1;
        } else {
            d->text->setPreedit("");
            d->text->setCursorPosition(0);
        }
    } else {
        const int cursor_pos_relative_word_begin(r.start - r.cursor_position);
        const int word_begin_relative_cursor_pos(r.cursor_position - r.start);
        const QString word(surrounding_text.mid(r.start, r.length));
        Replacement word_r(cursor_pos_relative_word_begin, r.length,
                           word_begin_relative_cursor_pos);

        d->text->setPreedit(word, word_begin_relative_cursor_pos);
        // computeCandidates can change preedit face, so needs to happen
        // before sending preedit:
        d->word_engine->computeCandidates(d->text.data());
        sendPreeditString(d->text->preedit(), d->text->preeditFace(), word_r);
        // Qt is going to send us an event with cursor position places
        // at the beginning of replaced word and surrounding text
        // without the replaced word. We want to ignore it.
        d->ignore_next_cursor_position = r.start;
        d->ignore_next_surrounding_text = QString(surrounding_text).remove(r.start, r.length);
    }
}

void AbstractTextEditor::onKeyboardStateChanged(QString state) {
    Q_D(AbstractTextEditor);

    d->keyboardState = state;
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
    text()->setPrimaryCandidate(candidate);
}

//! \brief AbstractTextEditor::checkPreeditReentry  Checks to see whether we should
//! place a word back in to pre-edit after a character has been deleted or focus
//! has changed
void AbstractTextEditor::checkPreeditReentry(bool uncommittedDelete)
{
    Q_D(AbstractTextEditor);

    if(!isPreeditEnabled()) {
        return;
    }

    if(!text()->preedit().isEmpty()) {
        d->word_engine->computeCandidates(d->text.data());
        return;
    }

    int currentOffset = text()->surroundingOffset();
    if(currentOffset > 1 && !text()->surrounding().isEmpty()) {
        QString lastChar;
        if(uncommittedDelete) {
            // -2 for just deleted character that hasn't been committed and to reach character before cursor
            lastChar = text()->surrounding().at(currentOffset-2);
        } else {
            lastChar = text()->surrounding().at(currentOffset-1);
        }
        if(!QRegExp("\\W+").exactMatch(lastChar) && !d->word_engine->languageFeature()->isSymbol(lastChar)) {
            QStringList leftWords = text()->surroundingLeft().trimmed().split(QRegExp("[\\W\\d]+"));
            int trimDiff = text()->surroundingLeft().size() - text()->surroundingLeft().trimmed().size();
            if(leftWords.last().isEmpty()) {
                // If removed char was punctuation trimming will result in an empty entry
                leftWords.removeLast();
                trimDiff += 1;
            }
            if(d->text->surroundingRight().left(1).contains(QRegExp("[\\w]"))) {
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

            replaceTextWithPreedit(recreatedPreedit, 0, 0, recreatedPreedit.size());
        }

    }

    d->word_engine->computeCandidates(d->text.data());
}


} // namespace MaliitKeyboard

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

#include "spellchecker.h"

#ifdef HAVE_HUNSPELL
#include "hunspell/hunspell.hxx"
#else
class Hunspell
{
public:
    Hunspell(const char *, const char *, const char * = NULL) : encoding("UTF-8") {}
    int add_dic (const char *, const char * = NULL) { return 0; }
    char *get_dic_encoding() { return encoding.data(); }
    int spell(const char *, int * = NULL, char ** = NULL) { return 1; }
    int suggest(char *** lst, const char *) { if (lst) { *lst = NULL; } return 0; }
    void free_list(char ***, int) {}
    int add(const char *) { return 0; }
private:
    // Using QByteArray here instead of just returning "UTF-8" in get_dic_encoding
    // to avoid a following warning:
    // warning: deprecated conversion from string constant to ‘char*’ [-Wwrite-strings]
    QByteArray encoding;
};
#endif

#include <QFile>
#include <QTextStream>
#include <QTextCodec>
#include <QStringList>
#include <QDebug>

namespace MaliitKeyboard {
namespace Logic {

//! \class SpellChecker
//! Checks spelling and suggest words. Currently Spellchecker is
//! implemented by using Hunspell.

struct SpellCheckerPrivate
{
    Hunspell hunspell; //!< The spellchecker backend, Hunspell.
    QTextCodec *codec; //!< Which codec to use.
    bool enabled; //!< Whether the spellchecker is enabled.
    QSet<QString> ignored_words; //!< The words to ignore.
    QString user_dictionary_file;

    SpellCheckerPrivate(const QString &dictionary_path,
                        const QString &user_dictionary);
};


SpellCheckerPrivate::SpellCheckerPrivate(const QString &dictionary_path,
                                         const QString &user_dictionary)
    // XXX: toUtf8? toLatin1? toAscii? toLocal8Bit?
    : hunspell((dictionary_path + ".aff").toUtf8().constData(),
               (dictionary_path + ".dic").toUtf8().constData())
    , codec(QTextCodec::codecForName(hunspell.get_dic_encoding()))
    , enabled(false)
    , ignored_words()
    , user_dictionary_file(user_dictionary)
{
    if (not codec) {
        qWarning () << __PRETTY_FUNCTION__ << ":Could not find codec for" << hunspell.get_dic_encoding() << "- turning off spellchecking and suggesting.";
        return;
    }

    if (not user_dictionary.isEmpty() and QFile::exists(user_dictionary)) {
        QFile file(user_dictionary);
        if (file.open(QFile::ReadOnly)) {
            QTextStream stream(&file);
            while (!stream.atEnd()) {
                hunspell.add(codec->fromUnicode(stream.readLine()));
            }
        }
    }

    enabled = true;
}


SpellChecker::~SpellChecker()
{}


//! \param dictionary_path The directory path to the (system) dictionaries.
//! \param user_dictionary The file path to the user's own dictionary.
SpellChecker::SpellChecker(const QString &dictionary_path,
                           const QString &user_dictionary)
    : d_ptr(new SpellCheckerPrivate(dictionary_path, user_dictionary))
{}


//! \brief Checks whether given word is spelled correctly.
//!
//! Ignored words are treated as having correct spelling. \sa ignoreWord.
//! \param word word to check for spelling.
//! \return \c true if the word has correct spelling (or is ignored),
//!         otherwise \c false.
bool SpellChecker::spell(const QString &word)
{
    Q_D(SpellChecker);

    if (not d->enabled or d->ignored_words.contains(word)) {
        return true;
    }

    return d->hunspell.spell(d->codec->fromUnicode(word));
}


//! \brief Gives suggestions for a given word.
//! \param word Base for suggestions.
//! \param limit Suggestion count limit (-1 for no limits).
//! \return a list of suggestions.
QStringList SpellChecker::suggest(const QString &word,
                                  int limit)
{
    Q_D(SpellChecker);

    if (not d->enabled) {
        return QStringList();
    }

    char** suggestions = NULL;
    const int suggestions_count = d->hunspell.suggest(&suggestions, d->codec->fromUnicode(word));

    // Less than zero means some error.
    if (suggestions_count < 0) {
        qWarning() << __PRETTY_FUNCTION__ << ": Failed to get suggestions for" << word << ".";
        return QStringList();
    }

    QStringList result;
    const int final_limit((limit < 0) ? suggestions_count : qMin(limit, suggestions_count));

    for (int index(0); index < final_limit; ++index) {
        result << d->codec->toUnicode(suggestions[index]);
    }
    d->hunspell.free_list(&suggestions, suggestions_count);
    return result;
}


//! \brief Marks a given word as ignored.
//! \param word The word to ignore - it will not be checked for spelling.
void SpellChecker::ignoreWord(const QString &word)
{
    Q_D(SpellChecker);

    if (not d->enabled) {
        return;
    }

    d->ignored_words.insert(word);
}

//! \brief Adds a given word to user dictionary.
//! \param word The word to be added to user dictionary - it will be used for
//!             spellchecking and suggesting.
void SpellChecker::addToUserWordlist(const QString &word)
{
    Q_D(SpellChecker);

    if (not d->enabled) {
        return;
    }

    QFile user_dictionary(d->user_dictionary_file);
    QDir::home().mkpath(QFileInfo(user_dictionary).absolutePath());
    if (user_dictionary.open(QFile::Append)) {
        QTextStream stream(&user_dictionary);
        stream << word << endl;
    }

    // Non-zero return value means some error.
    if (d->hunspell.add(d->codec->fromUnicode(word))) {
        qWarning() << __PRETTY_FUNCTION__ << ": Failed to add '" << word << "' to user dictionary.";
    }
}

// static
QString SpellChecker::dictPath()
{
    return QString(HUNSPELL_DICT_PATH);
}

}} // namespace Logic, MaliitKeyboard

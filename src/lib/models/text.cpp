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

#include "text.h"

//! \class Text
//! \brief Represents the text state of the editor
//!
//! Both MaliitKeyboard::AbstractTextEditor and
//! MaliitKeyboard::Logic::AbstractWordEngine operate on the text model.

namespace MaliitKeyboard {
namespace Model {

//! C'tor
Text::Text()
    : m_preedit()
    , m_surrounding()
    , m_surrounding_offset(0)
    , m_face(PreeditDefault)
    , m_cursor_position(0)
{}

//! Returns current preedit.
QString Text::preedit() const
{
    return m_preedit;
}

//! Set current preedit.
//! \param preedit The updated preedit.
//! \param cursor_pos_override New cursor position.
//!
//! If \a cursor_pos_override is lower than 0 then cursor is set at
//! the end of preedit.
void Text::setPreedit(const QString &preedit,
                      int cursor_pos_override /* = -1 */)
{
    const int preedit_len(preedit.length());

    if (cursor_pos_override < 0 or cursor_pos_override > preedit_len) {
        cursor_pos_override = preedit_len;
    }

    m_preedit = preedit;
    m_cursor_position = cursor_pos_override;
}

//! Append to preedit.
//! \param appendix the string to append to current preedit.
void Text::appendToPreedit(const QString &appendix)
{
    m_preedit.insert(m_cursor_position, appendix);
    m_cursor_position += appendix.length();
}

//! Commits current preedit. Insert preedit into surrounding text and
//! updates surrounding offset to match expected cursor position.
void Text::commitPreedit()
{
    // FIXME: Guessing the surrounding text like this might not be quite right;
    // we would expect the text editor to just update the surrounding text.
    // Raises the question whether we should have commitPreedit here at all,
    // but it does preserve some consistency at least.
    m_surrounding = m_preedit;
    m_surrounding_offset = m_preedit.length();
    m_preedit.clear();
    m_primary_candidate.clear();
    m_face = PreeditDefault;
    m_cursor_position = 0;
}

//! Returns the primary candidate, usually provided by word engine.
QString Text::primaryCandidate() const
{
    return m_primary_candidate;
}

//! Set the primary candidate.
//! \param candidate the primary candidate
void Text::setPrimaryCandidate(const QString &candidate)
{
    m_primary_candidate = candidate;
}

//! Returns text surrounding cursor position.
QString Text::surrounding() const
{
    return m_surrounding;
}

//! Returns text left of cursor position. Depends on surroundingOffset.
QString Text::surroundingLeft() const
{
    return m_surrounding.left(m_surrounding_offset);
}

//! Returns text right of cursor position. Depends on surroundingOffset.
QString Text::surroundingRight() const
{
    return m_surrounding.mid(m_surrounding_offset);
}

//! Set text surrounding cursor position.
//! \param surrounding the updated surrounding text.
void Text::setSurrounding(const QString &surrounding)
{
    m_surrounding = surrounding;
}

//! Returns offset of cursor position in surrounding text.
uint Text::surroundingOffset() const
{
    return m_surrounding_offset;
}

//! Set offset of cursor position in surrounding text. Affects
//! surroundingLeft and surroundingRight.
//! \param offset the updated offset.
void Text::setSurroundingOffset(uint offset)
{
    m_surrounding_offset = offset;
}

//! Returns face of preedit.
Text::PreeditFace Text::preeditFace() const
{
    return m_face;
}

//! Sets face of preedit.
//! \param face new face of preedit.
void Text::setPreeditFace(PreeditFace face)
{
    m_face = face;
}

//! Returns cursor position in preedit.
int Text::cursorPosition() const
{
    return m_cursor_position;
}

//! Sets cursor position in preedit.
//! \param cursor_position new position.
void Text::setCursorPosition(int cursor_position)
{
    m_cursor_position = cursor_position;
}

}} // namespace Model, MaliitKeyboard

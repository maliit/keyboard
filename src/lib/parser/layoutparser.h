// -*- mode: C++; c-basic-offset: 4; indent-tabs-mode: nil; c-file-offsets: ((innamespace . 0)); -*-
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

#ifndef MALIIT_KEYBOARD_LAYOUTPARSER_H
#define MALIIT_KEYBOARD_LAYOUTPARSER_H

#include <QXmlStreamReader>
#include <QStringList>

#include "alltagtypes.h"

#include "tagbindingcontainer.h"
#include "tagbinding.h"
#include "tagextended.h"
#include "tagkeyboard.h"
#include "tagkey.h"
#include "taglayout.h"
#include "tagmodifiers.h"
#include "tagrowcontainer.h"
#include "tagrowelement.h"
#include "tagrow.h"
#include "tagsection.h"
#include "tagspacer.h"

namespace MaliitKeyboard {

class LayoutParser
{
public:
    explicit LayoutParser(QIODevice *device);

    bool parse();
    bool isLanguageFile();

    const QString errorString() const;

    const TagKeyboardPtr keyboard() const;
    const QStringList imports() const;
    const QStringList symviews() const;
    const QStringList numbers() const;
    const QStringList phonenumbers() const;

private:
    QXmlStreamReader m_xml;
    TagKeyboardPtr m_keyboard;
    QStringList m_imports;
    QStringList m_symviews;
    QStringList m_numbers;
    QStringList m_phonenumbers;

    void parseKeyboard();
    void parseImport();
    void parseNewStyleImport();
    void parseImportChild(QStringList *target_list);
    bool validateOldStyleImport();
    void parseLayout();
    void parseSection(const TagLayoutPtr &layout);
    void parseRow(const TagRowContainerPtr &row_container);
    void parseKey(const TagRowPtr &row);
    void parseBinding(const TagBindingContainerPtr &binding_container);
    void parseModifiers(const TagBindingPtr &binding);
    void parseExtended(const TagKeyPtr &key);
    void parseSpacer(const TagRowPtr &row);
    void goToRootElement();
    void readToEnd();

    void error(const QString &message);

    bool boolValue(const QStringRef &value, bool defaultValue);

    template <class E>
    E enumValue(const char * const attribute, const QStringList &values, E defaultValue);
};

} // namespace MaliitKeyboard

#endif // MALIIT_KEYBOARD_LAYOUTPARSER_H

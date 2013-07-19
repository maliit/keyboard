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

#ifndef MALIIT_KEYBOARD_ALL_TAG_TYPES_H
#define MALIIT_KEYBOARD_ALL_TAG_TYPES_H

#include <QList>
#include <QSharedPointer>

namespace MaliitKeyboard {

class TagBinding;
class TagBindingContainer;
class TagExtended;
class TagKeyboard;
class TagKey;
class TagLayout;
class TagModifiers;
class TagRowContainer;
class TagRowElement;
class TagRow;
class TagSection;
class TagSpacer;

// There may be some warnings caused by having forward declared shared
// pointers to forward declared classes. We ignore them as this likely
// is a bug in either Qt (QTBUG-18092) or g++ (seemingly 4.6 as I
// don't have this issue in 4.7) or both.
typedef QSharedPointer<TagBinding> TagBindingPtr;
typedef QSharedPointer<TagBindingContainer> TagBindingContainerPtr;
typedef QSharedPointer<TagExtended> TagExtendedPtr;
typedef QSharedPointer<TagKeyboard> TagKeyboardPtr;
typedef QSharedPointer<TagKey> TagKeyPtr;
typedef QSharedPointer<TagLayout> TagLayoutPtr;
typedef QSharedPointer<TagModifiers> TagModifiersPtr;
typedef QSharedPointer<TagRowContainer> TagRowContainerPtr;
typedef QSharedPointer<TagRowElement> TagRowElementPtr;
typedef QSharedPointer<TagRow> TagRowPtr;
typedef QSharedPointer<TagSection> TagSectionPtr;
typedef QSharedPointer<TagSpacer> TagSpacerPtr;

typedef QList<TagModifiersPtr> TagModifiersPtrs;
typedef QList<TagSectionPtr> TagSectionPtrs;
typedef QList<TagLayoutPtr> TagLayoutPtrs;
typedef QList<TagRowElementPtr> TagRowElementPtrs;
typedef QList<TagRowPtr> TagRowPtrs;

} // namespace MaliitKeyboard

#endif // MALIIT_KEYBOARD_ALL_TAG_TYPES_H

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

#include "hitlogic.h"

namespace MaliitKeyboard {
namespace Logic {
namespace {

//! Find whether pos hit an filtered element, with its rectangle being
//! translated to origin.
//! \param filtered the list of filtered elements.
//! \param origin the origin to use for translation.
//! \param pos the position to test against the element bounding box.
template<class T>
T findFilteredElement(const QVector<T> &filtered,
                      const QPoint &origin,
                      const QPoint &pos)
{
    Q_FOREACH (const T &current, filtered) {
        if (current.rect().translated(origin).contains(pos)) {
            return current;
        }
    }

    // No filtered element found:
    return T();
}

//! From a list of elements of type T, find out whether pos (in same coordinate
//! system as geometry) hits one of the elements, if their bounding box is
//! translated to geometry's top left corner.
//!     Returns the found element or a default constructed elment, if pos did
//! not hit any of the provided elements.
//! \param elements the list of provided elements.
//! \param geometry the geometry that pos relates to.
//! \param pos the position to test on whether it hit an element.
//! \param filtered the list of filtered elements.
//! \param behaviour controls the behaviour of elements in filtered list,
//!                  whether to ignore any hit element that is also in filtered
//!                  list, or whether to only accept if hit element is in
//!                  filtered list.
template<class T>
T elementHit(const QVector<T> &elements,
             const QRect &geometry,
             const QPoint &pos,
             const QVector<T> &filtered,
             FilterBehaviour behaviour)
{
    // TODO: assume pos in screen coordinates and translate here?
    if (geometry.contains(pos)) {
        const QPoint &origin(geometry.topLeft());

        // FIXME: use binary range search
        Q_FOREACH (const T &current, elements) {
            const T &from_filter = findFilteredElement<T>(filtered, origin, pos);

            if (current.rect().translated(origin).contains(pos)) {
                switch (behaviour) {
                case IgnoreIfInFilter:
                    if (current != from_filter) {
                        return current;
                    }

                    break;

                case AcceptIfInFilter:
                    if (current == from_filter) {
                        return current;
                    }

                    break;
                }
            }
        }
    }

    // No element hit:
    return T();
}

}

//! \sa elementHit
Key keyHit(const QVector<Key> &keys,
           const QRect &geometry,
           const QPoint &pos,
           const QVector<Key> &filtered_keys,
           FilterBehaviour behaviour)
{
    return elementHit<Key>(keys, geometry, pos, filtered_keys, behaviour);
}

//! \sa elementHit
WordCandidate wordCandidateHit(const QVector<WordCandidate> &candidates,
                               const QRect &geometry,
                               const QPoint &pos,
                               const QVector<WordCandidate> &filtered_candidates,
                               FilterBehaviour behaviour)
{
    return elementHit<WordCandidate>(candidates, geometry, pos, filtered_candidates, behaviour);
}

}} // namespace Logic, MaliitKeyboard

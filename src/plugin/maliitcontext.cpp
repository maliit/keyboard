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

#include "maliitcontext.h"
#include "inputmethod.h"

namespace MaliitKeyboard {
//! \class MaliitContext
//! \brief Helper class to export properties and static data to QML


class MaliitContextPrivate
{
public:
    InputMethod * const input_method;
    SharedStyle style;

    explicit MaliitContextPrivate(InputMethod * const new_input_method,
                                  const SharedStyle &new_style);
};


MaliitContextPrivate::MaliitContextPrivate(InputMethod *const new_input_method,
                                           const SharedStyle &new_style)
    : input_method(new_input_method)
    , style(new_style)
{
    Q_ASSERT(input_method != 0);
    Q_ASSERT(not style.isNull());
}


MaliitContext::MaliitContext(InputMethod *const input_method,
                             const SharedStyle &style,
                             QObject *parent)
    : QObject(parent)
    , d_ptr(new MaliitContextPrivate(input_method, style))
{}


MaliitContext::~MaliitContext()
{}


//! \brief Converts image name to a fully qualified file name.
//!
//! Takes the currently chosen styling profile into account.
//! \param base_name Base name of the image.
QString MaliitContext::image(const QString &base_name) const
{
    if (not base_name.isEmpty()) {
        Q_D(const MaliitContext);
        return d->style->directory(Style::Images) + "/" + base_name;
    }

    return QString();
}


//! \brief Triggers user-initiated hiding of virtual keyboard.
void MaliitContext::hide()
{
    Q_D(MaliitContext);
    d->input_method->hide();
}


void MaliitContext::selectLeftLayout()
{
    Q_D(MaliitContext);
    d->input_method->onLeftLayoutSelected();
}


void MaliitContext::selectRightLayout()
{
    Q_D(MaliitContext);
    d->input_method->onRightLayoutSelected();
}

} // namespace MaliitKeyboard

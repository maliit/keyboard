/*
 * This file is part of Maliit Plugins
 *
 * Copyright (C) 2014 Canonical Ltda
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

#include "inputmethotdextensions.h"

#include <QtCore>

/*!
    \qmltype InputMethod
    \inqmlmodule Ubuntu.Keyboard 0.1
    \brief The InputMethod is a helper class used to attach properties used by Ubuntu Keyboard into input fields

    Example:
    \qml
    Item {
        TextField {
            id: field
            InputMethod.extensions:  {
                 'enterKeyText': 'My Enter',
                 'enterKeyEnabled': true,
                 'enterKeyHighlighted': true,
            }
        }
    }
    \endqml
*/

UbuntuKeyboardInputMethodExtension::UbuntuKeyboardInputMethodExtension(QObject *parent)
    : QObject(parent),
      m_inputText(0)
{
    m_inputText = findInput(parent);
}

/*!
    Allow to change the default visual of the action Key, the value is a dictonary,
    and the valid keys for the dictionary are:
    \list
    \li - enterKeyText (string) - The text to be displayed in the "Enter" key of the virtual keyboard
    \li - enterKeyEnabled (boolean) - If the "Enter" key of the virtual keyboard should be enabled or not
    \li - enterKeyHighlighted (boolean) - If the "Enter" key of the virtual keyboard should be highlighted or not
    \endlist

    \qmlproperty var extensions
*/
QVariantMap UbuntuKeyboardInputMethodExtension::inputMethodExtensions() const
{
    return m_extensions;
}

void UbuntuKeyboardInputMethodExtension::setInputMethodExtensions(const QVariantMap &map)
{
    if (m_extensions != map) {
        m_extensions = map;
        if (m_inputText) {
            m_inputText->setProperty("__inputMethodExtensions", m_extensions);
        }
        Q_EMIT inputMethodExtensionsChanged();
    }
}

QObject *UbuntuKeyboardInputMethodExtension::findInput(QObject *parent)
{
    static QStringList inputClasses;
    if (inputClasses.isEmpty()) {
        inputClasses << "QQuickTextInput"
                     << "QQuickTextEdit";
    }
    Q_FOREACH(QObject *c, parent->children()) {
        if (inputClasses.contains(c->metaObject()->className())) {
            return c;
        }
        QObject *cc = findInput(c);
        if (cc) {
            return cc;
        }
    }
    return 0;
}

UbuntuKeyboardInputMethod::UbuntuKeyboardInputMethod(QObject *parent)
    : QObject(parent)
{
}

UbuntuKeyboardInputMethodExtension *UbuntuKeyboardInputMethod::qmlAttachedProperties(QObject *obj)
{
    return new UbuntuKeyboardInputMethodExtension(obj);
}

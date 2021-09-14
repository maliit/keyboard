/*
 * Copyright (c) 2021 Rodney Dawes
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
#include "iconprovider.h"

#include <QDebug>
#include <QGuiApplication>
#include <QIcon>
#include <QObject>

namespace MaliitKeyboard
{

IconProvider::IconProvider(Theme* theme)
    : QQuickImageProvider(QQuickImageProvider::Pixmap)
    , m_theme(theme)
{
    // Append our icons dir as a fallback theme search path
    auto fallbacks = QIcon::fallbackSearchPaths();
    fallbacks.append(MALIIT_KEYBOARD_DATA_DIR "/icons");
    QIcon::setFallbackSearchPaths(fallbacks);

    // Get the app instance to be able to handle layout direction changes
    static auto app = dynamic_cast<QGuiApplication*>(QGuiApplication::instance());
    if (app == nullptr) {
        qCritical() << "Failed to acquire application instance.";
    } else {
        QObject::connect(app, &QGuiApplication::layoutDirectionChanged,
                         [this](Qt::LayoutDirection direction) {
                             updateLayoutDirection(direction);
                         });
    }

    // Connect to theme change so we ensure we grab the correct icons
    QObject::connect(m_theme, &Theme::themeChanged,
                     [this]() {
                         updateThemeName();
                     });
}

QPixmap IconProvider::requestPixmap(const QString& id,
                                    QSize* size,
                                    const QSize& requestedSize)
{
    // Ensure we have the correct theme set.
    updateThemeName();

    // Append "-rtl" if using RTL layout, so we use correct icons
    auto name = m_layoutDirection == Qt::RightToLeft ? id + QStringLiteral("-rtl") : id;
    auto parts = name.split(QLatin1Char('-'));
    auto icon = QIcon::fromTheme(name);
    while (icon.isNull() && parts.length() > 0) {
        parts.removeLast();
        icon = QIcon::fromTheme(parts.join(QChar('-')));
    }
    if (icon.isNull()) {
        qCritical() << "Keyboard icon not found in theme.";
        return QPixmap();
    }

    // Grab the pixmap, set the outbound size, and return the pixmap
    auto pixmap = icon.pixmap(requestedSize);
    if (size) {
        *size = pixmap.size();
    }
    return pixmap;
}

void IconProvider::updateLayoutDirection(Qt::LayoutDirection direction)
{
    m_layoutDirection = direction;
}

void IconProvider::updateThemeName()
{
    if (QIcon::themeName() != m_theme->iconTheme()) {
        QIcon::setThemeName(m_theme->iconTheme());
    }
}

}

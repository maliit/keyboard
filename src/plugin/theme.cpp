/*
 * Copyright (c) 2020 Jan Arne Petersen
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

#include "theme.h"

#include <QColor>
#include <QFileInfo>
#include <QJsonDocument>
#include <QStandardPaths>
#include <QUrl>

#include "keyboardsettings.h"

namespace MaliitKeyboard
{

Theme::Theme(const KeyboardSettings *settings, QObject *parent)
    : QObject(parent)
    , m_settings(settings)
{
    connect(settings, &KeyboardSettings::themeChanged, this, &Theme::loadTheme);
    loadTheme(settings->theme());
}

Theme::~Theme() = default;

QString Theme::iconTheme() const
{
    if (m_themeData.contains("iconTheme")) {
        return m_themeData.value("iconTheme").toString();
    }

    return QStringLiteral();
}

QColor Theme::colorByName(const char *name, const QColor &defaultColor) const
{
    if (m_themeData.contains(name)) {
        return QColor(m_themeData.value(name).toString());
    }
    return defaultColor;
}

QColor Theme::charKeyColor() const
{
    return colorByName("charKeyColor", QColor("white"));
}

QColor Theme::selectionColor() const
{
    return colorByName("selectionColor", QColor("#19B6EE"));
}

QColor Theme::backgroundColor() const
{
    return colorByName("backgroundColor", QColor("#f7f7f7"));
}

QColor Theme::dividerColor() const
{
    return colorByName("dividerColor", QColor("#cdcdcd"));
}

QColor Theme::annotationFontColor() const
{
    return colorByName("annotationFontColor", QColor("#333333"));
}

QColor Theme::charKeyPressedColor() const
{
    return colorByName("charKeyPressedColor", QColor("#d9d9d9"));
}

QColor Theme::fontColor() const
{
    return colorByName("fontColor", QColor("#333333"));
}

QColor Theme::actionKeyColor() const
{
    return colorByName("actionKeyColor", QColor("#cdcdcd"));
}

QColor Theme::actionKeyPressedColor() const
{
    return colorByName("actionKeyPressedColor", QColor("#aeaeae"));
}

QColor Theme::popupBorderColor() const
{
    return colorByName("popupBorderColor", QColor("#888888"));
}

QColor Theme::charKeyBorderColor() const
{
    return colorByName("charKeyBorderColor", QColor("#888888"));
}

QColor Theme::actionKeyBorderColor() const
{
    return colorByName("actionKeyBorderColor", QColor("#888888"));
}

bool Theme::keyBorderEnabled() const
{
    return m_themeData.value("keyBorderEnabled").toBool(false);
}

QString Theme::fontFamily() const
{
    return m_themeData.value("fontFamily").toString("Noto");
}

QString Theme::annotationFontFamily() const
{
    return m_themeData.value("annotationFontFamily").toString("Noto");
}

double Theme::spaceOpacity() const
{
    return m_themeData.value("spaceOpacity").toDouble(0.33);
}


void Theme::loadTheme(const QString& theme)
{
    const auto &themeFile = QFileInfo(theme).isRelative() ?
        QStandardPaths::locate(QStandardPaths::GenericDataLocation, QStringLiteral("/maliit/keyboard2/themes/%1.json").arg(theme)) :
        theme;

    QFile file(themeFile);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    const auto &content = file.readAll();

    const auto &document = QJsonDocument::fromJson(content);

    m_themeData = document.object();

    emit themeChanged();
}

}

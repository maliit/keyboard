/*
 * Copyright (c) 2020 Jan Arne Petersen
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; version 3.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
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

QUrl Theme::iconsPath() const
{
    if (m_themeData.contains("iconsDir")) {
        const auto &icons = m_themeData.value("iconsDir").toString();
        if (QFileInfo(icons).isRelative())
            return QUrl().resolved(icons);
        return QUrl::fromLocalFile(icons);
    }

    return QUrl::fromLocalFile(MALIIT_KEYBOARD_DATA_DIR "/icons");
}

QUrl Theme::imagesPath() const
{
    if (m_themeData.contains("imagesDir")) {
        const auto &images = m_themeData.value("imagesDir").toString();
        if (QFileInfo(images).isRelative())
            return QUrl().resolved(images);
        return QUrl::fromLocalFile(images);
    }

    return QUrl::fromLocalFile(MALIIT_KEYBOARD_DATA_DIR "/images");
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
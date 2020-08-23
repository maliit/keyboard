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

#ifndef THEME_H
#define THEME_H

#include <QObject>
#include <QJsonObject>

namespace MaliitKeyboard
{

class KeyboardSettings;

class Theme : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QUrl iconsPath READ iconsPath NOTIFY themeChanged)
    Q_PROPERTY(QUrl imagesPath READ imagesPath NOTIFY themeChanged)

    Q_PROPERTY(QColor fontColor READ fontColor NOTIFY themeChanged)
    Q_PROPERTY(QColor selectionColor READ selectionColor NOTIFY themeChanged)
    Q_PROPERTY(QColor backgroundColor READ backgroundColor NOTIFY themeChanged)
    Q_PROPERTY(QColor dividerColor READ dividerColor NOTIFY themeChanged)
    Q_PROPERTY(QColor annotationFontColor READ annotationFontColor NOTIFY themeChanged)
    Q_PROPERTY(QColor charKeyColor READ charKeyColor NOTIFY themeChanged)
    Q_PROPERTY(QColor charKeyPressedColor READ charKeyPressedColor NOTIFY themeChanged)
    Q_PROPERTY(QColor actionKeyColor READ actionKeyColor NOTIFY themeChanged)
    Q_PROPERTY(QColor actionKeyPressedColor READ actionKeyPressedColor NOTIFY themeChanged)
    Q_PROPERTY(QColor popupBorderColor READ popupBorderColor NOTIFY themeChanged)
    Q_PROPERTY(QColor charKeyBorderColor READ charKeyBorderColor NOTIFY themeChanged)
    Q_PROPERTY(QColor actionKeyBorderColor READ actionKeyBorderColor NOTIFY themeChanged)
    Q_PROPERTY(bool keyBorderEnabled READ keyBorderEnabled NOTIFY themeChanged)

public:
    explicit Theme(const KeyboardSettings *settings, QObject *parent = nullptr);
    ~Theme() override;

    [[nodiscard]] QUrl iconsPath() const;
    [[nodiscard]] QUrl imagesPath() const;

    [[nodiscard]] QColor fontColor() const;
    [[nodiscard]] QColor selectionColor() const;
    [[nodiscard]] QColor backgroundColor() const;
    [[nodiscard]] QColor dividerColor() const;
    [[nodiscard]] QColor annotationFontColor() const;
    [[nodiscard]] QColor charKeyColor() const;
    [[nodiscard]] QColor charKeyPressedColor() const;
    [[nodiscard]] QColor actionKeyColor() const;
    [[nodiscard]] QColor actionKeyPressedColor() const;
    [[nodiscard]] QColor popupBorderColor() const;
    [[nodiscard]] QColor charKeyBorderColor() const;
    [[nodiscard]] QColor actionKeyBorderColor() const;
    [[nodiscard]] bool keyBorderEnabled() const;

    void loadTheme(const QString &theme);

Q_SIGNALS:
    void themeChanged();

private:
    QColor colorByName(const char *name, const QColor &defaultColor) const;

    const KeyboardSettings *m_settings;
    QJsonObject m_themeData;
};

}

#endif //THEME_H

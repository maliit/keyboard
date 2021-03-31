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

    Q_PROPERTY(QString fontFamily READ fontFamily NOTIFY themeChanged)
    Q_PROPERTY(QString annotationFontFamily READ fontFamily NOTIFY themeChanged)
    Q_PROPERTY(double spaceOpacity READ spaceOpacity NOTIFY themeChanged)

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
    [[nodiscard]] QString fontFamily() const;
    [[nodiscard]] QString annotationFontFamily() const;
    [[nodiscard]] double spaceOpacity() const;

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

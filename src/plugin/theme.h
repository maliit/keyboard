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

namespace MaliitKeyboard
{

class Theme : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QUrl iconsPath READ iconsPath NOTIFY iconsPathChanged)
    Q_PROPERTY(QUrl imagesPath READ imagesPath NOTIFY imagesPathChanged)

public:
    explicit Theme(QObject *parent = nullptr);
    ~Theme() override;

    [[nodiscard]] QUrl iconsPath() const;
    [[nodiscard]] QUrl imagesPath() const;

Q_SIGNALS:
    void iconsPathChanged();
    void imagesPathChanged();
};

}

#endif //THEME_H

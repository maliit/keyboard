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

#include <QUrl>

namespace MaliitKeyboard
{

Theme::Theme(QObject *parent)
    : QObject(parent)
{

}

Theme::~Theme() = default;

QUrl Theme::iconsPath() const
{
    return QUrl::fromLocalFile(MALIIT_KEYBOARD_DATA_DIR "/icons");
}

QUrl Theme::imagesPath() const
{
    return QUrl::fromLocalFile(MALIIT_KEYBOARD_DATA_DIR "/images");
}

}
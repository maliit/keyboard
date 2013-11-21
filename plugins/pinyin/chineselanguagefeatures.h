/*
 * Copyright 2013 Canonical Ltd.
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

#ifndef CHINESELANGUAGEFEATURES_H
#define CHINESELANGUAGEFEATURES_H

#include "abstractlanguagefeatures.h"
#include <QObject>

class ChineseLanguageFeatures : public QObject, public AbstractLanguageFeatures
{
    Q_OBJECT
public:
    explicit ChineseLanguageFeatures(QObject *parent = 0);
    virtual ~ChineseLanguageFeatures();

    virtual bool activateAutoCaps(const QString &preedit) const;
    virtual QString appendixForReplacedPreedit(const QString &preedit) const;
};

#endif // CHINESELANGUAGEFEATURES_H

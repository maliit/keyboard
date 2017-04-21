/*
 * Copyright 2016 Canonical Ltd.
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

#ifndef CHEWINGLANGUAGEFEATURES_H
#define CHEWINGLANGUAGEFEATURES_H

#include "abstractlanguagefeatures.h"
#include <QObject>

class ChewingLanguageFeatures : public QObject, public AbstractLanguageFeatures
{
    Q_OBJECT
public:
    explicit ChewingLanguageFeatures(QObject *parent = nullptr);
    ~ChewingLanguageFeatures() override;

    bool alwaysShowSuggestions() const override;
    bool autoCapsAvailable() const override;
    bool activateAutoCaps(const QString &preedit) const override;
    QString appendixForReplacedPreedit(const QString &preedit) const override;
    bool isSeparator(const QString &text) const override;
    bool isSymbol(const QString &text) const override;
    bool ignoreSimilarity() const override;
    bool wordEngineAvailable() const override;
    QString fullStopSequence() const override;
    bool restorePreedit() const override;
    bool commitOnSpace() const override;
    bool showPrimaryInPreedit() const override;
};

#endif // CHINESELANGUAGEFEATURES_H

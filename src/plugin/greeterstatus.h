/*
 * Copyright 2014 Canonical Ltd.
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

#ifndef GREETERSTATUS_H
#define GREETERSTATUS_H

#include <QObject>

class GreeterStatus : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool greeterActive READ greeterActive NOTIFY greeterActiveChanged)

public:
    GreeterStatus(QObject *parent = nullptr);
    bool greeterActive() const;

Q_SIGNALS:
    void greeterActiveChanged();

private Q_SLOTS:
    void greeterPropertiesChanged(const QString &interface, const QVariantMap &changed, const QStringList &invalidated);

private:
    bool m_greeterActive;
};

#endif // GREETERSTATUS_H

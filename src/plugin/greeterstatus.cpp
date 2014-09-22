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

#include <QDBusConnection>
#include <QDBusInterface>
#include <QDBusReply>
#include "greeterstatus.h"

GreeterStatus::GreeterStatus(QObject *parent) : QObject(parent)
{
    QDBusConnection connection = QDBusConnection::sessionBus();
    QDBusInterface greeterPropsIface("com.canonical.UnityGreeter",
                                     "/",
                                     "org.freedesktop.DBus.Properties");
    QDBusReply<QVariant> reply = greeterPropsIface.call("Get", "com.canonical.UnityGreeter", "IsActive");
    m_greeterActive = reply.isValid() && reply.value().toBool();
    connection.connect("com.canonical.UnityGreeter",
                       "/",
                       "org.freedesktop.DBus.Properties",
                       "PropertiesChanged",
                       this,
                       SLOT(greeterPropertiesChanged(QString, QVariantMap, QStringList)));
}

bool GreeterStatus::greeterActive() const
{
    return m_greeterActive;
}

void GreeterStatus::greeterPropertiesChanged(const QString &interface, const QVariantMap &changed, const QStringList &invalidated)
{
    Q_UNUSED(invalidated);

    if (interface == "com.canonical.UnityGreeter") {
        if (changed.contains("IsActive")) {
            m_greeterActive = changed.value("IsActive").toBool();
            Q_EMIT greeterActiveChanged();
        }
    }
}


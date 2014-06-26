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
 *
 * Author: Lars Uebernickel <lars.uebernickel@canonical.com>
 */

#ifndef __QGSETTINGS_H__
#define __QGSETTINGS_H__

#include <QObject>
#include <QStringList>

/**
 * @brief QGSettings provides access to application settings stored with GSettings.
 *
 * GSettings does not allow keys to contain anything other than lower-case
 * characters and dashes. This class converts all keys to camelCase, to make it easier
 * for people used to Qt's naming convention.
 */
class QGSettings: public QObject
{
    Q_OBJECT

public:
    /**
     * @brief Create a QGSettings object for a given schema_id and path.
     * @param schema_id The id of the schema
     * @param path If non-empty, specifies the path for a relocatable schema
     */
    QGSettings(const QByteArray &schema_id, const QByteArray &path = QByteArray(), QObject *parent = NULL);

    ~QGSettings();

    /**
     * @brief Gets the value that is stored at key
     * @param key The key for which to retrieve the value (in camelCase)
     *
     * It is an error if key does not exist in the schema associated with this
     * QGSettings object.
     */
    QVariant get(const QString &key) const;

    /**
     * @brief Sets the value at key to value
     * @key The key for which to set the value (in camelCase)
     * @value The value to set
     *
     * It is an error if key does not exist in the schema associated with this
     * QGSettings object.
     *
     * Not all values that a QVariant can hold can be serialized into a
     * setting. Basic types (integers, doubles, strings) and string lists are
     * supported.
     */
    void set(const QString &key, const QVariant &value);

    /**
     * @brief Sets the value at key to value
     * @key The key for which to set the value
     * @value The value to set
     *
     * Behaves just like ::set(key, value), but returns false instead of
     * printing a warning if the key couldn't be set.
     *
     * @return whether the key was set
     */
    bool trySet(const QString &key, const QVariant &value);

    /**
     * \brief Retrieves the list of avaliable keys
     */
    QStringList keys() const;

    /**
     * \brief Returns the list of values that key can assume
     *
     * Returns an empty list if the schema doesn't contain that information for
     * the key.
     */
    QVariantList choices(const QString &key) const;

    /**
     * @brief Resets the setting for @key to its default value.
     */
    void reset(const QString &key);

    /**
     * @brief Checks if a schema with the given id is installed.
     */
    static bool isSchemaInstalled(const QByteArray &schema_id);

Q_SIGNALS:
    /**
     * \brief Emitted when the value associated with key has changed
     */
    void changed(const QString &key);

private:
    struct QGSettingsPrivate *priv;
    friend struct QGSettingsPrivate;
};

#endif

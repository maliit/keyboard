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

#include "qgsettings.h"

#include "qconftypes.h"
#include "util.h"
#include <gio/gio.h>

struct QGSettingsPrivate
{
    QByteArray schema_id;
    QByteArray path;
    GSettings *settings;
    GSettingsSchema *schema;
    gulong signal_handler_id;

    static void settingChanged(GSettings *settings, const gchar *key, gpointer user_data);
};

void QGSettingsPrivate::settingChanged(GSettings *, const gchar *key, gpointer user_data)
{
    QGSettings *self = (QGSettings *)user_data;

    // work around https://bugreports.qt.io/browse/QTBUG-32859 and http://pad.lv/1460970
    QMetaObject::invokeMethod(self, "changed", Qt::QueuedConnection, Q_ARG(QString, qtify_name(key)));
}

QGSettings::QGSettings(const QByteArray &schema_id, const QByteArray &path, QObject *parent):
    QObject(parent)
{
    priv = new QGSettingsPrivate;
    priv->schema_id = schema_id;
    priv->path = path;

    if (priv->path.isEmpty())
        priv->settings = g_settings_new(priv->schema_id.constData());
    else
        priv->settings = g_settings_new_with_path(priv->schema_id.constData(), priv->path.constData());

    g_object_get (priv->settings, "settings-schema", &priv->schema, NULL);
    priv->signal_handler_id = g_signal_connect(priv->settings, "changed", G_CALLBACK(QGSettingsPrivate::settingChanged), this);
}

QGSettings::~QGSettings()
{
    if (priv->schema) {
        g_settings_sync ();
        g_signal_handler_disconnect(priv->settings, priv->signal_handler_id);
        g_object_unref (priv->settings);
        g_settings_schema_unref (priv->schema);
    }
    delete priv;
}

QVariant QGSettings::get(const QString &key) const
{
    gchar *gkey = unqtify_name(key);
    GVariant *value = g_settings_get_value(priv->settings, gkey);
    QVariant qvalue = qconf_types_to_qvariant(value);
    g_variant_unref(value);
    g_free(gkey);
    return qvalue;
}

void QGSettings::set(const QString &key, const QVariant &value)
{
    if (!this->trySet(key, value))
        qWarning("unable to set key '%s' to value '%s'", key.toUtf8().constData(), value.toString().toUtf8().constData());
}

bool QGSettings::trySet(const QString &key, const QVariant &value)
{
    gchar *gkey = unqtify_name(key);
    bool success = false;

    /* fetch current value to find out the exact type */
    GVariant *cur = g_settings_get_value(priv->settings, gkey);

    GVariant *new_value = qconf_types_collect_from_variant(g_variant_get_type (cur), value);
    if (new_value)
        success = g_settings_set_value(priv->settings, gkey, new_value);

    g_free(gkey);
    g_variant_unref (cur);

    return success;
}

QStringList QGSettings::keys() const
{
    QStringList list;
    gchar **keys = g_settings_schema_list_keys(priv->schema);
    for (int i = 0; keys[i]; i++)
        list.append(qtify_name(keys[i]));

    g_strfreev(keys);
    return list;
}

QVariantList QGSettings::choices(const QString &qkey) const
{
    gchar *key = unqtify_name (qkey);
    GSettingsSchemaKey *schema_key = g_settings_schema_get_key (priv->schema, key);
    GVariant *range = g_settings_schema_key_get_range(schema_key);
    g_settings_schema_key_unref (schema_key);
    g_free(key);

    if (range == nullptr)
        return QVariantList();

    const gchar *type;
    GVariant *value;
    g_variant_get(range, "(&sv)", &type, &value);

    QVariantList choices;
    if (g_str_equal(type, "enum")) {
        GVariantIter iter;
        GVariant *child;

        g_variant_iter_init (&iter, value);
        while ((child = g_variant_iter_next_value(&iter))) {
            choices.append(qconf_types_to_qvariant(child));
            g_variant_unref(child);
        }
    }

    g_variant_unref (value);
    g_variant_unref (range);

    return choices;
}

void QGSettings::reset(const QString &qkey)
{
    gchar *key = unqtify_name(qkey);
    g_settings_reset(priv->settings, key);
    g_free(key);
}

bool QGSettings::isSchemaInstalled(const QByteArray &schema_id)
{
    GSettingsSchemaSource *source = g_settings_schema_source_get_default ();
    GSettingsSchema *schema = g_settings_schema_source_lookup (source, schema_id.constData(), TRUE);
    if (schema) {
        g_settings_schema_unref (schema);
        return true;
    } else {
        return false;
    }
}

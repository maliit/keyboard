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
 * Authors: Lars Uebernickel <lars.uebernickel@canonical.com>
 *          Ryan Lortie <desrt@desrt.ca>
 */

#include <glib.h>
#include <QString>

/* convert 'some-key' to 'someKey' or 'SomeKey'.
 * the second form is needed for appending to 'set' for 'setSomeKey'
 */
QString qtify_name(const char *name)
{
    bool next_cap = false;
    QString result;

    while (*name) {
        if (*name == '-') {
            next_cap = true;
        } else if (next_cap) {
            result.append(QChar(*name).toUpper().toLatin1());
            next_cap = false;
        } else {
            result.append(*name);
        }

        name++;
    }

    return result;
}

/* Convert 'someKey' to 'some-key'
 *
 * This is the inverse function of qtify_name, iff qtify_name was called with a
 * valid gsettings key name (no capital letters, no consecutive dashes).
 *
 * Returns a newly-allocated string.
 */
gchar * unqtify_name(const QString &name)
{
    const gchar *p;
    QByteArray bytes;
    GString *str;

    bytes = name.toUtf8();
    str = g_string_new (nullptr);

    for (p = bytes.constData(); *p; p++) {
        const QChar c(*p);
        if (c.isUpper()) {
            g_string_append_c (str, '-');
            g_string_append_c (str, c.toLower().toLatin1());
        }
        else {
            g_string_append_c (str, *p);
        }
    }

    return g_string_free(str, FALSE);
}

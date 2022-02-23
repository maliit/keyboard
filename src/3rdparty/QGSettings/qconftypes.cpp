/*
 * Copyright © 2011 Canonical Limited
 *
 * This library is free software: you can redistribute it and/or modify it under the terms of version 3 of the
 * GNU Lesser General Public License as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the
 * implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
 * License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License along with this program.  If not,
 * see <http://www.gnu.org/licenses/>.
 *
 * Author: Ryan Lortie <desrt@desrt.ca>
 */

#include "qconftypes.h"

#include <QStringList>

/* This file is responsible for conversion between C++ values and GVariant *.
 *
 * We use the QVariant type system (but not QVariant) to express the types of native C++ values.  This is
 * necessary because we must expose these types to Qt via the QMetaObject system.
 *
 * GSettings uses the GVariant type system to specify the types of the keys in a schema.
 *
 * We therefore have a non-injective, non-surjective partial function from GVariant types to QVariant types.
 *
 * The non-injectivity is mandated by the fact that the QVariant type system is less expressive than the
 * GVariant type system when it comes to specifying the sizes of integers.  The non-surjectivity is mandated by
 * the fact that the GVariant type system has less breadth than the QVariant type system (no date, time, font,
 * etc. types).
 *
 * Due to the non-injectivity, we can only ever go in the direction from GVariant to QVariant; all native type
 * to GVariant conversions must be done in the presence of GVariant type information.  For example, we are
 * unsure if 'Int' is supposed to be a 32 or 16 bit integer.
 *
 * The function is partial: not all GVariant types are handled.  This can be improved in the future, but for now
 * it is probably sufficient to handle the common types that are used in 99% of cases.
 *
 *  GVariant Type Name/Code      C++ Type Name          QVariant Type Name
 *  --------------------------------------------------------------------------
 *  boolean            b         bool                   QVariant::Bool
 *  byte               y         char                   QVariant::Char
 *  int16              n         int                    QVariant::Int
 *  uint16             q         unsigned int           QVariant::UInt
 *  int32              i         int                    QVariant::Int
 *  uint32             u         unsigned int           QVariant::UInt
 *  int64              x         long long              QVariant::LongLong
 *  uint64             t         unsigned long long     QVariant::ULongLong
 *  double             d         double                 QVariant::Double
 *  string             s         QString                QVariant::String
 *  string array*      as        QStringList            QVariant::StringList
 *  byte array         ay        QByteArray             QVariant::ByteArray
 *  dictionary         a{ss}     QVariantMap            QVariant::Map
 *
 * [*] not strictly an array, but called as such for sake of
 *     consistency with the 'a' appearing in the DBus type system
 *
 * We provide three functions here: the (one-way) mapping from GVariantType to QVariant::Type, plus two helpers
 * for converting GVariant to and from native C++ types.  The signatures of these helpers are decided by the
 * fact that they always need to know the GVariant type information (as discussed above).
 *
 * The three functions should all be kept completely in sync with each other in terms of what types they support
 * and how they elect to map those types.  Not-reached assertions are used in the two value conversion functions
 * in the cases that the type conversion function would have failed (ie: that could should be unreachable).
 */

QVariant::Type qconf_types_convert(const GVariantType *gtype)
{
    switch (g_variant_type_peek_string(gtype)[0]) {
    case G_VARIANT_CLASS_BOOLEAN:
        return QVariant::Bool;

    case G_VARIANT_CLASS_BYTE:
        return QVariant::Char;

    case G_VARIANT_CLASS_INT16:
        return QVariant::Int;

    case G_VARIANT_CLASS_UINT16:
        return QVariant::UInt;

    case G_VARIANT_CLASS_INT32:
        return QVariant::Int;

    case G_VARIANT_CLASS_UINT32:
        return QVariant::UInt;

    case G_VARIANT_CLASS_INT64:
        return QVariant::LongLong;

    case G_VARIANT_CLASS_UINT64:
        return QVariant::ULongLong;

    case G_VARIANT_CLASS_DOUBLE:
        return QVariant::Double;

    case G_VARIANT_CLASS_STRING:
        return QVariant::String;

    case G_VARIANT_CLASS_ARRAY:
        if (g_variant_type_equal(gtype, G_VARIANT_TYPE_STRING_ARRAY))
            return QVariant::StringList;

        else if (g_variant_type_equal(gtype, G_VARIANT_TYPE_BYTESTRING))
            return QVariant::ByteArray;

        else if (g_variant_type_equal(gtype, G_VARIANT_TYPE ("a{ss}")))
            return QVariant::Map;

        // fall through
    default:
        return QVariant::Invalid;
    }
}

QVariant qconf_types_to_qvariant(GVariant *value)
{
    switch (g_variant_classify(value)) {
    case G_VARIANT_CLASS_BOOLEAN:
        return QVariant((bool) g_variant_get_boolean(value));

    case G_VARIANT_CLASS_BYTE:
        return QVariant((char) g_variant_get_byte(value));

    case G_VARIANT_CLASS_INT16:
        return QVariant((int) g_variant_get_int16(value));

    case G_VARIANT_CLASS_UINT16:
        return QVariant((unsigned int) g_variant_get_uint16(value));

    case G_VARIANT_CLASS_INT32:
        return QVariant((int) g_variant_get_int32(value));

    case G_VARIANT_CLASS_UINT32:
        return QVariant((unsigned int) g_variant_get_uint32(value));

    case G_VARIANT_CLASS_INT64:
        return QVariant((long long) g_variant_get_int64(value));

    case G_VARIANT_CLASS_UINT64:
        return QVariant((unsigned long long) g_variant_get_uint64(value));

    case G_VARIANT_CLASS_DOUBLE:
        return QVariant(g_variant_get_double(value));

    case G_VARIANT_CLASS_STRING:
        return QVariant(g_variant_get_string(value, nullptr));

    case G_VARIANT_CLASS_ARRAY:
        if (g_variant_is_of_type(value, G_VARIANT_TYPE_STRING_ARRAY)) {
            GVariantIter iter;
            QStringList list;
            const gchar *str;

            g_variant_iter_init (&iter, value);
            while (g_variant_iter_next (&iter, "&s", &str))
                list.append (str);

            return QVariant(list);
        } else if (g_variant_is_of_type(value, G_VARIANT_TYPE_BYTESTRING)) {
            return QVariant(QByteArray(g_variant_get_bytestring(value)));
        } else if (g_variant_is_of_type(value, G_VARIANT_TYPE("a{ss}"))) {
            GVariantIter iter;
            QMap<QString, QVariant> map;
            const gchar *key; 
            const gchar *val;

            g_variant_iter_init (&iter, value);
            while (g_variant_iter_next (&iter, "{&s&s}", &key, &val))
                map.insert(key, QVariant(val));

            return map;
        }

        // fall through
    default:
        g_assert_not_reached();
    }
}

GVariant *qconf_types_collect_from_variant(const GVariantType *gtype, const QVariant &v)
{
  switch (g_variant_type_peek_string(gtype)[0]) {
    case G_VARIANT_CLASS_BOOLEAN:
        return g_variant_new_boolean(v.toBool());

    case G_VARIANT_CLASS_BYTE:
        return g_variant_new_byte(v.toChar().cell());

    case G_VARIANT_CLASS_INT16:
        return g_variant_new_int16(v.toInt());

    case G_VARIANT_CLASS_UINT16:
        return g_variant_new_uint16(v.toUInt());

    case G_VARIANT_CLASS_INT32:
        return g_variant_new_int32(v.toInt());

    case G_VARIANT_CLASS_UINT32:
        return g_variant_new_uint32(v.toUInt());

    case G_VARIANT_CLASS_INT64:
        return g_variant_new_int64(v.toLongLong());

    case G_VARIANT_CLASS_UINT64:
        return g_variant_new_int64(v.toULongLong());

    case G_VARIANT_CLASS_DOUBLE:
        return g_variant_new_double(v.toDouble());

    case G_VARIANT_CLASS_STRING:
        return g_variant_new_string(v.toString().toUtf8());

    case G_VARIANT_CLASS_ARRAY:
        if (g_variant_type_equal(gtype, G_VARIANT_TYPE_STRING_ARRAY)) {
            const QStringList list = v.toStringList();
            GVariantBuilder builder;

            g_variant_builder_init(&builder, G_VARIANT_TYPE_STRING_ARRAY);

            Q_FOREACH (const QString& string, list)
              g_variant_builder_add(&builder, "s", string.toUtf8().constData());

            return g_variant_builder_end(&builder);
        } else if (g_variant_type_equal(gtype, G_VARIANT_TYPE_BYTESTRING)) {
            const QByteArray array = v.toByteArray();
            gsize size = array.size();
            gpointer data;

#ifdef GLIB_VERSION_2_68
             data = g_memdup2(array.data(), size);
#else
             data = g_memdup(array.data(), size);
#endif

            return g_variant_new_from_data(G_VARIANT_TYPE_BYTESTRING,
                                           data, size, TRUE, g_free, data);
        } else if (g_variant_type_equal(gtype, G_VARIANT_TYPE("a{ss}"))) {
            GVariantBuilder builder;
            g_variant_builder_init(&builder, G_VARIANT_TYPE ("a{ss}"));
            QMapIterator<QString, QVariant> it(v.toMap());
            while (it.hasNext()) {
                it.next();
                QByteArray key = it.key().toUtf8();
                QByteArray val = it.value().toByteArray();
                g_variant_builder_add (&builder, "{ss}", key.constData(), val.constData());
            }
            return g_variant_builder_end (&builder);
        }

        // fall through
    default:
        return nullptr;
  }
}

// vim:sw=4 tw=112

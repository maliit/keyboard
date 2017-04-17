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

#ifndef _qconftypes_h_
#define _qconftypes_h_

#include <QVariant>
#include <glib.h>

QVariant::Type qconf_types_convert(const GVariantType *gtype);
GVariant * qconf_types_collect(const GVariantType *gtype, const void *argument);
GVariant *qconf_types_collect_from_variant(const GVariantType *gtype, const QVariant &v);
void qconf_types_unpack(GVariant *value, void *argument);
QVariant qconf_types_to_qvariant(GVariant *value);

#endif /* _qconftypes_h_ */

/*
 * This file is part of Maliit Plugins
 *
 * Copyright (C) 2012 Openismus GmbH. All rights reserved.
 *
 * Contact: maliit-discuss@lists.maliit.org
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * Redistributions of source code must retain the above copyright notice, this list
 * of conditions and the following disclaimer.
 * Redistributions in binary form must reproduce the above copyright notice, this list
 * of conditions and the following disclaimer in the documentation and/or other materials
 * provided with the distribution.
 * Neither the name of Nokia Corporation nor the names of its contributors may be
 * used to endorse or promote products derived from this software without specific
 * prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL
 * THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#include "layout.h"
#include "keyarea.h"
#include "key.h"
#include "keydescription.h"

#include <QtGui/QGuiApplication>
#include <QScreen>

namespace MaliitKeyboard {
namespace Model {
namespace {
QUrl toUrl(const QString &directory,
            const QString &base_name)
{
    if (not (directory.isEmpty() || base_name.isEmpty())) {
        return QUrl(directory + "/" + base_name);
    }

    return QUrl();

}
}


class LayoutPrivate
{
public:
    QString title;
    KeyArea key_area;
    QString image_directory;
    QHash<int, QByteArray> roles;
    Layout::State state;
    QString activeViewId;

    explicit LayoutPrivate();
};


LayoutPrivate::LayoutPrivate()
    : title()
    , key_area()
    , image_directory()
    , roles()
    , state(Layout::DefaultState)
{
    // Model roles are used as variables in QML, hence the under_score naming
    // convention:
    roles[Layout::RoleKeyRectangle] = "key_rectangle";
    roles[Layout::RoleKeyReactiveArea] = "key_reactive_area";
    roles[Layout::RoleKeyBackground] = "key_background";
    roles[Layout::RoleKeyBackgroundBorders] = "key_background_borders";
    roles[Layout::RoleKeyText] = "key_text";
    roles[Layout::RoleKeyFont] = "key_font";
    roles[Layout::RoleKeyFontColor] = "key_font_color";
    roles[Layout::RoleKeyFontSize] = "key_font_size";
    roles[Layout::RoleKeyFontStretch] = "key_font_stretch";
    roles[Layout::RoleKeyIcon] = "key_icon";
    roles[Layout::RoleKeyActionInsert] = "key_action_insert";
    roles[Layout::RoleKeyAction] = "key_action_type";
}


Layout::Layout(QObject *parent)
    : QAbstractListModel(parent)
    , d_ptr(new LayoutPrivate)
{}


Layout::~Layout()
{}


void Layout::setTitle(const QString &title)
{
    Q_D(Layout);

    if (d->title != title) {
        d->title = title;
        Q_EMIT titleChanged(d->title);
    }
}


QString Layout::title() const
{
    Q_D(const Layout);
    return d->title;
}


void Layout::setKeyArea(const KeyArea &area)
{
    beginResetModel();

    Q_D(Layout);
    const bool geometry_changed(d->key_area.rect() != area.rect());
    const bool background_changed(d->key_area.area().background() != area.area().background());
    const bool background_borders_changed(d->key_area.area().backgroundBorders() != area.area().backgroundBorders());
    const bool visible_changed((d->key_area.keys().isEmpty() && not area.keys().isEmpty())
                               || (not d->key_area.keys().isEmpty() && area.keys().isEmpty()));
    const bool origin_changed(d->key_area.origin() != area.origin());

    d->key_area = area;

    if (origin_changed) {
        Q_EMIT originChanged(d->key_area.origin());
    }

    if (geometry_changed) {
        Q_EMIT widthChanged(width());
        Q_EMIT heightChanged(height());
    }

    if (background_changed) {
        Q_EMIT backgroundChanged(background());
    }

    if (background_borders_changed) {
        Q_EMIT backgroundBordersChanged(backgroundBorders());
    }

    if (visible_changed) {
        Q_EMIT visibleChanged(not d->key_area.keys().isEmpty());
    }

    endResetModel();
}


KeyArea Layout::keyArea() const
{
    Q_D(const Layout);
    return d->key_area;
}


void Layout::replaceKey(int index,
                        const Key &key)
{
    Q_D(Layout);
    d->key_area.rKeys().replace(index, key);
    Q_EMIT dataChanged(this->index(index, 0), this->index(index, 0));
}


bool Layout::isVisible() const
{
    Q_D(const Layout);
    return (not d->key_area.keys().isEmpty());
}


int Layout::width() const
{
    Q_D(const Layout);
    return d->key_area.rect().width();
}


int Layout::height() const
{
    Q_D(const Layout);
    return d->key_area.rect().height();
}

QPoint Layout::origin() const
{
    Q_D(const Layout);
    return d->key_area.origin();
}


QUrl Layout::background() const
{
    Q_D(const Layout);
    return toUrl(d->image_directory, d->key_area.area().background());
}


QRectF Layout::backgroundBorders() const
{
    Q_D(const Layout);

    const QMargins &m(d->key_area.area().backgroundBorders());
    return QRectF(m.left(), m.top(), m.right(), m.bottom());
}

Layout::State Layout::state() const
{
    Q_D(const Layout);
    return d->state;
}

void Layout::setState(Model::Layout::State state)
{
    Q_D(Layout);
    d->state = state;
    Q_EMIT stateChanged(state);
}

QString Layout::activeView() const
{
    Q_D(const Layout);
    return d->activeViewId;
}

void Layout::setActiveView(const QString& activeViewId)
{
    Q_D(Layout);
    d->activeViewId = activeViewId;
}

void Layout::setImageDirectory(const QString &directory)
{
    Q_D(Layout);

    if (d->image_directory != directory) {
        d->image_directory = directory;
        // TODO: Make sure we don't accidentially invalidate the whole model twice
        beginResetModel();
        backgroundChanged(background());
        endResetModel();
    }
}


int Layout::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    Q_D(const Layout);
    return d->key_area.keys().count();
}


QVariant Layout::data(const QModelIndex &index,
                      int role) const
{
    Q_D(const Layout);

    const QVector<Key> &keys(d->key_area.keys());
    const Key &key(index.row() < keys.count()
                   ? keys.at(index.row())
                   : Key());

    switch(role) {
    case RoleKeyReactiveArea:
        return QVariant(key.rect());

    case RoleKeyRectangle: {
        const QRect &r(key.rect());
        const QMargins &m(key.margins());

        return QVariant(QRectF(m.left(), m.top(),
                               r.width() - (m.left() + m.right()),
                               r.height() - (m.top() + m.bottom())));
    }

    case RoleKeyBackground:
        return QVariant(toUrl(d->image_directory, key.area().background()));

    case RoleKeyBackgroundBorders: {
        // Neither QML nor QVariant support QMargins type.
        // We need to transform QMargins into a QRectF so that we can abuse
        // left, top, right, bottom (of the QRectF) *as if* it was a QMargins.
        const QMargins &m(key.area().backgroundBorders());
        return QVariant(QRectF(m.left(), m.top(), m.right(), m.bottom()));
    }

    case RoleKeyText:
        return QVariant(key.label());

    case RoleKeyFont:
        return QVariant(QString());

    case RoleKeyFontColor:
        // FIXME: QML expects QVariant(QColor(...)) here, but then we'd have a QtGui dependency, no?
        return QVariant(QString());

    case RoleKeyFontSize:
        // FIXME: Using qMax to suppress warning about "invalid" 0.0 font sizes in QFont::setPointSizeF.
        return QVariant(1);

    case RoleKeyFontStretch:
        return QVariant(1);

    case RoleKeyIcon:
        return QVariant(toUrl(d->image_directory, key.icon()));

    case RoleKeyActionInsert:
        return QVariant(key.action() == Key::ActionInsert);

    case RoleKeyAction:
        return QVariant(key.action());
    }

    qWarning() << __PRETTY_FUNCTION__
               << "Invalid index or role (" << index.row() << role << ").";

    return QVariant();
}


QHash<int, QByteArray> Layout::roleNames() const
{
    Q_D(const Layout);
    return d->roles;
}


QVariant Layout::data(int index,
                      const QString &role) const
{

    const QModelIndex idx(this->index(index, 0));
    return data(idx, roleNames().key(role.toLatin1()));
}


}} // namespace Model, MaliitKeyboard

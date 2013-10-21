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

#ifndef SCENERECTWATCHER_H
#define SCENERECTWATCHER_H

#include <QObject>

#include <QQuickItem>
#include <QList>
#include <QPointer>
#include <QTimer>

class SceneRectWatcher : public QObject {
    Q_OBJECT
public:
    SceneRectWatcher(QObject *parent = 0);
    void setItem(QQuickItem *target);

Q_SIGNALS:
    void sceneRectChanged();

private Q_SLOTS:
    void onAscendantChanged();
    void notifySceneRectChange();

private:
    void listenToAscendantsChanges();
    void disconnectFromAscendantsChanges();

    QList<QMetaObject::Connection> m_connections;
    QPointer<QQuickItem> m_target;

    // We send sceneRectChanged() with a delay avoid flooding sceneRectChanged()
    // listeners during animations and also because it's common that more than
    // one property changes during the same event loop iteration (e.g. ancestor.x()
    // and ancestor.y())
    QTimer m_sceneRectChangedTimer;
};

#endif // SCENERECTWATCHER_H

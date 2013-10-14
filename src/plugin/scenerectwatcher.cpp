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

#include <scenerectwatcher.h>

SceneRectWatcher::SceneRectWatcher(QObject *parent)
    : QObject(parent)
{
    m_sceneRectChangedTimer.setInterval(200);
    m_sceneRectChangedTimer.setSingleShot(true);
    connect(&m_sceneRectChangedTimer, &QTimer::timeout,
            this, &SceneRectWatcher::sceneRectChanged);
}

void SceneRectWatcher::setItem(QQuickItem *target)
{
    if (m_target == target)
        return;

    m_sceneRectChangedTimer.stop();
    m_target = target;
    listenToAscendantsChanges();
}

void SceneRectWatcher::listenToAscendantsChanges()
{
    disconnectFromAscendantsChanges();

    if (!m_target)
        return;

    QQuickItem* item = m_target.data();

    m_connections.append(connect(item, &QQuickItem::widthChanged,
                                 this, &SceneRectWatcher::notifySceneRectChange));
    m_connections.append(connect(item, &QQuickItem::heightChanged,
                                 this, &SceneRectWatcher::notifySceneRectChange));

    while (item != NULL) {
        m_connections.append(connect(item, &QQuickItem::parentChanged,
                                     this, &SceneRectWatcher::onAscendantChanged));
        m_connections.append(connect(item, &QQuickItem::xChanged,
                                     this, &SceneRectWatcher::notifySceneRectChange));
        m_connections.append(connect(item, &QQuickItem::yChanged,
                                     this, &SceneRectWatcher::notifySceneRectChange));
        m_connections.append(connect(item, &QQuickItem::transformOriginChanged,
                                     this, &SceneRectWatcher::notifySceneRectChange));
        m_connections.append(connect(item, &QQuickItem::rotationChanged,
                                     this, &SceneRectWatcher::notifySceneRectChange));
        m_connections.append(connect(item, &QQuickItem::scaleChanged,
                                     this, &SceneRectWatcher::notifySceneRectChange));
        item = item->parentItem();
    }
}

void SceneRectWatcher::disconnectFromAscendantsChanges()
{
    // disconnect all previously connected signals
    Q_FOREACH (QMetaObject::Connection connection, m_connections) {
        disconnect(connection);
    }
    m_connections.clear();
}

void SceneRectWatcher::onAscendantChanged()
{
    // Simple approach: Just rebuild connections from scratch.
    listenToAscendantsChanges();

    notifySceneRectChange();
}

void SceneRectWatcher::notifySceneRectChange()
{
    if (!m_sceneRectChangedTimer.isActive()) {
        m_sceneRectChangedTimer.start();
    }
}

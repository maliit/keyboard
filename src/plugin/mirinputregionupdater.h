/*
 * Copyright (C) 2016 Canonical, Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; version 3.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef MIR_INPUT_REGION_UPDATER_H_
#define MIR_INPUT_REGION_UPDATER_H_

#include <QObject>
#include <QPointer>
#include <QWindow>

#include "keyboardgeometry.h"

class MirConnection;
class MirRectangle;
class MirSurface;
typedef struct MirSurface MirWindow;

/*
  Updates the input region of the mir window backing up the given QWindow
  according to the visible rect of the given KeyboardGeometry
 */
class MirInputRegionUpdater : public QObject
{
    Q_OBJECT

public:
    MirInputRegionUpdater(QWindow *window, KeyboardGeometry *geometry);
    virtual ~MirInputRegionUpdater();

private Q_SLOT:
    void update();

private:
    QPointer<QWindow> m_window;
    QPointer<KeyboardGeometry> m_kbdGeometry;
    MirConnection *m_mirConnection;
    MirWindow *m_mirWindow;
    MirRectangle *m_lastRectangle;
};

#endif // MIR_INPUT_REGION_UPDATER_H_

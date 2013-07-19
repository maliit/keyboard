/*
 * This file is part of Maliit Plugins
 *
 * Copyright (C) 2012 One Laptop per Child Association
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

#include "utils.h"
#include "logic/layout.h"
#include "view/renderer.h"
#include "view/glass.h"


#include <maliit/plugins/abstractwidgetssurface.h>
#include <maliit/plugins/abstractsurfacefactory.h>

#include <QtCore>
#include <QtGui>
#include <QtTest>

using namespace MaliitKeyboard;

namespace {
const QSize g_screen_size(200, 100);
bool g_create_overlay_surfaces(false);
}

namespace Maliit {
namespace Plugins {

class SurfaceProbe
    : public virtual AbstractSurface
{
private:
    QSize m_size;
    QPoint m_relpos;
    QGraphicsScene *m_scene;
    QGraphicsView *m_view;
    QGraphicsItem *m_root;

public:
    explicit SurfaceProbe()
        : m_size()
        , m_relpos()
    {}

    virtual ~SurfaceProbe() {}

    //! \reimp
    void show() {}
    void hide() {}

    QSize size() const
    {
        return m_size;
    }

    void setSize(const QSize &size)
    {
        m_size = size;
    }

    QPoint relativePosition() const
    {
        return m_relpos;
    }

    void setRelativePosition(const QPoint &position)
    {
        m_relpos = position;
    }

    QSharedPointer<AbstractSurface> parent() const
    {
        return QSharedPointer<AbstractSurface>();
    }

    QPoint translateEventPosition(const QPoint &event_position,
                                  const QSharedPointer<AbstractSurface> &event_surface = QSharedPointer<AbstractSurface>()) const
    {
        Q_UNUSED(event_surface)
        return event_position;
    }
};

class FactoryProbe
    : public AbstractSurfaceFactory
{
public:
    explicit FactoryProbe() {}
    virtual ~FactoryProbe() {}

    //! \reimp
    QSize screenSize() const
    {
        return g_screen_size;
    }

    bool supported(AbstractSurface::Options options) const
    {
        Q_UNUSED(options);
        return true;
    }

    QSharedPointer<AbstractSurface> create(AbstractSurface::Options options,
                                           const QSharedPointer<AbstractSurface> &parent = QSharedPointer<AbstractSurface>())
    {
        Q_UNUSED(parent)

        if (not g_create_overlay_surfaces
            && (options & AbstractSurface::PositionOverlay)) {
            return QSharedPointer<AbstractSurface>();
        }

        QSharedPointer<AbstractSurface> surface(new SurfaceProbe);
        return surface;
    }
};

class GraphicsViewSurfaceProbe
    : public SurfaceProbe, public AbstractGraphicsViewSurface
{
private:
    QGraphicsScene *m_scene;
    QGraphicsView *m_view;
    QGraphicsItem *m_root;

public:
    explicit GraphicsViewSurfaceProbe()
        : SurfaceProbe()
        , m_scene(new QGraphicsScene)
        , m_view(new QGraphicsView(m_scene))
        , m_root(new QGraphicsRectItem(0))
    {
        m_scene->addItem(m_root);
    }

    virtual ~GraphicsViewSurfaceProbe() {}

    QGraphicsScene * scene() const
    {
        return m_scene;
    }

    QGraphicsView * view() const
    {
        return m_view;
    }

    QGraphicsItem * root() const
    {
        return m_root;
    }

    void clear() {};
};

class GraphicsViewFactoryProbe
    : public FactoryProbe
{
public:
    explicit GraphicsViewFactoryProbe() {}
    virtual ~GraphicsViewFactoryProbe() {}

    //! \reimp
    QSharedPointer<AbstractSurface> create(AbstractSurface::Options options,
                                           const QSharedPointer<AbstractSurface> &parent = QSharedPointer<AbstractSurface>())
    {
        Q_UNUSED(parent)

        if (not g_create_overlay_surfaces
            && (options & AbstractSurface::PositionOverlay)) {
            return QSharedPointer<AbstractSurface>();
        }

        QSharedPointer<AbstractSurface> surface(new GraphicsViewSurfaceProbe);
        return surface;
    }
};

}} // namespace Plugins, Maliit

Q_DECLARE_METATYPE(Maliit::Plugins::AbstractSurfaceFactory *);
Q_DECLARE_METATYPE(Maliit::Plugins::FactoryProbe *);

class TestRendering
    : public QObject
{
    Q_OBJECT

private:
    Q_SLOT void initTestCase()
    {}

    Q_SLOT void testSurfaceFactory_data()
    {
        using namespace Maliit::Plugins;
        AbstractSurfaceFactory *invalid(0);
        AbstractSurfaceFactory *probe(new FactoryProbe);
        AbstractSurfaceFactory *specialized_probe(new GraphicsViewFactoryProbe);

        QTest::addColumn<Maliit::Plugins::AbstractSurfaceFactory *>("factory");
        QTest::addColumn<bool>("create_overlay_surfaces");
        QTest::addColumn<bool>("expected_surface_valid");

        QTest::newRow("invalid factory")
            << invalid << false << false;
        QTest::newRow("invalid factory (w/ overlay surfaces)")
            << invalid << true << false;

        QTest::newRow("use valid, but wrong factory probe")
            << probe << false << false;
        QTest::newRow("use valid, but wrong factory probe (w/ overlay surfaces)")
            << probe << true << false;

        QTest::newRow("use valid, specialized factory probe")
            << specialized_probe << false << true;
        QTest::newRow("use valid, specialized factory probe (w/ overlay surfaces)")
            << specialized_probe << true << true;
    }

    Q_SLOT void testSurfaceFactory()
    {
        QFETCH(Maliit::Plugins::AbstractSurfaceFactory *, factory);
        QFETCH(bool, create_overlay_surfaces);
        QFETCH(bool, expected_surface_valid);

        g_create_overlay_surfaces = create_overlay_surfaces;

        Renderer renderer;
        renderer.setSurfaceFactory(factory);

        Glass glass;
        glass.setSurface(renderer.surface());
        glass.setExtendedSurface(renderer.extendedSurface());

        Logic::Layout layout;
        glass.addLayout(&layout);

        QCOMPARE(not renderer.surface().isNull(), expected_surface_valid);
        QCOMPARE(not renderer.extendedSurface().isNull(), expected_surface_valid && create_overlay_surfaces);

        renderer.show();
        renderer.hide();
    }
};

QTEST_MAIN(TestRendering)
#include "main.moc"

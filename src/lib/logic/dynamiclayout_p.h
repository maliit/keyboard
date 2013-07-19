
//#include "dynamiclayout.h"

namespace MaliitKeyboard {
namespace Logic {

class DynamicLayout;

class DynamicLayoutPrivate {
public:

    struct DynamicLayoutStorage {
        DynamicLayoutStorage()
            : widthStretchFactor(1.0),
              heightStretchFactor(1.0),
              numberOfRows(0)
        {
        }

        int keypadHeight;
        int invisibleTouchAreaHeight;
        int wordRibbonHeight;
        int keyboardHeight; // everything visible incl wordribbon

        QRect windowGeometryRect;
        QRect windowGeometryRectInverted;

        // includes wordribbon
        QRect keyboardRect;

        qreal keyboardScreenWidthRatio;
        qreal keyHeight;
        qreal keyWidthSmall;
        qreal keyWidthMedium;
        qreal keyWidthLarge;
        qreal keyWidthXLarge;
        qreal keyWidthXXLarge;
        qreal keyWidthStretched;
        QMargins keyAreaBorders;
        qreal fontSize;
        qreal fontSizeSmall;
        QByteArray fontColor;
        QByteArray fontFamily;
        qreal widthStretchFactor;
        qreal heightStretchFactor;
        qreal keypadTopMargin;
        qreal keyboardBottomMargin;

        QByteArray keyBackgroundNormal;
        QByteArray keyBackgroundSpecial;
        QByteArray keyBackgroundDead;

        int numberOfRows;
        qreal spaceBetweenRows;
        qreal spaceBetweenKeys;
    };

    DynamicLayoutStorage* storage(LayoutHelper::Orientation orientation)
    {
        initDynamicLayout();

        if (orientation == LayoutHelper::Landscape)
            return landscapeStorage;
        else if (orientation == LayoutHelper::Portrait)
            return portraitStorage;

        return portraitStorage;
    }

    qreal widthStretchFactor(LayoutHelper::Orientation orientation)
    {
        switch (orientation) {
        case LayoutHelper::Landscape:
            return landscapeStorage->widthStretchFactor;
            break;
        case LayoutHelper::Portrait:
            return portraitStorage->widthStretchFactor;
            break;
        default: return 1.0;
        }
    }

    qreal heightStretchFactor(LayoutHelper::Orientation orientation)
    {
        switch (orientation) {
        case LayoutHelper::Landscape:
            return landscapeStorage->heightStretchFactor;
            break;
        case LayoutHelper::Portrait:
            return portraitStorage->heightStretchFactor;
            break;
        default: return 1.0;
        }
    }

    DynamicLayout* q;
    bool initialized;
    bool wordRibbonEnabled;

    DynamicLayoutStorage* portraitStorage;
    DynamicLayoutStorage* landscapeStorage;
    DynamicLayoutStorage* genericStorage;

    DynamicLayoutPrivate(DynamicLayout* _q) :
        q(_q),
        initialized(false),
        wordRibbonEnabled(false),
        portraitStorage(new DynamicLayoutStorage),
        landscapeStorage(new DynamicLayoutStorage),
        genericStorage(new DynamicLayoutStorage)
    {
    }

    // ToDo this needs to be refactored
    void initDynamicLayout()
    {
        q->instance();

        if (!initialized) {
            QQuickView quickView;
            quickView.setSource(QUrl::fromLocalFile(MALIIT_KEYBOARD_DATA_DIR "/maliit-ui-constants.qml"));
            QQuickItem* quickItem = quickView.rootObject();

            const QRect rLandscape = qGuiApp->primaryScreen()->mapBetween(
                        qGuiApp->primaryScreen()->primaryOrientation(),
                        Qt::LandscapeOrientation,
                        QGuiApplication::primaryScreen()->geometry());

            const QRect rInvertedLandscape = qGuiApp->primaryScreen()->mapBetween(
                        qGuiApp->primaryScreen()->primaryOrientation(),
                        Qt::InvertedLandscapeOrientation,
                        QGuiApplication::primaryScreen()->geometry());

            const QRect rPortrait = qGuiApp->primaryScreen()->mapBetween(
                        qGuiApp->primaryScreen()->primaryOrientation(),
                        Qt::PortraitOrientation,
                        QGuiApplication::primaryScreen()->geometry());

            const QRect rInvertedPortrait = qGuiApp->primaryScreen()->mapBetween(
                        qGuiApp->primaryScreen()->primaryOrientation(),
                        Qt::InvertedPortraitOrientation,
                        QGuiApplication::primaryScreen()->geometry());


            // generic

            genericStorage->keyAreaBorders = QMargins(quickItem->property("key_area_borders").toInt(),
                                                      quickItem->property("key_area_borders").toInt(),
                                                      quickItem->property("key_area_borders").toInt(),
                                                      quickItem->property("key_area_borders").toInt()
                                                      );

            genericStorage->fontColor = quickItem->property("font_color").toByteArray();
            genericStorage->fontFamily = quickItem->property("font_family").toByteArray();

            landscapeStorage->wordRibbonHeight = quickItem->property("landscape_wordribbon_height").toInt();
            portraitStorage->wordRibbonHeight = quickItem->property("portrait_wordribbon_height").toInt();

            // portrait

            qreal portraitHeightRatio = quickItem->property("phone_keyboard_height_portrait").toReal();
            if (qGuiApp->primaryScreen()->primaryOrientation() == Qt::LandscapeOrientation)
                portraitHeightRatio = quickItem->property("tablet_keyboard_height_portrait").toReal();

            portraitStorage->invisibleTouchAreaHeight = quickItem->property("portrait_invisible_touch_area").toInt();

            portraitStorage->keypadHeight = rPortrait.height() * portraitHeightRatio;
            portraitStorage->keyboardHeight = portraitStorage->keypadHeight
                    + portraitStorage->wordRibbonHeight;
            portraitStorage->keyboardScreenWidthRatio = quickItem->property("portrait_keyboard_screen_width_ratio").toReal();

            const int portraitKeyboardWidth = rPortrait.width() * portraitStorage->keyboardScreenWidthRatio;
            const int xp = (rPortrait.width() - (portraitKeyboardWidth)) / 2;

            // point of origin differs when primary orientation is different
            int yp = 0;
            if(qGuiApp->primaryScreen()->primaryOrientation() == Qt::PortraitOrientation)
                yp = rPortrait.height() - (portraitStorage->keypadHeight
                                           + portraitStorage->wordRibbonHeight
                                           + portraitStorage->invisibleTouchAreaHeight);

            portraitStorage->windowGeometryRect = QRect(xp, yp, portraitKeyboardWidth,
                                                        portraitStorage->keypadHeight
                                                        + portraitStorage->wordRibbonHeight
                                                        + portraitStorage->invisibleTouchAreaHeight);

            const int invertedPortraitKeyboardWidth = rInvertedPortrait.width() * portraitStorage->keyboardScreenWidthRatio;
            const int xpi = (rInvertedPortrait.width() - (portraitKeyboardWidth)) / 2;

            // point of origin differs when primary orientation is different
            int ypi = 0;
            if(qGuiApp->primaryScreen()->primaryOrientation() == Qt::LandscapeOrientation)
                ypi = rInvertedPortrait.height() - (portraitStorage->keypadHeight
                                                    + portraitStorage->wordRibbonHeight
                                                    + portraitStorage->invisibleTouchAreaHeight);

            portraitStorage->windowGeometryRectInverted = QRect(xpi, ypi, invertedPortraitKeyboardWidth,
                                                                portraitStorage->keypadHeight
                                                                + portraitStorage->wordRibbonHeight
                                                                + portraitStorage->invisibleTouchAreaHeight);

            portraitStorage->keypadTopMargin = quickItem->property("portrait_top_margin").toInt();
            landscapeStorage->keypadTopMargin = 0.02 * rLandscape.height(); //DynamicLayoutHelper->property("landscape_top_margin").toInt();

            portraitStorage->keyboardBottomMargin = quickItem->property("portrait_bottom_margin").toInt();
            landscapeStorage->keyboardBottomMargin = 0.02 * rLandscape.height();//DynamicLayoutHelper->property("landscape_bottom_margin").toInt();

            // landscape

            qreal landscapeHeightRatio = quickItem->property("phone_keyboard_height_landscape").toReal();
            if (qGuiApp->primaryScreen()->primaryOrientation() == Qt::LandscapeOrientation)
                landscapeHeightRatio = quickItem->property("tablet_keyboard_height_landscape").toReal();

            landscapeStorage->invisibleTouchAreaHeight = quickItem->property("landscape_invisible_touch_area").toInt();
            landscapeStorage->keyboardHeight = landscapeStorage->keypadHeight
                    + landscapeStorage->wordRibbonHeight;

            landscapeStorage->keypadHeight = rLandscape.height() * landscapeHeightRatio;

            landscapeStorage->keyboardScreenWidthRatio = quickItem->property("landscape_keyboard_screen_width_ratio").toReal();

            const int landscapeKeyboardWidth = rLandscape.width() * landscapeStorage->keyboardScreenWidthRatio;
            const int xl = (rLandscape.width() - (landscapeKeyboardWidth)) / 2;
            const int yl = rLandscape.height() - (landscapeStorage->keypadHeight
                                                  + landscapeStorage->wordRibbonHeight
                                                  + landscapeStorage->invisibleTouchAreaHeight);

            landscapeStorage->windowGeometryRect = QRect(xl, yl, landscapeKeyboardWidth,
                                                         landscapeStorage->keypadHeight
                                                         + landscapeStorage->wordRibbonHeight
                                                         + landscapeStorage->invisibleTouchAreaHeight);

            const int invertedLandscapeKeyboardWidth = rInvertedLandscape.width() * landscapeStorage->keyboardScreenWidthRatio;
            const int xli = (rInvertedLandscape.width() - (invertedLandscapeKeyboardWidth)) / 2;
            const int yli = 0;

            landscapeStorage->windowGeometryRectInverted = QRect(xli, yli, invertedLandscapeKeyboardWidth,
                                                                 landscapeStorage->keypadHeight
                                                                 + landscapeStorage->wordRibbonHeight
                                                                 + landscapeStorage->invisibleTouchAreaHeight);

            // reference
            const QSize referenceSize(
                        quickItem->property("reference_width").toInt(),
                        quickItem->property("reference_height").toInt());

            landscapeStorage->widthStretchFactor = (qreal) landscapeStorage->windowGeometryRect.width() / referenceSize.width();
            portraitStorage->widthStretchFactor = (qreal) portraitStorage->windowGeometryRect.width() / referenceSize.width();

            genericStorage->keyHeight = quickItem->property("key_height").toReal();
            genericStorage->keyWidthSmall = quickItem->property("key_width_small").toReal();
            genericStorage->keyWidthMedium = quickItem->property("key_width_medium").toReal();
            genericStorage->keyWidthLarge = quickItem->property("key_width_large").toReal();
            genericStorage->keyWidthXLarge = quickItem->property("key_width_xlarge").toReal();
            genericStorage->keyWidthXXLarge = quickItem->property("key_width_xxlarge").toReal();


            landscapeStorage->spaceBetweenRows = quickItem->property("landscape_space_between_rows").toReal();
            landscapeStorage->spaceBetweenKeys = quickItem->property("landscape_space_between_keys").toReal();

            portraitStorage->spaceBetweenRows = quickItem->property("portrait_space_between_rows").toReal();
            portraitStorage->spaceBetweenKeys = quickItem->property("portrait_space_between_keys").toReal();

            // S M L XL XXL Stretched

            genericStorage->fontSize = quickItem->property("font_size").toReal();
            genericStorage->fontSizeSmall = quickItem->property("font_size_small").toReal();

            genericStorage->keyBackgroundNormal = quickItem->property("key_background_normal").toByteArray();
            genericStorage->keyBackgroundSpecial = quickItem->property("key_background_special").toByteArray();
            genericStorage->keyBackgroundDead = quickItem->property("key_background_deadkey").toByteArray();

            initialized = true;

            qDebug() << "landscape         " << rLandscape << landscapeStorage->windowGeometryRect;
            qDebug() << "landscape inverted" << rInvertedLandscape << landscapeStorage->windowGeometryRectInverted;

            qDebug() << "portrait          " << rPortrait << portraitStorage->windowGeometryRect;
            qDebug() << "portrait inverted " << rInvertedPortrait << portraitStorage->windowGeometryRectInverted;
        }
    }
};

}
}

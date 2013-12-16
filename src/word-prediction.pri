# to be included at bottom of .pro files
enable-presage {
#    DEFINES += HAVE_PRESAGE
    LIBS += -lpresage
}

enable-hunspell {
    CONFIG += link_pkgconfig
    PKGCONFIG += hunspell
    DEFINES += HAVE_HUNSPELL
}

enable-pinyin {
    CONFIG += link_pkgconfig
    PKGCONFIG += glib-2.0
    PKGCONFIG += libpinyin
    DEFINES += HAVE_PINYIN
}

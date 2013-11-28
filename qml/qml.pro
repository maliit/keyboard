include(../config.pri)

TARGET = dummy
TEMPLATE = lib

qml.path = $$UBUNTU_KEYBOARD_DATA_DIR
qml.files = *.qml *.js

qml_keys.path = "$$UBUNTU_KEYBOARD_DATA_DIR/keys"
qml_keys.files = keys/*.qml keys/*.js

qml_languages.path = "$$UBUNTU_KEYBOARD_DATA_DIR/languages"
qml_languages.files = languages/*.qml languages/*.js

lang_ar.path = "$$UBUNTU_KEYBOARD_DATA_DIR/languages/ar"
lang_ar.files = languages/ar/*.qml languages/ar/*.js

lang_cs.path = "$$UBUNTU_KEYBOARD_DATA_DIR/languages/cs"
lang_cs.files = languages/cs/*.qml languages/cs/*.js

lang_da.path = "$$UBUNTU_KEYBOARD_DATA_DIR/languages/da"
lang_da.files = languages/da/*.qml languages/da/*.js

lang_de.path = "$$UBUNTU_KEYBOARD_DATA_DIR/languages/de"
lang_de.files = languages/de/*.qml languages/de/*.js

#lang_en.path = "$$UBUNTU_KEYBOARD_DATA_DIR/languages/en"
#lang_en.files = languages/en/*.qml languages/en/*.js

lang_es.path = "$$UBUNTU_KEYBOARD_DATA_DIR/languages/es"
lang_es.files = languages/es/*.qml languages/es/*.js

lang_fi.path = "$$UBUNTU_KEYBOARD_DATA_DIR/languages/fi"
lang_fi.files = languages/fi/*.qml languages/fi/*.js

lang_fr.path = "$$UBUNTU_KEYBOARD_DATA_DIR/languages/fr"
lang_fr.files = languages/fr/*.qml languages/fr/*.js

lang_he.path = "$$UBUNTU_KEYBOARD_DATA_DIR/languages/he"
lang_he.files = languages/he/*.qml languages/he/*.js

lang_hu.path = "$$UBUNTU_KEYBOARD_DATA_DIR/languages/hu"
lang_hu.files = languages/hu/*.qml languages/hu/*.js

lang_it.path = "$$UBUNTU_KEYBOARD_DATA_DIR/languages/it"
lang_it.files = languages/it/*.qml languages/it/*.js

lang_nl.path = "$$UBUNTU_KEYBOARD_DATA_DIR/languages/nl"
lang_nl.files = languages/nl/*.qml languages/nl/*.js

lang_pl.path = "$$UBUNTU_KEYBOARD_DATA_DIR/languages/pl"
lang_pl.files = languages/pl/*.qml languages/pl/*.js

lang_pt.path = "$$UBUNTU_KEYBOARD_DATA_DIR/languages/pt"
lang_pt.files = languages/pt/*.qml languages/pt/*.js

lang_ru.path = "$$UBUNTU_KEYBOARD_DATA_DIR/languages/ru"
lang_ru.files = languages/ru/*.qml languages/ru/*.js

lang_sv.path = "$$UBUNTU_KEYBOARD_DATA_DIR/languages/sv"
lang_sv.files = languages/sv/*.qml languages/sv/*.js

lang_zh.path = "$$UBUNTU_KEYBOARD_DATA_DIR/languages/zh"
lang_zh.files = languages/zh/*.qml languages/zh/*.js


images.path = "$$UBUNTU_KEYBOARD_DATA_DIR/images"
images.files = images/*.png images/*.sci

INSTALLS += qml qml_keys qml_languages images \
    lang_ar \
    lang_cs \
    lang_da \
    lang_de \
#    lang_en \
    lang_es \
    lang_fi \
    lang_fr \
    lang_he \
    lang_hu \
    lang_it \
    lang_nl \
    lang_pl \
    lang_pt \
    lang_ru \
    lang_sv \
    lang_zh

# for QtCreator
OTHER_FILES += \
    maliit-keyboard.qml \
    maliit-keyboard-extended.qml \
    maliit-magnifier.qml \
    Keyboard.qml \
    KeyboardContainer.qml \
    Popper.qml \
    WordRibbon.qml \
    keys/key_constants.js \
    keys/ActionKey.qml \
    keys/CharKey.qml \
    keys/BackspaceKey.qml \
    keys/DropShadow.qml \
    keys/ExtendedKeysSelector.qml \
    keys/KeyPad.qml \
    keys/LanguageKey.qml \
    keys/LanguageMenu.qml \
    keys/Magnifier.qml \
    keys/OneTwoKey.qml \
    keys/PressArea.qml \
    keys/ReturnKey.qml \
    keys/ShiftKey.qml \
    keys/SpaceKey.qml \
    keys/Spacer.qml \
    keys/SymbolShiftKey.qml \
    keys/UrlKey.qml \
    languages/Keyboard_numbers.qml \
    languages/Keyboard_symbols.qml \
    languages/Keyboard_telephone.qml \
    languages/ar/Keyboard_ar.qml \
    languages/ar/Keyboard_ar_email.qml \
    languages/ar/Keyboard_ar_url.qml \
    languages/ar/Keyboard_ar_url_search.qml \
    languages/cs/Keyboard_cs.qml \
    languages/cs/Keyboard_cs_email.qml \
    languages/cs/Keyboard_cs_url.qml \
    languages/cs/Keyboard_cs_url_search.qml \
    languages/da/Keyboard_da.qml \
    languages/da/Keyboard_da_email.qml \
    languages/da/Keyboard_da_url.qml \
    languages/da/Keyboard_da_url_search.qml \
    languages/de/Keyboard_de.qml \
    languages/de/Keyboard_de_email.qml \
    languages/de/Keyboard_de_url.qml \
    languages/de/Keyboard_de_url_search.qml \
#    languages/en/Keyboard_en.qml \
#    languages/en/Keyboard_en_email.qml \
#    languages/en/Keyboard_en_url.qml \
#    languages/en/Keyboard_en_url_search.qml \
    languages/es/Keyboard_es.qml \
    languages/es/Keyboard_es_email.qml \
    languages/es/Keyboard_es_url.qml \
    languages/es/Keyboard_es_url_search.qml \
    languages/fi/Keyboard_fi.qml \
    languages/fi/Keyboard_fi_email.qml \
    languages/fi/Keyboard_fi_url.qml \
    languages/fi/Keyboard_fi_url_search.qml \
    languages/fr/Keyboard_fr.qml \
    languages/fr/Keyboard_fr_email.qml \
    languages/fr/Keyboard_fr_url.qml \
    languages/fr/Keyboard_fr_url_search.qml \
    languages/hu/Keyboard_hu.qml \
    languages/hu/Keyboard_hu_email.qml \
    languages/hu/Keyboard_hu_url.qml \
    languages/hu/Keyboard_hu_url_search.qml \
    languages/he/Keyboard_he.qml \
    languages/he/Keyboard_he_email.qml \
    languages/he/Keyboard_he_url.qml \
    languages/he/Keyboard_he_url_search.qml \
    languages/it/Keyboard_it.qml \
    languages/it/Keyboard_it_email.qml \
    languages/it/Keyboard_it_url.qml \
    languages/it/Keyboard_it_url_search.qml \
    languages/nl/Keyboard_nl.qml \
    languages/nl/Keyboard_nl_email.qml \
    languages/nl/Keyboard_nl_url.qml \
    languages/nl/Keyboard_nl_url_search.qml \
    languages/pl/Keyboard_pl.qml \
    languages/pl/Keyboard_pl_email.qml \
    languages/pl/Keyboard_pl_url.qml \
    languages/pl/Keyboard_pl_url_search.qml \
    languages/pt/Keyboard_pt.qml \
    languages/pt/Keyboard_pt_email.qml \
    languages/pt/Keyboard_pt_url.qml \
    languages/pt/Keyboard_pt_url_search.qml \
    languages/ru/Keyboard_ru.qml \
    languages/ru/Keyboard_ru_email.qml \
    languages/ru/Keyboard_ru_url.qml \
    languages/ru/Keyboard_ru_url_search.qml \
    languages/sv/Keyboard_sv.qml \
    languages/sv/Keyboard_sv_email.qml \
    languages/sv/Keyboard_sv_url.qml \
    languages/sv/Keyboard_sv_url_search.qml \
    languages/zh/Keyboard_zh_cn_pinyin.qml \
    languages/zh/Keyboard_zh_email.qml \
    languages/zh/Keyboard_zh_url.qml \
    languages/zh/Keyboard_zh_url_search.qml

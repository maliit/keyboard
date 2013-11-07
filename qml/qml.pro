include(../config.pri)

TARGET = dummy
TEMPLATE = lib

qml.path = $$UBUNTU_KEYBOARD_DATA_DIR
qml.files = *.qml *.js

qml_keys.path = "$$UBUNTU_KEYBOARD_DATA_DIR/keys"
qml_keys.files = keys/*.qml keys/*.js

qml_languages.path = "$$UBUNTU_KEYBOARD_DATA_DIR/languages"
qml_languages.files = languages/*.qml languages/*.js

lang_de.path = "$$UBUNTU_KEYBOARD_DATA_DIR/languages/de"
lang_de.files = languages/de/*.qml languages/de/*.js

lang_en.path = "$$UBUNTU_KEYBOARD_DATA_DIR/languages/en"
lang_en.files = languages/en/*.qml languages/en/*.js

lang_es.path = "$$UBUNTU_KEYBOARD_DATA_DIR/languages/es"
lang_es.files = languages/es/*.qml languages/es/*.js

lang_fr.path = "$$UBUNTU_KEYBOARD_DATA_DIR/languages/fr"
lang_fr.files = languages/fr/*.qml languages/fr/*.js

lang_it.path = "$$UBUNTU_KEYBOARD_DATA_DIR/languages/it"
lang_it.files = languages/it/*.qml languages/it/*.js

lang_nl.path = "$$UBUNTU_KEYBOARD_DATA_DIR/languages/nl"
lang_nl.files = languages/nl/*.qml languages/nl/*.js

lang_pt.path = "$$UBUNTU_KEYBOARD_DATA_DIR/languages/pt"
lang_pt.files = languages/pt/*.qml languages/pt/*.js

lang_ru.path = "$$UBUNTU_KEYBOARD_DATA_DIR/languages/ru"
lang_ru.files = languages/ru/*.qml languages/ru/*.js

lang_sv.path = "$$UBUNTU_KEYBOARD_DATA_DIR/languages/sv"
lang_sv.files = languages/sv/*.qml languages/sv/*.js

lang_zh_cn.path = "$$UBUNTU_KEYBOARD_DATA_DIR/languages/zh_cn"
lang_zh_cn.files = languages/zh_cn/*.qml languages/zh_cn/*.js


images.path = "$$UBUNTU_KEYBOARD_DATA_DIR/images"
images.files = images/*.png images/*.sci

INSTALLS += qml qml_keys qml_languages images \
    lang_de \
    lang_en \
    lang_es \
    lang_fr \
    lang_it \
    lang_nl \
    lang_pt \
    lang_ru \
    lang_sv \
    lang_zh_cn

# for QtCreator
OTHER_FILES += \
    maliit-keyboard.qml \
    maliit-keyboard-extended.qml \
    maliit-magnifier.qml \
    maliit-ui-constants.qml \
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
    languages/en/Keyboard_en.qml \
    languages/en/Keyboard_en_email.qml \
    languages/en/Keyboard_en_url.qml \
    languages/en/Keyboard_en_url_search.qml \
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
    languages/zh_cn/Keyboard_zh_cn_pinyin.qml \
    languages/pl/Keyboard_pl_email.qml \
    languages/pl/Keyboard_pl_url.qml \
    languages/pl/Keyboard_pl_url_search.qml

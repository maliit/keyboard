#!/bin/sh

adb shell sudo -iu phablet gsettings set com.lomiri.keyboard.maliit auto-capitalization false
adb shell sudo -iu phablet gsettings set com.lomiri.keyboard.maliit auto-completion false
adb shell sudo -iu phablet gsettings set com.lomiri.keyboard.maliit key-press-feedback false
adb shell sudo -iu phablet gsettings set com.lomiri.keyboard.maliit predictive-text false
adb shell sudo -iu phablet gsettings set com.lomiri.keyboard.maliit spell-checking false
adb shell sudo -iu phablet gsettings set com.lomiri.keyboard.maliit enabled-languages "['en_us', 'es', 'fr', 'pt', 'de', 'zh_cn_pinyin']"

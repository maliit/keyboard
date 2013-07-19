
#include "coreutils.h"

#include "models/key.h"

namespace MaliitKeyboard {
namespace CoreUtils {
namespace {

const char *const g_action_key_id = "actionKey";

} // unnamed namespace

const QString &pluginDataDirectory()
{
    static const QByteArray env_data_directory = qgetenv("MALIIT_PLUGINS_DATADIR");
    static const QString data_directory = QString::fromUtf8(env_data_directory.isEmpty()
                                                            ? MALIIT_PLUGINS_DATA_DIR
                                                            : env_data_directory);

    return data_directory;
}

const QString &maliitKeyboardDataDirectory()
{
    static const QByteArray env_data_directory = qgetenv("MALIIT_KEYBOARD_DATADIR");
    static const QString data_directory = QString::fromUtf8(env_data_directory.isEmpty()
                                                            ? MALIIT_KEYBOARD_DATA_DIR
                                                            : env_data_directory);

    return data_directory;
}

const QString &maliitKeyboardStyleProfilesDirectory()
{
    static const QString styles_profiles_directory(maliitKeyboardDataDirectory() + "/styles");
    return styles_profiles_directory;
}

QString idFromKey(const Key &key)
{
    switch (key.action()) {
    case Key::ActionReturn:
        return g_action_key_id;

    case Key::ActionInsert:
        return key.label().text();

    default:
        // TODO: handle more key actions if needed.
        return QString();
    }
}

}} // namespace CoreUtils, MaliitKeyboard

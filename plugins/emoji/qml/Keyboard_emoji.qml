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

import QtQuick 2.4
import QtQuick.LocalStorage 2.0
import keys 1.0
import "emoji.js" as Emoji

KeyPad {
    anchors.fill: parent

    content: c1
    symbols: "languages/Keyboard_symbols.qml"

    Component.onCompleted: {
        panel.switchBack = true;
    }

    QtObject {
        id: internal
        property int offset: 0
        property int maxRecent: 40
        property var chars
        property var db
        
        Component.onCompleted: {
            db = LocalStorage.openDatabaseSync("Emoji", "1.0", "Storage for emoji keyboard layout", 1000000);

            db.transaction(
                function(tx) {
                    // Create the database if it doesn't already exist
                    tx.executeSql('CREATE TABLE IF NOT EXISTS Recent(emoji VARCHAR(16), time TIMESTAMP DEFAULT CURRENT_TIMESTAMP)');
                    tx.executeSql('CREATE TABLE IF NOT EXISTS State(contentX INTEGER)');

                    var recentEmoji = [];
                    var rs = tx.executeSql('SELECT emoji FROM Recent ORDER BY time ASC');
                    if (rs.rows.length == 0) {
                        // Pre-populate recent list with the most popular emoji
                        var popularEmoji = ['😂', '♥', '❤', '😍', '😒',
                                            '😊', '😭', '😘', '💕', '☺',
                                            '😩', '👌', '😔', '😏', '😁',
                                            '♻', '😉', '👍', '🙏', '🙈',
                                            '😎', '😢', '✌', '👀', '😅',
                                            '✨', '😴', '😄', '💜', '💯',
                                            '💔', '😑', '💖', '💙', '😕',
                                            '💁', '😜', '😞', '😋', '😐'];
                        for (var i = 0; i < popularEmoji.length; i++) {
                            tx.executeSql('INSERT INTO Recent (emoji) VALUES (?)', popularEmoji[i]);
                        }
                        recentEmoji = popularEmoji;
                    } else {
                        for (var i = 0; i < rs.rows.length; i++) {
                            recentEmoji.push(rs.rows.item(i).emoji);
                        }
                    }
                    internal.chars = recentEmoji.concat(Emoji.emoji);

                    for (var i = 0; i < internal.chars.length; i++) {
                        c1.model.append({char: internal.chars[i]});                        
                    }

                    rs = tx.executeSql('SELECT contentX FROM State');
                    if (rs.rows.length > 0) {
                        c1.contentX = rs.rows.item(0).contentX;
                    } else {
                        tx.executeSql('INSERT INTO State VALUES(0)');
                        // Start on the smiley page
                        c1.positionViewAtIndex(40, GridView.Beginning)
                        internal.updatePositionDb();
                    }
                }
            );
        }

        function updatePositionDb() {
            db.transaction(
                function(tx) {
                    tx.executeSql('UPDATE State SET contentX=?', c1.contentX);
                }
            );
        }

        function updateRecent(emoji) {
            // Hide the magnifier before we reposition the key
            magnifier.shown = false;
            magnifier.currentlyAssignedKey = null;
            db.transaction(
                function(tx) {
                    // If this emoji is already in the recent list we just
                    // move it to the top of the list, otherwise we add it
                    // to the start and delete the oldest one
                    var position = -1;
                    for (var i = 0; i < maxRecent; i++) {
                        if (c1.model.get(i).char == emoji) {
                            position = i;
                            break;
                        }
                    }
                    if (position != -1) {
                        c1.model.move(position, 0, 1);
                    } else {
                        c1.model.move(maxRecent - 1, 0, 1);
                        c1.model.setProperty(0, "char", emoji);
                    }
                    tx.executeSql('DELETE FROM Recent WHERE emoji = ?', emoji);
                    tx.executeSql('INSERT INTO Recent(emoji) VALUES(?)', emoji);
                    var rs = tx.executeSql('SELECT COUNT(emoji) as totalRecent FROM Recent');
                    if (rs.rows.item(0).totalRecent > maxRecent) {
                        tx.executeSql('DELETE FROM Recent ORDER BY time ASC LIMIT ?', rs.rows.item(0).totalRecent - maxRecent);
                    }
                }
            );     
        }
    }

    GridView {
        id: c1
        objectName: "emojiGrid"
        property int lastVisibleIndex: indexAt(contentX + (width / 2), 0);
        property int numberOfRows: 5
        property int maxNrOfKeys: 10
        anchors.top: parent.top
        anchors.bottom: categories.top
        anchors.left: parent.left
        anchors.right: parent.right
        model: ListModel { }
        flow: GridView.FlowTopToBottom
        flickDeceleration: units.gu(500)
        snapMode: GridView.SnapToRow
        cellWidth: panel.keyWidth
        cellHeight: panel.keyHeight
        cacheBuffer: 1000
        onContentXChanged: {
            magnifier.shown = false;
            magnifier.currentlyAssignedKey = null;
        }
        onMovementEnded: {
            internal.updatePositionDb();
        }
        delegate: CharKey {
            label: model.char
            shifted: label
            normalColor: UI.backgroundColor
            pressedColor: UI.backgroundColor
            fontSize: fullScreenItem.landscape ? height / 1.8 : height / 2.5
            onReleased: {
                internal.updateRecent(label)
            }
        }

     }

     Row {
        id: categories
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        height: panel.keyHeight

        spacing: 0

        LanguageKey {
            id: languageMenuButton
            label: "ABC"
            shifted: label
            normalColor: UI.backgroundColor
            pressedColor: UI.backgroundColor
        }

        CategoryKey {
            label: "⏱"
            highlight: c1.lastVisibleIndex < internal.maxRecent
            onPressed: {
                if (maliit_input_method.useHapticFeedback)
                    pressEffect.start();
                c1.positionViewAtIndex(0, GridView.Beginning)
                internal.updatePositionDb();
            }
        }           
 
        CategoryKey {
            label: "😀"
            highlight: c1.lastVisibleIndex >= internal.maxRecent && c1.lastVisibleIndex < 540 + internal.maxRecent
            onPressed: {
                if (maliit_input_method.useHapticFeedback)
                    pressEffect.start();
                c1.positionViewAtIndex(internal.maxRecent, GridView.Beginning)
                internal.updatePositionDb();
            }
        }

        CategoryKey {
            label: "🐶"
            highlight: c1.lastVisibleIndex >= 540 + internal.maxRecent && c1.lastVisibleIndex < 701 + internal.maxRecent
            onPressed: {
                if (maliit_input_method.useHapticFeedback)
                    pressEffect.start();
                c1.positionViewAtIndex(540 + internal.maxRecent, GridView.Beginning)
                internal.updatePositionDb();
            }
        }

        CategoryKey {
            label: "🍏"
            highlight: c1.lastVisibleIndex >= 701 + internal.maxRecent && c1.lastVisibleIndex < 786 + internal.maxRecent
            onPressed: {
                if (maliit_input_method.useHapticFeedback)
                    pressEffect.start();
                c1.positionViewAtIndex(701 + internal.maxRecent, GridView.Beginning)
                internal.updatePositionDb();
            }
        }

        CategoryKey {
            label: "🎾"
            highlight: c1.lastVisibleIndex >= 786 + internal.maxRecent && c1.lastVisibleIndex < 931 + internal.maxRecent
            onPressed: {
                if (maliit_input_method.useHapticFeedback)
                    pressEffect.start();
                c1.positionViewAtIndex(786 + internal.maxRecent, GridView.Beginning)
                internal.updatePositionDb();
            }
        }

        CategoryKey {
            label: "🚗"
             highlight: c1.lastVisibleIndex >= 931 + internal.maxRecent && c1.lastVisibleIndex < 1050 + internal.maxRecent
            onPressed: {
                if (maliit_input_method.useHapticFeedback)
                    pressEffect.start();
                c1.positionViewAtIndex(931 + internal.maxRecent, GridView.Beginning)
                internal.updatePositionDb();
            }
        }

        CategoryKey {
            label: "💡"
            highlight: c1.lastVisibleIndex >= 1050 + internal.maxRecent && c1.lastVisibleIndex < 1230 + internal.maxRecent
            onPressed: {
                if (maliit_input_method.useHapticFeedback)
                    pressEffect.start();
                c1.positionViewAtIndex(1050 + internal.maxRecent, GridView.Beginning)
                internal.updatePositionDb();
            }
        }

        CategoryKey {
            label: "❤"
            highlight: c1.lastVisibleIndex >= 1230 + internal.maxRecent && c1.lastVisibleIndex < 1514 + internal.maxRecent
            onPressed: {
                if (maliit_input_method.useHapticFeedback)
                    pressEffect.start();
                c1.positionViewAtIndex(1230 + internal.maxRecent, GridView.Beginning)
                internal.updatePositionDb();
            }
        }

        CategoryKey {
            label: "🌍"
            highlight: c1.lastVisibleIndex >= 1514 + internal.maxRecent
            onPressed: {
                if (maliit_input_method.useHapticFeedback)
                    pressEffect.start();
                c1.positionViewAtIndex(1514 + internal.maxRecent, GridView.Beginning)
                internal.updatePositionDb();
            }
        }

        BackspaceKey {
            padding: 0
            normalColor: UI.backgroundColor
            pressedColor: UI.backgroundColor
        }
    }
}

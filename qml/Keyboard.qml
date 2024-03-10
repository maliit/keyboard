/*
 * This file is part of Maliit plugins
 *
 * Copyright (C) 2012 Openismus GmbH
 *
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

import QtQuick 2.4
import QtQuick.Controls 2.4

import MaliitKeyboard 2.0

import "keys/"

Item {
    id: fullScreenItem
    objectName: "fullScreenItem"

    property bool landscape: width > height
    readonly property bool tablet: landscape ? width >= Device.gu(90) : height >= Device.gu(90)

    property bool cursorSwipe: false
    property int prevSwipePositionX
    property int prevSwipePositionY
    property int cursorSwipeDuration: 5000
    property var timerSwipe: swipeTimer

    property variant input_method: Keyboard
    property variant event_handler: MaliitEventHandler

    onXChanged: fullScreenItem.reportKeyboardVisibleRect();
    onYChanged: fullScreenItem.reportKeyboardVisibleRect();
    onWidthChanged: fullScreenItem.reportKeyboardVisibleRect();
    onHeightChanged: fullScreenItem.reportKeyboardVisibleRect();
    
    Item {
        id: canvas
        objectName: "MaliitKeyboard" // Allow us to specify a specific keyboard within autopilot.

        anchors.bottom: parent.bottom
        anchors.left: parent.left

        width: parent.width
        height: fullScreenItem.height * (fullScreenItem.landscape ? Device.keyboardHeightLandscape
                                                                  : Device.keyboardHeightPortrait)

        visible: true

        property bool wordribbon_visible: WordEngine.enabled
        onWordribbon_visibleChanged: fullScreenItem.reportKeyboardVisibleRect();

        property bool languageMenuShown: false
        property alias languageMenu: languageMenu
        property bool extendedKeysShown: false

        property bool firstShow: true
        property bool hidingComplete: false

        property string layoutId: "freetext"

        onXChanged: fullScreenItem.reportKeyboardVisibleRect();
        onYChanged: fullScreenItem.reportKeyboardVisibleRect();
        onWidthChanged: fullScreenItem.reportKeyboardVisibleRect();
        onHeightChanged: fullScreenItem.reportKeyboardVisibleRect();

        opacity: Keyboard.opacity

        MouseArea {
            id: swipeArea

            property int jumpBackThreshold: Device.gu(10)

            anchors.fill: parent

            drag.target: keyboardSurface
            drag.axis: Drag.YAxis;
            drag.minimumY: 0
            drag.maximumY: parent.height
            //fix for lp:1277186
            //only filter children when wordRibbon visible
            drag.filterChildren: wordRibbon.visible
            // Avoid conflict with extended key swipe selection and cursor swipe mode
            enabled: !canvas.extendedKeysShown && !fullScreenItem.cursorSwipe

            onReleased: {
                if (keyboardSurface.y > jumpBackThreshold) {
                    MaliitGeometry.shown = false;
                } else {
                    bounceBackAnimation.from = keyboardSurface.y
                    bounceBackAnimation.start();
                }
            }

            Page {
                id: keyboardSurface
                objectName: "keyboardSurface"

                x:0
                y:0
                width: parent.width
                height: canvas.height

                onXChanged: fullScreenItem.reportKeyboardVisibleRect();
                onYChanged: fullScreenItem.reportKeyboardVisibleRect();
                onWidthChanged: fullScreenItem.reportKeyboardVisibleRect();
                onHeightChanged: fullScreenItem.reportKeyboardVisibleRect();

                WordRibbon {
                    id: wordRibbon
                    objectName: "wordRibbon"

                    // Hide thte word ribbon when in emoji or cursor mode
                    visible: !fullScreenItem.cursorSwipe && canvas.wordribbon_visible && keypad.state !== "EMOJI"

                    anchors.bottom: keyboardComp.top
                    width: parent.width;

                    // Use a size proportional to the height of keyboard keys
                    height: visible ? keypad.keyHeight * 0.5 : 0
                    onHeightChanged: fullScreenItem.reportKeyboardVisibleRect();
                }
                
                ActionsToolbar {
                    id: toolbar
                    objectName: "actionsToolbar"
                    
                    visible: fullScreenItem.cursorSwipe
                    height: Device.wordRibbonHeight
                }
                    
                Rectangle {
                    width: parent.width
                    height: 1
                    color: "#888888"
                    anchors.bottom: wordRibbon.visible ? wordRibbon.top : keyboardComp.top
                }

                Item {
                    id: keyboardComp
                    objectName: "keyboardComp"

                    visible: !fullScreenItem.cursorSwipe
                    height: parent.height
                    width: parent.width
                    anchors.bottom: parent.bottom

                    onHeightChanged: fullScreenItem.reportKeyboardVisibleRect();

                    KeyboardContainer {
                        id: keypad

                        anchors.fill: parent
                        anchors.topMargin: wordRibbon.visible ? 0 : Device.top_margin
                        anchors.bottomMargin: Device.bottom_margin
                    }

                    LanguageMenu {
                        id: languageMenu
                        objectName: "languageMenu"
                        anchors.centerIn: parent
                        height: contentHeight > keypad.height ? keypad.height : contentHeight
                        width: Device.gu(30);
                    }
                } // keyboardComp
            }
        }

        PropertyAnimation {
            id: bounceBackAnimation
            // Animations don't have an "enabled" property, so just set the
            // target to null if animation is disabled, which effectively also
            // disables the animation.
            target: Keyboard.animationEnabled ? keyboardSurface : null
            properties: "y"
            easing.type: Easing.OutBounce;
            easing.overshoot: 2.0
            to: 0
        }

        state: "HIDDEN"

        states: [
            State {
                name: "SHOWN"
                PropertyChanges { target: keyboardSurface; y: 0; }
                onCompleted: {
                    canvas.firstShow = false;
                    canvas.hidingComplete = false;
                }
                when: MaliitGeometry.shown === true
            },

            State {
                name: "HIDDEN"
                PropertyChanges { target: keyboardSurface; y: canvas.height }
                onCompleted: {
                    canvas.languageMenu.close();
                    keypad.closeExtendedKeys();
                    keypad.activeKeypadState = "NORMAL";
                    keypad.state = "CHARACTERS";
                    Keyboard.close();
                    canvas.hidingComplete = true;
                    reportKeyboardVisibleRect();
                    
                    // Exit cursor swipe mode when the keyboard hides
                    fullScreenItem.exitSwipeMode();
                }
                // Wait for the first show operation to complete before
                // allowing hiding, as the conditions when the keyboard
                // has never been visible can trigger a hide operation
                when: MaliitGeometry.shown === false && canvas.firstShow === false
            }
        ]
        transitions: Transition {
            enabled: Keyboard.animationEnabled
            NumberAnimation { target: keyboardSurface; properties: "y"; duration: 165}
        }

        Connections {
            target: input_method
            function onActivateAutocaps() {
                if (keypad.state == "CHARACTERS" && keypad.activeKeypadState != "CAPSLOCK" && !cursorSwipe) {
                    keypad.activeKeypadState = "SHIFTED";
                    keypad.autoCapsTriggered = true;
                } else {
                    keypad.delayedAutoCaps = true;
                }
            }

            function onKeyboardReset() {
                keypad.state = "CHARACTERS"
            }
            function onDeactivateAutocaps() {
                if(keypad.autoCapsTriggered) {
                    keypad.activeKeypadState = "NORMAL";
                    keypad.autoCapsTriggered = false;
                }
                keypad.delayedAutoCaps = false;
            }
        }
        
        FloatingActions {
            id: floatingActions
            objectName: "floatingActions"
            
            z: 1
            visible: fullScreenItem.cursorSwipe && !cursorSwipeArea.pressed
        }

        MouseArea {
            id: cursorSwipeArea

            property point firstPress
            property point lastRelease
            property bool selectionMode: false
            
            anchors {
                fill: parent
                topMargin: toolbar.height
            }
            
            enabled: cursorSwipe

            // An invisible text field to be able to get selection colors from
            // thte qqc2 style in use, for selection mode
            TextField {
                id: textArea
                width: 0
                height: 0
                visible: false
            }

            Rectangle {
                anchors.fill: parent
                visible: parent.enabled
                color: cursorSwipeArea.selectionMode ? textArea.selectionColor : "#888888"
                
                Label {
                    visible: !cursorSwipeArea.pressed
                    horizontalAlignment: Text.AlignHCenter
                    // FIXME: selected font color should differ
                    color: cursorSwipeArea.selectionMode ? textArea.selectedTextColor : "#313131"
                    wrapMode: Text.WordWrap
                    
                    anchors {
                        left: parent.left
                        right: parent.right
                        verticalCenter: parent.verticalCenter
                    }
                    
                    text: cursorSwipeArea.selectionMode ? qsTr("Swipe to move selection") + "\n\n" + qsTr("Double-tap to exit selection mode")
                                : qsTr("Swipe to move cursor") + "\n\n" + qsTr("Double-tap to enter selection mode")
                }
            }
            
            function exitSelectionMode() {
                selectionMode = false
                fullScreenItem.timerSwipe.restart()
            }
            
            onSelectionModeChanged: {
                if (fullScreenItem.cursorSwipe) {
                    Feedback.keyPressed();
                }
            }
            
            onMouseXChanged: {
                processSwipe(mouseX, mouseY)
            }

            onMouseYChanged: {
                processSwipe(mouseX, mouseY)
            }

            onPressed: {
                prevSwipePositionX = mouseX
                prevSwipePositionY = mouseY
                fullScreenItem.timerSwipe.stop()

                if (firstPress === Qt.point(0,0)) {
                    firstPress = Qt.point(mouse.x, mouse.y)
                }
            }

            onReleased: {
                if (!cursorSwipeArea.selectionMode) {
                    fullScreenItem.timerSwipe.restart()
                    firstPress = Qt.point(0,0)
                } else {
                    fullScreenItem.timerSwipe.stop()
                    
                    // TODO: Disabled word selection until input_method.hasSelection is fixed in QtWebEngine
                    // ubports/ubuntu-touch#1157 <https://github.com/ubports/ubuntu-touch/issues/1157>
                    /*
                    if(!input_method.hasSelection){
                        fullScreenItem.selectWord()
                        cursorSwipeArea.selectionMode = false
                        fullScreenItem.timerSwipe.restart()
                    }
                    */
                }

                lastRelease = Qt.point(mouse.x, mouse.y)
            }
            
            onDoubleClicked: {
                // We avoid triggering double click accidentally by using a threshold
                var xReleaseDiff = Math.abs(lastRelease.x - mouse.x)
                var yReleaseDiff = Math.abs(lastRelease.y - mouse.y)

                var threshold = Device.gu(2)

                if (xReleaseDiff < threshold && yReleaseDiff < threshold) {
                    if (!cursorSwipeArea.selectionMode) {
                        cursorSwipeArea.selectionMode = true
                        fullScreenItem.timerSwipe.stop()
                    } else {
                        exitSelectionMode();
                    }
                }

                firstPress = Qt.point(0,0)
            }
        }
    } // canvas

    Timer {
        id: swipeTimer
        interval: cursorSwipeDuration
        running: false
        onTriggered: {
            fullScreenItem.exitSwipeMode();
        }
    }
    
    onCursorSwipeChanged:{
        if (cursorSwipe && input_method.hasSelection) {
            cursorSwipeArea.selectionMode = true
        }
        
        Feedback.keyPressed();
    }
    
    function exitSwipeMode() {
        fullScreenItem.cursorSwipe = false
        fullScreenItem.timerSwipe.stop()
        cursorSwipeArea.selectionMode = false
        
        // We only enable autocaps after cursor movement has stopped
        if (keypad.delayedAutoCaps) {
            keypad.activeKeypadState = "SHIFTED"
            keypad.delayedAutoCaps = false
        } else {
            keypad.activeKeypadState = "NORMAL"
        }
    }

    function reportKeyboardVisibleRect() {

        var vx = 0;
        var vy = wordRibbon.y;
        var vwidth = keyboardSurface.width;
        var vheight = keyboardComp.height + wordRibbon.height;

        var obj = mapFromItem(keyboardSurface, vx, vy, vwidth, vheight);
        // Report visible height of the keyboard to support anchorToKeyboard
        obj.height = fullScreenItem.height - obj.y;

        // Work around QT bug: https://bugreports.qt-project.org/browse/QTBUG-20435
        // which results in a 0 height being reported incorrectly immediately prior
        // to the keyboard closing animation starting, which causes us to report
        // an extra visibility change for the keyboard.
        if (obj.height <= 0 && !canvas.hidingComplete) {
            return;
        }

        MaliitGeometry.visibleRect = Qt.rect(obj.x, obj.y, obj.width, obj.height);
    }

    // Autopilot needs to be able to move the cursor even when the layout
    // doesn't provide arrow keys (e.g. in phone mode)
    function sendLeftKey() {
        event_handler.onKeyReleased("", "left");
    }
    function sendRightKey() {
        event_handler.onKeyReleased("", "right");
    }
    function sendUpKey() {
        event_handler.onKeyReleased("", "up");
    }
    function sendDownKey() {
        event_handler.onKeyReleased("", "down");
    }
    function sendHomeKey() {
        event_handler.onKeyReleased("", "home");
    }
    function sendEndKey() {
        event_handler.onKeyReleased("", "end");
    }
    function selectLeft() {
        event_handler.onKeyReleased("SelectPreviousChar", "keysequence");
    }
    function selectRight() {
        event_handler.onKeyReleased("SelectNextChar", "keysequence");
    }
    function selectUp() {
        event_handler.onKeyReleased("SelectPreviousLine", "keysequence");
    }
    function selectDown() {
        event_handler.onKeyReleased("SelectNextLine", "keysequence");
    }
    function selectWord() {
        event_handler.onKeyReleased("MoveToPreviousWord", "keysequence");
        event_handler.onKeyReleased("SelectNextWord", "keysequence");
    }
    function selectStartOfLine() {
        event_handler.onKeyReleased("SelectStartOfLine", "keysequence");
    }
    function selectEndOfLine() {
        event_handler.onKeyReleased("SelectEndOfLine", "keysequence");
    }
    function selectStartOfDocument() {
        event_handler.onKeyReleased("SelectStartOfDocument", "keysequence");
    }
    function selectEndOfDocument() {
        event_handler.onKeyReleased("SelectEndOfDocument", "keysequence");
    }
    function selectAll() {
        event_handler.onKeyReleased("SelectAll", "keysequence");
    }
    function moveToStartOfLine() {
        event_handler.onKeyReleased("MoveToStartOfLine", "keysequence");
    }
    function moveToEndOfLine() {
        event_handler.onKeyReleased("MoveToEndOfLine", "keysequence");
    }
    function moveToStartOfDocument() {
        event_handler.onKeyReleased("MoveToStartOfDocument", "keysequence");
    }
    function moveToEndOfDocument() {
        event_handler.onKeyReleased("MoveToEndOfDocument", "keysequence");
    }
    function redo() {
        event_handler.onKeyReleased("Redo", "keysequence");
    }
    function undo() {
        event_handler.onKeyReleased("Undo", "keysequence");
    }
    function paste() {
        event_handler.onKeyReleased("Paste", "keysequence");
    }
    function copy() {
        event_handler.onKeyReleased("Copy", "keysequence");
    }
    function cut() {
        event_handler.onKeyReleased("Cut", "keysequence");
    }

    function processSwipe(positionX, positionY) {
        if (positionX < prevSwipePositionX - Device.gu(1)) {
            if(cursorSwipeArea.selectionMode){
                selectLeft();
            }else{
                sendLeftKey();
            }
            prevSwipePositionX = positionX
        } else if (positionX > prevSwipePositionX + Device.gu(1)) {
            if(cursorSwipeArea.selectionMode){
                selectRight();
            }else{
                sendRightKey();
            }
            prevSwipePositionX = positionX
        }
        if (positionY < prevSwipePositionY - Device.gu(4)) {
            if(cursorSwipeArea.selectionMode){
                selectUp();
            }else{
                sendUpKey();
            }
            prevSwipePositionY = positionY
        } else if (positionY > prevSwipePositionY + Device.gu(4)) {
            if(cursorSwipeArea.selectionMode){
                selectDown();
            }else{
                sendDownKey();
            }
            prevSwipePositionY = positionY
        }
    }

} // fullScreenItem

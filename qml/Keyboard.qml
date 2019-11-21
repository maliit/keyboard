/*
 * This file is part of Maliit plugins
 *
 * Copyright (C) 2012 Openismus GmbH
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

import QtQuick 2.4
import "constants.js" as Const
import "theme_loader.js" as Theme
import "keys/"
import "keys/key_constants.js" as UI
import Ubuntu.Components 1.3
import QtFeedback 5.0
import QtMultimedia 5.0

Item {
    id: fullScreenItem
    objectName: "fullScreenItem"

    property bool landscape: width > height
    readonly property bool tablet: landscape ? width >= units.gu(90) : height >= units.gu(90)

    property bool cursorSwipe: false
    property int prevSwipePositionX
    property int prevSwipePositionY
    property int cursorSwipeDuration: 5000
    property var timerSwipe: swipeTimer
    property var theme: Theme.defaultTheme

    property variant input_method: maliit_input_method
    property variant event_handler: maliit_event_handler

    onXChanged: fullScreenItem.reportKeyboardVisibleRect();
    onYChanged: fullScreenItem.reportKeyboardVisibleRect();
    onWidthChanged: fullScreenItem.reportKeyboardVisibleRect();
    onHeightChanged: fullScreenItem.reportKeyboardVisibleRect();
    
    Component.onCompleted: Theme.load(maliit_input_method.theme)

    Item {
        id: canvas
        objectName: "ubuntuKeyboard" // Allow us to specify a specific keyboard within autopilot.

        anchors.bottom: parent.bottom
        anchors.left: parent.left

        width: parent.width
        height: fullScreenItem.height * (fullScreenItem.landscape ? fullScreenItem.tablet ? UI.tabletKeyboardHeightLandscape 
                                                                                          : UI.phoneKeyboardHeightLandscape
                                                                  : fullScreenItem.tablet ? UI.tabletKeyboardHeightPortrait 
                                                                                          : UI.phoneKeyboardHeightPortrait)
                                      + wordRibbon.height + borderTop.height

        property int keypadHeight: height;

        visible: true

        property bool wordribbon_visible: maliit_word_engine.enabled
        onWordribbon_visibleChanged: fullScreenItem.reportKeyboardVisibleRect();

        property bool languageMenuShown: false
        property bool extendedKeysShown: false

        property bool firstShow: true
        property bool hidingComplete: false

        property string layoutId: "freetext"

        onXChanged: fullScreenItem.reportKeyboardVisibleRect();
        onYChanged: fullScreenItem.reportKeyboardVisibleRect();
        onWidthChanged: fullScreenItem.reportKeyboardVisibleRect();
        onHeightChanged: fullScreenItem.reportKeyboardVisibleRect();

        opacity: maliit_input_method.opacity

        MouseArea {
            id: swipeArea

            property int jumpBackThreshold: units.gu(10)

            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: parent.top
            height: (parent.height - canvas.keypadHeight) + wordRibbon.height +
                    borderTop.height

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
                    maliit_geometry.shown = false;
                } else {
                    bounceBackAnimation.from = keyboardSurface.y
                    bounceBackAnimation.start();
                }
            }

            Item {
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

                Rectangle {
                    width: parent.width
                    height: units.dp(1)
                    color: fullScreenItem.theme.dividerColor
                    anchors.bottom: wordRibbon.visible ? wordRibbon.top : keyboardComp.top
                }

                WordRibbon {
                    id: wordRibbon
                    objectName: "wordRibbon"

                    visible: canvas.wordribbon_visible

                    anchors.bottom: keyboardComp.top
                    width: parent.width;

                    height: canvas.wordribbon_visible ? (fullScreenItem.tablet ? units.gu(UI.tabletWordribbonHeight)
                                                                               : units.gu(UI.phoneWordribbonHeight))
                                                      : 0
                    onHeightChanged: fullScreenItem.reportKeyboardVisibleRect();
                }
                //TODO: Sets the theme for all UITK components used in the OSK. Replace those components to remove the need for this.
                ActionItem {
                    id: dummy
                    
                    visible: false
                    theme.name: fullScreenItem.theme.toolkitTheme
                }                
                
                ActionsToolbar {
                    id: toolbar
                    objectName: "actionsToolbar"
                    
                    z: 1
                    visible: fullScreenItem.cursorSwipe
                    height: fullScreenItem.tablet ? units.gu(UI.tabletWordribbonHeight) : units.gu(UI.phoneWordribbonHeight)
                    state: wordRibbon.visible ? "wordribbon" : "top"
                }
                    

                Item {
                    id: keyboardComp
                    objectName: "keyboardComp"

                    height: canvas.keypadHeight - wordRibbon.height + keypad.anchors.topMargin
                    width: parent.width
                    anchors.bottom: parent.bottom

                    onHeightChanged: fullScreenItem.reportKeyboardVisibleRect();

                    Rectangle {
                        id: background

                        anchors.fill: parent

                        color: fullScreenItem.theme.backgroundColor
                    }
                
                    Item {
                        id: borderTop
                        width: parent.width
                        anchors.top: parent.top.bottom
                        height: wordRibbon.visible ? 0 : units.gu(UI.top_margin)
                    }

                    KeyboardContainer {
                        id: keypad

                        anchors.top: borderTop.bottom
                        anchors.bottom: background.bottom
                        anchors.bottomMargin: units.gu(UI.bottom_margin)
                        width: parent.width
                        hideKeyLabels: fullScreenItem.cursorSwipe

                        onPopoverEnabledChanged: fullScreenItem.reportKeyboardVisibleRect();
                    }

                    LanguageMenu {
                        id: languageMenu
                        objectName: "languageMenu"
                        anchors.centerIn: parent
                        height: contentHeight > keypad.height ? keypad.height : contentHeight
                        width: units.gu(30);
                        enabled: canvas.languageMenuShown
                        visible: canvas.languageMenuShown
                    }
                } // keyboardComp
            }
        }

        PropertyAnimation {
            id: bounceBackAnimation
            target: keyboardSurface
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
                when: maliit_geometry.shown === true
            },

            State {
                name: "HIDDEN"
                PropertyChanges { target: keyboardSurface; y: canvas.height }
                onCompleted: {
                    canvas.languageMenuShown = false;
                    keypad.closeExtendedKeys();
                    keypad.activeKeypadState = "NORMAL";
                    keypad.state = "CHARACTERS";
                    maliit_input_method.close();
                    canvas.hidingComplete = true;
                    reportKeyboardVisibleRect();
                    // Switch back to the previous layout if we're in
                    // in a layout like emoji that requests switchBack
                    if (keypad.switchBack && maliit_input_method.previousLanguage) {
                        keypad.switchBack = false;
                        maliit_input_method.activeLanguage = maliit_input_method.previousLanguage;
                    }
                    
                    // Exit cursor swipe mode when the keyboard hides
                    fullScreenItem.exitSwipeMode();
                }
                // Wait for the first show operation to complete before
                // allowing hiding, as the conditions when the keyboard
                // has never been visible can trigger a hide operation
                when: maliit_geometry.shown === false && canvas.firstShow === false
            }
        ]
        transitions: Transition {
            UbuntuNumberAnimation { target: keyboardSurface; properties: "y"; }
        }

        Connections {
            target: input_method
            onActivateAutocaps: {
                if (keypad.state == "CHARACTERS" && keypad.activeKeypadState != "CAPSLOCK" && !cursorSwipe) {
                    keypad.activeKeypadState = "SHIFTED";
                    keypad.autoCapsTriggered = true;
                } else {
                    keypad.delayedAutoCaps = true;
                }
            }

            onKeyboardReset: {
                keypad.state = "CHARACTERS"
            }
            onDeactivateAutocaps: {
                if(keypad.autoCapsTriggered) {
                    keypad.activeKeypadState = "NORMAL";
                    keypad.autoCapsTriggered = false;
                }
                keypad.delayedAutoCaps = false;
            }
            onThemeChanged: Theme.load(target.theme)
        }
        
        FloatingActions {
            id: floatingActions
            objectName: "floatingActions"
            
            z: 1
            visible: fullScreenItem.cursorSwipe && !cursorSwipeArea.pressed && !bottomSwipe.pressed
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

            Rectangle {
                anchors.fill: parent
                visible: parent.enabled
                color: cursorSwipeArea.selectionMode ? fullScreenItem.theme.selectionColor : fullScreenItem.theme.charKeyPressedColor
                
                Label {
                    visible: !cursorSwipeArea.pressed && !bottomSwipe.pressed
                    horizontalAlignment: Text.AlignHCenter
                    color: cursorSwipeArea.selectionMode ? UbuntuColors.porcelain : fullScreenItem.theme.fontColor
                    wrapMode: Text.WordWrap
                    
                    anchors {
                        left: parent.left
                        right: parent.right
                        verticalCenter: parent.verticalCenter
                    }
                    
                    text: cursorSwipeArea.selectionMode ? i18n.tr("Swipe to move selection") + "\n\n" + i18n.tr("Double-tap to exit selection mode")
                                : i18n.tr("Swipe to move cursor") + "\n\n" + i18n.tr("Double-tap to enter selection mode")
                }
            }
            
            function exitSelectionMode() {
                selectionMode = false
                fullScreenItem.timerSwipe.restart()
            }
            
            onSelectionModeChanged: {
                if (fullScreenItem.cursorSwipe) {
                    fullScreenItem.keyFeedback();
                }
            }
            
            onMouseXChanged: {
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
                var xReleaseDiff = Math.abs(lastRelease.x - firstPress.x)
                var yReleaseDiff = Math.abs(lastRelease.y - firstPress.y)
                
                var threshold = units.gu(2)

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
        
        SwipeArea{
            id: bottomSwipe
            
            property bool draggingCustom: distance >= units.gu(4)
            property bool readyToSwipe: false

            height: units.gu(1.5)
            anchors{
                left: parent.left
                right: parent.right
                bottom: parent.bottom
            }
            direction: SwipeArea.Upwards
            immediateRecognition: true

            onDraggingCustomChanged:{
                if (dragging && !fullScreenItem.cursorSwipe) {
                    readyToSwipe = false
                    swipeDelay.restart()
                    fullScreenItem.cursorSwipe = true
                }
            }

            onTouchPositionChanged: {
                if (fullScreenItem.cursorSwipe && readyToSwipe) {
                    fullScreenItem.processSwipe(touchPosition.x, touchPosition.y)
                }
            }

            onPressedChanged: {
                if (!pressed) {
                    fullScreenItem.timerSwipe.restart()
                }else{
                    fullScreenItem.timerSwipe.stop()
                }
            }
            
            Timer {
                id: swipeDelay
                interval: 100
                running: false
                onTriggered: {
                    fullScreenItem.prevSwipePositionX = bottomSwipe.touchPosition.x
                    fullScreenItem.prevSwipePositionY = bottomSwipe.touchPosition.y
                    bottomSwipe.readyToSwipe = true
                }
            }
        }

        Icon {
            id: bottomHint
            name: "toolkit_bottom-edge-hint"
            visible: !fullScreenItem.cursorSwipe
            width: units.gu(3)
            anchors {
                horizontalCenter: parent.horizontalCenter
                bottom: parent.bottom
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
        
        fullScreenItem.keyFeedback();
    }
    
    SoundEffect {
        id: audioFeedback
        source: maliit_input_method.audioFeedbackSound
    }
    
    HapticsEffect {
        id: pressEffect
        attackIntensity: 0.0
        attackTime: 50
        intensity: 1.0
        duration: 10
        fadeTime: 50
        fadeIntensity: 0.0
    }
    
    Connections {
        target: maliit_input_method
        onAudioFeedbackSoundChanged: audioFeedback.source = sound;
    }
    
    function keyFeedback() {
        if (maliit_input_method.useHapticFeedback) {
            pressEffect.start()
        }
        
        if (maliit_input_method.useAudioFeedback) {
            audioFeedback.play()
        }
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

        maliit_geometry.visibleRect = Qt.rect(obj.x, obj.y, obj.width, obj.height);
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
        // TODO: Removed input_method.surrounding* from the criteria until they are fixed in QtWebEngine
        // ubports/ubuntu-touch#1157 <https://github.com/ubports/ubuntu-touch/issues/1157>
        if (positionX < prevSwipePositionX - units.gu(1) /*&& input_method.surroundingLeft != ""*/) {
            if(cursorSwipeArea.selectionMode){
                selectLeft();
            }else{
                sendLeftKey();
            }
            prevSwipePositionX = positionX
        } else if (positionX > prevSwipePositionX + units.gu(1) /*&& input_method.surroundingRight != ""*/) {
            if(cursorSwipeArea.selectionMode){
                selectRight();
            }else{
                sendRightKey();
            }
            prevSwipePositionX = positionX
        } 

        if (positionY < prevSwipePositionY - units.gu(4)) {
            if(cursorSwipeArea.selectionMode){
                selectUp();
            }else{
                sendUpKey();
            }
            prevSwipePositionY = positionY
        } else if (positionY > prevSwipePositionY + units.gu(4)) {
            if(cursorSwipeArea.selectionMode){
                selectDown();
            }else{
                sendDownKey();
            }
            prevSwipePositionY = positionY
        }
    }

} // fullScreenItem

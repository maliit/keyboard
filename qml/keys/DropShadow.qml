
// from qt-components/desktop

import QtQuick 2.0

ShaderEffect {
    id: effect
    anchors.fill: itemSource
    anchors.leftMargin: -4 + Math.min(xOffset, 0)
    anchors.rightMargin: -4 - Math.max(xOffset, 0)
    anchors.topMargin: -4 + Math.min(yOffset, 0)
    anchors.bottomMargin: -4 - Math.max(yOffset, 0)

    property alias itemSource: innerSource.sourceItem
    property real xOffset: 2
    property real yOffset: 2
    property real intensity: 0.2

    ShaderEffectSource {
        id: innerSource
        x: -4 + Math.min(parent.xOffset, 0)
        y: -4 + Math.min(parent.yOffset, 0)
        smooth: true
        sourceRect: Qt.rect(x, y, effect.width, effect.height)
        hideSource: true
        visible: false
    }

    ShaderEffectSource {
        id: outerSource
        smooth: true
        sourceRect: Qt.rect(0, 0, effect.width, effect.height)
        sourceItem: ShaderEffect {
            width: effect.width
            height: effect.height
            property variant delta: Qt.size(1.0 / width, 0.0)
            property variant source: innerSource
            fragmentShader: shadowSource.fragmentShader
        }
        visible: false
    }

    ShaderEffectSource {
        id: shadowSource
        smooth: true
        property string fragmentShader: "
            uniform sampler2D source;
            uniform highp vec2 delta;
            varying highp vec2 qt_TexCoord0;
            void main() {
                gl_FragColor = 0.0538 * texture2D(source, qt_TexCoord0 - 3.182 * delta)
                             + 0.3229 * texture2D(source, qt_TexCoord0 - 1.364 * delta)
                             + 0.2466 * texture2D(source, qt_TexCoord0)
                             + 0.3229 * texture2D(source, qt_TexCoord0 + 1.364 * delta)
                             + 0.0538 * texture2D(source, qt_TexCoord0 + 3.182 * delta);
            }"

        sourceRect: Qt.rect(0, 0, effect.width, effect.height)
        sourceItem: ShaderEffect {
            width: effect.width
            height: effect.height
            property variant delta: Qt.size(0.0, 1.0 / height)
            property variant source: outerSource
            fragmentShader: shadowSource.fragmentShader
        }
        visible: false
    }

    property variant source: innerSource
    property variant shadow: shadowSource
    property variant delta: Qt.size(-xOffset / width, -yOffset / height)
    fragmentShader: "
        uniform sampler2D source;
        uniform sampler2D shadow;
        uniform highp float intensity;
        uniform highp vec2 delta;
        varying highp vec2 qt_TexCoord0;
        void main() {
            lowp vec4 fg = texture2D(source, qt_TexCoord0);
            lowp vec4 bg = texture2D(shadow, qt_TexCoord0 + delta);
            gl_FragColor = fg + vec4(0., 0., 0., intensity * bg.a) * (1. - fg.a);
        }"
}

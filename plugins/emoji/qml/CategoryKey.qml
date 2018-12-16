/*
 * Copyright 2016 Canonical Ltd.
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
import keys 1.0

CharKey {
    shifted: label
    overridePressArea: true
    width: panel.keyWidth * 0.89
    normalColor: fullScreenItem.theme.backgroundColor
    borderColor: normalColor
    pressedColor: normalColor
    fontSize: fullScreenItem.landscape ? height / 1.8 : height / 2.5
    layer.enabled: !highlight
    layer.effect: ShaderEffect {
        fragmentShader: "
            uniform lowp sampler2D source; // this item
            uniform lowp float qt_Opacity; // inherited opacity of this item
            varying highp vec2 qt_TexCoord0;
            void main() {
                lowp vec4 p = texture2D(source, qt_TexCoord0);
                lowp float g = dot(p.xyz, vec3(0.344, 0.5, 0.156));
                gl_FragColor = vec4(g, g, g, p.a) * qt_Opacity;
            }"
    }
}

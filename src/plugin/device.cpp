/*
 * Copyright (c) 2020 Jan Arne Petersen
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

#include "device.h"

#include "keyboardsettings.h"

#include <QFile>
#include <QFileInfo>
#include <QJsonDocument>
#include <QScreen>
#include <QStandardPaths>
#include <QWindow>

#include <cmath>

namespace MaliitKeyboard
{

Device::Device(const KeyboardSettings *settings, QObject *parent)
    : QObject(parent)
    , m_settings(settings)
{
    connect(settings, &KeyboardSettings::deviceChanged, this, &Device::loadDevice);
    loadDevice(settings->device());
}

void Device::setWindow(QWindow *window)
{
    if (m_window)
        disconnect(window, &QWindow::screenChanged, this, nullptr);

    m_window = window;

    if (m_window) {
        connect(window, &QWindow::screenChanged, this, &Device::updateScreen);
        updateScreen(m_window->screen());
    }

    updateValues();
}

void Device::updateValues()
{
    m_devicePixelRatio = m_window ? m_window->devicePixelRatio() : 1.0;
    m_gridUnit = byName("defaultGridUnitPx", 8) * m_devicePixelRatio;
    Q_EMIT valuesChanged();
}

void Device::updateScreen(QScreen *screen)
{
    if (m_screen)
        disconnect(m_screen, &QScreen::physicalDotsPerInchChanged,
                   this, &Device::updateValues);
    m_screen = screen;

    if (m_screen)
        connect(m_screen, &QScreen::physicalDotsPerInchChanged, this, &Device::updateValues);
}

double Device::dp(double value) const
{
    const auto ratio = m_gridUnit / byName("defaultGridUnitPx", 8);
    if (value <= 2.0) {
        // for values under 2dp, return only multiples of the value
        return std::round(value * std::floor(ratio)) / m_devicePixelRatio;
    } else {
        auto result = std::round(value * ratio) / m_devicePixelRatio;
        return result;
    }
}

double Device::gu(double value) const
{
    return std::round(value * m_gridUnit) / m_devicePixelRatio;
}

void Device::loadDevice(const QString& device)
{
    const auto &deviceFile = QFileInfo(device).isRelative() ?
                            QStandardPaths::locate(QStandardPaths::GenericDataLocation, QStringLiteral("/maliit/keyboard2/devices/%1.json").arg(device)) :
                            device;

    QFile file(deviceFile);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    const auto &content = file.readAll();

    const auto &document = QJsonDocument::fromJson(content);

    m_deviceData = document.object();

    emit valuesChanged();
}

double Device::keyMargins() const
{
    return gu(byName("keyMargins", 0.25));
}

double Device::fontSize() const
{
    return gu(byName("fontSize", 2.5));
}

bool Device::fontBold() const
{
    return m_deviceData.value("fontBold").toBool(false);
}

double Device::annotationFontSize() const
{
    return dp(byName("annotationFontSize", 10));
}

double Device::annotationTopMargin() const
{
    return gu(byName("annotationTopMargin", 0.35));
}

double Device::annotationRightMargin() const
{
    return gu(byName("annotationRightMargin", 0.65));
}

double Device::magnifierHorizontalPadding() const
{
    return gu(byName("magnifierHorizontalPadding", 1.8));
}

double Device::magnifierVerticalPadding() const
{
    return gu(byName("magnifierVerticalPadding", 1.0));
}

double Device::actionKeyPadding() const
{
    return gu(byName("actionKeyPadding", 2.0));
}

double Device::symbolShiftKeyFontSize() const
{
    return gu(byName("symbolShiftKeyFontSize", 2.0));
}

double Device::smallFontSize() const
{
    return gu(byName("smallFontSize", 1.5));
}

double Device::popoverCellPadding() const
{
    return gu(byName("popoverCellPadding", 2.2));
}

double Device::popoverTopMargin() const
{
    return dp(byName("popoverTopMargin", 10));
}

double Device::popoverEdgeMargin() const
{
    return gu(byName("popoverEdgeMargin", 2.2));
}

double Device::popoverSquat() const
{
    return gu(byName("popoverSquat", 3));
}

double Device::top_margin() const
{
    return gu(byName("top_margin", 1));
}

double Device::bottom_margin() const
{
    return gu(byName("bottom_margin", 0));
}

double Device::row_margin() const
{
    return gu(byName("row_margin", 0));
}

double Device::rowMarginLandscape() const
{
    return dp(byName("rowMarginLandscape", 4));
}

double Device::rowMarginPortrait() const
{
    return dp(byName("rowMarginPortrait", 7));
}

double Device::emailLayoutUrlKeyPadding() const
{
    return gu(byName("emailLayoutUrlKeyPadding", 1.5));
}

double Device::wordRibbonHeight() const
{
    return gu(byName("wordRibbonHeight", 4));
}

double Device::wordRibbonFontSize() const
{
    return dp(byName("wordRibbonFontSize", 14));
}

double Device::keyboardHeightPortrait() const
{
    return byName("keyboardHeightPortrait", 0.48);
}

double Device::keyboardHeightLandscape() const
{
    return byName("keyboardHeightLandscape", 0.49);
}

double Device::byName(const char *name, double defaultValue) const
{
    return m_deviceData.value(name).toDouble(defaultValue);
}

double Device::flickMargin() const
{
    return gu(byName("flickMargin", 1.5));
}

double Device::flickBorderWidth() const
{
    return gu(byName("flickBorderWidth", 0.1));
}

Device::~Device() = default;

}

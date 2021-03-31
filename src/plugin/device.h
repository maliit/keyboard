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

#ifndef DEVICE_H
#define DEVICE_H

#include <QObject>
#include <QJsonObject>

class QScreen;
class QWindow;

namespace MaliitKeyboard
{

class KeyboardSettings;

class Device: public QObject
{
    Q_OBJECT

    Q_PROPERTY(double keyMargins READ keyMargins NOTIFY valuesChanged)
    Q_PROPERTY(double fontSize READ fontSize NOTIFY valuesChanged)
    Q_PROPERTY(bool fontBold READ fontBold NOTIFY valuesChanged)
    Q_PROPERTY(double annotationFontSize READ annotationFontSize NOTIFY valuesChanged)
    Q_PROPERTY(double annotationTopMargin READ annotationTopMargin NOTIFY valuesChanged)
    Q_PROPERTY(double annotationRightMargin READ annotationRightMargin NOTIFY valuesChanged)
    Q_PROPERTY(double magnifierHorizontalPadding READ magnifierHorizontalPadding NOTIFY valuesChanged)
    Q_PROPERTY(double magnifierVerticalPadding READ magnifierVerticalPadding NOTIFY valuesChanged)
    Q_PROPERTY(double actionKeyPadding READ actionKeyPadding NOTIFY valuesChanged)
    Q_PROPERTY(double symbolShiftKeyFontSize READ symbolShiftKeyFontSize NOTIFY valuesChanged)
    Q_PROPERTY(double smallFontSize READ smallFontSize NOTIFY valuesChanged)
    Q_PROPERTY(double popoverCellPadding READ popoverCellPadding NOTIFY valuesChanged)
    Q_PROPERTY(double popoverTopMargin READ popoverTopMargin NOTIFY valuesChanged)
    Q_PROPERTY(double popoverEdgeMargin READ popoverEdgeMargin NOTIFY valuesChanged)
    Q_PROPERTY(double popoverSquat READ popoverSquat NOTIFY valuesChanged)
    Q_PROPERTY(double top_margin READ top_margin NOTIFY valuesChanged)
    Q_PROPERTY(double bottom_margin READ bottom_margin NOTIFY valuesChanged)
    Q_PROPERTY(double row_margin READ row_margin NOTIFY valuesChanged)
    Q_PROPERTY(double rowMarginLandscape READ rowMarginLandscape NOTIFY valuesChanged)
    Q_PROPERTY(double rowMarginPortrait READ rowMarginPortrait NOTIFY valuesChanged)
    Q_PROPERTY(double emailLayoutUrlKeyPadding READ emailLayoutUrlKeyPadding NOTIFY valuesChanged)
    Q_PROPERTY(double wordRibbonHeight READ wordRibbonHeight NOTIFY valuesChanged)
    Q_PROPERTY(double wordRibbonFontSize READ wordRibbonFontSize NOTIFY valuesChanged)
    Q_PROPERTY(double keyboardHeightPortrait READ keyboardHeightPortrait NOTIFY valuesChanged)
    Q_PROPERTY(double keyboardHeightLandscape READ keyboardHeightLandscape NOTIFY valuesChanged)
    Q_PROPERTY(double flickMargin READ flickMargin NOTIFY valuesChanged)
    Q_PROPERTY(double flickBorderWidth READ flickBorderWidth NOTIFY valuesChanged)

public:
    explicit Device(const KeyboardSettings *settings, QObject *parent = nullptr);
    ~Device() override;

    void setWindow(QWindow *window);

    Q_INVOKABLE [[nodiscard]] double gu(double value) const;
    Q_INVOKABLE [[nodiscard]] double dp(double value) const;

    [[nodiscard]] double keyMargins() const;
    [[nodiscard]] double fontSize() const;
    [[nodiscard]] bool fontBold() const;
    [[nodiscard]] double annotationFontSize() const;
    [[nodiscard]] double annotationTopMargin() const;
    [[nodiscard]] double annotationRightMargin() const;
    [[nodiscard]] double magnifierHorizontalPadding() const;
    [[nodiscard]] double magnifierVerticalPadding() const;
    [[nodiscard]] double actionKeyPadding() const;
    [[nodiscard]] double symbolShiftKeyFontSize() const;
    [[nodiscard]] double smallFontSize() const;
    [[nodiscard]] double popoverCellPadding() const;
    [[nodiscard]] double popoverTopMargin() const;
    [[nodiscard]] double popoverEdgeMargin() const;
    [[nodiscard]] double popoverSquat() const;
    [[nodiscard]] double top_margin() const;
    [[nodiscard]] double bottom_margin() const;
    [[nodiscard]] double row_margin() const;
    [[nodiscard]] double rowMarginLandscape() const;
    [[nodiscard]] double rowMarginPortrait() const;
    [[nodiscard]] double emailLayoutUrlKeyPadding() const;
    [[nodiscard]] double wordRibbonHeight() const;
    [[nodiscard]] double wordRibbonFontSize() const;
    [[nodiscard]] double keyboardHeightPortrait() const;
    [[nodiscard]] double keyboardHeightLandscape() const;
    [[nodiscard]] double flickMargin() const;
    [[nodiscard]] double flickBorderWidth() const;

Q_SIGNALS:
    void valuesChanged();

private:
    void updateValues();
    void updateScreen(QScreen *screen);
    void loadDevice(const QString& device);

    [[nodiscard]] double byName(const char *name, double defaultValue) const;

    const KeyboardSettings *m_settings;
    double m_devicePixelRatio = 1.0;
    double m_gridUnit = 8.0;
    QJsonObject m_deviceData;

    QWindow *m_window = nullptr;
    QScreen *m_screen = nullptr;
};

}

#endif //DEVICE_H

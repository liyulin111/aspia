//
// Aspia Project
// Copyright (C) 2018 Dmitry Chapyshev <dmitry@aspia.ru>
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program. If not, see <https://www.gnu.org/licenses/>.
//

#ifndef ASPIA_CLIENT__UI__DESKTOP_WINDOW_H_
#define ASPIA_CLIENT__UI__DESKTOP_WINDOW_H_

#include <QPointer>
#include <QWidget>

#include "base/macros_magic.h"
#include "client/connect_data.h"
#include "protocol/desktop_session.pb.h"

class QHBoxLayout;
class QScrollArea;

namespace aspia {

class Clipboard;
class DesktopFrame;
class DesktopPanel;
class DesktopWidget;

class DesktopWindow : public QWidget
{
    Q_OBJECT

public:
    DesktopWindow(ConnectData* connect_data, QWidget* parent = nullptr);
    ~DesktopWindow() = default;

    void resizeDesktopFrame(const QPoint& top_left, const QSize& screen_size);
    void drawDesktopFrame();
    DesktopFrame* desktopFrame();
    void injectCursor(const QCursor& cursor);
    void injectClipboard(const proto::desktop::ClipboardEvent& event);

    void setSupportedVideoEncodings(quint32 video_encodings);
    void setSupportedFeatures(quint32 features);
    bool requireConfigChange(proto::desktop::Config* config);

signals:
    void windowClose();
    void sendConfig(const proto::desktop::Config& config);
    void sendKeyEvent(quint32 usb_keycode, quint32 flags);
    void sendPointerEvent(const QPoint& pos, quint32 mask);
    void sendClipboardEvent(const proto::desktop::ClipboardEvent& event);

protected:
    // QWidget implementation.
    void timerEvent(QTimerEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;
    void closeEvent(QCloseEvent* event) override;

    bool eventFilter(QObject* object, QEvent* event) override;

private slots:
    void onPointerEvent(const QPoint& pos, quint32 mask);
    void changeSettings();
    void autosizeWindow();

private:
    ConnectData* connect_data_;

    quint32 supported_video_encodings_ = 0;
    quint32 supported_features_ = 0;

    QPointer<QHBoxLayout> layout_;
    QPointer<QScrollArea> scroll_area_;
    QPointer<DesktopPanel> panel_;
    QPointer<DesktopWidget> desktop_;
    QPointer<Clipboard> clipboard_;

    int scroll_timer_id_ = 0;
    QPoint scroll_delta_;

    bool is_maximized_ = false;

    QPoint screen_top_left_;

    DISALLOW_COPY_AND_ASSIGN(DesktopWindow);
};

} // namespace aspia

#endif // ASPIA_CLIENT__UI__DESKTOP_WINDOW_H_

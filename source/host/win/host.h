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

#ifndef HOST__WIN__HOST_H
#define HOST__WIN__HOST_H

#include <QPointer>
#include <QUuid>

#include "base/macros_magic.h"
#include "proto/common.pb.h"

namespace ipc {
class Channel;
class Server;
} // namespace ipc

namespace net {
class ChannelHost;
} // namespace net

namespace host {

class HostProcess;
class SessionFake;

class Host : public QObject
{
    Q_OBJECT

public:
    Host(QObject* parent = nullptr);
    ~Host();

    net::ChannelHost* networkChannel() const { return network_channel_; }
    void setNetworkChannel(net::ChannelHost* network_channel);

    const QUuid& uuid() const { return uuid_; }
    void setUuid(const QUuid& uuid);

    const QString& userName() const;
    proto::SessionType sessionType() const;

    QString remoteAddress() const;

    bool start();

public slots:
    void stop();
    void sessionChanged(uint32_t event, uint32_t session_id);

signals:
    void finished(Host* host);

protected:
    // QObject implementation.
    void timerEvent(QTimerEvent* event) override;

private slots:
    void ipcServerStarted(const QString& channel_id);
    void ipcNewConnection(ipc::Channel* channel);
    void attachSession(uint32_t session_id);
    void dettachSession();

private:
    bool startFakeSession();

    enum class State { STOPPED, STARTING, STOPPING, DETACHED, ATTACHED };
    static const uint32_t kInvalidSessionId = 0xFFFFFFFF;

    QUuid uuid_;

    uint32_t session_id_ = kInvalidSessionId;
    int attach_timer_id_ = 0;
    State state_ = State::STOPPED;

    net::ChannelHost* network_channel_ = nullptr;
    QPointer<ipc::Channel> ipc_channel_;
    QPointer<HostProcess> session_process_;
    QPointer<SessionFake> fake_session_;

    DISALLOW_COPY_AND_ASSIGN(Host);
};

} // namespace host

#endif // HOST__WIN__HOST_H

/*
* RTBDS keeps files synchronized in real-time between several machines.
* Copyright (C) 2018  Guillaume TRUCHOT - guillaume.truchot@outlook.com
* 
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU Affero General Public License as published
* by the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
* 
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU Affero General Public License for more details.
* 
* You should have received a copy of the GNU Affero General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "NetworkClient.h"

#include <functional>

rtbds::client::backend::network::NetworkClient::NetworkClient(const std::string &serverAddress, std::uint16_t serverPort) :
	m_ioContext{},
	m_ioContextWork{ m_ioContext },
	m_recvTimer{ m_ioContext },
	m_serverAddress{ serverAddress },
	m_serverPort{ serverPort }
{
	m_recvTimer.async_wait(std::bind(&rtbds::client::backend::network::NetworkClient::receiveNetworkMessage, this));
	m_recvTimer.expires_from_now(std::chrono::nanoseconds{ RECV_NETWORK_MESSAGE_TIMER_INTERVAL_NS });
}


void rtbds::client::backend::network::NetworkClient::run()
{
	m_ioContext.post(std::bind(&rtbds::client::backend::network::NetworkClient::connectToServer, this));
	m_ioContext.run();
}


void rtbds::client::backend::network::NetworkClient::connectToServer()
{
	// TODO: implement

	this->clientAuthenticationResult(false, rtbds::client::backend::exceptions::BackendException{});
}

void rtbds::client::backend::network::NetworkClient::receiveNetworkMessage()
{
	m_recvTimer.async_wait(std::bind(&rtbds::client::backend::network::NetworkClient::receiveNetworkMessage, this));
	m_recvTimer.expires_from_now(std::chrono::nanoseconds{ RECV_NETWORK_MESSAGE_TIMER_INTERVAL_NS });
}

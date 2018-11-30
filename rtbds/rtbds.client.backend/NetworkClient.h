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
#pragma once

#include <boost/asio.hpp>
#include <boost/signals2.hpp>

#include "transporter/INetworkMessage.h"

#include "BackendException.h"

namespace rtbds
{
	namespace client
	{
		namespace backend
		{
			namespace network
			{
				class NetworkClient
				{
				public:
					NetworkClient(const std::string &serverAddress, std::uint16_t serverPort);

					void run();

					boost::signals2::signal<void(bool, const rtbds::client::backend::exceptions::BackendException&)> clientAuthenticationResult;
					boost::signals2::signal<void(const transporter::network::messages::NetworkMessagePtr&)> networkMessageReceived;


				private:
					static const unsigned int RECV_NETWORK_MESSAGE_TIMER_INTERVAL_NS = 10 * 1000000; // ns = ms * 1000000

					void connectToServer();
					void receiveNetworkMessage();

					boost::asio::io_context m_ioContext;
					boost::asio::io_context::work m_ioContextWork;
					boost::asio::steady_timer m_recvTimer;
					std::string m_serverAddress;
					std::uint16_t m_serverPort;
				};
			}
		}
	}
}

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

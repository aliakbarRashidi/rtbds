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

#include "ClientBackend.h"

#include <functional>

#include "NetworkClient.h"
#include "Rsa.h"

rtbds::client::backend::ClientBackend::ClientBackend(const Args &args, rtbds::client::backend::IClientFrontend &clientFrontend) :
	m_args{ args },
	m_clientFrontend{ clientFrontend },
	m_ioContext{},
	m_ioContextWork{ m_ioContext },
	m_networkClient{ new rtbds::client::backend::network::NetworkClient{ args.ServerAddress, args.ServerPort } },
	m_networkClientThread{ &rtbds::client::backend::network::NetworkClient::run, m_networkClient.get() },
	m_authenticated{ false }
{
	m_networkClient->clientAuthenticationResult.connect([this](bool success, const rtbds::client::backend::exceptions::BackendException &e) -> void { this->on_clientAuthenticationResult(success, e); });
}

rtbds::client::backend::ClientBackend::~ClientBackend()
{
}


void rtbds::client::backend::ClientBackend::run()
{
	if (m_args.Mode == "genrsa")
	{
		m_ioContext.post(std::bind(&rtbds::client::backend::ClientBackend::rsaGenerateKeypair, this));
	}

	else if (m_args.Mode == "sync")
	{
		m_ioContext.post(std::bind(&rtbds::client::backend::ClientBackend::syncDirectories, this));
	}

	else if (m_args.Mode == "syncd")
	{
		m_ioContext.post(std::bind(&rtbds::client::backend::ClientBackend::syncDirectoriesDaemon, this));
	}

	m_ioContext.run();
	m_networkClientThread.join();
}


void rtbds::client::backend::ClientBackend::rsaGenerateKeypair()
{
	std::string passphrase = rsaKeypairGenerationPassphraseNeeded().get();
	
	try
	{
		rtbds::client::backend::security::cryptography::asymmetric::Rsa rsa{ m_args.RsaKeySize };

		rsa.saveKeyPair(passphrase, m_args.RsaPrivateKeyPath, m_args.RsaPublicKeyPath);

		rsaKeypairGenerationResult(true, rtbds::client::backend::exceptions::BackendException{});
	}

	catch (const rtbds::client::backend::exceptions::BackendException &e)
	{
		rsaKeypairGenerationResult(false, e);
	}
}

void rtbds::client::backend::ClientBackend::syncDirectories()
{
	// TODO: implement
}

void rtbds::client::backend::ClientBackend::syncDirectoriesDaemon()
{
	// TODO: implement
}


void rtbds::client::backend::ClientBackend::on_clientAuthenticationResult(bool success, const rtbds::client::backend::exceptions::BackendException &e) noexcept
{
	this->clientAuthenticationResult(success, e);
}

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

#include "ClientBackend.h"

#include "BackendException.h"
#include "Rsa.h"

rtbds::client::backend::ClientBackend::ClientBackend(const Args &args) :
	m_args{ args }
{
}


void rtbds::client::backend::ClientBackend::run()
{
	if (m_args.Mode == "genrsa")
	{
		this->rsaGenerateKeypair();
	}

	else if (m_args.Mode == "sync")
	{
		this->syncDirectories();
	}

	else if (m_args.Mode == "syncd")
	{
		this->syncDirectoriesDaemon();
	}

	m_ioContext.run();
}


void rtbds::client::backend::ClientBackend::rsaGenerateKeypair()
{
	std::string passphrase = rsaKeypairGenerationPassphraseNeeded().get();
	
	try
	{
		rtbds::client::backend::security::cryptography::asymmetric::Rsa rsa{ m_args.RsaKeySize };

		rsa.saveKeyPair(passphrase, m_args.RsaPrivateKeyPath, m_args.RsaPublicKeyPath);

		rsaKeypairGenerationFinished(true, "");
	}

	catch (const rtbds::client::backend::exceptions::BackendException &e)
	{
		rsaKeypairGenerationFinished(false, e.what());
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

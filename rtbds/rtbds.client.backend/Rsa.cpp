#include "Rsa.h"

#include <boost/filesystem.hpp>

#include <botan/pkcs8.h>
#include <botan/x509_key.h>

#include "BackendException.h"

rtbds::client::backend::security::cryptography::asymmetric::Rsa::Rsa(std::size_t bits) :
	m_autoSeededRng{},
	m_rsaPrivateKey{ nullptr }
{
	if (bits > 0 && bits % 1024 == 0)
	{
		m_rsaPrivateKey = std::unique_ptr<Botan::Private_Key>{ new Botan::RSA_PrivateKey{ m_autoSeededRng, bits } };
	}

	else
	{
		throw rtbds::client::backend::exceptions::BackendException{ "RSA key size must be a multiple of 1024" };
	}
}

rtbds::client::backend::security::cryptography::asymmetric::Rsa::Rsa(const std::string &passphrase, const std::string &privateKeyPath) :
	m_autoSeededRng{},
	m_rsaPrivateKey{ Botan::PKCS8::load_key(privateKeyPath, m_autoSeededRng, passphrase) }
{
	if (m_rsaPrivateKey == nullptr)
	{
		throw rtbds::client::backend::exceptions::BackendException{ "Couldn't decode RSA private key in " + privateKeyPath };
	}

	if (dynamic_cast<Botan::RSA_PrivateKey*>(m_rsaPrivateKey.get()) == nullptr)
	{
		throw rtbds::client::backend::exceptions::BackendException{ "Private key in " + privateKeyPath + " is not RSA" };
	}
}


void rtbds::client::backend::security::cryptography::asymmetric::Rsa::saveKeyPair(const std::string &passphrase, const std::string &privateKeyPath, const std::string &publicKeyPath)
{
	Botan::RSA_PublicKey publicKey{ dynamic_cast<Botan::RSA_PublicKey&>(*m_rsaPrivateKey) };
	std::string privateKeyPem = Botan::PKCS8::PEM_encode(*m_rsaPrivateKey, m_autoSeededRng, passphrase);
	std::string publicKeyPem = Botan::X509::PEM_encode(publicKey);
	boost::filesystem::ofstream privateKeyPemFile{};
	boost::filesystem::ofstream publicKeyPemFile{};

	if (boost::filesystem::exists(privateKeyPath))
	{
		throw rtbds::client::backend::exceptions::BackendException{ "File " + privateKeyPath + " already exists" };
	}

	if (boost::filesystem::exists(publicKeyPath))
	{
		throw rtbds::client::backend::exceptions::BackendException{ "File " + publicKeyPath + " already exists" };
	}


	privateKeyPemFile.open(privateKeyPath);
	publicKeyPemFile.open(publicKeyPath);

	if (!privateKeyPemFile.is_open())
	{
		throw rtbds::client::backend::exceptions::BackendException{ "Can't open file " + privateKeyPath };
	}

	if (!publicKeyPemFile.is_open())
	{
		throw rtbds::client::backend::exceptions::BackendException{ "Can't open file " + publicKeyPath };
	}


	privateKeyPemFile.write(privateKeyPem.c_str(), privateKeyPem.size());
	publicKeyPemFile.write(publicKeyPem.c_str(), publicKeyPem.size());

	privateKeyPemFile.close();
	publicKeyPemFile.close();
}

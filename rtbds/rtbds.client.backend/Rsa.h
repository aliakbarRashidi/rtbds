#pragma once

#include "RtbdsClientBackend.h"

#include <memory>
#include <string>

#include <botan/auto_rng.h>
#include <botan/rsa.h>

namespace rtbds
{
	namespace client
	{
		namespace backend
		{
			namespace security
			{
				namespace cryptography
				{
					namespace asymmetric
					{
						class RTBDSCLIENTBACKEND_DLL Rsa
						{
						public:
							Rsa(std::size_t bits);
							Rsa(const std::string &passphrase, const std::string &privateKeyPath);

							void saveKeyPair(const std::string &passphrase, const std::string &privateKeyPath, const std::string &publicKeyPath);


						private:
							Botan::AutoSeeded_RNG m_autoSeededRng;
							std::unique_ptr<Botan::Private_Key> m_rsaPrivateKey;
						};
					}
				}
			}
		}
	}
}

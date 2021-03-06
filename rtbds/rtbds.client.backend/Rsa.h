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

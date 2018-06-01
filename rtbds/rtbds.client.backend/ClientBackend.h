#pragma once

#include "RtbdsClientBackend.h"

#include <cstdint>

#include <string>
#include <vector>

#include <boost/asio.hpp>
#include <boost/signals2.hpp>

namespace rtbds
{
	namespace client
	{
		namespace backend
		{
			class RTBDSCLIENTBACKEND_DLL ClientBackend
			{
			public:
				struct Args
				{
					std::string Mode;
					std::string ServerAddress;
					std::uint16_t ServerPort;
					std::vector<std::string> SyncDirectories;
					std::vector<bool> SyncDirectoriesEnableStorageEncryption;
					std::string RsaPrivateKeyPath;
					std::uint32_t RsaKeySize;
					std::string RsaPublicKeyPath;
				};

				ClientBackend(const Args &args);

				void run();

				boost::signals2::signal<std::string()> rsaKeypairGenerationPassphraseNeeded;
				boost::signals2::signal<void(bool, const std::string&)> rsaKeypairGenerationFinished;


			private:
				void rsaGenerateKeypair();
				void syncDirectories();
				void syncDirectoriesDaemon();


				Args m_args;
				boost::asio::io_context m_ioContext;
			};
		}
	}
}

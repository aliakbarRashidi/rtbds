#pragma once

#include "RtbdsClientBackend.h"

#include <cstdint>

#include <memory>
#include <string>
#include <thread>
#include <vector>

#include <boost/asio.hpp>
#include <boost/signals2.hpp>

#include "BackendException.h"
#include "IClientFrontend.h"

namespace rtbds
{
	namespace client
	{
		namespace backend
		{
			namespace network
			{
				class NetworkClient;
			}

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

				ClientBackend(const Args &args, rtbds::client::backend::IClientFrontend &clientFrontend);
				~ClientBackend();

				void run();

				boost::signals2::signal<std::string()> rsaKeypairGenerationPassphraseNeeded;
				boost::signals2::signal<void(bool, const rtbds::client::backend::exceptions::BackendException&)> rsaKeypairGenerationResult;
				boost::signals2::signal<void(bool, const rtbds::client::backend::exceptions::BackendException&)> clientAuthenticationResult;


			private:
				void rsaGenerateKeypair();
				void syncDirectories();
				void syncDirectoriesDaemon();

				void on_clientAuthenticationResult(bool success, const rtbds::client::backend::exceptions::BackendException &e) noexcept;


				Args m_args;
				rtbds::client::backend::IClientFrontend &m_clientFrontend;
				boost::asio::io_context m_ioContext;
				boost::asio::io_context::work m_ioContextWork;
				std::unique_ptr<rtbds::client::backend::network::NetworkClient> m_networkClient;
				std::thread m_networkClientThread;
				bool m_authenticated;
			};
		}
	}
}

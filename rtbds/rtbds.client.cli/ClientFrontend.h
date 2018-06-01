#pragma once

#include "rtbds.client.backend/IClientFrontend.h"

#include <memory>
#include <string>

#include <boost/asio.hpp>
#include <boost/program_options.hpp>

#include "rtbds.client.backend/ClientBackend.h"

namespace rtbds
{
	namespace client
	{
		namespace cli
		{
			class ClientFrontend : public rtbds::client::backend::IClientFrontend
			{
			public:
				int run(int argc, char *argv[]);

				virtual std::string on_rsaKeypairGenerationPassphraseNeeded() noexcept override;
				virtual void on_rsaKeypairGenerationFinished(bool success, const std::string &msg) noexcept override;


			private:
				template<typename T>
				void checkOptionRequirement(const boost::program_options::variables_map &options, const std::string &opt, const std::string &requiredOpt, const T &optValue = T{}, bool requireOptValue = false)
				{
					if (!requireOptValue && options.count(opt) && !options.count(requiredOpt))
					{
						throw boost::program_options::error{ "Option " + requiredOpt + " is required when " + opt + " is present" };
					}

					if (requireOptValue && options.count(opt) && options[opt].as<T>() == optValue && !options.count(requiredOpt))
					{
						std::ostringstream error{};

						error << "Option " << requiredOpt << " is required when " << opt << " is " << optValue;
						throw boost::program_options::error{ error.str() };
					}
				}

				static void setStdinEcho(bool echo);


				std::unique_ptr<rtbds::client::backend::ClientBackend> m_clientBackend;
				boost::asio::io_context m_ioContext;
			};
		}
	}
}

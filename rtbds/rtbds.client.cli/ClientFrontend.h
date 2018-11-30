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
				ClientFrontend();

				int run(int argc, char *argv[]);

				std::string on_rsaKeypairGenerationPassphraseNeeded() noexcept;
				void on_rsaKeypairGenerationResult(bool success, const rtbds::client::backend::exceptions::BackendException &e) noexcept;
				void on_clientAuthenticationResult(bool success, const rtbds::client::backend::exceptions::BackendException &e) noexcept;


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
				boost::asio::io_context::work m_ioContextWork;
			};
		}
	}
}

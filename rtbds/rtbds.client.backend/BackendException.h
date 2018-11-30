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

#include <exception>
#include <string>

namespace rtbds
{
	namespace client
	{
		namespace backend
		{
			namespace exceptions
			{
#if defined(_MSC_VER)
				class RTBDSCLIENTBACKEND_DLL std::exception; // NOTE: exporting std::exception because BackendException inherits from it and otherwise MSVC will produce a C4251 warning
#endif

				class RTBDSCLIENTBACKEND_DLL BackendException : std::exception
				{
				public:
					BackendException(const std::string &msg = "");

					virtual const char *what() const noexcept override;


				private:
					// NOTE: warning C4251 "class X needs to have dll-interface to be used by clients of class..." can be safely disabled as it is private and won't be used by any client
#if defined(_MSC_VER)
#pragma warning( push )
#pragma warning( disable : 4251)
#endif
					std::string m_msg;
#if defined(_MSC_VER)
#pragma warning( pop )
#endif
				};
			}
		}
	}
}

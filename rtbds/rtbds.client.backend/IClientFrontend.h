#pragma once

#include "RtbdsClientBackend.h"

namespace rtbds
{
	namespace client
	{
		namespace backend
		{
			class IClientFrontend
			{
			public:
				virtual ~IClientFrontend() = default;
			};
		}
	}
}

#pragma once

#include "RtbdsClientBackend.h"

#include <string>

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

				virtual std::string on_rsaKeypairGenerationPassphraseNeeded() noexcept = 0;
				virtual void on_rsaKeypairGenerationFinished(bool success, const std::string &msg) noexcept = 0;
			};
		}
	}
}

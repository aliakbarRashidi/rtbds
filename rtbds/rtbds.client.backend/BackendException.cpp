#include "BackendException.h"

rtbds::client::backend::exceptions::BackendException::BackendException(const std::string &msg) : std::exception{},
m_msg{ msg }
{
}

const char *rtbds::client::backend::exceptions::BackendException::what() const noexcept
{
	return m_msg.c_str();
}

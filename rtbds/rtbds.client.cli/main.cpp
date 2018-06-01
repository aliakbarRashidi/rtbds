#include "ClientFrontend.h"

int main(int argc, char *argv[])
{
	rtbds::client::cli::ClientFrontend clientFrontend{};

	return clientFrontend.run(argc, argv);
}

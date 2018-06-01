#include "ClientFrontend.h"

#include <cstdlib>

#include <cstdint>

#include <iostream>
#include <string>
#include <thread>
#include <vector>

#if defined(_WIN32)
#include <windows.h>
#else
#include <termios.h>
#include <unistd.h>
#endif

#include <boost/program_options.hpp>

#include "RtbdsClientCli.h"

int rtbds::client::cli::ClientFrontend::run(int argc, char *argv[])
{
	rtbds::client::backend::ClientBackend::Args clientBackendArgs{};
	boost::program_options::options_description genericOptionsDesc{ "Generic options" };
	boost::program_options::options_description serverOptionsDesc{ "Server options" };
	boost::program_options::options_description synchronizationOptionsDesc{ "Synchronization options" };
	boost::program_options::options_description rsaKeysOptionsDesc{ "RSA Keys options" };
	boost::program_options::variables_map options{};

	serverOptionsDesc.add_options()
		("server-address,a", boost::program_options::value<std::string>(&clientBackendArgs.ServerAddress), "Address of the server to connect to (can be either an IP address or a domain name)")
		("server-port,p", boost::program_options::value<std::uint16_t>(&clientBackendArgs.ServerPort)->default_value(23502), "Port of the server to connect to")
		;

	synchronizationOptionsDesc.add_options()
		("sync-directory,d", boost::program_options::value<std::vector<std::string>>(&clientBackendArgs.SyncDirectories), "Directory to synchronize")
		("sync-directory-enable-storage-encryption,e", boost::program_options::value<std::vector<bool>>(&clientBackendArgs.SyncDirectoriesEnableStorageEncryption), "Enable storage encryption for matching -d argument (defaults to true if not specified)")
		;

	rsaKeysOptionsDesc.add_options()
		("rsa-private-key-path,k", boost::program_options::value<std::string>(&clientBackendArgs.RsaPrivateKeyPath), "Path of the RSA private key to write (when generating a keypair) or to use")
		("rsa-key-size", boost::program_options::value<std::uint32_t>(&clientBackendArgs.RsaKeySize), "RSA key size to generate (when generating a keypair)")
		("rsa-public-key-path", boost::program_options::value<std::string>(&clientBackendArgs.RsaPublicKeyPath), "Path of the RSA public key to write (when generating a keypair)")
		;

	genericOptionsDesc.add_options()
		("mode,m", boost::program_options::value<std::string>(&clientBackendArgs.Mode), "- \"genrsa\" to generate an RSA private/public keypair\n- \"sync\" to synchronize directories and exit\n- \"syncd\" to start synchronizing directories (daemon mode)")
		("help,h", "Print help")
		("version,v", "Print version")
		;
	genericOptionsDesc.add(serverOptionsDesc);
	genericOptionsDesc.add(synchronizationOptionsDesc);
	genericOptionsDesc.add(rsaKeysOptionsDesc);

	try
	{
		boost::program_options::store(boost::program_options::parse_command_line(argc, argv, genericOptionsDesc), options);
		boost::program_options::notify(options);

		checkOptionRequirement(options, "mode", "rsa-private-key-path", std::string{ "genrsa" }, true);
		checkOptionRequirement(options, "mode", "rsa-key-size", std::string{ "genrsa" }, true);
		checkOptionRequirement(options, "mode", "rsa-public-key-path", std::string{ "genrsa" }, true);
		checkOptionRequirement(options, "mode", "server-address", std::string{ "sync" }, true);
		checkOptionRequirement(options, "mode", "server-address", std::string{ "syncd" }, true);
		checkOptionRequirement(options, "mode", "rsa-private-key-path", std::string{ "sync" }, true);
		checkOptionRequirement(options, "mode", "rsa-private-key-path", std::string{ "syncd" }, true);
		checkOptionRequirement(options, "mode", "sync-directory", std::string{ "sync" }, true);
		checkOptionRequirement(options, "mode", "sync-directory", std::string{ "syncd" }, true);
	}

	catch (const boost::program_options::error &e)
	{
		std::cerr << e.what() << std::endl << std::endl;
		std::cout << genericOptionsDesc << std::endl;

		return EXIT_FAILURE;
	}


	if (options.count("version"))
	{
		std::cout << "rtbds.client.cli version " << RTBDSCLIENTCLI_VERSION << ", rtbds.client.backend version " << RTBDSCLIENTBACKEND_VERSION << std::endl << std::endl;
	}

	if (options.count("help") || (!options.count("mode") && !options.count("help") && !options.count("version")))
	{
		std::cout << genericOptionsDesc << std::endl;
	}

	m_clientBackend = std::unique_ptr<rtbds::client::backend::ClientBackend>{ new rtbds::client::backend::ClientBackend{ clientBackendArgs } };
	m_clientBackend->rsaKeypairGenerationPassphraseNeeded.connect([this]() -> std::string { return this->on_rsaKeypairGenerationPassphraseNeeded(); });
	m_clientBackend->rsaKeypairGenerationFinished.connect([this](bool success, const std::string &msg) -> void { this->on_rsaKeypairGenerationFinished(success, msg); });

	std::thread clientBackendThread{ std::bind(&rtbds::client::backend::ClientBackend::run, m_clientBackend.get()) };
	
	m_ioContext.run();

	clientBackendThread.join();

	return EXIT_SUCCESS;
}


std::string rtbds::client::cli::ClientFrontend::on_rsaKeypairGenerationPassphraseNeeded() noexcept
{
	std::string passphrase{};
	std::string passphraseAgain{};

	std::cout << "You must enter a passphrase to protect the private key. You can leave it empty if you want (no passphrase). Note: input echo is disabled for security reasons." << std::endl;
	
	do
	{
		ClientFrontend::setStdinEcho(false);
		std::cout << "Passphrase: ";
		std::cin >> passphrase;

		std::cout << std::endl << "Passphrase (confirmation): ";
		std::cin >> passphraseAgain;
		ClientFrontend::setStdinEcho(true);

		std::cout << std::endl;

		if (passphrase != passphraseAgain)
		{
			std::cout << "Confirmation doesn't match passphrase. Try again." << std::endl << std::endl;
		}
	} while (passphrase != passphraseAgain);

	return passphrase;
}

void rtbds::client::cli::ClientFrontend::on_rsaKeypairGenerationFinished(bool success, const std::string &msg) noexcept
{
	if (success)
	{
		std::cout << "RSA keypair successfully generated and saved" << std::endl;
	}

	else
	{
		std::cerr << "RSA keypair generation failed: " << msg << std::endl;
	}
}


void rtbds::client::cli::ClientFrontend::setStdinEcho(bool echo)
{
#if defined(_WIN32)
	HANDLE hStdin = ::GetStdHandle(STD_INPUT_HANDLE);
	DWORD mode = -1;

	::GetConsoleMode(hStdin, &mode);

	if (!echo)
	{
		mode &= ~ENABLE_ECHO_INPUT;
	}

	else
	{
		mode |= ENABLE_ECHO_INPUT;
	}

	::SetConsoleMode(hStdin, mode);
#else
	struct termios tty{};

	::tcgetattr(STDIN_FILENO, &tty);

	if (!echo)
	{
		tty.c_lflag &= ~ECHO;
	}

	else
	{
		tty.c_lflag |= ECHO;
	}

	::tcsetattr(STDIN_FILENO, TCSANOW, &tty);
#endif
}

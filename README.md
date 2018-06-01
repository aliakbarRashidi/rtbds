# Real-Time Bidirectional Delta Synchronization

RTBDS (**R**eal-**T**ime **B**idirectional **D**elta **S**ynchronization) is a cross-platform tool designed to keep files synchronized in real-time between several machines.

The main features of RTBDS are:
* **Real-Time synchronization**: files are synchronized in real-time thanks to events emitted by the RTBDS clients and server
* **Bidirectional synchronization**: both client and server listen for synchronization events, allowing for synchronization in both ways
* **Remote delta compression**: RTBDS uses [librsync](https://github.com/librsync/librsync)'s delta compression to update files efficiently and quickly
* **Encryption**: all network traffic is encrypted using TLS and files are stored encrypted using AES (can be disabled if speed is preferred over security)
* **Cross-platform**: both client and server can run on Linux, Windows and Mac OS X


## How does it work?

RTBDS relies on a **client-server architecture**.

The server is the central point where up-to-date files are kept and distributed to the clients. Files modified by a client are immediately updated on the server, so other clients can get the updates and stay synchronized.


### Details about delta compression and encryption

**Delta compression** is a method of transmitting data in the form of **differences** (between two files) rather than complete files. It allows for very fast updates even with large files and low bandwith (depending on how much the file has been modified of course).

Delta compression is a key feature of RTBDS, and so is the **security (confidentiality and integrity)** of the data.

However, delta compression doesn't play well with encryption: indeed, encrypted data tends to **completely change** even when only a few bits of unencrypted data have been modified. This is great for confidentiality (and we want that), but totally breaks delta compression.

That's why **not only files** are kept on an RTBDS server, **but also files' signatures and deltas**. Indeed, since the server can't decrypt files for security reasons, it also can't apply the delta patches to have an up-to-date file. Instead, **clients send the encrypted signature files and delta patches** to the server, so other clients can download them and apply the patches themselves.

However, this technique has disadvantages (see table below). Since confidentiality isn't always needed, RTBDS offers two synchronization strategies:
* **Full end-to-end encryption**: network traffic is encrypted, as well as stored files on the server
* **Network traffic encryption only** : network traffic is encrypted, but files stored on the server are in clear

Note: **integrity of data is guaranteed with both strategies**.

| Strategy | Advantage | Disadvantage |
| --- | --- | --- |
| Full end-to-end encryption | Confidentiality of the files is guaranteed even if the server is malicious/hacked | Bigger storage space needed server-side, longer synchronization |
| Network traffic encryption only | Very fast synchronization | Confidentiality isn't guaranteed on the server (but integrity still is) |


### Strategies comparison

* Strategies below are described from the **client point-of-view** (so "_locally_" means client-side and "_remotely_" means server-side)
* It is assumed that an out-of-date client (which doesn't have the latest version of a file) can't update a file on the server

| Strategy | Synchronization Direction | Process |
| --- | --- | --- |
| Full end-to-end encryption | Client -> Server | <ol><li>Server (out-of-date) file signature is downloaded locally</li><li>Delta patch is computed locally from server (out-of-date) file signature and client (up-to-date) file</li><li>Delta patch is encrypted using AES-CBC and uploaded to the server</li><li>Client (up-to-date) file signature is computed locally</li><li>Client (up-to-date) file signature is encrypted using AES-CBC and uploaded to the server for future use</li></ol> |
| Full end-to-end encryption | Client <- Server | <ol><li>Server (up-to-date) delta patch(es) is/are downloaded locally and decrytped</li><li>Delta patch(es) is/are applied locally on (out-of-date) client file</li></ol> |
| Network traffic encryption only | Client -> Server | <ol><li>Server (out-of-date) file signature is downloaded locally</li><li>Delta patch is computed locally from server (out-of-date) file signature and (up-to-date) client file</li><li>Delta patch is uploaded to the server</li><li>Delta patch is applied on the server (out-of-date) file</li><li>Server (now up-to-date) file signature is computed for future use</li></ol> |
| Network traffic encryption only | Client <- Server | <ol><li>Client (out-of-date) file signature is computed locally</li><li>Client (out-of-date) file signature is uploaded to the server</li><li>Delta patch is computed remotely from client (out-of-date) file signature and server (up-to-date) file</li><li>Delta patch is uploaded to the client</li><li>Delta patch is applied locally on client (out-of-date) file</li></ol> |


## Development

RTBDS is written in **modern C++11** and uses the following libraries:
* [librsync](https://github.com/librsync/librsync) for delta compression
* [botan](https://github.com/randombit/botan) for cryptography
* [boost::asio](https://github.com/boostorg/asio) for networking
* [boost::filesystem](https://github.com/boostorg/filesystem) for filesystem-related operations
* [boost::program_options](https://github.com/boostorg/program_options) for parsing command line options
* [boost::signals2](https://github.com/boostorg/signals2) for signals and slots mechanism
* [transporter](https://github.com/GuiTeK/transporter) for network messages
* [nlohmann/json](https://github.com/nlohmann/json) for JSON parsing


### Compiling on Windows

1. Clone the repository: `git clone git@github.com:GuiTeK/rtbds.git`
2. Initialize and update submodules (namely [transporter library](https://github.com/GuiTeK/transporter)): `git submodule update --init --recursive`
2. Install [vcpkg](https://github.com/Microsoft/vcpkg)
3. Run `vcpkg install librsync botan boost-asio boost-filesystem boost-program-options boost-signals2 nlohmann-json`

You're all set!


## Contributing

You can contribute to the project by:
* Creating an [issue](https://github.com/GuiTeK/rtbds/issues) to signal a bug or to suggest a feature
* Fixing bugs or implementing features (see [Creating a pull request - GitHub](https://help.github.com/articles/creating-a-pull-request/))
* Writing documentation

Make sure you read [CONTRIBUTING.md](https://github.com/GuiTeK/rtbds/blob/master/CONTRIBUTING.md) before writing code.

Any contribution is very much appreciated!

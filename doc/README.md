REBEL Coin
=============

Setup
---------------------
[REBEL Coin](https://github.com/RebelStation/REBELCoin/releases/tag/v1.0) is the original REBELCoin client and it builds the backbone of the network. However, it downloads and stores the entire history of REBELCoin transactions; depending on the speed of your computer and network connection, the synchronization process can take anywhere from a few hours to a day or more. Thankfully you only have to do this once.

Running
---------------------
The following are some helpful notes on how to run REBEL Coin on your native platform.

### Unix

Unpack the files into a directory and run:

- `bin/rebelcoin-qt` (GUI) or
- `bin/rebelcoind` (headless)

If this is the first time running REBEL Coin, you'll need to install the sapling params by running the included `install-params.sh` script, which copies the two params files to `$HOME/.rebelcoin-params`

### Windows

Unpack the files into a directory, and then run rebelcoin-qt.exe.

### macOS

Drag REBELCoin-Qt to your applications folder, and then run REBELCoin-Qt.

### Need Help?

* See the documentation at the [REBELCoin Wiki](https://github.com/RebelStation/REBELCoin/wiki)
for help and more information.
* Ask for help on the [REBELCoin Forum](https://forum.rebelstation.org /).
* Join our Discord server [Discord Server](https://discord.gg/8XJwU2r3Un )

Building
---------------------
The following are developer notes on how to build REBEL Coin on your native platform. They are not complete guides, but include notes on the necessary libraries, compile flags, etc.

- [Dependencies](dependencies.md)
- [macOS Build Notes](build-osx.md)
- [Unix Build Notes](build-unix.md)
- [Windows Build Notes](build-windows.md)
- [Gitian Building Guide](gitian-building.md)

Development
---------------------
The REBELCoin repo's [root README](/README.md) contains relevant information on the development process and automated testing.

- [Developer Notes](developer-notes.md)
- [Release Notes](release-notes.md)
- [Release Process](release-process.md)
- [Translation Process](translation_process.md)
- [Unauthenticated REST Interface](REST-interface.md)
- [Dnsseed Policy](dnsseed-policy.md)

### Resources
* Discuss on the [REBELCoin](https://forum.rebelstation.org /) forum.
* Join the [REBELCoin Discord](https://discord.gg/8XJwU2r3Un ).

### Miscellaneous
- [Assets Attribution](assets-attribution.md)
- [Files](files.md)
- [Reduce Memory](reduce-memory.md)
- [Tor Support](tor.md)
- [Init Scripts (systemd/upstart/openrc)](init.md)

License
---------------------
Distributed under the [MIT software license](/COPYING).
This product includes software developed by the OpenSSL Project for use in the [OpenSSL Toolkit](https://www.openssl.org/). This product includes
cryptographic software written by Eric Young ([eay@cryptsoft.com](mailto:eay@cryptsoft.com)), and UPnP software written by Thomas Bernard.

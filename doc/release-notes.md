Sapling Parameters
==================

In order to run, REBEL Coin requires two files, `sapling-output.params` and `sapling-spend.params` (with total size ~50 MB), to be saved in a specific location.

For the following packages, no action is required by the user:
- macOS release `dmg` binaries will use the params that are bundled into the .app bundle.
- Windows installer `.exe` will automatically copy the files in the proper location.
- Linux `PPA/Snap` installs will automatically copy the files in the proper location.

For the other packages, the user must save the param files in the proper location, before being able to run REBELCoin v1.0.0:
- macOS/Linux `tar.gz` tarballs include a bash script (`install-params.sh`) to copy the parameters in the appropriate location.
- Windows `.zip` users need to manually copy the files from the `share/rebelcoin` folder to the `%APPDATA%\REBELCoinParams` directory.
- self compilers can run the script from the repository sources (`params/install-params.sh`), or copy the files directly from the `params` subdirectory.

Compatibility
==============

REBEL Coin is extensively tested on multiple operating systems using the Linux kernel, macOS 10.12+, and Windows 7 and later.

REBEL Coin should also work on most other Unix-like systems but is not frequently tested on them.


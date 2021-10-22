
Debian
====================
This directory contains files used to package rebelcoind/rebelcoin-qt
for Debian-based Linux systems. If you compile rebelcoind/rebelcoin-qt yourself, there are some useful files here.

## rebelcoin: URI support ##


rebelcoin-qt.desktop  (Gnome / Open Desktop)
To install:

	sudo desktop-file-install rebelcoin-qt.desktop
	sudo update-desktop-database

If you build yourself, you will either need to modify the paths in
the .desktop file or copy or symlink your rebelcoin-qt binary to `/usr/bin`
and the `../../share/pixmaps/rebelcoin128.png` to `/usr/share/pixmaps`

rebelcoin-qt.protocol (KDE)


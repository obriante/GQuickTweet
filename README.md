# GQuickTweet

[![TwitCrusader](https://raw.github.com/KernelMonkey/GQuickTweet/master/share/image/gqt_logo.png)]()


GTK+ Desktop Gadget for quickly sending tweets

## How to download this repository:

	$ git clone <project url>  --recursive

## Authors:
	Alfredo Liguoro  <freddy@haxaddicts.net>
	Orazio Briante <orazio.briante@hotmail.it>

## Required

*    gcc or cc
*    pkg-config
*    intltool
*    gtk+-2.0
*    libnotify >= 0.7.3

### libtwitc Required:

*   libcurl
*   oauth
*   libxml-2.0


## How to Generate Makefile:

	$ ./autogen.sh
  or

	$ sh autogen.sh


## Compile - Makefile:
    
	$ ./configure
	$ make
	$ su
   	# make install


## Unistalling - Makefile

	$ su
	# make uninstall


## Debug

To see more informations run:

	$ gquicktweet --debug

To debug the code, on compiling process, run:

	$ ./configure --enable-debug


## Reference:

Project libtwitc:

	repo: https://github.com/KernelMonkey/libtwitc

Project liblogc:

	repo: https://github.com/KernelMonkey/liblogc

TwitCrusader:

	website: http://it.twitcrusader.org/
	repo: https://github.com/PTKDev/ClientTwitter-TwitCrusader  

Lookshare-GUI
=============
*/ Install QT-SDK  4.8/*

1- Download the Qt Software Development Kit (SDK) Download Qt SDK ( If you are on 32-bit Ubuntu Linux then download 32-bit Qt SDK, if you are on 64-bit Ubuntu Linux then download 64-bit Qt SDK. )

2- Open up a terminal and enter the following commands below: 

sudo apt-get install synaptic
sudo apt-get update
sudo apt-get install qt4-dev-tools libqt4-dev libqt4-core libqt4-gui

3- To make Qt SDK executable for all users on your system enter the following command:

sudo -s chmod u+x QtSdk-offline-linux-x86_64-v1.2.1.run

4- Install the Qt SDK by issuing the following command :

sudo -s ./QtSdk-offline-linux-x86_64-v1.2.1.run -style cleanlooks

5- Once the Qt program is installed, open up a terminal and use a text editor such as nano or gedit to edit your /etc/profile. Type:

sudo -s nano /etc/profile or sudo -s gedit /etc/profile

6- Add the following lines to etc/profile:

PATH=/opt/QtSDK/Desktop/Qt/Qt_version_number/gcc/bin:$PAT
export PATH
7- Save the /etc/profile file and exit
8- Reload the /etc/profile file by issuing the following command:
. /etc/profile

*/ Install cmyth /*

1- Clone cmyth repository from http://cmyth.github.com

2- Build and install the code with the following command:

PREFIX=<path> scons install

*/ Install GStreamer /*

1- Install the gstreamer development files with following commands:

sudo apt-get install libvorbis-dev flex libasound2-dev libgstreamer0.10-dev libgstreamer-plugins-base0.10-dev

2- Download the latest GStreamer source from
http://gstreamer.freedesktop.org/src/gstreamer/
http://gstreamer.freedesktop.org/src/gst-plugins-base/
http://gstreamer.freedesktop.org/src/gst-plugins-good/

3- Make sure you have paths configured correctly in your ~/.bashrc file:
export LD_LIBRARY_PATH=/usr/local/lib
export PKG_CONFIG_PATH=/usr/local/lib/pkgconfig
export GST_PLUGIN_PATH=/usr/local/lib
export PATH=$PATH:/usr/local/lib:/usr/local
4- Extract, compile and install the files with following commands:
tar xvf gstreamer-0.11.1.tar.gz
cd gstreamer-0.11.1
./configure
make
sudo make install
cd ..
tar xvf gst-plugins-base-0.11.1.tar.gz
cd gst-plugins-base-0.11.1
./configure
sudo make
sudo make install
cd ..
tar xvf gst-plugins-good-0.10.30.tar.gz
cd gst-plugins-good-0.10.30
./configure
sudo make
sudo make install

*/ Build the Lookshare-GUI /* 

1- sudo qmake

2- sudo make

3- sudo make install

4- ./Lookshare-GUI

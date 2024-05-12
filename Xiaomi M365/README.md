# Xiaomi Mi365 scooter Controller

Script based on https://github.com/louiscb/Mi365Locker-RASPI that allows us to send lock and unlock packets to the scooter through its BLE module for linux systems.

## Installation

The code needs an executable bluepy-helper to be compiled from C source. This is done automatically if you use the recommended pip installation method (see below). Otherwise, you can rebuild it using the Makefile in the bluepy directory.

To install the current released version, on most Debian-based systems:

`$ sudo apt-get install python-pip libglib2.0-dev`
`$ sudo pip install bluepy`

On Fedora do:

`$ sudo dnf install python-pip glib2-devel`

For Python 3, you may need to use pip3:

`$ sudo apt-get install python3-pip libglib2.0-dev`
`$ sudo pip3 install bluepy`

## Usage

The program takes two arguments:

    The type of scan
    Command to send to the scooter

The first argument can either be scan which scans all available devices or saved which scans all known devices saved in a file. If during a scan the program succesfully locates a scooter it will save its address in the known devices file.

The second argument can be one the following commands:

    Lock - this locks the scooter
    Unlock - this unlocks the scooter

Hopefully more commmands will be added in the near future.

Root is required to run the progarm in order to gain access to the system's lower bluetooth functions.

Example to run the program:

`sudo python3 scooter-scan.py scan unlock`


# Levoit Air Purifier Integration for ESPHOME


Supported:
- Levoit 200s (Untested)
- Levoit 300s (Early Alpha)
- Levoit 300s (Early Alpha)



# Disassembly
For details on the teardown please see: [vigue.me](https://vigue.me/posts/levoit-air-purifier-esphome-conversion)
- Place upside down and remove base cover and filter to expose 8 screws (4 have washers)
- Remove all 8 screws (NOTE IF USING AN ELECTRIC SCREWDRIVER THE SCREWS ARE VERY SOFT METAL)
- Using a pry tool slide in between tabs
- Separate base and top sleeve
- Unplug logic board

# How to Flash (using esptool.py on linux)
- Create a new device in ESPhome (just choose esp32, as we'll overwrite the config)
- Copy the sample config for your purifier and copy it in (use the generated passwords for the original config, but this should totally replace the config)
- Install and Download the binary (Choose the modern format once compilation has completed)
- Solder wires to pins TXD0, RXD0, +3V3, GND
- Attach this to usb programmer.
- Solder wire to pin 100.  This needs to be connected to ground in order to put the ESP32 into programming mode.
- Connect pin 100 to ground using wire.
- On linux `ls /dev/` then plug in the programmer, `ls /dev/` again to determine which is the new device.
- Take a backup of the factory flash in case you ever want to go back (warranty etc)

  ```
  sudo esptool.py --port /dev/ttyUSB1 read_flash 0x000 ALL levoit-office-purifier-original-firmware.bin
  ```
  ```
  esptool.py v4.7.0
  Serial port /dev/ttyUSB1
  Connecting....
  Detecting chip type... Unsupported detection protocol, switching and trying again...
  Connecting...
  Detecting chip type... ESP32
  Chip is ESP32-S0WD (revision v1.0)
  Features: WiFi, BT, Single Core, VRef calibration in efuse, Coding Scheme None
  Crystal is 40MHz
  MAC: 94:3c:c6:56:62:ec
  Uploading stub...
  Running stub...
  Stub running...
  Detected flash size: 4MB
  ```
- Erase the flash
  ```
  sudo esptool.py --port /dev/ttyUSB1 erase_flash
  ```
- Write the firmware to the device
  ```
  sudo esptool.py --chip esp32 --port /dev/ttyUSB1 write_flash 0x00 purity-control-factory.bin
  ```
  ```
  esptool.py v4.7.0
  Serial port /dev/ttyUSB1
  Connecting......
  Chip is ESP32-S0WD (revision v1.0)
  Features: WiFi, BT, Single Core, VRef calibration in efuse, Coding Scheme None
  Crystal is 40MHz
  MAC: 94:3c:c6:56:fe:80
  Uploading stub...
  Running stub...
  Stub running...
  Configuring flash size...
  Flash will be erased from 0x00000000 to 0x000e3fff...
  Compressed 929920 bytes to 595609...
  Wrote 929920 bytes (595609 compressed) at 0x00000000 in 52.4 seconds (effective 141.9 kbit/s)...
  Hash of data verified.

  Leaving...
  Hard resetting via RTS pin...
  ```

- Unplug the programmer from the computer
- Plug the board into the purifier
- Disconnect the 100 pin from ground (so it's not in programming mode)
- Plug power into the board
- Test that the board appears in ESPhome.
- Test that OTA updates work (I missed out the ota config the first time I ran this but thankfully caught it before reassembly)
- Once satisfied it works, disconnect power and reassemble.
(NOTE IF USING AN ELECTRIC SCREWDRIVER THE SCREWS ARE VERY SOFT METAL)

# Contributions
Contributions and Issues are welcome via github issues and PRs.

Code by Aiden (@acvigue)

Other Contributors:

Haydon (@haydonryan) - Documentation, sample and testing

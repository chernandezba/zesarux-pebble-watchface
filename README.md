# zesarux-pebble-watchface

https://apps.rebble.io/en_US/application/692644f5703cec0009a280fd

When tapping Pebble, will get the last number of ZEsarUX users (total yesterday users)

- Compile:

pebble build

- Test on emulator:

pebble install --emulator basalt

- Install on Pebble watch:

 pebble install --phone 192.168.1.18

- To open the watchface app settings on the emulator. First open the watchface on emulator, then:

pebble emu-app-config

It will open a browser with the settings

- To simulate a tap on the emulator:

pebble emu-tap

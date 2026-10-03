# TODO

1. we don't use a step-down buck. Instead, we'll supply motors directly from the powerbank (5V). powerbank high V line will be directed to 5V line. Then, internal step-down buck will feed 3V3 line. From there we'll power up all the external elements, i.e. a sonar, bridge, (opt. encoders)

# Install

```bash
snap install turbowarp-desktop
```

# Run

## Scratch mode

### First run python client
```bash
./run_python_client.sh
```

### Then run scratch (turbowarp)
```bash
./run_turbowarp.sh
```

and open project `rover.sb3`

# Architecture

## Hardware

### Rover
MCU: [Waveshare ESP32-S3-Nano](https://www.waveshare.com/wiki/ESP32-S3-Nano)

### Software client

#### Backend

`./client`
python
websockets (port 7865)
python bleak lib (to connect with MCU via bluetooth low energy)

#### Bluetooth

**android**
*Bluefruit LE control*

**ubuntu**
*bluetoothctl*

#### UI

[scratch/turbowarp](https://desktop.turbowarp.org/)

# Troubleshooting

## Arduino-ide

```
[18644:1223/101056.126347:FATAL:setuid_sandbox_host.cc(158)] The SUID sandbox helper binary was found, but is not configured correctly. Rather than run without sandboxing I'm aborting now. You need to make sure that /home/justyna-halicz-szymanska/Programy/arduino-ide_2.3.7_Linux_64bit/chrome-sandbox is owned by root and has mode 4755.
Pułapka debuggera/breakpoint (zrzut pamięci)
```

solution
```
sudo chown root chrome-sandbox
sudo chmod g+x,u+s chrome-sandbox
```

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
MCU: Arduino nano esp32

### Software client

#### Backend

`./client`
python
websockets (port 7865)
python bleak lib (to connect with MCU via bluetooth low energy)

#### UI

scratch/turbowarp - https://desktop.turbowarp.org/

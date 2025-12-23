# libs: websockets, asyncio, bleak

import asyncio
from bleak import BleakClient, BleakScanner
import websockets
import random

CMD_UUID = "12345678-1234-1234-1234-1234567890ac"
ROBOT_NAME = "Robot"
robot = None

# ---- BLE ----
async def connect_robot():
    devices = await BleakScanner.discover()
    i = 1
    for d in devices:
        print(f"{i}: {d}")
        i += 1
        if d.name and ROBOT_NAME in d.name:
            return BleakClient(d.address)
    return None

async def handler(websocket):
    print("Scratch connected")
    async for message in websocket:
        print("From Scratch:", message)
        if message.startswith("F,"):
            print ("received forward(", message[2:], ")")
        elif message.startswith("P,"):
            print ("received right(", message[2:], ")")
        elif message.startswith("L,"):
            print ("received left(", message[2:], ")")
        elif message == "SONAR":
            print ("received SONAR")
            await websocket.send(str(random.randint(0, 100)))
        else:
            print ("unknown command")
        
        # tutaj parsujesz komendy i wysyłasz przez BLE

async def main():
    # robot = await connect_robot()
    if robot == None:
        print("Could not find the robot")
    async with websockets.serve(handler, "localhost", 8765):
        print("WS server started")
        await asyncio.Future()  # run forever

asyncio.run(main())
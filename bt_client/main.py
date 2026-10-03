# libs: websockets, asyncio, bleak

import asyncio
from bleak import BleakClient, BleakScanner
import websockets
import random

CMD_UUID = "beb5483e-36e1-4688-b7f5-ea07361b26a8"
ROBOT_NAME = "Tymek rover"
robot = None

# ---- BLE ----
async def connect_robot():
    devices = await BleakScanner.discover()
    i = 1
    for d in devices:
        print(f"{i}: {d}")
        i += 1
        if d.name and ROBOT_NAME in d.name:
            print(f"{d}")
            return BleakClient(d.address)
    return None

async def handler(websocket, robot):
    print("Scratch connected")
    print(f"robot {robot}")
    async for message in websocket:
        print("From Scratch:", message)
        if message.startswith("F,"):
            cmd = bytearray([0x01, 128 + 32, 128 + 32])
            await robot.write_gatt_char(CMD_UUID, cmd)
            print ("received forward(", message[2:], ")")
        elif message.startswith("P,"):
            cmd = bytearray([0x01, 128 - 32, 128 + 32])
            await robot.write_gatt_char(CMD_UUID, cmd)
            print ("received right(", message[2:], ")")
        elif message.startswith("L,"):
            cmd = bytearray([0x01, 128 + 32, 128 - 32])
            await robot.write_gatt_char(CMD_UUID, cmd)
            print ("received left(", message[2:], ")")
        elif message == "SONAR":
            print ("received SONAR")
            cmd = bytearray([0x02])
            await robot.write_gatt_char(CMD_UUID, cmd)
            await websocket.send(str(random.randint(0, 100)))
        else:
            print ("unknown command")
        
        # tutaj parsujesz komendy i wysyłasz przez BLE

async def main():
    robot = await connect_robot()
    if robot == None:
        print("Could not find the robot")
    async with robot:
        for service in robot.services:
            print("Service:", service.uuid)
            for char in service.characteristics:
                print("  Char:", char.uuid)

        async with websockets.serve(lambda ws: handler(ws, robot), "localhost", 8765):
            print("WS server started")
            await asyncio.Future()  # run forever

asyncio.run(main())

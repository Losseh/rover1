import asyncio

from bleak import BleakClient, BleakScanner


CMD_UUID = "beb5483e-36e1-4688-b7f5-ea07361b26a8"
ROBOT_NAME = "Tymek rover"


async def connect_robot():
    print("Scanning for robot...")

    devices = await BleakScanner.discover()

    for device in devices:
        print(f"Found: {device}")

        if device.name and ROBOT_NAME in device.name:
            print(f"Connecting to {device.name} ({device.address})...")
            return BleakClient(device.address)

    return None


async def keyboard_loop(robot):
    print()
    print("Connected.")
    print("Type a message and press Enter to send it.")
    print("Press Ctrl+C to exit.")
    print()

    while True:
        message = await asyncio.to_thread(input, "> ")

        data = message.encode("utf-8")

        print(f"Sending: {data.hex()}")

        await robot.write_gatt_char(CMD_UUID, data)


async def main():
    robot = await connect_robot()

    if robot is None:
        print(f"Could not find '{ROBOT_NAME}'")
        return

    try:
        async with robot:
            print("Connected to robot.")

            for service in robot.services:
                print("Service:", service.uuid)

                for char in service.characteristics:
                    print("  Char:", char.uuid)

            await keyboard_loop(robot)

    except KeyboardInterrupt:
        print("\nExiting...")


asyncio.run(main())



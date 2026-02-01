import time
import random
import logging
from datetime import datetime

# ---------------- Configuration ----------------
POLL_INTERVAL = 0.5  # seconds

REG_CONFIG = 0x00
REG_DATA   = 0x01
REG_STATUS = 0x02

# ---------------- Logging ----------------
logging.basicConfig(
    level=logging.INFO,
    format="%(asctime)s | %(message)s",
    datefmt="%H:%M:%S"
)

# ---------------- Custom Exceptions ----------------
class I2CTimeoutError(Exception):
    pass

class SensorDisconnectedError(Exception):
    pass

class InvalidSensorDataError(Exception):
    pass

# ---------------- Mock I2C Bus ----------------
class MockI2CBus:
    """
    Simulates an I2C bus for Windows testing.
    """

    def __init__(self):
        self.connected = True

    def write(self, register, value=None):
        if not self.connected:
            raise SensorDisconnectedError()

        # Random timeout simulation
        if random.random() < 0.05:
            raise I2CTimeoutError()

    def read_16bit(self, register):
        if not self.connected:
            raise SensorDisconnectedError()

        if random.random() < 0.05:
            raise I2CTimeoutError()

        # Random invalid data
        if random.random() < 0.1:
            return 0xFFFF

        return random.randint(1, 1000)

# ---------------- Sensor Logic ----------------
class Sensor:
    def __init__(self, bus: MockI2CBus):
        self.bus = bus

    def configure(self):
        self.bus.write(REG_CONFIG, 0x01)

    def is_connected(self):
        try:
            self.bus.write(REG_STATUS)
            return True
        except SensorDisconnectedError:
            return False

    def read_data(self):
        value = self.bus.read_16bit(REG_DATA)

        if value in (0x0000, 0xFFFF):
            raise InvalidSensorDataError()

        return value

# ---------------- Application Controller ----------------
class SensorManager:
    def __init__(self):
        self.bus = MockI2CBus()
        self.sensor = Sensor(self.bus)

    def connect(self):
        logging.info("Connecting to sensor...")
        self.sensor.configure()
        logging.info("Sensor connected and configured")

    def run(self):
        while True:
            try:
                if not self.sensor.is_connected():
                    raise SensorDisconnectedError()

                data = self.sensor.read_data()
                logging.info(f"Sensor reading: {data}")

            except InvalidSensorDataError:
                logging.warning("Invalid sensor data detected")

            except I2CTimeoutError:
                logging.error("I2C timeout occurred")

            except SensorDisconnectedError:
                logging.error("Sensor disconnected")
                time.sleep(1)
                self.bus.connected = True
                self.connect()

            time.sleep(POLL_INTERVAL)

# ---------------- Entry Point ----------------
if __name__ == "__main__":
    manager = SensorManager()
    manager.connect()
    manager.run()
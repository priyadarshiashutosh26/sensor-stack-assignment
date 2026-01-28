import time
import random
from datetime import datetime

ERROR_CODES = {
    "I2C_TIMEOUT": 101,
    "INVALID_DATA": 102,
    "SENSOR_DISCONNECTED": 103,
    "BUS_ERROR": 104
}

LOG_FILE = "error_codes.txt"

def log_error(code):
    with open(LOG_FILE, "a") as f:
        f.write(f"{code}\n")

def simulate_i2c_read():
    outcome = random.choice([
        "OK",
        "I2C_TIMEOUT",
        "INVALID_DATA",
        "SENSOR_DISCONNECTED",
        "BUS_ERROR"
    ])

    if outcome != "OK":
        error_code = ERROR_CODES[outcome]
        log_error(error_code)
        print(f"[{datetime.now()}] I2C Error: {outcome} ({error_code})")
    else:
        print(f"[{datetime.now()}] Sensor read OK")

def main():
    print("Starting I2C sensor simulation...\n")
    for _ in range(40):
        simulate_i2c_read()
        time.sleep(0.5)

if __name__ == "__main__":
    main()

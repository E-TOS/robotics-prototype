import struct
import serial


class Microcontroller:
    """Microcontroller or SoaC class acting as a `serial` wrapper.
    
    This will act as the mother class to represent the Arduino (Microcontroller) 
    that will contain other class component objects thanks to the wonders of
    OO composition design patterns.

    """

    def __init__(self, name, port, motors):
        self.name = name
        self.set_port(port)
        self.set_motors(motors)
        print(port.get_timeout())
        self.serial = serial.Serial(
            port.get_path(), port.get_baudrate(), timeout=port.get_timeout())

    def set_port(self, port):
        self.port = port

    def get_port(self):
        return self.port

    def set_motors(self, motors):
        self.motors = motors

    def get_motors(self):
        return self.motors

    def write(self, name, data):
        for motor in self.motors:
            if motor.name in name:
                print(data)
                motor.write(self.serial, data)

    def read(self, name):
        for motor in self.motors:
            if motor.name in name:
                return motor.read(self.serial)

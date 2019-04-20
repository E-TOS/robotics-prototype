#!/usr/bin/env python3
"""
The intention of this class is to provide an easy way to send/receive messages
accross a network using objects.

Warnings:
Note that testing sending and receiving using the same object won't work.
To do this locally you must create one Connection object specifically or sending,
the other only for receiving (and they must both be assigned different port numbers).
It has not yet been officially confirmed, but I think this issue does not exist when testing
on multiple machines with different IP addresses.
"""
import socket
import select

class Connection:
    __active_ctr = 0

    # during intialization, pass only the rover IP and common communication port
    def __init__(self, name, ip, port):
        type(self).__active_ctr += 1
        self.name = name
        self.ip = ip
        self.port = port
        print("total active:", type(self).__active_ctr)

    def __del__(self):
        type(self).__active_ctr -= 1

    def get_total_active(self):
        return type(self).__active_ctr

    def send(self, msg):
        print("UDP target IP:", self.ip)
        print("UDP target port:", self.port)
        print("message:", msg)

        # Internet --> AF_INET, UDP --> SOCK_DGRAM
        sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        sock.sendto(str.encode(msg), (self.ip, self.port))

    def receive(self, timeout=0):
        # Internet --> AF_INET, UDP --> SOCK_DGRAM
        sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        print("binding to")
        print("self.ip: ", self.ip)
        print("self.port: ", self.port)
        sock.bind((self.ip, self.port))

        # wait first until data available
        if timeout > 0:
            sock.setblocking(0)

            ready = select.select([sock], [], [], int(timeout))

            if ready[0]:
                data = sock.recv(1024)
                print("received message:", data.decode())

                return data.decode()
            else:
                print("Timeout limit exceeded, no data received")

                return ""

        else: # wait indefinitely
            while True:
                data, addr = sock.recvfrom(1024) # buffer size is 1024 bytes
                print("received message:", data.decode())

                return data.decode()

"""
c1 = Connection("c1", "127.0.0.1", 5005)
c2 = Connection("c2", "127.0.0.1", 5005)

c1.send("127.0.0.1", 5005)
c2.send("127.0.0.1", 5005)
c1.receive("127.0.0.1", 5005)
"""

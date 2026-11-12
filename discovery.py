"""Announce the host IP to ESP32 OTA clients on the local network."""

import socket


DISCOVERY_PORT = 8001
REQUEST = b"OTA_DISCOVER_V1"
REPLY = b"OTA_SERVER_V1"


def main():
    with socket.socket(socket.AF_INET, socket.SOCK_DGRAM) as sock:
        sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        sock.bind(("0.0.0.0", DISCOVERY_PORT))
        print(f"Listening for OTA discovery on UDP port {DISCOVERY_PORT}", flush=True)

        while True:
            message, address = sock.recvfrom(64)
            if message == REQUEST:
                sock.sendto(REPLY, address)
                print(f"Answered discovery request from {address[0]}", flush=True)


if __name__ == "__main__":
    main()

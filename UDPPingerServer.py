# UDPPingerServer.py
# We will need the following module to generate randomized lost packets

import random
from socket import *

# Create a UDP socket
# Notice the use of SOCK_DGRAM for UDP packets

serverSocket = socket(AF_INET, SOCK_DGRAM)

# Assign IP address and port number to socket
# Bind on all interfaces so clients can connect via localhost or external addresses.
serverSocket.bind(('0.0.0.0', 12000))

print("UDPPingerServer listening on 0.0.0.0:12000", flush=True)

while True:
    # Generate random number in the range of 0 to 10
    rand = random.randint(0, 10)
    
    # Receive the client packet along with the address it is coming from
    print("recieving")
    message, address = serverSocket.recvfrom(1024)
    # If rand is less is than 4, we consider the packet lost
    #and do not respond
    #if rand < 4:
        #continue
        
    # Otherwise, the server responds
    print(address)
    print(message)
    serverSocket.sendto(message, address)
    
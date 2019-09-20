import socket
import time

### Constants
PEER_ID = 1
SOCKET_PORT = 9000
UDP_BroadCast_PORT = 2000 #fixed for all peers

braodcaster = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)    # Create Datagram Socket (UDP)
braodcaster.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1) # Make Socket Reusable
braodcaster.setsockopt(socket.SOL_SOCKET, socket.SO_BROADCAST, 1) # Allow incoming broadcasts
braodcaster.setblocking(False)           
braodcaster.bind(("", SOCKET_PORT))

# Set a timeout so the socket does not block
# indefinitely when trying to receive data.
message = "mind cloud station , hello"

while True :
    try:
        braodcaster.sendto(message.encode(), ('<broadcast>', UDP_BroadCast_PORT))
        data, addr = braodcaster.recvfrom(1024)
        print(data.decode())
        if data:
            break
    except:
        pass
    
    time.sleep(1)
    
braodcaster.close()
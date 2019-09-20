import socket
import threading
import time

def running_server_indication():
    while True:
        print("running")
        time.sleep(10)
    
### Constants
PEER_ID = 1
UDP_SERVER_PORT = 9000
UDP_BroadCast_PORT = 2000  #fixed for all peers
DEFINED_MESSAGE = "mind cloud station , hello"
broadcast_listner = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)    # Create Datagram Socket (UDP)
broadcast_listner.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1) # Make Socket Reusable
broadcast_listner.setsockopt(socket.SOL_SOCKET, socket.SO_BROADCAST, 1) # Allow incoming broadcasts
broadcast_listner.setblocking(False)                                    # Set socket to non-blocking mode
broadcast_listner.bind(("", UDP_BroadCast_PORT ))
running_server_indication_thread = threading.Thread(target=running_server_indication)
running_server_indication_thread.start()
while True:
    try:
        data, addr = broadcast_listner.recvfrom(1024)
        data = data.decode()
        if data == DEFINED_MESSAGE:
            system_ip = ""
            host_info = socket.gethostbyname_ex(socket.gethostname())
            ip_list = host_info[2]
            for ip in ip_list:
                s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
                s.connect(('8.8.8.8', 53))
                system_ip = s.getsockname()[0]
            s.close()
            broadcast_listner.sendto(system_ip.encode(), addr)
    except:
        pass

broadcast_listner.close()
running_server_indication_thread.join()
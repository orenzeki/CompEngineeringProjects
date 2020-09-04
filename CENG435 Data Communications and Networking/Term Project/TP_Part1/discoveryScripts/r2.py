import socket
import time
import threading
import os
import sys


def main():
    r2_to_s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    r2_to_r1 = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    r2_to_r3 = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    r2_to_d = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    r1 = ("10.10.8.1", 3000)
    s = ("10.10.2.2", 3001)
    r3 = ("10.10.6.2", 3002)
    d = ("10.10.5.2", 3003)
    t1 = threading.Thread(target=threaded_send, args=(r2_to_s, s))		# thread that runs client for s communication
    t2 = threading.Thread(target=threaded_send, args=(r2_to_r1, r1))		# thread that runs client for r1 communication
    t3 = threading.Thread(target=threaded_send, args=(r2_to_r3, r3))		# thread that runs client for r3 communication
    t4 = threading.Thread(target=threaded_send, args=(r2_to_d, d))		# thread that runs client for d communication

    if os.path.isfile(os.path.join(sys.path[0], "link_costs.txt")):		# to remove existing "link_costs.txt" file in case of re-running the code
        os.remove(os.path.join(sys.path[0], "link_costs.txt"))
    t1.start()
    t2.start()
    t3.start()
    t4.start()
    t1.join()		# wait for t1 to finish
    t2.join()		# wait for t2 to finish
    t3.join()		# wait for t3 to finish
    t4.join()		# wait for t4 to finish

def threaded_send(sock, address):
    try:
        file = open("link_costs.txt", "a+")
        cost = 0
        for i in range(0, 100):
            msg = str(i)			# our message
            start_time = time.time()		# get time before sending messages in order to calculate RTT cost
            sock.sendto(msg.encode(), address)
            data, server = sock.recvfrom(1024)
            if data:
                end_time = time.time()		# get time after receiving the message
                cost += end_time - start_time	# add to total cost
            if i == 99:
                file.write(server[0] + " -> " + str(cost / 100) + "\n")		# write average cost in a file.
                file.close()		# close file
    finally:
        sock.close()			# close socket


if __name__ == '__main__':
    main()


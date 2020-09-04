import socket
import time
import threading
import os
import sys


def main():
    r1_to_r2 = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    r1_to_r2.bind(("10.10.8.1", 3000))
    r1_to_s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    s = ("10.10.1.1", 4000)
    r1_to_d = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    d = ("10.10.4.2", 4001)

    t1 = threading.Thread(target=threaded_ack, args=(r1_to_r2,)) 	# thread that runs server for r2 communication
    t2 = threading.Thread(target=threaded_send, args=(r1_to_s, s)) 	# thread that runs client for s communication
    t3 = threading.Thread(target=threaded_send, args=(r1_to_d, d))	# thread that runs client for d communication

    if os.path.isfile(os.path.join(sys.path[0], "link_costs.txt")):	# to remove existing "link_costs.txt" file in case of re-running the code
        os.remove(os.path.join(sys.path[0], "link_costs.txt"))
    t1.start()
    t2.start()
    t3.start()
    t1.join()		# wait for t1 to finish
    t2.join()		# wait for t2 to finish
    t3.join()		# wait for t3 to finish

def threaded_ack(sock):
    exp_msg_len = 100				# number of expected messages
    cur_msg_len = 0				# current number of incoming messages
    while True:
        a, b = sock.recvfrom(1024)
        if a:
            sock.sendto(a, b)
            cur_msg_len += 1
            if cur_msg_len == exp_msg_len:		# break from while loop if all the messages received
                print(b[0] + " -> [OK]\n")
                break

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
                file.write(server[0] + " -> " + str(cost / 100) + "\n") 	# write average cost in a file.
                file.close()		# close file
    finally:
        sock.close()		# close socket


if __name__ == '__main__':
    main()


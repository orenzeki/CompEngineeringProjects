import socket
import threading


def main():
    s_to_r2 = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    s_to_r2.bind(("10.10.2.2", 3001))
    s_to_r1 = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    s_to_r1.bind(("10.10.1.1", 4000))
    s_to_r3 = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    s_to_r3.bind(("10.10.3.1", 5000))

    t1 = threading.Thread(target=threaded_ack, args=(s_to_r2,))		# thread that runs server for r2 communication
    t2 = threading.Thread(target=threaded_ack, args=(s_to_r1,))		# thread that runs server for r1 communication
    t3 = threading.Thread(target=threaded_ack, args=(s_to_r3,))		# thread that runs server for r3 communication

    t1.start()
    t2.start()
    t3.start()
    t1.join()	# wait for t1 to finish
    t2.join()	# wait for t1 to finish
    t3.join()	# wait for t1 to finish


def threaded_ack(sock):
    exp_msg_len = 100		# number of expected messages
    cur_msg_len = 0		# current number of incoming messages
    while True:
        a, b = sock.recvfrom(1024)
        if a:
            sock.sendto(a, b)	# send back incoming messages
            cur_msg_len += 1
            if cur_msg_len == exp_msg_len:	# break from while loop if all the messages received
                print(b[0] + " -> [OK]\n")
                break


if __name__ == '__main__':
    main()

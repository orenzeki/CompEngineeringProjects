import socket

s_to_r3 = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
s_to_r3.bind(("10.10.3.2", 5000))

r3_to_d = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
r3 = ("10.10.7.1", 5001)
exp_msg_len = 100		# number of expected messages
cur_msg_len = 0			# current number of incoming messages
while True:
    a, b = s_to_r3.recvfrom(1024)
    if a:
        s_to_r3.sendto(a, r3)
        cur_msg_len+= 1
        print(a)
    if cur_msg_len == exp_msg_len:	# break from while loop if all the messages received
        break


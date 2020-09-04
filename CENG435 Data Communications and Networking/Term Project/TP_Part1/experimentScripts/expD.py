import socket
import ntplib

r3_to_d = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
r3_to_d.bind(("10.10.7.1", 5001))
exp_msg_len = 100		# number of expected messages
cur_msg_len = 0			# current number of incoming messages
cost = 0
try:
    file = open("delay_exp0.txt", "a+")
    while True:
        a, b = r3_to_d.recvfrom(1024)
        if a:
            cur_msg_len+=1
            ntp = ntplib.NTPClient()
            ntpResponse = ntp.request("172.17.3.27") 		#get the time from r3 node (we syncronized s and d nodes based on r3 node)
            if ntpResponse:
                dif= ntpResponse.tx_time - float(a.decode())	#calculate end-to-end delay
                cost += dif
                print(dif)
        if cur_msg_len == exp_msg_len:			# break from while loop if all the messages received
            print("-----" +str(cost/100)+"-----")
            file.write(str(cost/100)+"\n")			# write average cost in file
            file.close()		# close file
            break
except FileNotFoundError:
    print("File not found")

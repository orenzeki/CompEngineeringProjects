import socket
import ntplib


s_to_r3 = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
s = ("10.10.3.2", 5000)

try:
    for i in range(0, 100):
        ntp = ntplib.NTPClient()
        ntpResponse = ntp.request("172.17.3.27")		#get the time from r3 node (we syncronized s and d nodes based on r3 node)
        if ntpResponse:
            msg = str(ntpResponse.tx_time)			#this our message (the time based on r3 node just before sending the message)
            print(msg)
            s_to_r3.sendto(msg.encode(), s)
finally:
    s_to_r3.close()		# close socket


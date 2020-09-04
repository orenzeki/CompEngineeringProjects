import hashlib
import pickle
import socket
import sys
import threading
lock = threading.Lock()
ackNumber = 0

def main():
    if sys.argv[1] == "1":
        file = open('output1.txt', 'wb+')
        d_to_r3 = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        d_to_r3.bind(("10.10.7.1", 5000))
        while 1:
            message, b = d_to_r3.recvfrom(1024)
            if message:
                packet = pickle.loads(message)
                seqNumber = packet[0][0]
                checkSum = packet[0][1]
                data = packet[1]
                if checkSum == calculateChecksum(data):
                    d_to_r3.sendto(pickle.dumps(seqNumber + 1), b)
                    file.write(data)
                    print(data)
                else:
                    d_to_r3.sendto(pickle.dumps(seqNumber), b)
            if(seqNumber == 9999):
                file.close()
                break
    elif sys.argv[1] == "2":
        file = open('output2.txt', 'wb+')
        d_to_r1 = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        d_to_r1.bind(("10.10.4.2", 6000))
        d_to_r2 = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        d_to_r2.bind(("10.10.5.2", 7000))
        r1Thread = threading.Thread(target=receive, args=(d_to_r1, file))
        r2Thread = threading.Thread(target=receive, args=(d_to_r2, file))
        r1Thread.start()
        r2Thread.start()
        r1Thread.join()
        r2Thread.join()
    else:
        print("3")


def receive(sock, file):
    global ackNumber
    while 1:
        if(ackNumber == 10001):
            file.close()
            break
        message, b = sock.recvfrom(1024)
        if message:
            lock.acquire()
            packet = pickle.loads(message)
            seqNumber = packet[0][0]
            checkSum = packet[0][1]
            data = packet[1]
            if checkSum == calculateChecksum(data):
                sock.sendto(pickle.dumps(ackNumber + 1), b)
                file.write(data)
                print(packet)
            else:
                sock.sendto(pickle.dumps(ackNumber), b)    
            ackNumber+=1
            lock.release()
            

def calculateChecksum(message):
    checksum = hashlib.md5(message).hexdigest()
    return checksum


if __name__ == '__main__':
    main()


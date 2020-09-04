import hashlib
import socket
import sys
import pickle
import threading

seqNumber = -1
messageSize = 500
lock = threading.Lock()


def main():
    global seqNumber
    with open('input.txt', 'rb') as file:
        file_data = file.read()
    file.close()
    if sys.argv[1] == "1":
        print("Starting experiment 1...")
        r3_to_d = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        s_r3_d = ("10.10.7.1", 5000)
        ack_Number = 0
        for i in range(10000):
            seqNumber += 1
            message = file_data[i * messageSize:(i + 1) * messageSize]
            check_sum = calculateChecksum(message)
            packet = makePacket(message, check_sum)
            while ack_Number != (seqNumber + 1):
                r3_to_d.sendto(pickle.dumps(packet), s_r3_d)
                a, b = r3_to_d.recvfrom(1024)
                ack_Number = pickle.loads(a)
                print(seqNumber)

    elif sys.argv[1] == "2":
        print("Starting experiment 2...")
        r1_to_d = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        s_r1_d = ("10.10.4.2", 6000)
        r2_to_d = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        s_r2_d = ("10.10.5.2", 7000)
        r1Thread = threading.Thread(target=send, args=(file_data, r1_to_d, s_r1_d))
        r2Thread = threading.Thread(target=send, args=(file_data, r2_to_d, s_r2_d))
        r1Thread.start()
        r2Thread.start()
        r1Thread.join()
        r2Thread.join()
    else:
        print("incorrect arguments! Give 1 or 2 as the argument")


def send(data, sock, addr):
    global seqNumber
    ackNumber = 0
    while ackNumber < 10000:
        lock.acquire()
        message = data[ackNumber * messageSize:(ackNumber + 1) * messageSize]
        check_sum = calculateChecksum(message)
        packet = makePacket(message, check_sum)
        while ackNumber != (seqNumber + 1):
            sock.sendto(pickle.dumps(packet), addr)
            a, b = sock.recvfrom(1024)
            ackNumber = pickle.loads(a)
            print(ackNumber)
        seqNumber += 1
        lock.release()

def makePacket(message, checksum):
    global seqNumber
    header = [seqNumber, checksum]
    packet = [header, message]
    return packet


def calculateChecksum(message):
    checksum = hashlib.md5(message).hexdigest()
    return checksum


if __name__ == '__main__':
    main()


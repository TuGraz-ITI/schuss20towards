#!/usr/bin/env python3
import socket
import struct
import atexit
import pandas as pd

from collections import namedtuple
from datetime import datetime

UDP_IP="0.0.0.0"
UDP_PORT=5500

sock = socket.socket(socket.AF_INET,socket.SOCK_DGRAM)
sock.bind((UDP_IP, UDP_PORT))

#Scan=namedtuple("Scan",["chanspec","power","iq_prod","i_pwr","q_pwr","seq_nr","tsf_l", "tsf_h"])
Scan=namedtuple("Scan",["chanspec","power","iq_prod","i_pwr","q_pwr","seq_nr","tsf"])
#Scan=namedtuple("Scan",["chanspec","power","iq_prod","i_pwr","q_pwr","seq_nr"])

last=datetime.utcnow()
lst=[]

# Print iterations progress
def printProgressBar (iteration, total, prefix = '', suffix = '', decimals = 1, length = 100, fill = '█', printEnd = "\r"):
    """
    Call in a loop to create terminal progress bar
    @params:
        iteration   - Required  : current iteration (Int)
        total       - Required  : total iterations (Int)
        prefix      - Optional  : prefix string (Str)
        suffix      - Optional  : suffix string (Str)
        decimals    - Optional  : positive number of decimals in percent complete (Int)
        length      - Optional  : character length of bar (Int)
        fill        - Optional  : bar fill character (Str)
        printEnd    - Optional  : end character (e.g. "\r", "\r\n") (Str)
    """
    percent = ("{0:." + str(decimals) + "f}").format(100 * (iteration / float(total)))
    filledLength = int(length * iteration // total)
    bar = fill * filledLength + '-' * (length - filledLength)
    print('\r%s |%s| %s%% %s' % (prefix, bar, percent, suffix), end = printEnd)
    # Print New Line on Complete
    if iteration == total: 
        print()

def finish():
    global lst
    df=pd.DataFrame(lst)
    end=datetime.utcnow()
    end_time=end.timestamp()
    df.to_csv("rssi-%d.csv"%end_time)

atexit.register(finish)

SAMPLES=3e6
count=0;

try:
    while True:
        data, addr = sock.recvfrom(1024)
        #s=Scan._make(struct.unpack("<HIIIIIII",data))
        s=Scan._make(struct.unpack("<HIIIIIQ",data))
        time=datetime.utcnow()
        delta=time-last
        d=delta.seconds*1000000+delta.microseconds
        d=time.strftime('%Y-%m-%d %H:%M:%S.%f')
        #print("{0:10d} {1:10d} {2:10d}".format(d, s.power, s.seq_nr))
        #print("{0:s} {3:16d} {4:16d} {1:10d} {2:10d}".format(d, s.power, s.seq_nr, s.tsf_l, s.tsf_h))
        #print("{0:s} {3:16d} {1:10d} {2:10d}".format(d, s.power, s.seq_nr, s.tsf))
        count+=1
        if count==SAMPLES:
            exit(0)
        last=time
        lst.append({"time":d,"chanspec":s.chanspec,"power":s.power,"iq_prod":s.iq_prod,"i_pwr":s.i_pwr,"q_prw":s.q_pwr,"tsf":s.tsf})
        printProgressBar(count, SAMPLES, prefix = 'Progress:', suffix = 'Complete', length = 50)

        #lst.append({"time":time,"power":s.power})
except Exception as e:
    print(e)


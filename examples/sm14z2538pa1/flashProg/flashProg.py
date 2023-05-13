#!/usr/bin/python3.5
#import os
import string
import time
import subprocess
import  select

def cmd(command):
    CMD1 = "JLinkExe -device CC2640R2F -if JTAG -speed 4000  -jtagconf -1,-1 -autoconnect 1 -CommandFile cmdfile.txt"
    subp = subprocess.Popen(CMD1,shell=True,stdin=subprocess.PIPE,stdout=subprocess.PIPE,stderr=subprocess.PIPE)
    ysel = select.poll()
    ysel.register(subp.stdout,select.POLLIN)

    #time.sleep(1)
    i=0
    while i<3:
        if ysel.poll(2000): # ms
            print(subp.stdout.readline().strip())
            subp.stdout.flush()
            i = 0
        else :
            print("Wait:",i)
            i += 1


    time.sleep(1)
    subp.stdin.write("connect".encode())
    subp.stdin.flush()
    #subp.write()
    #subp.communicate('connect')
    subp.stdin.flush()
    i=0
    while i<3:
        if ysel.poll(2000):
            print(subp.stdout.readline().strip())
            subp.stdout.flush()
            i = 0
        else :
            print("Wait:",i)
            i += 1


    print("quit:")
    subp.stdin.write('q'.encode())
    subp.stdin.flush()
    i = 0
    while i<3:
        if ysel.poll(2000):
            print(subp.stdout.readline().strip())
            subp.stdout.flush()
            i = 0
        else :
            print("Wait:",i)
            i += 1




print("start...\r\n")



#cmd("df -h")

cmd("JLinkExe")

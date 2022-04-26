#!/usr/bin/python3
import subprocess
import sys
import signal
import time

bots = int(sys.argv[1])

threads = []

for i in range(bots):
    bot_name = "bot_%.3d" % i

    threads.append(subprocess.Popen("./5_MobHitterExample --address 192.168.122.28:10001 --login %s" % bot_name, shell=True))
    time.sleep(0.5)

def signal_handler(sig, frame):
    for thread in threads:
        thread.kill()
    print('You pressed Ctrl+C!')
    sys.exit(0)

signal.signal(signal.SIGINT, signal_handler)
signal.pause()


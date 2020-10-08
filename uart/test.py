#!/usr/bin/python3
import os
import sys
import fcntl
import errno
import time

buf_size = 128

def try_read(p):
    try:
        buf = os.read(p, 1)
    except OSError as err:
        if err.errno == errno.EAGAIN or err.errno == errno.EWOULDBLOCK:
            return 0
        else:
            raise

    return 1

def test1(pin, pout):
    ## write almost full buffer
    for i in range(buf_size - 1):
        os.write(pin, b'1')

    ## try read back, must get nothing
    if try_read(pout) != 0:
        print("first read has data")
        return 1

    ## write last byte to make full buffer
    os.write(pin, b'1')

    ## wait for program to process
    time.sleep(1)

    ## try read output, must get full buffer
    cnt = 0
    while try_read(pout) == 1:
        cnt += 1

    if cnt != buf_size:
        print("Secont read failed, cnt = ", cnt, "must be ", buf_size)
        return 1

    return 0


if __name__ == "__main__":
    if len(sys.argv) < 3:
        print("Usage %s pipe_in pipe_out" % (sys.argv[0]))
        exit(1)

    pin = os.open(sys.argv[1], os.O_WRONLY | os.O_NONBLOCK)
    pout = os.open(sys.argv[2], os.O_RDONLY | os.O_NONBLOCK)

    ret = test1(pin, pout)
    if ret == 0:
        print("Test 1: PASS")
    else:
        print("Test 1: FAIL")

    exit(ret)


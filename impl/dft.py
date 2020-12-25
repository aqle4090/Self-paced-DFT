from __future__ import print_function

import sys

sys.path.append('/home/xilinx')
from pynq import Overlay
from pynq import allocate
import numpy as np
from time import time

if __name__ == "__main__":
    print("Entry:", sys.argv[0])
    print("System argument(s):", len(sys.argv))

    print("Start of \"" + sys.argv[0] + "\"")

    ol = Overlay("/home/xilinx/IPBitFile/Yu_Chi/dft.bit")
    regIP = ol.dft_0
    
    numSamples = 8
    inBuffer_real = allocate(shape=(numSamples,), dtype=np.float)
    inBuffer_img = allocate(shape=(numSamples,), dtype=np.float)
    outBuffer_real = allocate(shape=(numSamples,), dtype=np.float)
    outBuffer_img = allocate(shape=(numSamples,), dtype=np.float)

    for i in range(numSamples):
        inBuffer_real[i] = i
        inBuffer_img[i] = 0.0
    timeKernelStart = time()
    #regIP.write(0x80, len(inBuffer0) * 4)
    regIP.write(0x10, inBuffer_real.device_address)
    regIP.write(0x18, inBuffer_img.device_address)
    regIP.write(0x20, outBuffer_real.device_address)
    regIP.write(0x28, outBuffer_img.device_address)
    regIP.write(0x00, 0x01)
    while (regIP.read(0x00) & 0x4) == 0x0:
        continue
    timeKernelEnd = time()
    print("Kernel execution time: " + str(timeKernelEnd - timeKernelStart) + " s")

    print(" Output = " + str(outBuffer_real[numSamples-1]))
    print("============================")
    print("Exit process")
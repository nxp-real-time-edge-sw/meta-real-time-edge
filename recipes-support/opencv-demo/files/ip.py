#
# Copyright 2020-2022 NXP
#
# Author: Sri Janani Ganeshan <srijanani.ganeshan@nxp.com>
#
# SPDX-License-Identifier: MIT
#

import cv2
import time
import os
def fun():
    width=1280
    height=720
    cap =cv2.VideoCapture(0)
    cap.set(cv2.CAP_PROP_FPS, 30)
    x=1
    start_time = time.time()
    count=0
    while(True):

        ret, frame = cap.read()
        count+=1
        if (time.time() - start_time) > x:
            F = count/(time.time() - start_time)
            print("FPS:",F)
            count = 0
            start_time=time.time()
            os.system("echo {} > /etc/tsn/qbv/file.txt".format(F))

        ret, buffer = cv2.imencode('.jpg',frame)
        frame = buffer.tobytes()
        yield (b'--frame\r\n'
                b'Content-Type:image/jpeg\r\n\r\n' + frame + b'\r\n')

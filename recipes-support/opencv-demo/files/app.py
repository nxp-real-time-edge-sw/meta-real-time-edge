#
# Copyright 2020-2022 NXP
#
# Author: Sri Janani Ganeshan <srijanani.ganeshan@nxp.com>
#
# SPDX-License-Identifier: MIT
#

#!/usr/bin/env python

from importlib import import_module
import os
from flask import Flask, render_template, Response
from ip import fun

app = Flask(__name__)


@app.route('/')
def index():
    """Video streaming home page."""
    return render_template('index.html')

@app.route('/video_feed')
def video_feed():
    print ("hello")
    """Video streaming route. Put this in the src attribute of an img tag."""
    return Response(fun(),mimetype='multipart/x-mixed-replace; boundary=frame')

if __name__ == '__main__':
    app.run(host='172.15.0.5',port=5000, threaded=True)

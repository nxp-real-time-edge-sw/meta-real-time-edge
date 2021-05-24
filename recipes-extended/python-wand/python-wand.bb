DESCRIPTION = "Wand is a ctypes-based simple ImageMagick binding for Python, supporting 2.7, 3.3+, and PyPy. All functionalities of MagickWand API are implemented in Wand."
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://LICENSE;md5=2188864d3101f5fe8ae2f89a3d7c81cb"

SRC_URI = "git://github.com/emcconville/wand.git;protocol=https"
SRCREV="cf0bd0e5f7803a03918bab956fd31ae6c87ae31d"

S = "${WORKDIR}/git"
PV = "Wand-0.6.6"

inherit setuptools3

DESCRIPTION = "CanFestival is an OpenSource CANOpen framework."
LICENSE = "GPLv2"
LIC_FILES_CHKSUM = "file://LICENCE;md5=085e7fb76fb3fa8ba9e9ed0ce95a43f9"

SRC_URI = "http://sources.buildroot.net/canfestival/canfestival-7740ac6fdedc23e1ed6908d3d7db54833c88572b.tar.gz \
	file://0001-install-pkgconfig-module-for-canfestival.patch \
	file://0002-allow-to-set-python-interpreter.patch \
	file://0003-canfestival-add-new-app.patch \
        file://0004-compile-CANopen-on-different-platforms.patch \
"
SRC_URI[sha256sum] = "e9ecd97e1a3384caec6304ed57b9f1af9b6ca8f3fe90a2e37173a0680722ee82"

S = "${WORKDIR}/${PN}-${PV}"

DEPENDS += "python3-native"

DISABLE_STATIC = ""
EXTRA_OECONF = ' --target=unix \
                 --arch=${TARGET_ARCH} \
                 --timers=unix \
                 --binutils=${TARGET_PREFIX} \
                 --cc="${CC}" \
                 --cxx="${CC}" \
                 --ld="${CC}" \
                 --prefix=/usr \
                 --can="socket" \
                 --SDO_MAX_LENGTH_TRANSFER=512 \
                 --SDO_BLOCK_SIZE=75 \
                 --SDO_MAX_SIMULTANEOUS_TRANSFERS=1 \
'
do_configure() {
    ./configure ${EXTRA_OECONF}
}

EXTRA_OEMAKE = "PYTHON=${STAGING_DIR_NATIVE}/usr/bin/python-native/python2"
do_compile() {
    oe_runmake all ${EXTRA_OEMAKE}
}

do_install() {
    oe_runmake -C ${S}/src install ${EXTRA_OEMAKE} DESTDIR=${D}
    oe_runmake -C ${S}/drivers install ${EXTRA_OEMAKE} DESTDIR=${D}
    oe_runmake -C ${S}/app install ${EXTRA_OEMAKE} DESTDIR=${D}
    mv ${D}/${libdir}/libcanfestival_can_socket.so ${D}/${libdir}/libcanfestival_can_socket.so.1.0
    ln -s -r ${D}/${libdir}/libcanfestival_can_socket.so.1.0 ${D}/${libdir}/libcanfestival_can_socket.so
}

INSANE_SKIP:${PN} = "ldflags"
INSANE_SKIP:${PN}-dev = "ldflags"

FILES_SOLIBSDEV = "${libdir}/libcanfestival_can_socket.so"

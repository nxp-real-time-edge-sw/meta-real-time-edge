
EXTRA_PROVIDES:append:ls1028a = " virtual/libgles3"

FILES:${PN}:ls1028a += "${libdir}/ld-linux-aarch64.so.1"

do_install:append:ls1028a() {
    ln -s ../../lib/ld-linux-aarch64.so.1 ${D}${libdir}/ld-linux-aarch64.so.1
}
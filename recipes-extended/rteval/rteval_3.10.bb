SUMMARY = "Real-Time performance evaluation"
HOMEPAGE = "https://wiki.linuxfoundation.org/realtime/documentation/howto/tools/rteval"

LICENSE = "GPL-2.0-or-later"
LIC_FILES_CHKSUM = "file://COPYING;md5=b234ee4d69f5fce4486a80fdaf4a4263"

SRC_URI = "git://git.kernel.org/pub/scm/utils/rteval/rteval.git;nobranch=1;protocol=https \
           file://rteval.conf \
           file://0001-rteval-Tailored-for-NXP-boards-and-Yocto-rootfs.patch \
"

SRCREV = "73caa5a9f85262954e277807b227b1dd7288e028"

inherit python_setuptools_build_meta

do_install:append () {
        install -d ${D}/${sysconfdir}
        install -m 0644 ${UNPACKDIR}/rteval.conf ${D}/${sysconfdir}/

        if [ -e ${D}/${sysconfdir}/rteval.conf ]; then
            sed -e '/stressng/d' -i ${D}/${sysconfdir}/rteval.conf
        fi
}

do_install:append:mx6ull-nxp-bsp () {
        if [ -e ${D}/${sysconfdir}/rteval.conf ]; then
            sed -e '/kcompile/d' -i ${D}/${sysconfdir}/rteval.conf
        fi
}

RDEPENDS:${PN} += " \
    rt-tests \
    stress-ng \
    python3-core \
    python3-lxml \
    libxml2 \
"

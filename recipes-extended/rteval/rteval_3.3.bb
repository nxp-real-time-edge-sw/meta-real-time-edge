SUMMARY = "Real-Time performance evaluation"
HOMEPAGE = "https://wiki.linuxfoundation.org/realtime/documentation/howto/tools/rteval"

LICENSE = "GPL-2.0-only"
LIC_FILES_CHKSUM = "file://COPYING;md5=b234ee4d69f5fce4486a80fdaf4a4263"

SRC_URI = "git://git.kernel.org/pub/scm/utils/rteval/rteval.git;nobranch=1 \
           file://rteval.conf \
           file://0001-rteval-Tailored-for-NXP-boards-and-Yocto-rootfs.patch \
           file://0002-rteval-setup.py-replace-distutils.core-with-setuptoo.patch \
"

SRCREV = "d83a407fb55bf2a759097f95a8e8337699b9dfa2"

S = "${WORKDIR}/git"

inherit setuptools3

do_install:append () {
        install -d ${D}/${sysconfdir}
       # install -m 0644 ${WORKDIR}/rteval.conf ${D}/${sysconfdir}/

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
    libxml2 \
    libxml2-python \
    python3-lxml \
    python3-xmlrpc \
"

SUMMARY = "yang model"
DESCRIPTION = "yang model files for sysrepo netopeer2 and sysrepo-tsn"
LICENSE = "Apache-2.0"
LIC_FILES_CHKSUM = "file://testall.sh;md5=b523c10b1e08d72d780d4414e0a1c125"

SRC_URI = "git://github.com/YangModels/yang.git;protocol=https;nobranch=1 \
           file://0001-ieee802-dot1q-sched.yang-fix-warning-in-libyang.patch \
           file://0002-ieee802-dot1q-bridge.yang-fix-warning-in-libyang.patch \
"
SRCREV = "b1df41d72ecbfb38c13e96eff42d8f027997baed"

S = "${WORKDIR}/git"

DEPENDS = ""
RDEPENDS:${PN} += "bash curl"

do_install:append () {
    install -d ${D}/etc/yang-model/

    install -m 0775 ${S}/standard/ietf/RFC/ietf-interfaces@2014-05-08.yang ${D}/etc/yang-model/
    install -m 0775 ${S}/standard/ieee/draft/802.1/Qcw/*.yang ${D}/etc/yang-model/
    install -m 0775 ${S}/standard/ietf/RFC/ietf-yang-types.yang ${D}/etc/yang-model/
    install -m 0775 ${S}/standard/ieee/published/802.1/ieee802-dot1q-types.yang ${D}/etc/yang-model/
    install -m 0775 ${S}/standard/ieee/published/802/ieee802-types.yang ${D}/etc/yang-model/
    install -m 0775 ${S}/standard/ieee/published/802.1/*.yang ${D}/etc/yang-model/
    install -m 0775 ${S}/standard/ieee/draft/802.1/Qcr/*.yang ${D}/etc/yang-model/
    install -m 0775 ${S}/standard/ietf/RFC/ietf-inet-types@2013-07-15.yang ${D}/etc/yang-model/
    install -m 0775 ${S}/standard/ietf/RFC/iana-if-type@2017-01-19.yang ${D}/etc/yang-model/
}

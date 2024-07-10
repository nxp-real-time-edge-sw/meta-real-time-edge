require ../../../../recipes-kernel/include/la93xx-repo.inc

KERNEL_SRC:real-time-edge = "${REAL_TIME_EDGE_LINUX_SRC};branch=${REAL_TIME_EDGE_LINUX_BRANCH}"
SRCBRANCH:real-time-edge = "${REAL_TIME_EDGE_LINUX_BRANCH}"
SRCREV:real-time-edge = "${REAL_TIME_EDGE_LINUX_SRCREV}"
SRC_URI = "${KERNEL_SRC}"

KBUILD_DEFCONFIG ?= "imx8mp_rfnm_defconfig"

FILESEXTRAPATHS:prepend := "${THISDIR}/${PN}:"
SRC_URI:append:real-time-edge = " \
    file://linux-imx8.config \
    file://linux-wifi.config \
    file://linux-baremetal.config \
    file://linux-baremetal-imx93.config \
    file://linux-selinux.config \
    file://linux-fec.config \
    file://linux-rpmsg-8m-buf.config \
"

SRC_URI:append:real-time-edge-plc = " \
    file://linux-fec-ecat.config \
    file://linux-imx6ullevk.config \
    file://0001-fec_ecat-add-fec-native-driver-for-raw-packet-proto.patch \
    file://0002-fec_ecat-imx6ullevk-rebind-fec1-to-fec_ecat-driver.patch \
    file://0001-FEC_ECAT-rebind-fec-MAC-to-fec-native-driver-for-imx.patch \
"

SRC_URI:append:imx8mp-rfnm = " \
"

do_merge_delta_config:append:imx8mp-rfnm() {
    cp ${S}/arch/${ARCH}/configs/imx8mp_rfnm_defconfig  ${WORKDIR}/defconfig
}

do_copy_defconfig:append:imx8mp-rfnm() {
	cp ${S}/arch/arm64/configs/imx8mp_rfnm_defconfig ${B}/.config
}

do_configure:prepend:real-time-edge() {
    mkdir -p ${WORKDIR}/source-date-epoch
    date '+%s' > ${WORKDIR}/source-date-epoch/__source_date_epoch.txt
}


#REAL_TIME_EDGE_LINUX_SRC ?= "git://github.com/nxp-real-time-edge-sw/real-time-edge-linux.git;protocol=https"
#REAL_TIME_EDGE_LINUX_BRANCH ?= "linux_5.15.71"
#REAL_TIME_EDGE_LINUX_SRCREV ?= "c36e774d0d9a6d996cfb1b34167a2c5db1689fef"
#REAL_TIME_EDGE_LINUX_SRC ?= "git://git@bitbucket.sw.nxp.com/dn5g/5g-linux.git;protocol=ssh"
REAL_TIME_EDGE_LINUX_SRC ?= "git://github.com/nxp-qoriq/linux.git;protocol=https"
REAL_TIME_EDGE_LINUX_BRANCH ?= "la12xx-linux-5.15-rt"
REAL_TIME_EDGE_LINUX_SRCREV ?= "2f07f238a3dd34a42b946ffd01d4866675da5be0"

KERNEL_SRC:real-time-edge = "${REAL_TIME_EDGE_LINUX_SRC};branch=${REAL_TIME_EDGE_LINUX_BRANCH}"
SRCBRANCH:real-time-edge = "${REAL_TIME_EDGE_LINUX_BRANCH}"
SRCREV:real-time-edge = "${REAL_TIME_EDGE_LINUX_SRCREV}"
SRC_URI = "${KERNEL_SRC}"

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
    file://rfnm-0001-Add-imx8mp-rfnm-board-suppport.patch \
    file://rfnm-0002-Basic-structure-for-a-bit-banging-ws2812b-LED-driver.patch \
    file://rfnm-0003-Enable-IIO-subsystem.patch \
    file://rfnm-0004-Add-delay-between-SPI-CS-and-clock-as-per-Granita-s-.patch \
    file://rfnm-0005-Defer-PCIe-init-until-Si5510-is-providing-a-clock.patch \
    file://rfnm-0006-Add-bootconfig-eeprom-driver.patch \
    file://rfnm-0007-Hack-the-TCPM-driver-to-support-USB-PD-without-reque.patch \
"

do_configure:prepend:real-time-edge() {
    mkdir -p ${WORKDIR}/source-date-epoch
    date '+%s' > ${WORKDIR}/source-date-epoch/__source_date_epoch.txt
}

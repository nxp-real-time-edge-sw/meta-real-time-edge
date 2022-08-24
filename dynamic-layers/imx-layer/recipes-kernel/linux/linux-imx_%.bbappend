
KERNEL_SRC:real-time-edge = "git://github.com/real-time-edge-sw/real-time-edge-linux.git;protocol=https;nobranch=1"
SRCBRANCH:real-time-edge = "linux_5.15.y"
SRCREV:real-time-edge = "be88a251b281cbe654728e45b9cb8cab14416355"

FILESEXTRAPATHS:prepend := "${THISDIR}/${PN}:"
SRC_URI:append:real-time-edge = " \
    file://linux-imx8.config \
    file://linux-wifi.config \
    file://linux-baremetal.config \
    file://linux-selinux.config \
    file://linux-fec.config \
    file://0001-arm64-dts-imx8mm-evk-rpmsg-increase-the-reserved-mem.patch \
    file://0002-virtio_rpmsg_bus-increase-buffer-size.patch \
    file://0003-rpmsg-imx_rpmsg_tty-increase-the-max-buffer-size.patch \
"

do_configure:prepend:real-time-edge() {
    mkdir -p ${WORKDIR}/source-date-epoch
    date '+%s' > ${WORKDIR}/source-date-epoch/__source_date_epoch.txt
}

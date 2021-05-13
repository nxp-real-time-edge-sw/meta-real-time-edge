
KERNEL_SRC_rt-edge = "git://bitbucket.sw.nxp.com/dnind/industry-linux.git;protocol=ssh"
SRCBRANCH_rt-edge = "linux_5.10.y"
SRCREV_rt-edge = "ad3906065edc78ca77f5d707d751d040018a591f"

FILESEXTRAPATHS_prepend := "${THISDIR}/${PN}:"
SRC_URI_append_rt-edge = " \
    file://linux-rt.config \
    file://linux-wifi.config \
    file://linux-baremetal.config \
    file://linux-baremetal-ls1021a.config \
    file://linux-baremetal-ls1028a.config \
    file://linux-baremetal-lx2160a.config \
    file://linux-selinux.config \
"

DELTA_KERNEL_DEFCONFIG_append_rt-edge = " \
    linux-rt.config \
    ${@bb.utils.contains('DISTRO_FEATURES', 'wifi',  \
    'linux-wifi.config', '', d)} \
"


KERNEL_SRC_industrial = "git://bitbucket.sw.nxp.com/dnind/industry-linux.git;protocol=ssh"
SRCBRANCH_industrial = "linux_5.10.y"
SRCREV_industrial = "ad3906065edc78ca77f5d707d751d040018a591f"

FILESEXTRAPATHS_prepend := "${THISDIR}/${PN}:"
SRC_URI_append_industrial = " \
    file://linux-rt.config \
    file://linux-imx8.config \
    file://linux-wifi.config \
    file://linux-baremetal.config \
    file://linux-selinux.config \
"

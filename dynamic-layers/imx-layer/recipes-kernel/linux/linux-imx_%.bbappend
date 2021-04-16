
KERNEL_SRC_industrial = "git://bitbucket.sw.nxp.com/dnind/industry-linux.git;protocol=ssh"
SRCBRANCH_industrial = "linux_5.10.y"
SRCREV_industrial = "8627c381a5b2b7f9084eaab0ee981ea949e8b2fb"

FILESEXTRAPATHS_prepend := "${THISDIR}/${PN}:"
SRC_URI_append_industrial += " \
    file://linux-rt.config \
    file://linux-imx8.config \
    file://linux-wifi.config \
    file://linux-baremetal.config \
    file://linux-selinux.config \
"

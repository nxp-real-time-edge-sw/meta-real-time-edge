
REAL_TIME_EDGE_LINUX_SRC ?= "git://github.com/nxp-real-time-edge-sw/real-time-edge-linux.git;protocol=https;"
REAL_TIME_EDGE_LINUX_BRANCH ?= "linux_6.18.2"
REAL_TIME_EDGE_LINUX_SRCREV ?= "c34e839e84c96fff2628bf4b582b2b6de467dbf9"

KERNEL_SRC:real-time-edge = "${REAL_TIME_EDGE_LINUX_SRC}"
KERNEL_BRANCH:real-time-edge = "${REAL_TIME_EDGE_LINUX_BRANCH}"
SRCREV:real-time-edge = "${REAL_TIME_EDGE_LINUX_SRCREV}"
SRC_URI = "${KERNEL_SRC};branch=${KERNEL_BRANCH}"

FILESEXTRAPATHS:prepend := "${THISDIR}/${PN}:"

SRC_URI:append:baremetal:ls1046a = " file://linux-baremetal-ls104xa.config"
SRC_URI:append:baremetal:ls1043a = " file://linux-baremetal-ls104xa.config"
SRC_URI:append:baremetal:ls1021a = " file://linux-baremetal-ls1021a.config"
SRC_URI:append:baremetal:ls1028a = " file://linux-baremetal-ls1028a.config"
SRC_URI:append:baremetal:lx2160a = " file://linux-baremetal-lx2160a.config"

EXTRA_KBUILD_DEFCONFIG:append:baremetal:ls1046a = " linux-baremetal-ls104xa.config"
EXTRA_KBUILD_DEFCONFIG:append:baremetal:ls1043a = " linux-baremetal-ls104xa.config"
EXTRA_KBUILD_DEFCONFIG:append:baremetal:ls1021a = " linux-baremetal-ls1021a.config"
EXTRA_KBUILD_DEFCONFIG:append:baremetal:ls1028a = " linux-baremetal-ls1028a.config"
EXTRA_KBUILD_DEFCONFIG:append:baremetal:lx2160a = " linux-baremetal-lx2160a.config"

do_configure:prepend:real-time-edge() {
    mkdir -p ${WORKDIR}/source-date-epoch
    date '+%s' > ${WORKDIR}/source-date-epoch/__source_date_epoch.txt
}

do_configure:append() {
    for deltacfg in ${EXTRA_KBUILD_DEFCONFIG}; do
        if [ -f "${UNPACKDIR}/${deltacfg}" ]; then
            ${S}/scripts/kconfig/merge_config.sh -m .config ${UNPACKDIR}/${deltacfg}
        fi
    done
    cp .config ${UNPACKDIR}/defconfig
}

# Common i.MX EVK Multi-machine Examples Support
# This file provides shared multiconfig support for building examples from multiple i.MX variants
#
# Usage:
#   Python: add_mcdepends_with_params(d, evk_machine, mc_map)
#   Shell:  install_examples_with_params <evk_machine> <mc_map>

# Add mcdepends for EVK machine at parse time
def add_mcdepends_with_params(d, evk_machine, mc_map):
    machine = d.getVar('MACHINE')

    if machine != evk_machine:
        return

    mc_list = mc_map.split()

    mcdepends = ""
    for mapping in mc_list:
        mc = mapping.split(':')[0]
        mcdepends += " mc::%s:packagegroup-real-time-edge-rtos:do_build" % (mc)

    if mcdepends:
        d.appendVarFlag('do_image', 'mcdepends', mcdepends)
        bb.note("Added mcdepends for %s: %s" % (evk_machine, mcdepends.strip()))

# Generic function to install examples from all multiconfigs into rootfs
# Parameters: $1 = evk_machine, $2 = mc_map
install_examples_with_params() {
    local evk_machine="$1"
    local mc_map="$2"

    if [ "${MACHINE}" != "${evk_machine}" ]; then
        return
    fi

    bbnote "Installing ${evk_machine} examples from all machine variants"

    install -d ${IMAGE_ROOTFS}/examples/${evk_machine}
    if [ -d "${IMAGE_ROOTFS}/examples/heterogeneous-multicore" ]; then
        mv ${IMAGE_ROOTFS}/examples/heterogeneous-multicore ${IMAGE_ROOTFS}/examples/${evk_machine}
    fi

    if [ -d "${IMAGE_ROOTFS}/examples/mcuxsdk" ]; then
        mv ${IMAGE_ROOTFS}/examples/mcuxsdk ${IMAGE_ROOTFS}/examples/${evk_machine}
    fi

    for mapping in ${mc_map}; do
        mc="${mapping%%:*}"
        MACHINE_NAME="${mapping#*:}"

        if [ -z "${MACHINE_NAME}" ]; then
            bbwarn "Invalid mapping format: ${mapping}"
            continue
        fi

        MC_DEPLOY="${TOPDIR}/tmp-${mc}/deploy/images/${MACHINE_NAME}"

        if [ -d "${MC_DEPLOY}/examples/heterogeneous-multicore" ]; then
            install -d ${IMAGE_ROOTFS}/examples/${MACHINE_NAME}/heterogeneous-multicore
            cp -r ${MC_DEPLOY}/examples/heterogeneous-multicore/* \
                ${IMAGE_ROOTFS}/examples/${MACHINE_NAME}/heterogeneous-multicore/ 2>/dev/null || true
            bbnote "Installed HMC examples for ${MACHINE_NAME}"
        fi

        if [ -d "${MC_DEPLOY}/examples/mcuxsdk" ]; then
            install -d ${IMAGE_ROOTFS}/examples/${MACHINE_NAME}/mcuxsdk
            cp -r ${MC_DEPLOY}/examples/mcuxsdk/* \
                ${IMAGE_ROOTFS}/examples/${MACHINE_NAME}/mcuxsdk/ 2>/dev/null || true
            bbnote "Installed MCUX examples for ${MACHINE_NAME}"
        fi
    done
}

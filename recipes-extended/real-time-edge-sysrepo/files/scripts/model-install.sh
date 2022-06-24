#!/bin/bash
#
# Copyright 2019-2021 NXP
#
# Licensed under the Apache License, Version 2.0
#

set -eu -o pipefail

shopt -s failglob

local_path=$(dirname $0)

: ${SYSREPOCTL:=/usr/bin/sysrepoctl}
: ${SYSREPOCFG:=/usr/bin/sysrepocfg}
: ${SYSREPOCTL_ROOT_PERMS:=-o root:root -p 666}
: ${YANG_DIR:=/etc/sysrepo-tsn/modules}

is_yang_module_installed() {
    module=$1

    $SYSREPOCTL -l | grep -c "^$module [^|]*|[^|]*| Installed .*$" > /dev/null
}

install_yang_module() {
    module=$1

    if ! is_yang_module_installed $module; then
        echo "- Installing module $module..."
        $SYSREPOCTL -i -g ${YANG_DIR}/$module.yang $SYSREPOCTL_ROOT_PERMS
    else
        echo "- Module $module already installed."
    fi
}

enable_yang_module_feature() {
    module=$1
    feature=$2

    if ! $SYSREPOCTL -l | grep -c "^$module [^|]*|[^|]*|[^|]*|[^|]*|[^|]*|[^|]*|.* $feature.*$" > /dev/null; then
        echo "- Enabling feature $feature in $module..."
        $SYSREPOCTL -m $module -e $feature
    else
        echo "- Feature $feature in $module already enabled."
    fi
}

install_yang_module ietf-interfaces
install_yang_module ieee802-dot1q-types
install_yang_module ieee802-dot1q-preemption
enable_yang_module_feature ieee802-dot1q-preemption frame-preemption

install_yang_module ieee802-dot1q-sched
enable_yang_module_feature ieee802-dot1q-sched scheduled-traffic

install_yang_module iana-if-type@2020-01-10

install_yang_module ieee802-dot1q-bridge
install_yang_module ietf-yang-types
install_yang_module ieee802-types
install_yang_module ieee802-dot1q-stream-filters-gates
enable_yang_module_feature ieee802-dot1q-stream-filters-gates closed-gate-state
install_yang_module ieee802-dot1q-psfp
enable_yang_module_feature ieee802-dot1q-psfp psfp
install_yang_module ieee802-dot1cb-stream-identification
install_yang_module ieee802-dot1q-qci-augment
install_yang_module ietf-ip
enable_yang_module_feature ietf-ip ipv4-non-contiguous-netmasks
install_yang_module nxp-bridge-vlan-tc-flower
install_yang_module ieee802-dot1cb-stream-identification-types
install_yang_module ieee802-dot1cb-frer

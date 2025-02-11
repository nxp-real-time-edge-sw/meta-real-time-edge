#!/usr/bin/env bash

# avoid problems with sudo PATH
if [ `id -u` -eq 0 ] && [ -n "$USER" ] && [ `command -v su` ]; then
    SYSREPOCTL=`su -c 'command -v sysrepoctl' -l $USER`
else
    SYSREPOCTL=`command -v sysrepoctl`
fi

MODDIR="/usr/share/yang/modules/sysrepo-tsn"
PERMS="600"
OWNER="root"
GROUP="root"

# modules to install
MODULES=(
"ietf-interfaces@2018-02-20.yang"
"ietf-ip@2018-02-22.yang -e ipv4-non-contiguous-netmasks"
"iana-if-type@2023-01-26.yang"
"ieee802-types@2023-10-22.yang"
"ieee802-dot1q-types@2023-10-26.yang"
"ieee802-dot1q-qci-augment@2024-12-18.yang"
"ieee802-dot1q-pb@2023-10-22.yang"
"ieee802-dot1q-bridge@2023-10-26.yang"
"ieee802-dot1cb-frer-types@2021-12-08.yang"
"ieee802-dot1cb-frer@2021-12-08.yang"
"ieee802-dot1cb-stream-identification-types@2021-12-09.yang"
"ieee802-dot1cb-stream-identification@2021-12-08.yang"
"ieee802-dot1q-preemption@2023-10-26.yang -e frame-preemption"
"ieee802-dot1q-preemption-bridge@2023-10-26.yang -e frame-preemption"
"ieee802-dot1q-psfp@2023-10-26.yang -e psfp"
"ieee802-dot1q-psfp-bridge@2023-10-26.yang"
"ieee802-dot1q-sched@2023-10-22.yang -e scheduled-traffic"
"ieee802-dot1q-sched-bridge@2023-10-26.yang"
"ieee802-dot1q-stream-filters-gates@2023-07-03.yang -e closed-gate-state"
"ieee802-dot1q-stream-filters-gates-bridge@2023-07-03.yang"
"ietf-system@2014-08-06.yang -e authentication -e local-users"
"nxp-bridge-vlan-tc-flower@2020-04-02.yang"
)

CMD_INSTALL=

# functions
INSTALL_MODULE_CMD() {
    CMD_INSTALL="'$SYSREPOCTL' -s '$MODDIR' -v3"

    CMD_INSTALL="$CMD_INSTALL -i $MODDIR/$1 -p '$PERMS'"
    if [ ! -z "${OWNER}" ]; then
        CMD_INSTALL="$CMD_INSTALL -o '$OWNER'"
    fi
    if [ ! -z "${GROUP}" ]; then
        CMD_INSTALL="$CMD_INSTALL -g '$GROUP'"
    fi

    eval $CMD_INSTALL
    CMD_INSTALL=
}

UPDATE_MODULE() {
    CMD="'$SYSREPOCTL' -U $MODDIR/$1 -s '$MODDIR' -v2"
    eval $CMD
    local rc=$?
    if [ $rc -ne 0 ]; then
        exit $rc
    fi
}

CHANGE_PERMS() {
    CMD="'$SYSREPOCTL' -c $1 -p '$PERMS' -v2"
    if [ ! -z "${OWNER}" ]; then
        CMD="$CMD -o '$OWNER'"
    fi
    if [ ! -z "${GROUP}" ]; then
        CMD="$CMD -g '$GROUP'"
    fi
    eval $CMD
    local rc=$?
    if [ $rc -ne 0 ]; then
        exit $rc
    fi
}

ENABLE_FEATURE() {
    "$SYSREPOCTL" -c $1 -e $2 -v2
    local rc=$?
    if [ $rc -ne 0 ]; then
        exit $rc
    fi
}

# get current modules
SCTL_MODULES=`$SYSREPOCTL -l`

for i in "${MODULES[@]}"; do
    name=`echo "$i" | sed 's/\([^@]*\).*/\1/'`

    SCTL_MODULE=`echo "$SCTL_MODULES" | grep "^$name \+|[^|]*| I"`
    if [ -z "$SCTL_MODULE" ]; then
        # prepare command to install module with all its features
        INSTALL_MODULE_CMD "$i"
        continue
    fi

    sctl_revision=`echo "$SCTL_MODULE" | sed 's/[^|]*| \([^ ]*\).*/\1/'`
    revision=`echo "$i" | sed 's/[^@]*@\([^\.]*\).*/\1/'`
    if [ "$sctl_revision" \< "$revision" ]; then
        # update module without any features
        file=`echo "$i" | cut -d' ' -f 1`
        UPDATE_MODULE "$file"
    fi

    sctl_owner=`echo "$SCTL_MODULE" | sed 's/\([^|]*|\)\{3\} \([^:]*\).*/\2/'`
    sctl_group=`echo "$SCTL_MODULE" | sed 's/\([^|]*|\)\{3\}[^:]*:\([^ ]*\).*/\2/'`
    sctl_perms=`echo "$SCTL_MODULE" | sed 's/\([^|]*|\)\{4\} \([^ ]*\).*/\2/'`
    if [ "$sctl_perms" != "$PERMS" ] || [ ! -z "${OWNER}" -a "$sctl_owner" != "$OWNER" ] || [ ! -z "${GROUP}" -a "$sctl_group" != "$GROUP" ]; then
        # change permissions/owner
        CHANGE_PERMS "$name"
    fi

    # parse sysrepoctl features and add extra space at the end for easier matching
    sctl_features="`echo "$SCTL_MODULE" | sed 's/\([^|]*|\)\{6\}\(.*\)/\2/'` "
    # parse features we want to enable
    features=`echo "$i" | sed 's/[^ ]* \(.*\)/\1/'`
    while [ "${features:0:3}" = "-e " ]; do
        # skip "-e "
        features=${features:3}
        # parse feature
        feature=`echo "$features" | sed 's/\([^[:space:]]*\).*/\1/'`

        # enable feature if not already
        sctl_feature=`echo "$sctl_features" | grep " ${feature} "`
        if [ -z "$sctl_feature" ]; then
            # enable feature
            ENABLE_FEATURE $name $feature
        fi

        # next iteration, skip this feature
        features=`echo "$features" | sed 's/[^[:space:]]* \(.*\)/\1/'`
    done
done

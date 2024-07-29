FILESEXTRAPATHS:prepend := "${THISDIR}/files:"

SRC_URI:append = " \
    file://0001-taprio-add-hold-and-release-command.patch \
    file://0001-tc-add-frer-support.patch \
"

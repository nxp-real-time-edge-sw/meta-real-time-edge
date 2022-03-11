FILESEXTRAPATHS:prepend:qoriq := "${THISDIR}/${BPN}:"

SRC_URI:append:qoriq = "\
    file://pip_stress-De-constify-prio_min.patch \
"

SRC_URI:append:imx6ull14x14evk = "\
    file://pip_stress-De-constify-prio_min.patch \
"

SRC_URI:append:imx8mm-lpddr4-evk = "\
    file://pip_stress-De-constify-prio_min.patch \
"

SRC_URI:append:imx8mp-lpddr4-evk = "\
    file://pip_stress-De-constify-prio_min.patch \
"


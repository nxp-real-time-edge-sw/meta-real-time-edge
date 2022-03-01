FILESEXTRAPATHS_prepend_qoriq := "${THISDIR}/${BPN}:"

SRC_URI_append_qoriq = "\
    file://pip_stress-De-constify-prio_min.patch \
"

SRC_URI_append_imx6ull14x14evk = "\
    file://pip_stress-De-constify-prio_min.patch \
"

SRC_URI_append_imx8mm-lpddr4-evk = "\
    file://pip_stress-De-constify-prio_min.patch \
"

SRC_URI_append_imx8mp-lpddr4-evk = "\
    file://pip_stress-De-constify-prio_min.patch \
"


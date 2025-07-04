FILESEXTRAPATHS:prepend := "${THISDIR}/files:"

SRC_URI:append = "\
	file://0012-chromium-met-EGL-API-GetProcAddress-failures.patch \
    file://0013-Only-DRI-for-x11.patch \
    file://0014-disbale-dri.patch \
"

GN_ARGS_DISABLE_GBM     = ""
GN_ARGS_DISABLE_GBM:mx6-nxp-bsp = "use_system_minigbm=false use_wayland_gbm=false"
GN_ARGS_DISABLE_GBM:mx7-nxp-bsp = "${GN_ARGS_DISABLE_GBM_mx6}"
GN_ARGS += "${GN_ARGS_DISABLE_GBM}"

CHROMIUM_EXTRA_ARGS:append:ls1028ardb = " --disable-features=VizDisplayCompositor"


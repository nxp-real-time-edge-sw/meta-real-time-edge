FILESEXTRAPATHS:prepend := "${THISDIR}/files:"

SRC_URI:append:ls1028ardb = "\
    file://0001-Fixed-chromium-flicker-with-g2d-renderer.patch \
    file://0002-chromium-met-EGL-API-GetProcAddress-failures.patch \
    file://0013-Only-DRI-for-x11.patch \
    file://0014-grit-util-py-remove-deprecated-mode-for-open.patch \
"

GN_ARGS_DISABLE_GBM     = ""
GN_ARGS_DISABLE_GBM:mx6-nxp-bsp = "use_system_minigbm=false use_wayland_gbm=false"
GN_ARGS_DISABLE_GBM:mx7-nxp-bsp = "${GN_ARGS_DISABLE_GBM_mx6}"
GN_ARGS += "${GN_ARGS_DISABLE_GBM}"

CHROMIUM_EXTRA_ARGS:append:ls1028ardb = " --disable-features=VizDisplayCompositor"

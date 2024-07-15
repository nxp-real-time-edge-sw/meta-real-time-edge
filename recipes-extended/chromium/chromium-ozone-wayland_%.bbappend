FILESEXTRAPATHS:prepend := "${THISDIR}/files:"

SRC_URI:remove:imx-nxp-bsp = " \
    file://0001-Fixed-chromium-flicker-with-g2d-renderer.patch \
    file://0002-chromium-met-EGL-API-GetProcAddress-failures.patch \
    file://0003-Disable-dri-for-imx-gpu.patch \
    file://0004-Fix-chromium-build-failure.patch \
    file://0005-Revert-ui-gbm_wrapper-Ensure-to-create-BOs-with-impo.patch \
    file://0006-Fixed-chromium-crash-after-upgrading.patch \
"

SRC_URI:append = "\
	file://0011-Fixed-chromium-flicker-with-g2d-renderer.patch \
	file://0012-chromium-met-EGL-API-GetProcAddress-failures.patch \
	file://0014-Fix-chromium-build-failure.patch \
    file://0015-Revert-ui-gbm_wrapper-Ensure-to-create-BOs-with-impo.patch \
    file://0016-Fixed-chromium-crash-after-upgrading.patch \
    file://0013-Only-DRI-for-x11.patch \
    file://0014-disbale-dri.patch \
"

GN_ARGS_DISABLE_GBM     = ""
GN_ARGS_DISABLE_GBM:mx6-nxp-bsp = "use_system_minigbm=false use_wayland_gbm=false"
GN_ARGS_DISABLE_GBM:mx7-nxp-bsp = "${GN_ARGS_DISABLE_GBM_mx6}"
GN_ARGS += "${GN_ARGS_DISABLE_GBM}"

CHROMIUM_EXTRA_ARGS:append:ls1028ardb = " --disable-features=VizDisplayCompositor"


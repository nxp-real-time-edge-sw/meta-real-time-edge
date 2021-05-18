# RT Edge base conf

DISTRO = "nxp-rt-edge"
DISTROOVERRIDES = "rt-edge"
DISTRO_NAME = "NXP RT Edge Distro"
DISTRO_VERSION = "1.12"

DISTRO_FEATURES_remove_mx8 = "jailhouse "
DISTRO_FEATURES_append_ls1043ardb = " jailhouse-rt-edge"
DISTRO_FEATURES_append_ls1046ardb = " jailhouse-rt-edge"
DISTRO_FEATURES_append_ls1028ardb = " jailhouse-industrial libbee libblep libnfc-nci \
				      wayland-protocols weston imx-gpu-viv libdrm kmscube"
DISTRO_FEATURES_append_mx8mp = " jailhouse-rt-edge"
DISTRO_FEATURES_append_mx8m = " wayland-protocols weston imx-gpu-viv libdrm kmscube"

DISTRO_FEATURES_remove = " x11"
PREFERRED_VERSION_weston = "9.0.0.imx"
PREFERRED_VERSION_wayland-protocols = "1.20.imx"
PREFERRED_VERSION_libdrm = "2.4.102.imx"
PREFERRED_PROVIDER_virtual/libgl  = "imx-gpu-viv"
PREFERRED_PROVIDER_virtual/libgles1  = "imx-gpu-viv"
PREFERRED_PROVIDER_virtual/libgles2  = "imx-gpu-viv"
PREFERRED_PROVIDER_virtual/egl  = "imx-gpu-viv"
LICENSE_FLAGS_WHITELIST_append = " commercial"

DELTA_KERNEL_DEFCONFIG_append = " \
    linux-rt.config \
"
DELTA_KERNEL_DEFCONFIG_append_mx8mm = " linux-fec.config"
DELTA_KERNEL_DEFCONFIG_append_mx8m = " linux-imx8m.config"
# RT Edge base conf

DISTRO = "nxp-rt-edge"
DISTROOVERRIDES = "rt-edge"
DISTRO_NAME = "NXP RT Edge Distro"
DISTRO_VERSION = "1.12"

DISTRO_FEATURES_remove_mx8 = "jailhouse "
DISTRO_FEATURES_append_ls1043ardb = " jailhouse-rt-edge"
DISTRO_FEATURES_append_ls1046ardb = " jailhouse-rt-edge"
DISTRO_FEATURES_append_ls1028ardb = " jailhouse-rt-edge libbee libblep libnfc-nci"
DISTRO_FEATURES_append_mx8mp = " jailhouse-rt-edge"
DELTA_KERNEL_DEFCONFIG_append_mx8mm = " linux-fec.config"

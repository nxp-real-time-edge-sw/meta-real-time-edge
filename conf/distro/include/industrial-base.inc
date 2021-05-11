# Industrial base conf

DISTRO = "nxp-industrial"
DISTROOVERRIDES = "industrial"
DISTRO_NAME = "NXP Industrial Distro"
DISTRO_VERSION = "1.12"

DISTRO_FEATURES_remove_mx8 = "jailhouse "
DISTRO_FEATURES_append_ls1043ardb = " jailhouse-industrial"
DISTRO_FEATURES_append_ls1046ardb = " jailhouse-industrial"
DISTRO_FEATURES_append_ls1028ardb = " jailhouse-industrial libbee libblep libnfc-nci"
DISTRO_FEATURES_append_mx8mp = " jailhouse-industrial"

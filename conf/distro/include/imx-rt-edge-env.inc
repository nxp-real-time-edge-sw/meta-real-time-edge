include conf/distro/include/fsl-imx-base.inc
include conf/distro/include/fsl-imx-preferred-env.inc
include conf/distro/include/rt-edge-base.inc
include conf/distro/include/igh-ethercat.inc
include conf/distro/include/libopen62541.inc

# Remove conflicting backends
DISTRO_FEATURES_remove = "directfb x11 "
DISTRO_FEATURES_append = " wayland pam systemd"
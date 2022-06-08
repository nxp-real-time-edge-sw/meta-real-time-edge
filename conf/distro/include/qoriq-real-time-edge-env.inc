require conf/distro/poky.conf
include conf/distro/include/real-time-edge-base.inc
include conf/machine/include/qoriq-wks-base.inc
include conf/distro/include/libopen62541.inc
include conf/distro/include/igh-ethercat.inc

TARGET_VENDOR = "-fsl"

SDK_VENDOR = "-fslsdk"
SDK_NAME_PREFIX = "${DISTRO}"

POKY_DEFAULT_DISTRO_FEATURES:remove = " ptest"
DISTRO_FEATURES:append = " opengl pam virtualization polkit ipv6"

LICENSE_FLAGS_WHITELIST = "non-commercial_netperf"

MKUBIFS_ARGS = "-m 512 -e 15360 -c 13000"
UBINIZE_ARGS = "-m 512 -p 16KiB -s 512"

PREFERRED_PROVIDER_qemu = "qemu-qoriq"

PREFERRED_VERSION_virtual/nativesdk-libiconv = "2.20"
PREFERRED_VERSION_tcpreplay = "3.4.4"
PREFERRED_PROVIDER_virtual/kernel = "linux-qoriq"
LTTNGUST:qoriq = ""
LTTNGTOOLS:qoriq = ""
LTTNGMODULES:qoriq = ""
MACHINE_HAS_VIVANTE_KERNEL_DRIVER_SUPPORT = "1"

RCWSD:ls1028ardb = "${@bb.utils.contains('DISTRO_FEATURES', 'sai', \
                    'R_SQPP_0x85bb/rcw_1500_gpu600_sai', \
                    'R_SQPP_0x85bb/rcw_1500_gpu600', d)}"

SOC_DEFAULT_IMAGE_FSTYPES:append = " wic.bmap wic.bz2 tar.bz2"
SOC_DEFAULT_IMAGE_FSTYPES:remove = " tar.gz ext2.gz.u-boot ext2.gz"
SOC_DEFAULT_IMAGE_FSTYPES:append:ls1012a = " cpio.gz cpio.gz.u-boot"
SOC_DEFAULT_IMAGE_FSTYPES:remove:ls1012a = "wic.bmap wic.bz2 tar.bz2"

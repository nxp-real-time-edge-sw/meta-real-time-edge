require conf/distro/fsl-qoriq.conf
require conf/distro/include/real-time-edge-base.inc

include conf/machine/include/qoriq-wks-base.inc
include conf/distro/include/libopen62541.inc
include conf/distro/include/igh-ethercat.inc

RCWSD:ls1028ardb = "${@bb.utils.contains('DISTRO_FEATURES', 'sai', \
                    'R_SQPP_0x85bb/rcw_1500_gpu600_sai', \
                    'R_SQPP_0x85bb/rcw_1500_gpu600', d)}"
RCWEMMC:ls1046ardb = "RR_FFSSPPPH_1133_5559/rcw_1800_emmcboot"
RCWQSPI:ls1046ardb = "RR_FFSSPPPH_1133_5559/rcw_1600_qspiboot_emmc"

SOC_DEFAULT_IMAGE_FSTYPES:append = " wic.bmap wic.zst tar.zst"
SOC_DEFAULT_IMAGE_FSTYPES:remove = " tar.gz ext2.gz.u-boot ext2.gz wic.bz2"
SOC_DEFAULT_IMAGE_FSTYPES:append:ls1012a = " cpio.gz cpio.gz.u-boot"
SOC_DEFAULT_IMAGE_FSTYPES:remove:ls1012a = " wic.bmap wic.zst tar.zst"

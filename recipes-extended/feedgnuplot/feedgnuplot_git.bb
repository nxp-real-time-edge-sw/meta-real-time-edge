DESCRIPTION = "General purpose pipe-oriented plotting tool."

LICENSE = "Artistic-1.0 | GPL-1.0-or-later"

LIC_FILES_CHKSUM = "file://LICENSE;md5=b7700a0c2f940e07a9a77c579e2f9841"

DEPENDS = "gnuplot list-moreutils-perl exporter-tiny-perl"

SRC_URI = "git://github.com/dkogan/feedgnuplot.git;protocol=https"

SRCREV = "09802f4248f9b39eba259b688d7e215bb407e4cb"
PV = "v1.58"

S = "${WORKDIR}/git"

inherit cpan

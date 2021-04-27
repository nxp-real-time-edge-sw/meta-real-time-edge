DESCRIPTION = "General purpose pipe-oriented plotting tool."

LICENSE = "Artisticv1 | GPLv1+"

LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/Artistic-1.0;md5=cda03bbdc3c1951996392b872397b798 \
file://${COMMON_LICENSE_DIR}/GPL-1.0;md5=e9e36a9de734199567a4d769498f743d"

DEPENDS = "gnuplot list-moreutils-perl exporter-tiny-perl"

SRC_URI = "git://github.com/dkogan/feedgnuplot.git;protocol=https"

SRCREV = "09802f4248f9b39eba259b688d7e215bb407e4cb"
PV = "v1.58"

S = "${WORKDIR}/git"

inherit cpan

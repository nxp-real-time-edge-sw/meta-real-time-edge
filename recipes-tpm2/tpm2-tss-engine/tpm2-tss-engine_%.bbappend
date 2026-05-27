# Disable man page generation - pandoc is not available in Yocto
do_configure:prepend() {
    # Remove pre-existing man directories so configure's HAVE_MAN_PAGES
    # condition (test -d "${srcdir}/man/man1") evaluates to false
    rm -rf ${S}/man/man1 ${S}/man/man3
}

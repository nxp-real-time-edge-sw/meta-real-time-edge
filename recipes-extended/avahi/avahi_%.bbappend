do_install:append() {
    mv ${D}${docdir}/avahi/ssh.service ${D}${sysconfdir}/avahi/services/
}

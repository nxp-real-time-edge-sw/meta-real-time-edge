do_install_append() {
    mv ${D}${docdir}/avahi/ssh.service ${D}${sysconfdir}/avahi/services/
}

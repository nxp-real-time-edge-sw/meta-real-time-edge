PACKAGECONFIG = "readline gdbm ${@bb.utils.filter('DISTRO_FEATURES', 'lto', d)}"

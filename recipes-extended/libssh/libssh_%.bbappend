PACKAGECONFIG = "${@bb.utils.contains('PTEST_ENABLED', '1', 'tests', '', d)}"


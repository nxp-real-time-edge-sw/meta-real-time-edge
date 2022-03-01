IGH_ETHERCAT_imx8mm-lpddr4-evk = " fec "
IGH_ETHERCAT_ls1028ardb = " enetc "
IGH_ETHERCAT_ls1046ardb = " dpaa1 backup"
IGH_ETHERCAT_ls1043ardb = " dpaa1 backup"
IGH_ETHERCAT ??= " "
PACKAGECONFIG_append_pn-igh-ethercat = " ${IGH_ETHERCAT}"


#!/bin/bash
#
# FSL Build Enviroment Setup Script
#
# Copyright (C) 2011-2015 Freescale Semiconductor
# Copyright 2017 NXP
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

. sources/meta-imx/tools/setup-utils.sh

CWD=`pwd`
PROGNAME="setup-environment"

exit_message ()
{
   echo "To return to this build environment later please run:"
   echo "    source setup-environment <build_dir>"

}

usage()
{
    echo -e "\nUsage: source imx-snapshot-yocto-setup.sh
    Optional parameters: [-b build-dir] [-h]"
echo "
    * [-b build-dir]: Build directory, if unspecified script uses 'build' as output directory
    * [-h]: help
"
}


clean_up()
{

    unset BUILD_DIR BACKEND IMXDISTRO WORKSPACE
    unset fsl_setup_help fsl_setup_error fsl_setup_flag
    unset usage clean_up
    unset ARM_DIR META_FSL_BSP_RELEASE
    exit_message clean_up
}

. sources/meta-imx/tools/setup-utils.sh

WORKSPACE=$PWD
PROGNAME="setup-environment"

# get command line options
OLD_OPTIND=$OPTIND
unset IMXDISTRO

while getopts "b:h" fsl_setup_flag
do
    case $fsl_setup_flag in
        b) BUILD_DIR="$OPTARG";
           echo -e "\n Build directory is " $BUILD_DIR
           ;;
        h) fsl_setup_help='true';
           ;;
        \?) fsl_setup_error='true';
           ;;
    esac
done
shift $((OPTIND-1))
if [ $# -ne 0 ]; then
    fsl_setup_error=true
    echo -e "Invalid command line ending: '$@'"
fi
OPTIND=$OLD_OPTIND
if test $fsl_setup_help; then
    usage && clean_up && return 1
elif test $fsl_setup_error; then
    clean_up && return 1
fi

if [ -z "$DISTRO" ]; then
    if [ -z "$IMXDISTRO" ]; then
        IMXDISTRO='nxp-real-time-edge'
    fi
else
    IMXDISTRO="$DISTRO"
fi

if [ -z "$BUILD_DIR" ]; then
    BUILD_DIR='build'
fi

if [ -z "$MACHINE" ]; then
    MACHINE='imx943-19x19-lpddr5-evk'
    echo setting to default machine to $MACHINE
fi

# Set up the basic yocto environment
MACHINE=$MACHINE DISTRO=$IMXDISTRO source $PROGNAME $BUILD_DIR

# On the first script run, backup the local.conf file
# Consecutive runs, it restores the backup and changes are appended on this one.
if [ ! -e conf/local.conf.org ]; then
    cp conf/local.conf conf/local.conf.org
else
    cp conf/local.conf.org conf/local.conf
fi

if [ ! -e conf/bblayers.conf.org ]; then
    cp conf/bblayers.conf conf/bblayers.conf.org
else
    cp conf/bblayers.conf.org conf/bblayers.conf
fi

echo "##i.MX Yocto Project Release layer" >> conf/bblayers.conf
hook_in_layer meta-imx/meta-imx-bsp
hook_in_layer meta-imx/meta-imx-sdk
hook_in_layer meta-imx-snapshot

#echo "IMXBOOT_VARIANT = \"netc\"" >> conf/local.conf
#echo "MACHINE_FEATURES:remove = \"nxpwifi-all-pcie nxpwifi-all-sdio jailhouse dpdk xen xen-tools \"" >> conf/local.conf

echo "BBLAYERS += \"\${BSPDIR}/sources/meta-arm/meta-arm\"" >> conf/bblayers.conf
echo "BBLAYERS += \"\${BSPDIR}/sources/meta-arm/meta-arm-toolchain\"" >> conf/bblayers.conf
echo "BBLAYERS += \"\${BSPDIR}/sources/meta-clang\"" >> conf/bblayers.conf
echo "BBLAYERS += \"\${BSPDIR}/sources/meta-openembedded/meta-gnome\"" >> conf/bblayers.conf
echo "BBLAYERS += \"\${BSPDIR}/sources/meta-openembedded/meta-networking\"" >> conf/bblayers.conf
echo "BBLAYERS += \"\${BSPDIR}/sources/meta-openembedded/meta-filesystems\"" >> conf/bblayers.conf
echo "BBLAYERS += \"\${BSPDIR}/sources/meta-qt6\"" >> conf/bblayers.conf
echo "BBLAYERS += \"\${BSPDIR}/sources/meta-security/meta-parsec\"" >> conf/bblayers.conf
echo "BBLAYERS += \"\${BSPDIR}/sources/meta-security/meta-tpm\"" >> conf/bblayers.conf
echo "BBLAYERS += \"\${BSPDIR}/sources/meta-virtualization\"" >> conf/bblayers.conf

echo "" >> conf/bblayers.conf
echo "# Real-time Edge Yocto Project Release layers" >> conf/bblayers.conf
echo "BBLAYERS += \"\${BSPDIR}/sources/meta-real-time-edge\"" >> conf/bblayers.conf

echo "" >> conf/bblayers.conf
echo "# RTOS layer" >> conf/bblayers.conf
echo "BBLAYERS += \"\${BSPDIR}/sources/meta-rtos-industrial\"" >> conf/bblayers.conf

echo BUILD_DIR=$BUILD_DIR
echo PWD is $PWD

cd $WORKSPACE/$BUILD_DIR

clean_up

#!/bin/sh
#
# Real-time Edge Yocto Project Build Environment Setup Script
#
# Copyright 2021-2023, 2025 NXP
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


ROOTDIR=`pwd`
PROGNAME="setup-environment"
PLATFORM="imx"

exit_message ()
{
   echo "To return to this build environment later please run:"
   echo "    source setup-environment <build_dir>"
}

usage()
{
    echo -e "\nUsage: source real-time-edge-setup-env.sh
    Optional parameters: [-b build-dir] [-h]"
echo "
    * [-b build-dir]: Build directory, if unspecified script uses 'build' as output directory
    * [-h]: help
    "
}

clean_up()
{
    unset CWD BUILD_DIR FSLDISTRO
    unset fsl_setup_help fsl_setup_error fsl_setup_flag
    unset usage clean_up
    unset ARM_DIR META_FSL_BSP_RELEASE
    exit_message clean_up
}

change_conf()
{
    # On the first script run, backup the local.conf file
    # Consecutive runs, it restores the backup and changes are appended on this one.
    if [ ! -e $BUILD_DIR/conf/local.conf.org ]; then
        cp $BUILD_DIR/conf/local.conf $BUILD_DIR/conf/local.conf.org
    else
        cp $BUILD_DIR/conf/local.conf.org $BUILD_DIR/conf/local.conf
    fi

    echo "" >> $BUILD_DIR/conf/local.conf
    echo "# Share cache" >> $BUILD_DIR/conf/local.conf
    echo "SSTATE_DIR ?= \"\${BSPDIR}/sstate-cache\"" >> $BUILD_DIR/conf/local.conf

    echo >> $BUILD_DIR/conf/local.conf
    echo "# Switch to Debian packaging and include package-management in the image" >> $BUILD_DIR/conf/local.conf
    echo "PACKAGE_CLASSES = \"package_deb\"" >> $BUILD_DIR/conf/local.conf
    echo "EXTRA_IMAGE_FEATURES += \"package-management\"" >> $BUILD_DIR/conf/local.conf
}

imx_add_layers()
{
    . ${ROOTDIR}/sources/meta-imx/tools/setup-utils.sh

    META_FSL_BSP_RELEASE="${ROOTDIR}/sources/meta-imx/meta-imx-bsp"

    echo "# i.MX Yocto Project Release layers" >> $BUILD_DIR/conf/bblayers.conf
    hook_in_layer meta-imx/meta-imx-bsp
    hook_in_layer meta-imx/meta-imx-sdk
    hook_in_layer meta-imx/meta-imx-ml
    hook_in_layer meta-imx/meta-imx-v2x
    hook_in_layer meta-nxp-demo-experience
    hook_in_layer meta-nxp-connectivity/meta-nxp-matter-baseline
    hook_in_layer meta-nxp-connectivity/meta-nxp-openthread

    echo "" >> $BUILD_DIR/conf/bblayers.conf
    echo "BBLAYERS += \"\${BSPDIR}/sources/meta-arm/meta-arm\"" >> $BUILD_DIR/conf/bblayers.conf
    echo "BBLAYERS += \"\${BSPDIR}/sources/meta-arm/meta-arm-toolchain\"" >> $BUILD_DIR/conf/bblayers.conf
    echo "BBLAYERS += \"\${BSPDIR}/sources/meta-browser/meta-chromium\"" >> $BUILD_DIR/conf/bblayers.conf
    echo "BBLAYERS += \"\${BSPDIR}/sources/meta-clang\"" >> $BUILD_DIR/conf/bblayers.conf
    echo "BBLAYERS += \"\${BSPDIR}/sources/meta-openembedded/meta-gnome\"" >> $BUILD_DIR/conf/bblayers.conf
    echo "BBLAYERS += \"\${BSPDIR}/sources/meta-openembedded/meta-networking\"" >> $BUILD_DIR/conf/bblayers.conf
    echo "BBLAYERS += \"\${BSPDIR}/sources/meta-openembedded/meta-filesystems\"" >> $BUILD_DIR/conf/bblayers.conf
    echo "BBLAYERS += \"\${BSPDIR}/sources/meta-qt6\"" >> $BUILD_DIR/conf/bblayers.conf
    echo "BBLAYERS += \"\${BSPDIR}/sources/meta-security/meta-parsec\"" >> $BUILD_DIR/conf/bblayers.conf
    echo "BBLAYERS += \"\${BSPDIR}/sources/meta-security/meta-tpm\"" >> $BUILD_DIR/conf/bblayers.conf
    echo "BBLAYERS += \"\${BSPDIR}/sources/meta-virtualization\"" >> $BUILD_DIR/conf/bblayers.conf

    # echo "BBLAYERS += \"\${BSPDIR}/sources/meta-cpan\"" >> $BUILD_DIR/conf/bblayers.conf

    echo "" >> $BUILD_DIR/conf/bblayers.conf
    echo "# RTOS layer" >> $BUILD_DIR/conf/bblayers.conf
    echo "BBLAYERS += \"\${BSPDIR}/sources/meta-rtos-industrial\"" >> $BUILD_DIR/conf/bblayers.conf

    echo "" >> $BUILD_DIR/conf/bblayers.conf
    echo "# Harpoon layer" >> $BUILD_DIR/conf/bblayers.conf
    echo "BBLAYERS += \"\${BSPDIR}/sources/meta-nxp-harpoon\"" >> $BUILD_DIR/conf/bblayers.conf

    echo "" >> $BUILD_DIR/conf/bblayers.conf
    echo "# AVB layer" >> $BUILD_DIR/conf/bblayers.conf
    echo "BBLAYERS += \"\${BSPDIR}/sources/meta-nxp-avb\"" >> $BUILD_DIR/conf/bblayers.conf
}

qoriq_add_layers()
{
    echo "BBLAYERS += \"\${BSPDIR}/sources/meta-browser/meta-chromium\"" >> $BUILD_DIR/conf/bblayers.conf
    echo "BBLAYERS += \"\${BSPDIR}/sources/meta-clang\"" >> $BUILD_DIR/conf/bblayers.conf

    echo "BBLAYERS += \"\${BSPDIR}/sources/meta-openembedded/meta-networking\"" >> $BUILD_DIR/conf/bblayers.conf
    echo "BBLAYERS += \"\${BSPDIR}/sources/meta-openembedded/meta-gnome\"" >> $BUILD_DIR/conf/bblayers.conf
    echo "BBLAYERS += \"\${BSPDIR}/sources/meta-openembedded/meta-filesystems\"" >> $BUILD_DIR/conf/bblayers.conf
    echo "BBLAYERS += \"\${BSPDIR}/sources/meta-openembedded/meta-webserver\"" >> $BUILD_DIR/conf/bblayers.conf
    echo "BBLAYERS += \"\${BSPDIR}/sources/meta-openembedded/meta-perl\"" >> $BUILD_DIR/conf/bblayers.conf

    echo "BBLAYERS += \"\${BSPDIR}/sources/meta-arm/meta-arm\"" >> $BUILD_DIR/conf/bblayers.conf
echo "BBLAYERS += \"\${BSPDIR}/sources/meta-arm/meta-arm-toolchain\"" >> $BUILD_DIR/conf/bblayers.conf

    echo "BBLAYERS += \"\${BSPDIR}/sources/meta-virtualization\"" >> $BUILD_DIR/conf/bblayers.conf
    echo "BBLAYERS += \"\${BSPDIR}/sources/meta-cloud-services\"" >> $BUILD_DIR/conf/bblayers.conf
    echo "BBLAYERS += \"\${BSPDIR}/sources/meta-security\"" >> $BUILD_DIR/conf/bblayers.conf

    echo "BBLAYERS += \"\${BSPDIR}/sources/meta-qoriq/meta-qoriq-bsp\"" >> $BUILD_DIR/conf/bblayers.conf
    echo "BBLAYERS += \"\${BSPDIR}/sources/meta-qoriq/meta-qoriq-sdk\"" >> $BUILD_DIR/conf/bblayers.conf

    # echo "BBLAYERS += \"\${BSPDIR}/sources/meta-cpan\"" >> $BUILD_DIR/conf/bblayers.conf
}

add_layers()
{
    if [ ! -e $BUILD_DIR/conf/bblayers.conf.org ]; then
        cp $BUILD_DIR/conf/bblayers.conf $BUILD_DIR/conf/bblayers.conf.org
    else
        cp $BUILD_DIR/conf/bblayers.conf.org $BUILD_DIR/conf/bblayers.conf
    fi

    echo "" >> $BUILD_DIR/conf/bblayers.conf

    if [ "${PLATFORM}" = "imx" ]
    then
        imx_add_layers
    else
        qoriq_add_layers
    fi

    echo "" >> $BUILD_DIR/conf/bblayers.conf
    echo "# Real-time Edge Yocto Project Release layers" >> $BUILD_DIR/conf/bblayers.conf
    echo "BBLAYERS += \"\${BSPDIR}/sources/meta-real-time-edge\"" >> $BUILD_DIR/conf/bblayers.conf
}

# get command line options
OLD_OPTIND=$OPTIND
unset FSLDISTRO
unset BUILD_DIR

while getopts "k:r:t:b:e:gh" fsl_setup_flag
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
    if [ -z "$FSLDISTRO" ]; then
        FSLDISTRO='nxp-real-time-edge'
        fi
else
    FSLDISTRO="$DISTRO"
fi

if [ -z "$BUILD_DIR" ]; then
    BUILD_DIR='build'
fi

if [ -z "$MACHINE" ]; then
    echo setting to default machine
    MACHINE='imx943evk'
fi

case $MACHINE in
imx*)
    PLATFORM="imx"
;;
*)
    PLATFORM="qoriq"
;;
esac

FSL_EULA_FILE=$ROOTDIR/sources/meta-real-time-edge/LICENSE.txt

# Set up the basic yocto environment
DISTRO=$FSLDISTRO MACHINE=$MACHINE . ./$PROGNAME $BUILD_DIR

# Point to the current directory since the last command changed the directory to $BUILD_DIR
BUILD_DIR=.

if [ ! -e $BUILD_DIR/conf/local.conf ]; then
    echo -e "\n ERROR - No build directory is set yet. Run the 'setup-environment' script before running this script to create " $BUILD_DIR
    echo -e "\n"
    return 1
fi

change_conf
add_layers

cd  $BUILD_DIR
clean_up
unset FSLDISTRO

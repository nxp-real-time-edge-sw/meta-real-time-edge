#!/bin/sh
#
# Real-time Edge Yocto Project Build Environment Setup Script
#
# Copyright 2021-2024 NXP
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
PLATFORM="qoriq"
RFNM_CLI_ENABLE="disable"

exit_message ()
{
   echo "To return to this build environment later please run:"
   echo "    source setup-environment <build_dir>"
}

usage()
{
    echo -e "\nUsage: source real-time-edge-setup-env.sh
    Optional parameters: [-b build-dir] [-i] [-h]"
echo "
    * [-b build-dir]: Build directory, if unspecified script uses 'build' as output directory
    * [-i internal]: Internal build using bitbucket repos instead of github for selected components.
    * [-c cli]: cli enable/disable.

    * [-h]: help
	"
}

clean_up()
{
    unset ROOTDIR BUILD_DIR FSLDISTRO FLATFORM
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

	echo >> $BUILD_DIR/conf/local.conf
	echo "# Switch to Debian packaging and include package-management in the image" >> $BUILD_DIR/conf/local.conf
	echo "PACKAGE_CLASSES = \"package_deb\"" >> $BUILD_DIR/conf/local.conf
	echo "EXTRA_IMAGE_FEATURES += \" package-management \"" >> $BUILD_DIR/conf/local.conf
	echo "EXTRA_IMAGE_FEATURES += \" debug-tweaks tools-debug eclipse-debug ssh-server-openssh \"" >> $BUILD_DIR/conf/local.conf
	echo "CORE_IMAGE_EXTRA_INSTALL += \" openssh-sftp openssh-sftp-server \"" >> $BUILD_DIR/conf/local.conf
	echo "IMAGE_INSTALL:append = \" kernel-module-la9310 userapp-la9310 freertos-la9310 arm-ral dpdk kernel-module-kpage-ncache \"" >> $BUILD_DIR/conf/local.conf
	echo "IMAGE_INSTALL:append = \" imx-test dtc python3-pip \"" >> $BUILD_DIR/conf/local.conf

	if [ "${RFNM_CLI_ENABLE}" = "enable" ]
	then
		echo "IMAGE_INSTALL:append = \" rfnm-cli-la9310 \"" >> $BUILD_DIR/conf/local.conf
	        #echo "IMAGE_INSTALL:append = \" rf-util-la9310 \"" >> $BUILD_DIR/conf/local.conf
	fi
	echo "IMAGE_INSTALL:remove = \" docker \"" >> $BUILD_DIR/conf/local.conf
	echo "MACHINE_FEATURES:append:imx8mp-rfnm = \" dpdk\"" >> $BUILD_DIR/conf/local.conf
	if [ "${MACHINE}" = "imx8mp-rfnm" ]
	then
		cd ${ROOTDIR}/sources/meta-imx/
		git am ${ROOTDIR}/sources/meta-real-time-edge/patches/0001-rfnm-Add-support-for-rfnm-dtb-support.patch
		git am ${ROOTDIR}/sources/meta-real-time-edge/patches/0002-rfnm-Temporarily-add-atf-binary-image-support.patch
		cd -
	fi

	echo "SKIP_RECIPE[imx-gpu-sdk] = \"skip\"" >> $BUILD_DIR/conf/local.conf
	echo "SKIP_RECIPE[gstreamer] = \"skip\"" >> $BUILD_DIR/conf/local.conf
	echo "SKIP_RECIPE[gnome-desktop] = \"skip\"" >> $BUILD_DIR/conf/local.conf
	echo "SKIP_RECIPE[kernel-module-nxp89xx] = \"skip\"" >> $BUILD_DIR/conf/local.conf
}

imx_add_layers()
{
	. ${ROOTDIR}/sources/meta-imx/tools/setup-utils.sh

	META_FSL_BSP_RELEASE="${ROOTDIR}/sources/meta-imx/meta-bsp"

	hook_in_layer meta-imx/meta-bsp
	hook_in_layer meta-imx/meta-sdk

	echo "" >> $BUILD_DIR/conf/bblayers.conf
	echo "BBLAYERS += \"\${BSPDIR}/sources/meta-clang\"" >> $BUILD_DIR/conf/bblayers.conf
	echo "BBLAYERS += \"\${BSPDIR}/sources/meta-openembedded/meta-gnome\"" >> $BUILD_DIR/conf/bblayers.conf
	echo "BBLAYERS += \"\${BSPDIR}/sources/meta-openembedded/meta-networking\"" >> $BUILD_DIR/conf/bblayers.conf
	echo "BBLAYERS += \"\${BSPDIR}/sources/meta-openembedded/meta-filesystems\"" >> $BUILD_DIR/conf/bblayers.conf

	echo "BBLAYERS += \"\${BSPDIR}/sources/meta-cpan\"" >> $BUILD_DIR/conf/bblayers.conf

	# Support integrating community meta-freescale instead of meta-fsl-arm
	if [ -d $ROOTDIR/sources/meta-freescale ]; then
	    echo meta-freescale directory found
	    # Change settings according to environment
	    sed -e "s,meta-fsl-arm\s,meta-freescale ,g" -i $BUILD_DIR/conf/bblayers.conf
	    sed -e "s,\$.BSPDIR./sources/meta-fsl-arm-extra\s,,g" -i $BUILD_DIR/conf/bblayers.conf
	fi

	echo "" >> $BUILD_DIR/conf/bblayers.conf

}

qoriq_add_layers()
{
	echo "BBLAYERS += \"\${BSPDIR}/sources/meta-clang\"" >> $BUILD_DIR/conf/bblayers.conf
	echo "BBLAYERS += \"\${BSPDIR}/sources/meta-openembedded/meta-networking\"" >> $BUILD_DIR/conf/bblayers.conf
	echo "BBLAYERS += \"\${BSPDIR}/sources/meta-openembedded/meta-gnome\"" >> $BUILD_DIR/conf/bblayers.conf
	echo "BBLAYERS += \"\${BSPDIR}/sources/meta-openembedded/meta-filesystems\"" >> $BUILD_DIR/conf/bblayers.conf
	echo "BBLAYERS += \"\${BSPDIR}/sources/meta-openembedded/meta-webserver\"" >> $BUILD_DIR/conf/bblayers.conf
	echo "BBLAYERS += \"\${BSPDIR}/sources/meta-openembedded/meta-perl\"" >> $BUILD_DIR/conf/bblayers.conf

	echo "BBLAYERS += \"\${BSPDIR}/sources/meta-security\"" >> $BUILD_DIR/conf/bblayers.conf

	echo "BBLAYERS += \"\${BSPDIR}/sources/meta-qoriq\"" >> $BUILD_DIR/conf/bblayers.conf
	echo "BBLAYERS += \"\${BSPDIR}/sources/meta-cpan\"" >> $BUILD_DIR/conf/bblayers.conf
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
	if [ "${RFNM_CLI_ENABLE}" = "enable" ]
	then
		echo "BBLAYERS += \"\${BSPDIR}/sources/meta-nxp-sdr\"" >> $BUILD_DIR/conf/bblayers.conf
	fi

}

# get command line options
OLD_OPTIND=$OPTIND
unset FSLDISTRO

while getopts "k:r:t:b:i:c:e:gh" fsl_setup_flag
do
	case $fsl_setup_flag in
	b) BUILD_DIR="$OPTARG";
		;;
	i) fsl_setup_internal='true';
		;;
	c) RFNM_CLI_ENABLE="enable"
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

case $MACHINE in
imx*)
	PLATFORM="imx"
;;
*)
	PLATFORM="qoriq"
;;
esac

if [ -z "$DISTRO" ]; then
	if [ -z "$FSLDISTRO" ]; then
		FSLDISTRO='nxp-real-time-edge'
		fi
else
	FSLDISTRO="$DISTRO"
fi

FSL_EULA_FILE=$ROOTDIR/sources/meta-real-time-edge/EULA.txt

if test $fsl_setup_help; then
	usage && clean_up && return 1
elif test $fsl_setup_error; then
	clean_up && return 1
fi

# Set up the basic yocto environment
if [ -z "$DISTRO" ]; then
   DISTRO=$FSLDISTRO MACHINE=$MACHINE . ./$PROGNAME $BUILD_DIR
else
   MACHINE=$MACHINE . ./$PROGNAME $BUILD_DIR
fi

BUILD_DIR=.

if [ ! -e $BUILD_DIR/conf/local.conf ]; then
    echo -e "\n ERROR - No build directory is set yet. Run the 'setup-environment' script before running this script to create " $BUILD_DIR
    echo -e "\n"
    return 1
fi

change_conf
add_layers

ln -sf ${ROOTDIR}/sources/meta-real-time-edge/recipes-kernel/include/la93xx-repo-ext.inc ${ROOTDIR}/sources/meta-real-time-edge/recipes-kernel/include/la93xx-repo.inc

#internal build 
if test $fsl_setup_internal; then
    	echo -e "\nSetting BUILD_TYPE as NXP bitbucket nxp-internal "
	echo "BUILD_TYPE = \"nxp-internal\"" >> $BUILD_DIR/conf/local.conf
	ln -sf ${ROOTDIR}/sources/meta-real-time-edge/recipes-kernel/include/la93xx-repo-int.inc ${ROOTDIR}/sources/meta-real-time-edge/recipes-kernel/include/la93xx-repo.inc
fi

cd  $BUILD_DIR
clean_up
unset FSLDISTRO

#!/bin/sh
###############################################################################
# Sysrepo Plugin Deployment Script
#
# Description:
#   This script installs sysrepo plugin shared libraries (.so files) from a
#   staging directory into the runtime plugins directory. The list of plugins
#   to install is provided in an external configuration file.
#
# How it works:
#   - Reads the plugin list from /etc/sysrepo-plugins-list.conf
#   - Removes existing plugin .so files in the runtime directory
#   - Copies plugins from the staging directory to the runtime directory
#   - Skips empty lines and commented lines beginning with '#'
#
# Paths:
#   STAGING: Directory where built plugins are initially placed
#   RUNTIME: Directory where sysrepo-plugind loads plugin .so files
#   PLUGINS_LIST: File listing plugins to install (one per line)
#
# Return codes:
#   0 - success or graceful exit
#
# Notes:
#   - Missing plugins are reported but do NOT cause the script to fail.
#   - The runtime directory is recreated fresh each time.
#
###############################################################################

STAGING="/usr/lib/sysrepo-plugind/staging"
RUNTIME="/usr/lib/sysrepo-plugind/plugins"
PLUGINS_LIST="/etc/sysrepo-plugins-list.conf"

if [ ! -f "$PLUGINS_LIST" ]; then
    echo "Plugin list file not found: $PLUGINS_LIST"
    exit 0
fi

if [ -d "$RUNTIME" ]; then
    rm -f -- "$RUNTIME"/*.so 2>/dev/null || true
fi

mkdir -p "$RUNTIME"

while IFS= read -r plugin; do
    # skip empty lines or comments
    [ -z "$plugin" ] && continue

    echo "$plugin" | grep -q "^#" && continue

    SRC="$STAGING/$plugin"
    DST="$RUNTIME/$(basename "$plugin")"
    if [ -f "$SRC" ]; then
        echo "Installing plugin: $plugin"
        ln -sfn -- "$SRC" "$DST"
    else
        echo "Missing plugin: $plugin"
    fi
done < "$PLUGINS_LIST"

exit 0

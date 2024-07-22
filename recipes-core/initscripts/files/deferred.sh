#!/bin/sh
#SPDX-License-Identifier: BSD-3-Clause
#Copyright 2024 NXP

echo "Running deferred probe Script"
VAR=$(lspci)
# Check if VAR contains "Inc Device 1c12"
if echo "$VAR" | grep -q "Inc Device 1c12"; then
  echo "device is up"
else
  # If the device is not found, read the deferred devices list into the variable DEFERRED_DEVICES
  DEFERRED_DEVICES=$(cat /sys/kernel/debug/devices_deferred)
  # Initialize flags for specific deferred devices
  HAS_0_0058=0
  HAS_PCIE=0
  # Check if the deferred devices list contains "0-0058" and set the flag if found
  if echo "$DEFERRED_DEVICES" | grep -q "0-0058"; then
      HAS_0_0058=1
  fi
  # Check if the deferred devices list contains "pcie" and set the flag if found
  if echo "$DEFERRED_DEVICES" | grep -q "pcie"; then
      HAS_PCIE=1
  fi
  # If both flags are set, trigger the deferred probe for the device
  if [ $HAS_0_0058 -eq 1 ] && [ $HAS_PCIE -eq 1 ]; then
      echo 1 > /sys/devices/platform/soc@0/30800000.bus/30a20000.i2c/i2c-0/0-0058/deferred_probe_trigger
  else
      echo "Both 0-0058 and pcie entries not found in deferred devices"
  fi
fi

# SPDX-License-Identifier: GPL-2.0
#
# Makefile for the DSA XDP examples (Yocto build)
#
# Two kinds of artefacts are produced:
#   - BPF object files (*.o)        : compiled by clang with -target bpf
#   - AF_XDP userspace applications : compiled by the (cross) C compiler
#
# The recipe is expected to pass in:
#   CC      cross C compiler (for the userspace apps)
#   CLANG   native clang     (for the BPF objects)
#   CFLAGS / LDFLAGS as provided by the Yocto environment
#
# libbpf and libxdp headers/libraries come from the target sysroot.

CC      ?= gcc
CLANG   ?= clang

CFLAGS  ?= -Wall -Wextra -O2 -g
LDLIBS  := -lxdp -lbpf -lelf -lz

# BPF compilation flags. The target architecture define helps the bpf
# target pick up the right asm/bitfield layout from the kernel headers.
BPF_CFLAGS := -O2 -g -target bpf

# Directory where the applications and BPF object files (*.o) are installed
# on the target. It is baked into the userspace applications via
# -DXDP_DSA_OBJDIR so that they load the .o files from this fixed location
# regardless of the current working directory. The recipe overrides this with
# the real install path; it can also be overridden at run time with the
# XDP_DSA_OBJDIR environment variable.
XDP_DSA_OBJDIR ?= /examples/xdp-dsa-examples
APP_CFLAGS := $(CFLAGS) -DXDP_DSA_OBJDIR=\"$(XDP_DSA_OBJDIR)\"

APPS     := xdp_dsa_rx_monitor xdp_dsa_port_rx
BPF_OBJS := xdp_dsa_redirect.o xdp_dsa_meta.o


.PHONY: all clean

all: $(BPF_OBJS) $(APPS)

# BPF objects (clang, bpf target). Built natively, not cross-compiled.
xdp_dsa_redirect.o: xdp_dsa_redirect.c xdp_dsa_meta.h
	$(CLANG) $(BPF_CFLAGS) -c $< -o $@


xdp_dsa_meta.o: xdp_dsa_meta.c xdp_dsa_meta.h
	$(CLANG) $(BPF_CFLAGS) -c $< -o $@

# Userspace applications (cross C compiler). APP_CFLAGS adds the
# -DXDP_DSA_OBJDIR define so the apps know where the .o files live.
xdp_dsa_rx_monitor: xdp_dsa_rx_monitor.c
	$(CC) $(APP_CFLAGS) $(LDFLAGS) -o $@ $< $(LDLIBS)

xdp_dsa_port_rx: xdp_dsa_port_rx.c xdp_dsa_meta.h
	$(CC) $(APP_CFLAGS) $(LDFLAGS) -o $@ $< $(LDLIBS)


clean:
	rm -f $(APPS) $(BPF_OBJS)

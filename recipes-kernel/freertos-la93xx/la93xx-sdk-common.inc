# Copyright 2023-2024 NXP

inherit cmake

python () {
    d.delVar('CFLAGS')
    d.delVar('CXXFLAGS')
    d.delVar('LDFLAGS')
}

# IMAGE_NO_MANIFEST = "1"

IMAGE_FSTYPES = "elf bin"

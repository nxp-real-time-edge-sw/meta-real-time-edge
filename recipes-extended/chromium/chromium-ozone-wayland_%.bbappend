# Compile using the built-in gn compiler in chromium-ozone-wayland
do_configure:prepend() {
    if [ -f ${S}/buildtools/linux64/gn ]; then
        export PATH="${S}/buildtools/linux64:$PATH"
        bbnote "Using GN from Chromium source: $(gn --version)"
    fi
}


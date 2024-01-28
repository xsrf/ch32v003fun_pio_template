Import("env")

# Custom LST from ELF
env.AddPostAction(
    "$BUILD_DIR/${PROGNAME}.elf",
    env.VerboseAction(" ".join([
        "riscv64-unknown-elf-objdump", "-S",
        "$BUILD_DIR/${PROGNAME}.elf", "> $BUILD_DIR/${PROGNAME}.lst"
    ]), "Building $BUILD_DIR/${PROGNAME}.lst")
)
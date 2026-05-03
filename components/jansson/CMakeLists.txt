idf_component_register(
    SRCS
        "dump.c"
        "error.c"
        "hashtable.c"
        "load.c"
        "memory.c"
        "pack_unpack.c"
        "strbuffer.c"
        "strconv.c"
        "value.c"
        "utf.c"
    INCLUDE_DIRS "includes"
)

target_compile_options(${COMPONENT_LIB} PRIVATE
    -Wno-error=format-truncation
)
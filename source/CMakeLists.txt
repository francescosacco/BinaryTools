add_library(hexlib
    hex.c
)

target_include_directories(hexlib PUBLIC
    ${PROJECT_SOURCE_DIR}/include
)

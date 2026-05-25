# Create the library and the public directory for the headers.

add_library(hexlib hex.c)
target_include_directories(hexlib PUBLIC ${PROJECT_SOURCE_DIR}/include)

add_library(utilslib utils.c)
target_include_directories(utilslib PUBLIC ${PROJECT_SOURCE_DIR}/include)

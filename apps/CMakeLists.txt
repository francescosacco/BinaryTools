add_executable(bin2hex
    bin2hex.c
)

target_link_libraries(bin2hex PRIVATE
    hexlib
)

add_executable(test_hex
    test_hex.c
)

target_link_libraries(test_hex PRIVATE
    hexlib
)

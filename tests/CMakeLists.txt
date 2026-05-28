# ---------------------------
# Unit Tests for hex module.
# ---------------------------
add_executable(test_hex test_hex.c)
target_link_libraries(test_hex PRIVATE hexlib)

# ---------------------------
# Unit Tests for utils module.
# ---------------------------
add_executable(test_utils test_utils.c)
target_link_libraries(test_utils PRIVATE utilslib)

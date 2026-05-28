# ---------------------------
# Application bin2hex
# ---------------------------
add_executable(bin2hex bin2hex.c)
target_link_libraries(bin2hex PRIVATE hexlib)

# ---------------------------
# Application hex2bin
# ---------------------------
add_executable(hex2bin hex2bin.c)
target_link_libraries(hex2bin PRIVATE hexlib)

# ---------------------------
# Application bin2intelhex
# ---------------------------
add_executable(bin2intelhex bin2intelhex.c)
target_link_libraries(bin2intelhex PRIVATE hexlib)
target_link_libraries(bin2intelhex PRIVATE utilslib)

# ---------------------------
# Application intelhex2bin
# ---------------------------
add_executable(intelhex2bin intelhex2bin.c)
target_link_libraries(intelhex2bin PRIVATE hexlib)
target_link_libraries(intelhex2bin PRIVATE utilslib)

# ---------------------------
# Application binCompare
# ---------------------------
add_executable(binCompare binCompare.c)

# ---------------------------
# Application binSplit
# ---------------------------
add_executable(binSplit binSplit.c)
target_link_libraries(binSplit PRIVATE utilslib)

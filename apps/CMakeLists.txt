add_executable(bin2hex bin2hex.c)
target_link_libraries(bin2hex PRIVATE hexlib)

add_executable(hex2bin hex2bin.c)
target_link_libraries(hex2bin PRIVATE hexlib)

add_executable(binCompare binCompare.c)

add_executable(binSplit binSplit.c)

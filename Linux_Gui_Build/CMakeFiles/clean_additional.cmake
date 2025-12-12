# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles/appwarships_autogen.dir/AutogenUsed.txt"
  "CMakeFiles/appwarships_autogen.dir/ParseCache.txt"
  "appwarships_autogen"
  )
endif()

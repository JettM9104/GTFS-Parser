# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles/MyGTFSApp_autogen.dir/AutogenUsed.txt"
  "CMakeFiles/MyGTFSApp_autogen.dir/ParseCache.txt"
  "MyGTFSApp_autogen"
  )
endif()

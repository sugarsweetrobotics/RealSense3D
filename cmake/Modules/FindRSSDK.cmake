
if ("${CMAKE_SYSTEM_NAME}" MATCHES "Linux")

endif  ("${CMAKE_SYSTEM_NAME}" MATCHES "Linux")



find_path(RSSDK_INCLUDE_DIR RealSense/Base.h
  HINTS "C:/Program Files (x86)/Intel/RSSDK/include")

find_path(RSSDK_LIB_DIR win32/libpxc.lib
  HINTS "C:/Program Files (x86)/Intel/RSSDK/lib")

find_library(RSSDK_LIBRARY libpxc libpxcmd
  HINTS "C:/Program Files (x86)/Intel/RSSDK/lib")

if ("${CMAKE_SYSTEM_NAME}" MATCHES "Linux")

endif  ("${CMAKE_SYSTEM_NAME}" MATCHES "Linux")



find_path(RSSDK_INCLUDE_DIRS RealSense/Base.h
  HINTS "C:/Program Files (x86)/Intel/RSSDK/include")

find_path(RSSDK_LIBRARY_DIRS win32/libpxc.lib
  HINTS "C:/Program Files (x86)/Intel/RSSDK/lib")

find_library(RSSDK_LIBRARY libpxc libpxcmd
  HINTS "C:/Program Files (x86)/Intel/RSSDK/lib/win32/")
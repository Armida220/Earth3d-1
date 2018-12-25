# Install script for directory: E:/prjs/FireBreath-master

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "C:/Program Files/FireBreath")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("E:/prjs/FireBreath-master/buildex/cmake_common/cmake_install.cmake")
  include("E:/prjs/FireBreath-master/buildex/boost/libs/thread/cmake_install.cmake")
  include("E:/prjs/FireBreath-master/buildex/ScriptingCore/cmake_install.cmake")
  include("E:/prjs/FireBreath-master/buildex/PluginCore/cmake_install.cmake")
  include("E:/prjs/FireBreath-master/buildex/NpapiCore/cmake_install.cmake")
  include("E:/prjs/FireBreath-master/buildex/ActiveXCore/cmake_install.cmake")
  include("E:/prjs/FireBreath-master/buildex/fblibs/log4cplus/cmake_install.cmake")
  include("E:/prjs/FireBreath-master/buildex/projects/BasicMediaPlayer/PluginAuto/cmake_install.cmake")
  include("E:/prjs/FireBreath-master/buildex/projects/BasicMediaPlayer/cmake_install.cmake")
  include("E:/prjs/FireBreath-master/buildex/fblibs/FBTestPlugin/log4cplus/cmake_install.cmake")
  include("E:/prjs/FireBreath-master/buildex/projects/FBTestPlugin/PluginAuto/cmake_install.cmake")
  include("E:/prjs/FireBreath-master/buildex/projects/FBTestPlugin/cmake_install.cmake")
  include("E:/prjs/FireBreath-master/buildex/unittest-cpp/UnitTest++/cmake_install.cmake")
  include("E:/prjs/FireBreath-master/buildex/ScriptingCoreTest/cmake_install.cmake")
  include("E:/prjs/FireBreath-master/buildex/ActiveXCoreTest/cmake_install.cmake")

endif()

if(CMAKE_INSTALL_COMPONENT)
  set(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
else()
  set(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
file(WRITE "E:/prjs/FireBreath-master/buildex/${CMAKE_INSTALL_MANIFEST}"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")

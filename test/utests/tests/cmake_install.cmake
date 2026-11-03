# Install script for directory: /home/teuser/Desktop/Kevin2/middleware-player-interface/test/utests/tests

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "")
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

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "1")
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

# Set default install directory permissions.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "/usr/bin/objdump")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("/home/teuser/Desktop/Kevin2/middleware-player-interface/test/utests/tests/GstUtilsTests/cmake_install.cmake")
  include("/home/teuser/Desktop/Kevin2/middleware-player-interface/test/utests/tests/GstPlayer/cmake_install.cmake")
  include("/home/teuser/Desktop/Kevin2/middleware-player-interface/test/utests/tests/GstHandlerControlTests/cmake_install.cmake")
  include("/home/teuser/Desktop/Kevin2/middleware-player-interface/test/utests/tests/TextStyleAttributes/cmake_install.cmake")
  include("/home/teuser/Desktop/Kevin2/middleware-player-interface/test/utests/tests/OcdmBasicSessionAdapterTests/cmake_install.cmake")
  include("/home/teuser/Desktop/Kevin2/middleware-player-interface/test/utests/tests/OCDMSessionAdapter/cmake_install.cmake")
  include("/home/teuser/Desktop/Kevin2/middleware-player-interface/test/utests/tests/DrmSecureClient/cmake_install.cmake")
  include("/home/teuser/Desktop/Kevin2/middleware-player-interface/test/utests/tests/DrmUrlTests/cmake_install.cmake")
  include("/home/teuser/Desktop/Kevin2/middleware-player-interface/test/utests/tests/base16Tests/cmake_install.cmake")
  include("/home/teuser/Desktop/Kevin2/middleware-player-interface/test/utests/tests/Base64PLAYER/cmake_install.cmake")
  include("/home/teuser/Desktop/Kevin2/middleware-player-interface/test/utests/tests/PluginsTests/cmake_install.cmake")
  include("/home/teuser/Desktop/Kevin2/middleware-player-interface/test/utests/tests/InterfacePlayerTests/cmake_install.cmake")
  include("/home/teuser/Desktop/Kevin2/middleware-player-interface/test/utests/tests/DrmSessionManagerTests/cmake_install.cmake")
  include("/home/teuser/Desktop/Kevin2/middleware-player-interface/test/utests/tests/PlayerUtilsTests/cmake_install.cmake")
  include("/home/teuser/Desktop/Kevin2/middleware-player-interface/test/utests/tests/PlayerExternalsRdkTests/cmake_install.cmake")
  include("/home/teuser/Desktop/Kevin2/middleware-player-interface/test/utests/tests/ClosedCaptionsTests/cmake_install.cmake")
  include("/home/teuser/Desktop/Kevin2/middleware-player-interface/test/utests/tests/PlayerIsoBmffTests/cmake_install.cmake")
  include("/home/teuser/Desktop/Kevin2/middleware-player-interface/test/utests/tests/PlayerExternalsTests/cmake_install.cmake")
  include("/home/teuser/Desktop/Kevin2/middleware-player-interface/test/utests/tests/PlayerJsonObjectTests/cmake_install.cmake")
  include("/home/teuser/Desktop/Kevin2/middleware-player-interface/test/utests/tests/ContentsecuritymanagerTests/cmake_install.cmake")
  include("/home/teuser/Desktop/Kevin2/middleware-player-interface/test/utests/tests/SubtitleTests/cmake_install.cmake")
  include("/home/teuser/Desktop/Kevin2/middleware-player-interface/test/utests/tests/PlayerCommonTests/cmake_install.cmake")
  include("/home/teuser/Desktop/Kevin2/middleware-player-interface/test/utests/tests/PlayerLogManagerTests/cmake_install.cmake")
  include("/home/teuser/Desktop/Kevin2/middleware-player-interface/test/utests/tests/VendorTests/cmake_install.cmake")
  include("/home/teuser/Desktop/Kevin2/middleware-player-interface/test/utests/tests/GstPluginsTests/cmake_install.cmake")
  include("/home/teuser/Desktop/Kevin2/middleware-player-interface/test/utests/tests/GstPlugingSubtecTests/cmake_install.cmake")
  include("/home/teuser/Desktop/Kevin2/middleware-player-interface/test/utests/tests/FireBoltTests/cmake_install.cmake")
  include("/home/teuser/Desktop/Kevin2/middleware-player-interface/test/utests/tests/Mp4DemuxTests/cmake_install.cmake")
  include("/home/teuser/Desktop/Kevin2/middleware-player-interface/test/utests/tests/InfraTests/cmake_install.cmake")
  include("/home/teuser/Desktop/Kevin2/middleware-player-interface/test/utests/tests/GstPluginL1Tests/cmake_install.cmake")
  include("/home/teuser/Desktop/Kevin2/middleware-player-interface/test/utests/tests/SecManagerThunderTests/cmake_install.cmake")
  include("/home/teuser/Desktop/Kevin2/middleware-player-interface/test/utests/tests/DrmOcdmTests/cmake_install.cmake")
  include("/home/teuser/Desktop/Kevin2/middleware-player-interface/test/utests/tests/DrmHelperTests/cmake_install.cmake")
  include("/home/teuser/Desktop/Kevin2/middleware-player-interface/test/utests/tests/DrmAes/cmake_install.cmake")
  include("/home/teuser/Desktop/Kevin2/middleware-player-interface/test/utests/tests/DrmTests/cmake_install.cmake")
  include("/home/teuser/Desktop/Kevin2/middleware-player-interface/test/utests/tests/PlayerDirectRialtoCCManagerTests/cmake_install.cmake")

endif()


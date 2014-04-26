# Install script for directory: K:/KL/klImaging/ipp-UIC.7.1.1.013/sources/uic

# Set the install prefix
IF(NOT DEFINED CMAKE_INSTALL_PREFIX)
  SET(CMAKE_INSTALL_PREFIX "C:/Program Files/uic")
ENDIF(NOT DEFINED CMAKE_INSTALL_PREFIX)
STRING(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
IF(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  IF(BUILD_TYPE)
    STRING(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  ELSE(BUILD_TYPE)
    SET(CMAKE_INSTALL_CONFIG_NAME "release")
  ENDIF(BUILD_TYPE)
  MESSAGE(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
ENDIF(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)

# Set the component getting installed.
IF(NOT CMAKE_INSTALL_COMPONENT)
  IF(COMPONENT)
    MESSAGE(STATUS "Install component: \"${COMPONENT}\"")
    SET(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  ELSE(COMPONENT)
    SET(CMAKE_INSTALL_COMPONENT)
  ENDIF(COMPONENT)
ENDIF(NOT CMAKE_INSTALL_COMPONENT)

IF(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  INCLUDE("K:/KL/klImaging/ipp-UIC.7.1.1.013/__cmake/uic.intel64.vc2010.s.mt/application/picnic/cmake_install.cmake")
  INCLUDE("K:/KL/klImaging/ipp-UIC.7.1.1.013/__cmake/uic.intel64.vc2010.s.mt/application/uic_transcoder_con/cmake_install.cmake")
  INCLUDE("K:/KL/klImaging/ipp-UIC.7.1.1.013/__cmake/uic.intel64.vc2010.s.mt/codec/image/bmp/common/cmake_install.cmake")
  INCLUDE("K:/KL/klImaging/ipp-UIC.7.1.1.013/__cmake/uic.intel64.vc2010.s.mt/codec/image/bmp/dec/cmake_install.cmake")
  INCLUDE("K:/KL/klImaging/ipp-UIC.7.1.1.013/__cmake/uic.intel64.vc2010.s.mt/codec/image/bmp/enc/cmake_install.cmake")
  INCLUDE("K:/KL/klImaging/ipp-UIC.7.1.1.013/__cmake/uic.intel64.vc2010.s.mt/codec/image/jpeg/common/cmake_install.cmake")
  INCLUDE("K:/KL/klImaging/ipp-UIC.7.1.1.013/__cmake/uic.intel64.vc2010.s.mt/codec/image/jpeg/dec/cmake_install.cmake")
  INCLUDE("K:/KL/klImaging/ipp-UIC.7.1.1.013/__cmake/uic.intel64.vc2010.s.mt/codec/image/jpeg/enc/cmake_install.cmake")
  INCLUDE("K:/KL/klImaging/ipp-UIC.7.1.1.013/__cmake/uic.intel64.vc2010.s.mt/codec/image/jpeg2000/common/cmake_install.cmake")
  INCLUDE("K:/KL/klImaging/ipp-UIC.7.1.1.013/__cmake/uic.intel64.vc2010.s.mt/codec/image/jpeg2000/dec/cmake_install.cmake")
  INCLUDE("K:/KL/klImaging/ipp-UIC.7.1.1.013/__cmake/uic.intel64.vc2010.s.mt/codec/image/jpeg2000/enc/cmake_install.cmake")
  INCLUDE("K:/KL/klImaging/ipp-UIC.7.1.1.013/__cmake/uic.intel64.vc2010.s.mt/codec/image/jpegxr/common/cmake_install.cmake")
  INCLUDE("K:/KL/klImaging/ipp-UIC.7.1.1.013/__cmake/uic.intel64.vc2010.s.mt/codec/image/jpegxr/dec/cmake_install.cmake")
  INCLUDE("K:/KL/klImaging/ipp-UIC.7.1.1.013/__cmake/uic.intel64.vc2010.s.mt/codec/image/jpegxr/enc/cmake_install.cmake")
  INCLUDE("K:/KL/klImaging/ipp-UIC.7.1.1.013/__cmake/uic.intel64.vc2010.s.mt/codec/image/png/common/cmake_install.cmake")
  INCLUDE("K:/KL/klImaging/ipp-UIC.7.1.1.013/__cmake/uic.intel64.vc2010.s.mt/codec/image/png/dec/cmake_install.cmake")
  INCLUDE("K:/KL/klImaging/ipp-UIC.7.1.1.013/__cmake/uic.intel64.vc2010.s.mt/codec/image/png/enc/cmake_install.cmake")
  INCLUDE("K:/KL/klImaging/ipp-UIC.7.1.1.013/__cmake/uic.intel64.vc2010.s.mt/codec/image/zlib/cmake_install.cmake")
  INCLUDE("K:/KL/klImaging/ipp-UIC.7.1.1.013/__cmake/uic.intel64.vc2010.s.mt/core/uic/cmake_install.cmake")
  INCLUDE("K:/KL/klImaging/ipp-UIC.7.1.1.013/__cmake/uic.intel64.vc2010.s.mt/io/uic_io/cmake_install.cmake")

ENDIF(NOT CMAKE_INSTALL_LOCAL_ONLY)

IF(CMAKE_INSTALL_COMPONENT)
  SET(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
ELSE(CMAKE_INSTALL_COMPONENT)
  SET(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
ENDIF(CMAKE_INSTALL_COMPONENT)

FILE(WRITE "K:/KL/klImaging/ipp-UIC.7.1.1.013/__cmake/uic.intel64.vc2010.s.mt/${CMAKE_INSTALL_MANIFEST}" "")
FOREACH(file ${CMAKE_INSTALL_MANIFEST_FILES})
  FILE(APPEND "K:/KL/klImaging/ipp-UIC.7.1.1.013/__cmake/uic.intel64.vc2010.s.mt/${CMAKE_INSTALL_MANIFEST}" "${file}\n")
ENDFOREACH(file)

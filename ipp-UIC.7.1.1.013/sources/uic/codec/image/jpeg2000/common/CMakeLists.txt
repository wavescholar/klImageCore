include_directories (
  ${CMAKE_SOURCE_DIR}/codec/image/jpeg2000/common/src
  ${CMAKE_SOURCE_DIR}/codec/image/jpeg2000/common/src/codestream
  ${CMAKE_SOURCE_DIR}/codec/image/jpeg2000/common/src/codestream/common
  ${CMAKE_SOURCE_DIR}/codec/image/jpeg2000/common/src/codestream/ebcot
  ${CMAKE_SOURCE_DIR}/codec/image/jpeg2000/common/src/codestream/mainheader
  ${CMAKE_SOURCE_DIR}/codec/image/jpeg2000/common/src/codestream/marker
  ${CMAKE_SOURCE_DIR}/codec/image/jpeg2000/common/src/codestream/packetheader
  ${CMAKE_SOURCE_DIR}/codec/image/jpeg2000/common/src/codestream/quant
  ${CMAKE_SOURCE_DIR}/codec/image/jpeg2000/common/src/codestream/tile
  ${CMAKE_SOURCE_DIR}/codec/image/jpeg2000/common/src/codestream/wavelet
  ${CMAKE_SOURCE_DIR}/codec/image/jpeg2000/common/src/common
  ${CMAKE_SOURCE_DIR}/codec/image/jpeg2000/common/src/common/container
  ${CMAKE_SOURCE_DIR}/codec/image/jpeg2000/common/src/common/geometry
  ${CMAKE_SOURCE_DIR}/codec/image/jpeg2000/common/src/common/image
  ${CMAKE_SOURCE_DIR}/codec/image/jpeg2000/common/src/common/ippwrapper
  ${CMAKE_SOURCE_DIR}/codec/image/jpeg2000/common/src/common/stream
  ${CMAKE_SOURCE_DIR}/codec/image/jpeg2000/common/src/common/string
  ${CMAKE_SOURCE_DIR}/codec/image/jpeg2000/common/src/file
)

make_library( longname static )
make_use_openmp( )

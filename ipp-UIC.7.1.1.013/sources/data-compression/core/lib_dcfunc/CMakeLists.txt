include_directories (
  ${CMAKE_SOURCE_DIR}/core/lib_dcfunc/include
  ${CMAKE_SOURCE_DIR}/core/lib_zlib/include
)

make_library( shortname static )

include_directories (
  ${CMAKE_SOURCE_DIR}/codec/image/jpeg/common/include
  ${CMAKE_SOURCE_DIR}/codec/image/jpeg/dec/include
  ${CMAKE_SOURCE_DIR}/core/uic/include
)

make_library( longname static )
make_use_openmp( )

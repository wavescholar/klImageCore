include_directories (
  ${CMAKE_SOURCE_DIR}/core/lib_bzip2/include
)

add_definitions( -D_LIB )

make_library( shortname static )
make_use_openmp( )

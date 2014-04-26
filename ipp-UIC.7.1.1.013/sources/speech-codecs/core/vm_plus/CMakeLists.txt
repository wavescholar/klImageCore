include_directories (
  ${CMAKE_SOURCE_DIR}/core/vm/include
  ${CMAKE_SOURCE_DIR}/core/vm_plus/include
  ${CMAKE_SOURCE_DIR}/core/umc/include
)

make_library( shortname static )

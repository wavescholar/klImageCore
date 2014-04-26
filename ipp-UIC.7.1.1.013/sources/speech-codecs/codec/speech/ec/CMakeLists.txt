include_directories (
  ${CMAKE_SOURCE_DIR}/core/usc/include
  ${CMAKE_SOURCE_DIR}/codec/speech/common/include
  ${CMAKE_SOURCE_DIR}/codec/speech/ec/include
)

make_library( longname static )

include_directories (
  ${CMAKE_SOURCE_DIR}/core/usc/include
  ${CMAKE_SOURCE_DIR}/codec/speech/common/include
  ${CMAKE_SOURCE_DIR}/codec/speech/g722.1/include
)

make_library( longname static )

include_directories (
  ${CMAKE_SOURCE_DIR}/core/usc/include
  ${CMAKE_SOURCE_DIR}/codec/speech/common/include
  ${CMAKE_SOURCE_DIR}/codec/speech/g729i/include
  ${CMAKE_SOURCE_DIR}/codec/speech/vad/include
)

make_library( longname static )

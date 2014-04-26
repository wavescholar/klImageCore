include_directories (
  ${CMAKE_SOURCE_DIR}/application/ippsdrv/include
  ${CMAKE_SOURCE_DIR}/application/ippsdrv/src/drvold
  ${CMAKE_SOURCE_DIR}/application/ippsdrv/src/drvnew/sys
)

list( APPEND LIBS_IPP ippcore )

if( Windows )
  add_definitions( -D_AFXDLL )
endif( )

file( GLOB_RECURSE sources "src/*.cpp" "res/*.rc" )

make_executable( shortname )
set_target_properties( ${target} PROPERTIES WIN32_EXECUTABLE "true" )

add_custom_command(
  TARGET ${target} POST_BUILD
  COMMAND cmake -E copy ARGS ${CMAKE_CURRENT_SOURCE_DIR}/lib/${__ARCH}/ippsdrv.sys ${CMAKE_BIN_DIR}/${__CONFIG}
  COMMAND cmake -E copy ARGS ${CMAKE_CURRENT_SOURCE_DIR}/lib/${__ARCH}/ippsexp.sys ${CMAKE_BIN_DIR}/${__CONFIG}
  COMMAND cmake -E copy ARGS ${CMAKE_CURRENT_SOURCE_DIR}/lib/${__ARCH}/ippsold.sys ${CMAKE_BIN_DIR}/${__CONFIG}
)

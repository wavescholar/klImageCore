set( target csharp )
list_component(  )

file( MAKE_DIRECTORY ${CMAKE_BIN_DIR}/${__CONFIG} )
file( GLOB_RECURSE sources.interface "src/interface/[^.]*.cs" )

foreach( source ${sources.interface} )
  get_filename_component( name ${source} NAME_WE )
  list( APPEND interfaces ${name}_cs.dll )

  if( ${name} MATCHES ippdefs )
    add_custom_command( OUTPUT ${name}_cs.dll COMMAND csc.exe ARGS
      /target:library /unsafe /platform:anycpu
      /out:${CMAKE_BIN_DIR}/${__CONFIG}/${name}_cs.dll
      ${name}.cs WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/src/interface  )
  else ( )
    add_custom_command( OUTPUT ${name}_cs.dll COMMAND csc.exe ARGS
      /target:library /unsafe /platform:anycpu
      /r:${CMAKE_BIN_DIR}/${__CONFIG}/ippdefs_cs.dll
      /out:${CMAKE_BIN_DIR}/${__CONFIG}/${name}_cs.dll
      ${name}.cs DEPENDS ippdefs_cs.dll WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/src/interface  )
  endif( )
endforeach( )


add_custom_target( app ALL
  csc.exe
  /target:exe /unsafe /platform:anycpu /out:stripps.exe
  /r:System.dll,ippdefs_cs.dll,ippcore_cs.dll,ippch_cs.dll,ipps_cs.dll
  /recurse:${CMAKE_CURRENT_SOURCE_DIR}\\src\\stripps\\*.cs

  COMMAND csc.exe
  /target:exe /unsafe /platform:anycpu /out:ttippi.exe
  /r:System.dll,System.Windows.Forms.dll,System.Drawing.dll
  /r:ippdefs_cs.dll,ippcore_cs.dll,ippj_cs.dll,ippi_cs.dll,ipps_cs.dll
  /recurse:${CMAKE_CURRENT_SOURCE_DIR}\\src\\ttippi\\*.cs

  COMMAND csc.exe
  /target:winexe /unsafe /platform:anycpu /out:wavelet.exe
  /r:System.dll,System.Windows.Forms.dll,System.Drawing.dll
  /r:ippdefs_cs.dll,ippi_cs.dll
  /recurse:${CMAKE_CURRENT_SOURCE_DIR}\\src\\wavelet\\*.cs
  /res:${CMAKE_CURRENT_SOURCE_DIR}\\src\\wavelet\\Resources\\wavelet.AboutForm.resources
  /res:${CMAKE_CURRENT_SOURCE_DIR}\\src\\wavelet\\Resources\\wavelet.MainForm.resources
  /res:${CMAKE_CURRENT_SOURCE_DIR}\\src\\wavelet\\Resources\\wavelet.Properties.resources

  DEPENDS ${interfaces} WORKING_DIRECTORY ${CMAKE_BIN_DIR}/${__CONFIG} )

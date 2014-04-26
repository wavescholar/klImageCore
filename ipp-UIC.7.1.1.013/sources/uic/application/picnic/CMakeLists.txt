find_package( Qt4 4.8.1 QUIET COMPONENTS QtCore QtGui )

if( NOT QT4_FOUND )
  message( WARNING "Qt4 not found! Add 'qmake' to PATH variable!" )
  return( )
else ( )
  include( ${QT_USE_FILE} )
endif( )

include_directories (
  ${CMAKE_SOURCE_DIR}/application/picnic/include
  ${CMAKE_SOURCE_DIR}/codec/image/jpeg2000/enc/include
  ${CMAKE_SOURCE_DIR}/codec/image/jpeg2000/dec/include
  ${CMAKE_SOURCE_DIR}/codec/image/jpegxr/enc/include
  ${CMAKE_SOURCE_DIR}/codec/image/jpegxr/dec/include
  ${CMAKE_SOURCE_DIR}/codec/image/jpeg/common/include
  ${CMAKE_SOURCE_DIR}/codec/image/jpeg/enc/include
  ${CMAKE_SOURCE_DIR}/codec/image/jpeg/dec/include
  ${CMAKE_SOURCE_DIR}/codec/image/bmp/enc/include
  ${CMAKE_SOURCE_DIR}/codec/image/bmp/dec/include
  ${CMAKE_SOURCE_DIR}/codec/image/png/enc/include
  ${CMAKE_SOURCE_DIR}/codec/image/png/dec/include
  ${CMAKE_SOURCE_DIR}/core/uic/include
  ${CMAKE_SOURCE_DIR}/io/uic_io/include
)

list( APPEND LIBS_OWN uic_io core_uic jpeg2000_common jpeg2000_enc jpeg2000_dec jpegxr_common jpegxr_enc jpegxr_dec jpeg_common jpeg_enc jpeg_dec bmp_enc bmp_dec png_common png_enc png_dec zlib )
list( APPEND LIBS_IPP ippcore ipps ippi ippj ippcv ippcc ippdc ippch )

if( Windows )
  if( IPP_MT )
    list( APPEND LIBS libiomp5md )
  endif( )

elseif( Linux )
  list( APPEND LIBS glib-2.0 gtk-x11-2.0 )
  if( IPP_MT )
    list( APPEND LIBS imf irc )
    list( APPEND LIBS iomp5 )
    list( APPEND LIBS pthread )
  endif( )

elseif( Darwin )
  if( IPP_MT )
    list( APPEND LIBS imf irc )
    list( APPEND LIBS iomp5 )
  endif( )

endif( )

qt4_wrap_cpp( sources.moc
  include/mainwindow.h
  include/fileopendlg.h
  include/filesavedlg.h
  include/imagewidget.h
  include/optionsdialog.h
)

qt4_add_resources( sources.qrc
  src/resources.qrc
)

list( APPEND sources.plus
  ${sources.moc}
  ${sources.qrc}
)

make_executable( shortname )

target_link_libraries( ${target} ${QT_LIBRARIES} )

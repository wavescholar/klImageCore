include_directories (
  ${CMAKE_SOURCE_DIR}/application/uic_transcoder_con/include
  ${CMAKE_SOURCE_DIR}/codec/image/bmp/common/include
  ${CMAKE_SOURCE_DIR}/codec/image/bmp/dec/include
  ${CMAKE_SOURCE_DIR}/codec/image/bmp/enc/include
  ${CMAKE_SOURCE_DIR}/codec/image/png/dec/include
  ${CMAKE_SOURCE_DIR}/codec/image/png/enc/include
  ${CMAKE_SOURCE_DIR}/codec/image/jpeg/common/include
  ${CMAKE_SOURCE_DIR}/codec/image/jpeg/dec/include
  ${CMAKE_SOURCE_DIR}/codec/image/jpeg/enc/include
  ${CMAKE_SOURCE_DIR}/codec/image/jpegxr/dec/include
  ${CMAKE_SOURCE_DIR}/codec/image/jpegxr/enc/include
  ${CMAKE_SOURCE_DIR}/codec/image/jpeg2000/dec/include
  ${CMAKE_SOURCE_DIR}/codec/image/jpeg2000/enc/include
  ${CMAKE_SOURCE_DIR}/codec/image/jpeg2000/common/src/common
  ${CMAKE_SOURCE_DIR}/io/uic_io/include
  ${CMAKE_SOURCE_DIR}/core/uic/include
)

list( APPEND LIBS_OWN uic_io core_uic bmp_dec bmp_enc jpeg_common jpeg_dec jpeg_enc jpegxr_common jpegxr_dec jpegxr_enc jpeg2000_common jpeg2000_dec jpeg2000_enc png_common png_dec png_enc zlib )
list( APPEND LIBS_IPP ippcore ippi ippj ipps ippcc ippdc ippch )

if( Windows )
  if( IPP_MT )
    list( APPEND LIBS svml_dispmt libircmt libiomp5md )
  endif( )

elseif( Linux )
  list( APPEND LIBS pthread )

  if( IPP_MT )
    list( APPEND LIBS imf irc )
    list( APPEND LIBS iomp5 )
  endif( )

elseif( Darwin )
  if( IPP_MT )
    list( APPEND LIBS imf irc )
    list( APPEND LIBS iomp5 )
  endif( )

endif( )

make_executable( shortname )


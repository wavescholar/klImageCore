list( APPEND LIBS_IPP ippcore ipps ippch )

if( Windows )
  if( IPP_MT )
    list( APPEND LIBS libmmt libiomp5md )
  endif( )

elseif( Linux )
  if( IPP_MT )
    list( APPEND LIBS iomp5 )
    list( APPEND LIBS pthread   )
  endif( )

elseif( Darwin )
  if( IPP_MT )
    list( APPEND LIBS iomp5 )
  endif( )

endif( )

make_executable( shortname )

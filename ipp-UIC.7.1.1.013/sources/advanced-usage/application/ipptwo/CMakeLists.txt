if( __LINKAGE MATCHES d )
  return( )
endif( )

if( Windows )
  list( APPEND LIBS_OWN customdll )
elseif( Linux )
  list( APPEND LIBS_OWN customso  )
elseif( Darwin )
  list( APPEND LIBS_OWN customdyld )
endif( )

make_executable( shortname )

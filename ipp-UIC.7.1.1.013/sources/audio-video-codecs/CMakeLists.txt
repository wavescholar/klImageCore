cmake_minimum_required( VERSION 2.8.8 FATAL_ERROR )
project( audio-video-codecs )

include( ../../builder/FindFunctions.cmake )
include( ../../builder/FindGlobals.cmake   )
include( ../../builder/FindIPP.cmake       )

if( Windows )
  include( ../../builder/FindMSDK.cmake    )
endif( )

if( ${__GENERATOR} MATCHES vc2008 )
  include( ../../builder/FindDXSDK.cmake    )
endif( )

create_build( )

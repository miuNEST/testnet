file(GLOB HEADERS "include/graphene/poc/*.hpp")

add_library( graphene_poc
             poc.cpp
           )

target_link_libraries( graphene_poc graphene_chain graphene_app )
target_include_directories( graphene_poc
                            PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include" )

install( TARGETS
   graphene_poc

   RUNTIME DESTINATION bin
   LIBRARY DESTINATION lib
   ARCHIVE DESTINATION lib
)

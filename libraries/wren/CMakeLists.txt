file(GLOB headers "include/graphene/wren/*.hpp" "include/graphene/wren/*.h")

aux_source_directory(./ SRC)
#aux_source_directory(./*.cpp SRC_CPP)
add_library( graphene_wren
${SRC}
include/graphene/wren/wren.hpp
smart_contract_output.cpp
)

target_link_libraries( graphene_wren graphene_chain fc )
target_include_directories( graphene_wren
PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include/graphene/wren" )
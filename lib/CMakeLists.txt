add_mutiny_sources(
  "${MT_LIB_DIR}/mutiny.cpp"
  "${MT_LIB_DIR}/args.cpp"
  "${MT_LIB_DIR}/compiler.cpp"
)

include("${MT_LIB_DIR}/src_unit/CMakeLists.txt")
include("${MT_LIB_DIR}/util/CMakeLists.txt")
include("${MT_LIB_DIR}/basic/CMakeLists.txt")
include("${MT_LIB_DIR}/syntax/CMakeLists.txt")
include("${MT_LIB_DIR}/ast/CMakeLists.txt")
include("${MT_LIB_DIR}/code_gen/CMakeLists.txt")

add_mutiny_sources(
  "${MT_LIB_DIR}/mutiny.c"
  "${MT_LIB_DIR}/settings.c"
  "${MT_LIB_DIR}/args.c"
  "${MT_LIB_DIR}/compiler.c"
)

include("${MT_LIB_DIR}/translation_unit/CMakeLists.txt")
include("${MT_LIB_DIR}/ast/CMakeLists.txt")
include("${MT_LIB_DIR}/util/CMakeLists.txt")

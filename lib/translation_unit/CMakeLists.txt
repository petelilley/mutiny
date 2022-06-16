add_mutiny_sources(
  "${MT_LIB_DIR}/translation_unit/translation_unit.c"
)

include("${MT_LIB_DIR}/translation_unit/parser/CMakeLists.txt")
include("${MT_LIB_DIR}/translation_unit/lexer/CMakeLists.txt")
add_mutiny_sources(
  "${MT_LIB_DIR}/codegen/codegen.cpp"
  "${MT_LIB_DIR}/codegen/backend.cpp"
)

include("${MT_LIB_DIR}/codegen/llvm/CMakeLists.txt")

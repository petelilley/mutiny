#include <mutiny/mutiny.hpp>
#include <mutiny/compiler.hpp>

int main(int argc, const char** argv) {
  mt::Compiler::get().setup(argc, argv);
  return mt::Compiler::get().exec();
}
#include <mutiny/code_gen/code_gen.hpp>
#include <llvm-10/llvm/IR/Constants.h>
#include <llvm-10/llvm/ADT/APFloat.h>
#include <llvm-10/llvm/ADT/APSInt.h>

using namespace mt;

CodeGen::CodeGen()
: context(), builder(context) {

}

CodeGen::~CodeGen() = default;

void CodeGen::exec(ASTNode& ast, b8 optimize) {
  ast_start = ast_curr = &ast;

}

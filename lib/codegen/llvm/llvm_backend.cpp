#include <mutiny/codegen/llvm/llvm_backend.hpp>

using namespace mt;

LLVMBackend::LLVMBackend() {

}

LLVMBackend::~LLVMBackend() {

}

LLVMBackend::LLVMBackend(LLVMBackend&&) {

}

void LLVMBackend::exec(ASTNode& ast) {
  ast_start = ast_curr = &ast;
}

void LLVMBackend::optimize() {

}

void LLVMBackend::dump_ir() {

}

void LLVMBackend::dump_bc() {

}

void LLVMBackend::dump_asm() {

}

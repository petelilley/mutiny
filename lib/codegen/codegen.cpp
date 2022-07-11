#include <mutiny/codegen/codegen.hpp>
#include <mutiny/codegen/llvm/llvm_backend.hpp>

using namespace mt;

CodeGenerator::CodeGenerator() = default;

CodeGenerator::~CodeGenerator() = default;

void CodeGenerator::exec(ASTNode& ast, b8 optimize) {
  // TODO: Backend selection.
  backend = std::unique_ptr<CodeGenBackend>(new LLVMBackend());

  if (!backend) {
    result = Result::INVALID_BACKEND;
    return;
  }

  backend->exec(ast);

  if (optimize) {
    backend->optimize();
  }

  if (backend->get_result() != CodeGenBackend::Result::SUCCESS) {
    result = Result::FAILURE;
  }
}

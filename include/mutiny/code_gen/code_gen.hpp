#pragma once

#include <mutiny/mutiny.hpp>
#include <mutiny/basic/status.hpp>
#include <mutiny/ast/ast.hpp>

#include <llvm-10/llvm/IR/LLVMContext.h>
#include <llvm-10/llvm/IR/IRBuilder.h>
#include <llvm-10/llvm/IR/Module.h>
#include <llvm-10/llvm/IR/Value.h>

namespace mt {

class CodeGen {
public:
  CodeGen();
  ~CodeGen();

  void exec(ASTNode& ast, b8 optimize);

  enum class Result {
    SUCCESS = 0,
    FAILURE,
  };

  inline Result get_result() const;

protected:
  Result result = Result::SUCCESS;

  ASTNode* ast_start = nullptr,
         * ast_curr = nullptr;

  llvm::LLVMContext context;
  llvm::IRBuilder<> builder;
  std::unique_ptr<llvm::Module> llvm_module;
};

} // namespace mt

#include <mutiny/code_gen/code_gen.ipp>

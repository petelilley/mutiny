#pragma once

#include <mutiny/mutiny.hpp>
#include <mutiny/ast/ast.hpp>

namespace mt {

class CodeGenBackend {
public:
  CodeGenBackend();
  virtual ~CodeGenBackend();

  enum class Result {
    SUCCESS = 0,
    INVALID_BACKEND,
    FAILURE,
  };

  inline Result get_result() const;

  virtual void exec(ASTNode& ast) = 0;
  virtual void optimize() = 0;

  virtual void dump_ir() = 0;
  virtual void dump_bc() = 0;
  virtual void dump_asm() = 0;

protected:
  Result result = Result::SUCCESS;

  ASTNode* ast_start = nullptr,
         * ast_curr = nullptr;
};

} // namespace mt

#include <mutiny/codegen/backend.ipp>

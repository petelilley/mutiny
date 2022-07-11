#pragma once

#include <mutiny/mutiny.hpp>
#include <mutiny/basic/status.hpp>
#include <mutiny/ast/ast.hpp>
#include <mutiny/codegen/backend.hpp>

namespace mt {

class CodeGenerator {
public:
  CodeGenerator();
  ~CodeGenerator();

  void exec(ASTNode& ast, b8 optimize);

  enum class Result {
    SUCCESS = 0,
    INVALID_BACKEND,
    FAILURE,
  };

  inline Result get_result() const;

protected:
  Result result = Result::SUCCESS;

  std::unique_ptr<CodeGenBackend> backend = nullptr;
};

} // namespace mt

#include <mutiny/codegen/codegen.ipp>

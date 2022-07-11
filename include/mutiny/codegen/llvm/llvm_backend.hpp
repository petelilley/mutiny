#include <mutiny/codegen/backend.hpp>

namespace mt {

class LLVMBackend : public CodeGenBackend {
public:
  LLVMBackend();
  virtual ~LLVMBackend();
  LLVMBackend(LLVMBackend&&);
  LLVMBackend(const LLVMBackend&) = delete;

  void exec(ASTNode& ast);
  void optimize();

  void dump_ir();
  void dump_bc();
  void dump_asm();

private:
};

} // namespace mt

#pragma once

#include <mutiny/mutiny.hpp>
#include <mutiny/basic/status.hpp>
#include <mutiny/util/file.hpp>
#include <mutiny/util/logger.hpp>

namespace mt {

class Token;
class ASTNode;

class Parser {
public:
  Parser(Status& status);
  ~Parser();

  void exec();

private:
  Status& status;
};

} // namespace mt
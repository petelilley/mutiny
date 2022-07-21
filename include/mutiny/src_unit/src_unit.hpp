#pragma once

#include <mutiny/mutiny.hpp>
#include <mutiny/src_unit/lexer/lexer.hpp>
#include <mutiny/src_unit/parser/parser.hpp>
#include <mutiny/basic/status.hpp>
#include <mutiny/util/logger.hpp>
#include <mutiny/util/file.hpp>

namespace mt {

/**
 * @brief Represents a single unit of the mutiny source code being compiled.
 */
class SrcUnit {
public:
  SrcUnit(std::filesystem::path path, b8 warning_as_error);
  ~SrcUnit();
  SrcUnit(SrcUnit&&);
  SrcUnit(const SrcUnit&) = delete;

  /**
   * @brief Executes the lexical analysis stage of the translation unit.
   */
  void exec_lexer();

  /**
   * @brief Executes the parsing stage of the translation unit.
   */
  void exec_parser();

  enum class Result {
    SUCCESS = 0,
    EMPTY_FILE,
    INVALID_TOKENS,
    INVALID_SYNTAX,
  };

  /**
   * @brief Returns the result of the translation unit.
   * 
   * @return The result of the translation unit.
   */
  constexpr Result get_result() const;

  /**
   * @brief Dumps all the message logs to the console.
   */
  void dump_logs();

  /**
   * @brief Returns the status object of the translation unit.
   * 
   * @return The status object of the translation unit.
   */
  constexpr const Status& get_status() const;

  /**
   * @brief Returns the AST representation of the translation unit.
   * 
   * @return The AST representation of the translation unit.
   */
  inline std::optional<ASTNode> get_ast();

private:
  Result result = Result::SUCCESS;

  InputFile src_file;
  
  Status status;

  Lexer lexer;
  Parser parser;
};

} // namespace mt

#include <mutiny/src_unit/src_unit.ipp>
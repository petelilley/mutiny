#pragma once

#include <mutiny/mutiny.hpp>
#include <mutiny/translation_unit/lexer/lexer.hpp>
#include <mutiny/translation_unit/parser/parser.hpp>
#include <mutiny/util/logger.hpp>
#include <mutiny/util/file.hpp>

namespace mt {

class TranslationUnit {
public:
  TranslationUnit(std::filesystem::path path);
  ~TranslationUnit();
  TranslationUnit(TranslationUnit&&);
  TranslationUnit(const TranslationUnit&) = delete;

  /**
   * @brief Executes the lexical analysis stage of the translation unit.
   */
  void exec_lexer();

  /**
   * @brief Executes the parsing stage of the translation unit.
   */
  void exec_parser();

  enum class Status {
    SUCCESS = 0,
    INVALID_TOKENS,
    INVALID_SYNTAX,
  };

  /**
   * @brief Returns the status of the translation unit.
   * 
   * @return The status of the translation unit.
   */
  constexpr Status get_status() const { return status; }

  /**
   * @brief Dumps all the message logs to the console.
   */
  void dump_logs();

private:
  Status status = Status::SUCCESS;

  InputFile src_file;
  
  Logger log_out,
         log_err,
         log_warn;

  Lexer lexer;
  Parser parser;
};

} // namespace mt
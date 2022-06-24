#pragma once

#include <mutiny/mutiny.hpp>
#include <mutiny/translation_unit/lexer/lexer.hpp>
#include <mutiny/translation_unit/parser/parser.hpp>
#include <mutiny/util/logger.hpp>
#include <mutiny/util/file.hpp>

namespace mt {

class TranslationUnit {
public:
  TranslationUnit(std::filesystem::path path) noexcept;
  ~TranslationUnit() noexcept;
  TranslationUnit(TranslationUnit&&) noexcept;
  TranslationUnit(const TranslationUnit&) noexcept = delete;

  /**
   * @brief Executes the lexical analysis stage of the translation unit.
   */
  void exec_lexer() noexcept;

  /**
   * @brief Executes the parsing stage of the translation unit.
   */
  void exec_parser() noexcept;

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
  constexpr Status get_status() const noexcept { return status; }

  /**
   * @brief Dumps all the message logs to the console.
   */
  void dump_logs() noexcept;

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
#pragma once

#include <mutiny/mutiny.hpp>
#include <mutiny/basic/status.hpp>
#include <mutiny/util/file.hpp>
#include <mutiny/util/logger.hpp>
#include <mutiny/translation_unit/lexer/token.hpp>
#include <mutiny/ast/ast.hpp>

namespace mt {

class Parser {
public:
  Parser(Status& status);
  ~Parser();

  void exec(const std::vector<Token>* tokens);

private:
  Status& status;

  const std::vector<Token>* tokens = nullptr;
  std::vector<Token>::const_iterator current_token;

  inline b8 comp_token(Token::Kind kind);
  inline b8 comp_token_punct(Punct punct);
  inline b8 comp_token_keyword(Keyword keyword);

  Token::Kind match_token(Token::Kind kind);
  Punct match_token_punct(Punct punct);
  Keyword match_token_keyword(Keyword keyword);

  template<typename... Args>
  Token::Kind match_token(Token::Kind kind, Args... others);
  template<typename... Args>
  Punct match_token_punct(Punct punct, Args... others);
  template<typename ... Args>
  Keyword match_token_keyword(Keyword keyword, Args... others);

  std::optional<ASTNode> parse_global_decl();

  ASTNode ast;
};

} // namespace mt

#include <mutiny/translation_unit/parser/parser.ipp>
#include <mutiny/translation_unit/parser/parser.tpp>
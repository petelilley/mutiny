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
  Parser(InputFile& src_file, Status& status);
  ~Parser();

  void exec(const std::vector<Token>* tokens);

  inline ASTNode& get_ast();

private:
  InputFile& src_file;
  Status& status;

  const std::vector<Token>* tokens = nullptr;
  std::vector<Token>::const_iterator tok_iter;

  inline Token::Kind comp_token(Token::Kind kind);
  inline Punct comp_token(Punct punct);
  inline Keyword comp_token(Keyword keyword);

  inline Token::Kind peek_comp_token(Token::Kind kind, u32 offset = 1);
  inline Punct peek_comp_token(Punct punct, u32 offset = 1);
  inline Keyword peek_comp_token(Keyword keyword, u32 offset = 1);

  template<typename... Args>
  Token::Kind comp_token(Token::Kind kind, Args... others);
  template<typename... Args>
  Punct comp_token(Punct punct, Args... others);
  template<typename ... Args>
  Keyword comp_token(Keyword keyword, Args... others);

  std::optional<ASTNode> parse_global_decl();
  std::optional<ASTNode> parse_func_decl();
  std::optional<ASTNode> parse_func_decl_param_list();
  std::optional<ASTNode> parse_var_decl();

  std::optional<ASTNode> parse_var_type();

  std::optional<ASTNode> parse_expr();
  std::optional<ASTNode> parse_func_call();
  std::optional<ASTNode> parse_func_call_param_list();

  struct ExprFragment {
    enum class Kind {
      UNIT,
      OPERATOR,
    } kind;

    ASTNode node;
  };

  std::optional<ASTNode> parse_expr_unit();
  std::optional<ASTNode> parse_operator();

  std::optional<ASTNode> fragments_to_expr(std::vector<ExprFragment>& fragments);

  std::optional<ASTNode> parse_stmt_list();
  std::optional<ASTNode> parse_stmt();

  std::string unexpected_token(Token::Kind kind = Token::Kind::UNKNOWN);

  ASTNode ast;
};

} // namespace mt

#include <mutiny/translation_unit/parser/parser.ipp>
#include <mutiny/translation_unit/parser/parser.tpp>

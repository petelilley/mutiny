#include <mutiny/mutiny.hpp>
#include <mutiny/translation_unit/parser/parser.hpp>
#include <mutiny/translation_unit/lexer/token.hpp>

using namespace mt;

Parser::Parser(Status& status)
: status(status), ast(ASTNode::Kind::GLOBAL_SCOPE, {1, 1, 0}) { }

Parser::~Parser() {

}

void Parser::exec(const std::vector<Token>* _tokens) {
  tokens = _tokens;

  current_token = tokens->begin();

  std::optional<ASTNode> node;
  while (current_token.base()->get_kind() != Token::Kind::END_OF_FILE) {
    node = parse_global_decl();
    if (node) {
      ast.add_child(std::move(node.value()));
    }
    else {
      break;
    }
  }
}

Token::Kind Parser::match_token(Token::Kind kind) {
  if (current_token.base()->get_kind() == kind) {
    ++current_token;
    return kind;
  }

  std::cout << "expected token\n";
  return Token::Kind::UNKNOWN;
}

Punct Parser::match_token_punct(Punct punct) {
  auto base = current_token.base();
  if (base->get_kind() == Token::Kind::PUNCTUATOR && base->get_value<Punct>() == punct) {
    ++current_token;
    return punct;
  }

  std::cout << "expected punct\n";
  return Punct::UNKNOWN;
}

Keyword Parser::match_token_keyword(Keyword keyword) {
  auto base = current_token.base();
  if (base->get_kind() == Token::Kind::KEYWORD && base->get_value<Keyword>() == keyword) {
    ++current_token;
    return keyword;
  }

  std::cout << "expected keyword\n";
  return Keyword::UNKNOWN;
}

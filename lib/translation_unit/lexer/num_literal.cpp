#include <mutiny/mutiny.hpp>
#include <mutiny/translation_unit/lexer/lexer.hpp>
#include <mutiny/translation_unit/lexer/token.hpp>
#include <mutiny/util/file.hpp>
#include <mutiny/util/logger.hpp>

using namespace mt;

Token Lexer::tokenize_numeric_literal() {
  SourceLoc loc = { src_file.get_path(), file_iter.line_num(), file_iter.column_num(), 1 };
  
  const c8* first = &*file_iter;

  u64 int_val = 0;
  f128 float_val = 0.0;

  Token::Kind kind = Token::Kind::INT_LITERAL;

  std::string val_str;

  c8 c = *first;
  // Decimal number.
  if (c - '0' > 0 || c == '.' || *(file_iter + 1) == '.') {
    b8 is_float = (c == '.');

    for (c = *++file_iter; c && std::isdigit(c); c = *++file_iter) {
      ++loc.len;
    }
    
    if (!is_float && c == '.') {
      is_float = true;
      ++loc.len;
      for (c = *++file_iter; c && std::isdigit(c); c = *++file_iter) {
        ++loc.len;
      }
    }
    
    val_str.assign(std::string(first, loc.len));
    
    if (is_float) {
      float_val = std::stold(val_str);
      kind = Token::Kind::FLOAT_LITERAL;
    }
    else {
      int_val = std::stoull(val_str);
      kind = Token::Kind::INT_LITERAL;
    }
  }
  // Hexadecimal number.
  else if (*(file_iter + 1) == 'x' || *(file_iter + 1) == 'X') {
    ++file_iter;
    first += 2;

    c = *++file_iter;
    while (c && (std::isdigit(c) || std::isalpha(c))) {
      ++loc.len;

      if ((c > 'f' && c < 'z') || (c > 'F' && c < 'Z')) {
        status.report_syntax(Status::ReportContext::ERROR, src_file, {src_file.get_path(), file_iter.line_num(), file_iter.column_num(), 1}, fmt::format("Invalid suffix 'x{}' on integer literal", c));
      }
      c = *++file_iter;
    }
    
    if (loc.len == 1) {
      status.report_syntax(Status::ReportContext::ERROR, src_file, {src_file.get_path(), file_iter.line_num(), file_iter.column_num() - 1, 1}, "Invalid suffix 'x' on integer literal");
    }
    else {
      val_str.assign(std::string(first, loc.len));
      int_val = std::stoull(val_str, nullptr, 16);
    }
    
    kind = Token::Kind::INT_LITERAL;
  }
  // Octal number.
  else {
    for (c = *++file_iter; c && std::isdigit(c); c = *++file_iter) {
      ++loc.len;

      if (c - '0' > 7) {
        status.report_syntax(Status::ReportContext::ERROR, src_file, {src_file.get_path(), file_iter.line_num(), file_iter.column_num(), 1}, fmt::format("Invalid digit '{}' in octal literal", c));
      }
    }

    val_str.assign(std::string(first, loc.len));
    int_val = std::stoull(val_str, nullptr, 8);
    kind = Token::Kind::INT_LITERAL;
  }

  Token::ValueType val;

  if (kind == Token::Kind::INT_LITERAL) {
    val = int_val;
  }
  else {
    val = float_val;
  }

  return Token(kind, loc, val);
}
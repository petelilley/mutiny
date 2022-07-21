#include <mutiny/mutiny.hpp>
#include <mutiny/src_unit/lexer/lexer.hpp>
#include <mutiny/src_unit/lexer/token.hpp>
#include <mutiny/util/file.hpp>
#include <mutiny/util/logger.hpp>

using namespace mt;

Token Lexer::tokenize_numeric_literal() {
  SourceLoc loc(SourceLoc::at(src_file.get_path(), file_iter.line_num(), file_iter.column_num()));
  u64 len = 1;
  
  // The beginning of the numeric literal.
  const c8* first = &*file_iter;

  u64 int_val = 0;
  f128 float_val = 0.0;

  Token::Kind kind = Token::Kind::INT_LITERAL;

  std::string val_str;

  // The first character of the literal.
  c8 c = *first;

  b8 is_float = false;
  // Decimal number.
  if (c - '0' > 0 || c == '.' || *(file_iter + 1) == '.') {
    is_float = (c == '.');

    // Count the number of characters before a non-numeric character.
    for (c = *++file_iter; c && std::isdigit(c); c = *++file_iter) {
      ++len;
    }
    
    // A decimal point.
    if (!is_float && c == '.') {
      
      is_float = true;
      ++len;
      // Count the number of characters before a non-numeric character.
      for (c = *++file_iter; c && std::isdigit(c); c = *++file_iter) {
        ++len;
      }
    }
    
    // Take the substring containing the numeric literal.
    val_str.assign(first, len);
    
    if (is_float) {
      // Convert the string to a floating point number.
      float_val = std::stold(val_str);
      kind = Token::Kind::FLOAT_LITERAL;
    }
    else {
      // Convert the string to an integer number.
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
      ++len;

      // Check if the alphabetical character is not a valid hexadecimal digit.
      if ((c > 'f' && c < 'z') || (c > 'F' && c < 'Z')) {
        status.report_syntax(Status::ReportContext::ERROR, src_file, SourceLoc::at(src_file.get_path(), file_iter.line_num(), file_iter.column_num()), fmt::format("Invalid suffix 'x{}' on integer literal", c));
      }
      c = *++file_iter;
    }
    
    // Check if the hexadecimal number is empty.
    if (len == 1) {
      status.report_syntax(Status::ReportContext::ERROR, src_file, SourceLoc::at(src_file.get_path(), file_iter.line_num(), file_iter.column_num() - 1), "Invalid suffix 'x' on integer literal");
    }
    else {
      // Take the substring containing the numeric literal.
      val_str.assign(first, len);

      // Convert the string to an integer number with base 16.
      int_val = std::stoull(val_str, nullptr, 16);
    }
    
    kind = Token::Kind::INT_LITERAL;
  }
  // Octal number.
  else {
    for (c = *++file_iter; c && std::isdigit(c); c = *++file_iter) {
      ++len;

      // Check if the number is not a valid octal number.
      if (c - '0' > 7) {
        status.report_syntax(Status::ReportContext::ERROR, src_file, SourceLoc::at(src_file.get_path(), file_iter.line_num(), file_iter.column_num()), fmt::format("Invalid digit '{}' in octal literal", c));
      }
    }

    // Take the substring containing the numeric literal.
    val_str.assign(first, len);

    // Convert the string to an integer number with base 8.
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

  loc.col_f = loc.col_i + len - 1;

  return Token(kind, loc, val);
}

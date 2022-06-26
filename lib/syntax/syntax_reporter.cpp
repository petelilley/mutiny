#include <mutiny/mutiny.hpp>
#include <mutiny/syntax/syntax_reporter.hpp>

using namespace mt;

void SyntaxReporter::report_syntax(SyntaxReporter::Context ctx, Logger& log, InputFile& file, SourceLoc loc, std::string_view msg, std::string_view corr) {
  const char* title = "";
  LogStyle color = LogStyle::CLEAR;
  switch (ctx) {
    case SyntaxReporter::Context::NONE:
      break;
    case SyntaxReporter::Context::NOTE:
      title = "Note: ";
      color = LogStyle::CYAN;
      break;
    case SyntaxReporter::Context::WARNING:
      title = "Warning: ";
      color = LogStyle::YELLOW;
      break;
    case SyntaxReporter::Context::ERROR:
      title = "Error: ";
      color = LogStyle::RED;
      break;
  }
  log << color << LogStyle::BOLD << file.get_path() << ':' << loc.line << ',' << loc.col << ": " << LogStyle::CLEAR << LogStyle::BOLD << title << msg << LogStyle::CLEAR << "\n";

  if (!loc.len) return;

  // Extract the line from the file.
  std::string line_str = file.get_line(loc.line);

  if (line_str.empty()) return;

  // Skip leading whitespace.
  u64 start = line_str.find_first_not_of(' ');
  if (start == std::string::npos) start = 0;

  // Adjust the length of the token to fit the line.
  if (loc.col + loc.len > line_str.length()) {
    loc.len = line_str.length() - loc.col + 1;
  }

  u32 padding = 6;

  const std::string line_num_str = std::to_string(loc.line);

  // Adjust the padding to fit the line number.
  if (line_num_str.length() > padding) {
    padding = line_num_str.length() + 1;
  }

  // Display the line of the code where the error occurred.
  log << std::string(padding - line_num_str.length() - 1, ' ') << line_num_str << " |   " << &line_str.at(start) << '\n';

  const std::string line_start = std::string(padding, ' ') + '|' + std::string(2 + loc.col - start, ' ');

  // Show an arrow pointing to the beginning of the error text.
  log << line_start << LogStyle::BOLD << LogStyle::GREEN << '^';

  // Show the length of the error text.
  if (loc.len - 1 > 0) {
    log << std::string(loc.len - 1, '~');
  }
  log << LogStyle::CLEAR << '\n';

  if (corr.empty()) return;

  // Show a line to the correction.
  log << line_start << LogStyle::BOLD << LogStyle::GREEN << "|\n" << LogStyle::CLEAR;
  // Show the correction text.
  log << line_start << LogStyle::BOLD << LogStyle::GREEN << corr << LogStyle::CLEAR << '\n';
}
#include <mutiny/mutiny.hpp>
#include <mutiny/basic/status.hpp>

using namespace mt;

Status::Status(b8 warning_as_error)
: log_out(Logger::Stream::STDOUT, Logger::OutputMode::MANUAL),
  log_err(Logger::Stream::STDERR, Logger::OutputMode::MANUAL),
  warn_as_err(warning_as_error) { }

Status::~Status() = default;

Status::Status(Status&& other)
: log_out(std::move(other.log_out)), log_err(std::move(other.log_err)), warn_as_err(other.warn_as_err) { }

void Status::report(ReportContext ctx, InputFile& file, std::string_view msg) {
  const char* title = "";
  LogStyle color = LogStyle::CLEAR;
  Logger* log;
  switch (ctx) {
    case ReportContext::NONE:
      log = &log_out;
      break;
    case ReportContext::NOTE:
      title = "Note: ";
      color = LogStyle::CYAN;
      log = &log_out;
      break;
    case ReportContext::WARNING:
      title = "Warning: ";
      color = LogStyle::YELLOW;
      log = &log_err;
      warn_num++;
      if (!warn_as_err) break;
      // fall through
    case ReportContext::ERROR:
      title = "Error: ";
      color = LogStyle::RED;
      log = &log_err;
      err_num++;
      break;
  }
  *log << color << LogStyle::BOLD << file.get_path_str() << ": " << LogStyle::CLEAR << LogStyle::BOLD << title << msg << LogStyle::CLEAR << "\n";
}

void Status::report_syntax(ReportContext ctx, InputFile& file, SourceLoc loc, std::string_view msg, std::string_view note) {
  const char* title = "";
  LogStyle color = LogStyle::CLEAR;
  Logger* log;
  switch (ctx) {
    case ReportContext::NONE:
      log = &log_out;
      break;
    case ReportContext::NOTE:
      title = "Note: ";
      color = LogStyle::CYAN;
      log = &log_out;
      break;
    case ReportContext::WARNING:
      title = "Warning: ";
      color = LogStyle::YELLOW;
      log = &log_err;
      warn_num++;
      if (!warn_as_err) break;
      // fall through
    case ReportContext::ERROR:
      title = "Error: ";
      color = LogStyle::RED;
      log = &log_err;
      err_num++;
      break;
  }
  *log << color << LogStyle::BOLD << file.get_path_str() << ':' << loc.line_i << ',' << loc.col_i << ": " << LogStyle::CLEAR << LogStyle::BOLD << title << msg << LogStyle::CLEAR << "\n";

  // if (!loc.len) return;
  if (loc.line_i != loc.line_f) return;

  // Extract the line from the file.
  std::string line_str = file.get_line(loc.line_i);

  if (line_str.empty()) return;

  // Skip leading whitespace.
  u64 start = line_str.find_first_not_of(' ');
  if (start == std::string::npos) start = 0;

  if (loc.col_i > loc.col_f) {
    loc.col_i = loc.col_f;
  }

  u32 padding = 6;

  const std::string line_num_str = std::to_string(loc.line_i);

  // Adjust the padding to fit the line number.
  if (line_num_str.length() > padding) {
    padding = line_num_str.length() + 1;
  }

  // Display the line of the code where the error occurred.
  *log << std::string(padding - line_num_str.length() - 1, ' ') << line_num_str << " |   " << &line_str.at(start) << '\n';

  const std::string line_start = std::string(padding, ' ') + '|' + std::string(2 + loc.col_i - start, ' ');

  // Show an arrow pointing to the beginning of the error text.
  *log << line_start << LogStyle::BOLD << LogStyle::GREEN << '^';

  // Show the length of the error text.
  if (loc.col_f != loc.col_i) {
    *log << std::string(loc.col_f - loc.col_i, '~');
  }
  *log << LogStyle::CLEAR << '\n';

  if (note.empty()) return;

  // Show a line to the note.
  *log << line_start << LogStyle::BOLD << LogStyle::GREEN << "|\n" << LogStyle::CLEAR;
  // Show the note text.
  *log << line_start << LogStyle::BOLD << LogStyle::GREEN << note << LogStyle::CLEAR << '\n';
}

void Status::dump_logs() const {
  log_out.dump_buffer();
  log_err.dump_buffer();
}
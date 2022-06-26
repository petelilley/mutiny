#pragma once

#include <mutiny/mutiny.hpp>
#include <mutiny/util/logger.hpp>
#include <mutiny/util/file.hpp>
#include <mutiny/basic/source_loc.hpp>

namespace mt {

class Status {
public:
  Status(b8 warning_as_error);
  ~Status();
  Status(Status&&);
  Status(const Status&) = delete;
  
  enum class ReportContext {
    NONE,
    NOTE,
    WARNING,
    ERROR,
  };
  
  /**
   * @brief Reports a message concerning a file.
   * 
   * @param context The context of the message.
   * @param file The file the message is concerning.
   * @param message The message.
   */
  void report(ReportContext context, InputFile& file, std::string_view message);

  /**
   * @brief Reports a message concerning specific syntax in a file.
   * 
   * @param context The context of the message.
   * @param file The file the message is concerning.
   * @param location The location of the syntax in the file.
   * @param message The message.
   * @param note An optional note about the syntax.
   */
  void report_syntax(ReportContext context, InputFile& file, SourceLoc location, std::string_view message, std::string_view note = "");

  /**
   * @brief Dumps all the message logs to the console.
   */
  void dump_logs() const;

  /**
   * @brief Returns the number of errors encountered.
   * 
   * @return The number of errors encountered.
   */
  constexpr u32 get_error_num() const { return err_num; }

  /**
   * @brief Returns the number of warnings encountered.
   * 
   * @return The number of warnings encountered.
   */
  constexpr u32 get_warning_num() const { return warn_num; }

private:
  mutable Logger log_out,
                 log_err;

  u32 err_num = 0;
  u32 warn_num = 0;
  
  b8 warn_as_err = false;
};

} // namespace mt
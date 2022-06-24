#pragma once

#include <mutiny/mutiny.hpp>

namespace mt {

class InputFile {
public:
  explicit InputFile(std::filesystem::path);
  ~InputFile();

  /**
   * @brief Returns whether the file is open.
   * 
   * @return True if the file opened successfully, false otherwise.
   */
  constexpr bool is_open() const noexcept { return open; }

  /**
   * @brief Returns the path to the file.
   * 
   * @return The path to the file.
   */
  inline std::filesystem::path get_path() const noexcept { return path; }

  /**
   * @brief Returns whether the file is open.
   * 
   * @return True if the file opened successfully, false otherwise.
   */
  constexpr explicit operator bool() const noexcept { return is_open(); }
  
  /**
   * @brief Get the current line number.
   * 
   * @return The current line number.
   */
  constexpr u64 get_line_num() const noexcept { return line; }

  /**
   * @brief Get the current column number.
   * 
   * @return The current column number.
   */
  constexpr u64 get_column_num() const noexcept { return column; }

  /**
   * @brief Extracts a single character from the file.
   * 
   * @return The character extracted.
   */
  char next() noexcept;
  
  /**
   * @brief Extracts n characters from the file.
   * 
   * @param n The number of characters to extract.
   * @return The final character extracted.
   */
  char next(u64 n) noexcept;

  /**
   * @brief Returns the next character in the file without extracting it.
   * 
   * @return The next character in the file.
   */
  char peek_next() noexcept;
  
  /**
   * @brief Returns the next nth character in the file without extracting it.
   * 
   * @param n The number of characters to peek ahead in the file.
   * @return The next nth character in the file.
   */
  char peek_next(u64 n) noexcept;

  /**
   * @brief Returns the current character in the file.
   * 
   * @return The current character in the file.
   */
  inline char current() const noexcept { return is_open() ? content.at(pos) : 0; }

  /**
   * @brief Returns the previous character in the file without rewinding it.
   * 
   * @return The previous character in the file.
   */
  char peek_prev() noexcept;

  /**
   * @brief Returns the previous nth character in the file without rewinding it.
   * 
   * @param n The number of characters to peek back in the file.
   * @return The previous nth character in the file.
   */
  char peek_prev(u64 n) noexcept;
  
  /**
   * @brief Rewinds the file by one character.
   * 
   * @return The previous character in the file.
   */
  char prev() noexcept;

  /**
   * @brief Rewinds the file by n characters.
   * 
   * @param n The number of characters to rewind the file.
   * @return The previous nth character in the file.
   */
  char prev(u64 n) noexcept;

  /**
   * @brief Extracts a single character from the file.
   * 
   * @return The character extracted.
   */
  inline char operator++() noexcept { return is_open() ? next() : 0; }

  /**
   * @brief Rewinds the file by one character.
   * 
   * @return The previous character in the file.
   */
  inline char operator--() noexcept { return is_open() ? prev() : 0; }

  /**
   * @brief Extracts a single character from the file.
   * 
   * @return The current character.
   */
  inline char operator++(s32) noexcept { operator++(); return is_open() ? peek_prev() : 0; }

  /**
   * @brief Rewinds the file by one character.
   * 
   * @return The current character.
   */
  inline char operator--(s32) noexcept { operator--(); return is_open() ? peek_next() : 0; }

  /**
   * @brief Returns the next character in the file without extracting it.
   * 
   * @return The next character in the file.
   */
  inline explicit operator char() noexcept { return is_open() ? current() : 0; }

  /**
   * @brief Returns the next nth character in the file without extracting it.
   * 
   * @param n The number of characters to peek ahead in the file.
   * @return The next nth character in the file.
   */
  inline char operator+(u64 n) noexcept { return is_open() ? peek_next(n) : 0; }
  
  /**
   * @brief Returns the previous nth character in the file without rewinding it.
   * 
   * @param n The number of characters to peek back in the file.
   * @return The previous nth character in the file.
   */
  inline char operator-(u64 n) noexcept { return is_open() ? peek_prev(n) : 0; }

  /**
   * @brief Extracts n characters from the file.
   * 
   * @param n The number of characters to extract.
   * @return The final character extracted.
   */
  inline char operator+=(u64 n) noexcept { return is_open() ? next(n) : 0; }

  /**
   * @brief Rewinds the file by n characters.
   * 
   * @param n The number of characters to rewind the file.
   * @return The previous nth character in the file.
   */
  inline char operator-=(u64 n) noexcept { return is_open() ? prev(n) : 0; }

  /**
   * @brief Finds a line in the file and returns it.
   * 
   * @param line The index of the line to find.
   * @return The line as a string.
   */
  std::string get_line(u64 line) noexcept;
  
private:
  std::filesystem::path path;
  
  u64 line = 0,
      column = 0;
  
  bool open = false;
  
  std::string content = "";

  u64 pos = 0;

  std::vector<u64> line_locations;
};

} // namespace mt
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
  constexpr b8 is_open() const { return open; }

  /**
   * @brief Returns the path to the file.
   * 
   * @return The path to the file.
   */
  inline const char* get_path() const { return path.c_str(); }

  /**
   * @brief Returns whether the file is open.
   * 
   * @return True if the file opened successfully, false otherwise.
   */
  constexpr explicit operator b8() const { return is_open(); }
  
  /**
   * @brief Get the current line number.
   * 
   * @return The current line number.
   */
  constexpr u64 get_line_num() const { return line; }

  /**
   * @brief Get the current column number.
   * 
   * @return The current column number.
   */
  constexpr u64 get_column_num() const { return column; }

  /**
   * @brief Extracts a single character from the file.
   * 
   * @return The character extracted.
   */
  char next();
  
  /**
   * @brief Extracts n characters from the file.
   * 
   * @param n The number of characters to extract.
   * @return The final character extracted.
   */
  char next(u64 n);

  /**
   * @brief Returns the next character in the file without extracting it.
   * 
   * @return The next character in the file.
   */
  char peek_next();
  
  /**
   * @brief Returns the next nth character in the file without extracting it.
   * 
   * @param n The number of characters to peek ahead in the file.
   * @return The next nth character in the file.
   */
  char peek_next(u64 n);

  /**
   * @brief Returns the current character in the file.
   * 
   * @return The current character in the file.
   */
  inline char current() const { return (is_open() && pos < content.length()) ? content.at(pos) : 0; }

  /**
   * @brief Returns the previous character in the file without rewinding it.
   * 
   * @return The previous character in the file.
   */
  char peek_prev();

  /**
   * @brief Returns the previous nth character in the file without rewinding it.
   * 
   * @param n The number of characters to peek back in the file.
   * @return The previous nth character in the file.
   */
  char peek_prev(u64 n);
  
  /**
   * @brief Rewinds the file by one character.
   * 
   * @return The previous character in the file.
   */
  char prev();

  /**
   * @brief Rewinds the file by n characters.
   * 
   * @param n The number of characters to rewind the file.
   * @return The previous nth character in the file.
   */
  char prev(u64 n);

  /**
   * @brief Checks whether the next set of characters in the file match the given string.
   * 
   * @param str The string to check.
   * @return True if the next set of characters in the file match the given string, false otherwise.
   */
  b8 starts_with(std::string_view str) const;

  /**
   * @brief Extracts a single character from the file.
   * 
   * @return The character extracted.
   */
  inline char operator++() { return is_open() ? next() : 0; }

  /**
   * @brief Rewinds the file by one character.
   * 
   * @return The previous character in the file.
   */
  inline char operator--() { return is_open() ? prev() : 0; }

  /**
   * @brief Extracts a single character from the file.
   * 
   * @return The current character.
   */
  inline char operator++(s32) { operator++(); return is_open() ? peek_prev() : 0; }

  /**
   * @brief Rewinds the file by one character.
   * 
   * @return The current character.
   */
  inline char operator--(s32) { operator--(); return is_open() ? peek_next() : 0; }

  /**
   * @brief Returns the next character in the file without extracting it.
   * 
   * @return The next character in the file.
   */
  inline explicit operator char() { return is_open() ? current() : 0; }

  /**
   * @brief Returns the next nth character in the file without extracting it.
   * 
   * @param n The number of characters to peek ahead in the file.
   * @return The next nth character in the file.
   */
  inline char operator+(u64 n) { return is_open() ? peek_next(n) : 0; }
  
  /**
   * @brief Returns the previous nth character in the file without rewinding it.
   * 
   * @param n The number of characters to peek back in the file.
   * @return The previous nth character in the file.
   */
  inline char operator-(u64 n) { return is_open() ? peek_prev(n) : 0; }

  /**
   * @brief Extracts n characters from the file.
   * 
   * @param n The number of characters to extract.
   * @return The final character extracted.
   */
  inline char operator+=(u64 n) { return is_open() ? next(n) : 0; }

  /**
   * @brief Rewinds the file by n characters.
   * 
   * @param n The number of characters to rewind the file.
   * @return The previous nth character in the file.
   */
  inline char operator-=(u64 n) { return is_open() ? prev(n) : 0; }

  /**
   * @brief Finds a line in the file and returns it.
   * 
   * @param line The index of the line to find.
   * @return The line as a string.
   */
  std::string get_line(u64 line);
  
private:
  std::filesystem::path path;
  
  u64 line = 1,
      column = 1;
  
  b8 open = false;
  
  std::string content = "";

  u64 pos = 0;

  std::vector<u64> line_locations;
};

} // namespace mt
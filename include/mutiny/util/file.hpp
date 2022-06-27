#pragma once

#include <mutiny/mutiny.hpp>

namespace mt {

class InputFile {
public:
  explicit InputFile(std::filesystem::path);
  ~InputFile();

  static inline const c8 npos = 0;

  /**
   * @brief Returns whether the file is open.
   * 
   * @return True if the file opened successfully, false otherwise.
   */
  constexpr b8 is_open() const;

  /**
   * @brief Returns the path to the file.
   * 
   * @return The path to the file.
   */
  inline const c8* get_path() const;

  /**
   * @brief Returns whether the file is open.
   * 
   * @return True if the file opened successfully, false otherwise.
   */
  constexpr explicit operator b8() const;
  
  /**
   * @brief Get the current line number.
   * 
   * @return The current line number.
   */
  constexpr u64 get_line_num() const;

  /**
   * @brief Get the current column number.
   * 
   * @return The current column number.
   */
  constexpr u64 get_column_num() const;

  /**
   * @brief Extracts a single character from the file.
   * 
   * @return The character extracted.
   */
  const c8& next();
  
  /**
   * @brief Extracts n characters from the file.
   * 
   * @param n The number of characters to extract.
   * @return The final character extracted.
   */
  const c8& next(u64 n);

  /**
   * @brief Returns the next character in the file without extracting it.
   * 
   * @return The next character in the file.
   */
  const c8& peek_next();
  
  /**
   * @brief Returns the next nth character in the file without extracting it.
   * 
   * @param n The number of characters to peek ahead in the file.
   * @return The next nth character in the file.
   */
  const c8& peek_next(u64 n);

  /**
   * @brief Returns the current character in the file.
   * 
   * @return The current character in the file.
   */
  inline const c8& current() const;

  /**
   * @brief Returns the previous character in the file without rewinding it.
   * 
   * @return The previous character in the file.
   */
  const c8& peek_prev();

  /**
   * @brief Returns the previous nth character in the file without rewinding it.
   * 
   * @param n The number of characters to peek back in the file.
   * @return The previous nth character in the file.
   */
  const c8& peek_prev(u64 n);
  
  /**
   * @brief Rewinds the file by one character.
   * 
   * @return The previous character in the file.
   */
  const c8& prev();

  /**
   * @brief Rewinds the file by n characters.
   * 
   * @param n The number of characters to rewind the file.
   * @return The previous nth character in the file.
   */
  const c8& prev(u64 n);

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
  inline const c8& operator++();

  /**
   * @brief Rewinds the file by one character.
   * 
   * @return The previous character in the file.
   */
  inline const c8& operator--();

  /**
   * @brief Extracts a single character from the file.
   * 
   * @return The current character.
   */
  inline const c8& operator++(s32);

  /**
   * @brief Rewinds the file by one character.
   * 
   * @return The current character.
   */
  inline const c8& operator--(s32);

  /**
   * @brief Returns the next nth character in the file without extracting it.
   * 
   * @param n The number of characters to peek ahead in the file.
   * @return The next nth character in the file.
   */
  inline const c8& operator+(u64 n);
  
  /**
   * @brief Returns the previous nth character in the file without rewinding it.
   * 
   * @param n The number of characters to peek back in the file.
   * @return The previous nth character in the file.
   */
  inline const c8& operator-(u64 n);

  /**
   * @brief Extracts n characters from the file.
   * 
   * @param n The number of characters to extract.
   * @return The final character extracted.
   */
  inline const c8& operator+=(u64 n);

  /**
   * @brief Rewinds the file by n characters.
   * 
   * @param n The number of characters to rewind the file.
   * @return The previous nth character in the file.
   */
  inline const c8& operator-=(u64 n);

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

#include <mutiny/util/file.ipp>
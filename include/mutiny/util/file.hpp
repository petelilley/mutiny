#pragma once

#include <mutiny/mutiny.hpp>

namespace mt {

class InputFile {
public:
  explicit InputFile(std::filesystem::path path);
  ~InputFile();

  /**
   * @brief Returns whether the file is open.
   * 
   * @return True if the file opened successfully, false otherwise.
   */
  constexpr b8 is_open() const;

  /**
   * @brief Returns whether the file is open.
   * 
   * @return True if the file opened successfully, false otherwise.
   */
  constexpr explicit operator bool() const;

  /**
   * @brief Returns the path to the file.
   * 
   * @return The path to the file.
   */
  inline std::filesystem::path get_path() const;

  /**
   * @brief Returns the path to the file as a string.
   * 
   * @return The path to the file as a string.
   */
  inline const c8* get_path_str() const;

  /**
   * @brief Represents an iterator to the file.
   */
  class const_iterator {
  public:
    const_iterator(const InputFile& file);
    const_iterator(const InputFile& file, std::string::const_iterator iter);
    const_iterator(const const_iterator& other);
    inline const_iterator& operator++();
    inline const_iterator operator++(int);
    inline const_iterator& operator--();
    inline const_iterator operator--(int);
    inline const_iterator operator+(u64 n);
    inline const_iterator operator-(u64 n);
    inline const_iterator operator+=(u64 n);
    inline const_iterator operator-=(u64 n);
    inline b8 operator==(const_iterator other);
    inline b8 operator!=(const_iterator other);
    inline const c8& operator*();

    /**
     * @brief Checks whether the next set of characters in the file match the given string.
     * 
     * @param str The string to check.
     * @return True if the next set of characters in the file match the given string, false otherwise.
     */
    b8 starts_with(std::string_view str);

    /**
     * @brief Returns the current line number of the iterator.
     * 
     * @return The current line number of the iterator.
     */
    constexpr u64 line_num() const;

    /**
     * @brief Returns the current column number of the iterator.
     * 
     * @return The current column number of the iterator.
     */
    constexpr u64 column_num() const;
  
  private:
    const InputFile& file;
    std::string::const_iterator iter;
    
    u64 line = 1,
        column = 1;
  };

  /**
   * @brief Returns an iterator to the beginning of the file.
   * 
   * @return An iterator to the beginning of the file.
   */
  inline const_iterator cbegin();

  /**
   * @brief Returns an iterator to the end of the file.
   * 
   * @return An iterator to the end of the file.
   */
  inline const_iterator cend();

  /**
   * @brief Finds a line in the file and returns it.
   * 
   * @param line The index of the line to find.
   * @return The line as a string.
   */
  std::string get_line(u64 line);

private:
  b8 open = false;
  std::filesystem::path path;
  std::string content;
  std::vector<u64> line_locations;
};

} // namespace mt

#include <mutiny/util/file.ipp>

#include <mutiny/mutiny.hpp>
#include <mutiny/util/file.hpp>

using namespace mt;

using ifstream_it = std::istreambuf_iterator<char>;

InputFile::InputFile(std::filesystem::path _path)
: path(_path), line_locations({0}) {
  
  // Make sure it's not a directory.
  if (std::filesystem::is_directory(path)) return;
  
  // Open the file.
  std::ifstream f(path);
  if (!f) return;

  open = true;

  content.assign(ifstream_it(f), ifstream_it());
  
  for (u64 i = 0; i < content.size(); i++) {
    if (content.at(i) == '\n') {
      // Replace DOS line endings with Unix line endings.
      if (content.at(i - 1) == '\r') {
        content.erase(--i, 1);
      }
      
      // Record the line position.
      if (i + 1 < content.size()) {
        line_locations.push_back(i + 1);
      }
    }
    if (i + 1 == content.size()) {
      line_locations.push_back(i + 1);
    }
  }
}

InputFile::~InputFile() = default;

char InputFile::next() {
  if (!is_open()) return 0;
  
  if (pos >= content.size()) return 0;

  if (content.at(pos) == '\n') {
    line++;
    column = 1;
  }
  else {
    column++;
  }
  pos++;

  if (pos >= content.size()) return 0;
  
  return content.at(pos);
}

char InputFile::next(u64 n) {
  if (!is_open()) return 0;
  
  char c = 0;
  for (u64 i = 0; i < n; i++) {
    c = next();
  }
  return c;
}

char InputFile::peek_next() {
  if (!is_open()) return 0;
  if (pos + 1 >= content.size()) return 0;
  return content.at(pos + 1);
}

char InputFile::peek_next(u64 n) {
  if (!is_open()) return 0;
  if (pos + n >= content.length()) return 0;
  return content.at(pos + n);
}

char InputFile::peek_prev() {
  if (!is_open()) return 0;
  return content.at(pos - 1);
}

char InputFile::peek_prev(u64 n) {
  if (!is_open()) return 0;
  return content.at(pos - n);
}

char InputFile::prev() {
  if (!is_open()) return 0;

  if (!pos) return content.at(pos);
  
  if (column == 1) {
    line--;
    column = line_locations.at(line + 1) - line_locations.at(line);
  }
  else {
    column--;
  }
  pos--;
  
  return content.at(pos);
}

char InputFile::prev(u64 n) {
  if (!is_open()) return 0;
  
  char c = 0;
  for (u64 i = 0; i < n; i++) {
    c = prev();
  }
  return c;
}

b8 InputFile::starts_with(std::string_view str) const {
  return std::strncmp(str.data(), &content.at(pos), str.length()) == 0;
}

std::string InputFile::get_line(u64 line) {
  if (!is_open() || line >= line_locations.size()) return "";
  
  u64 start = line_locations.at(line - 1);
  u64 end = line_locations.at(line) - 1;

  return content.substr(start, end - start);
}
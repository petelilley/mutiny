#include <mutiny/mutiny.hpp>
#include <mutiny/util/file.hpp>

using namespace mt;

using ifstream_it = std::istreambuf_iterator<c8>;

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

const c8& InputFile::next() {
  if (!is_open()) return npos;
  
  if (pos >= content.size()) return npos;

  if (content.at(pos) == '\n') {
    line++;
    column = 1;
  }
  else {
    column++;
  }
  pos++;

  if (pos >= content.size()) return npos;
  
  return content.at(pos);
}

const c8& InputFile::next(u64 n) {
  if (!is_open()) return npos;
  
  const c8* c = &npos;
  for (u64 i = 0; i < n; i++) {
    c = &next();
  }
  return *c;
}

const c8& InputFile::peek_next() {
  if (!is_open()) return npos;
  if (pos + 1 >= content.size()) return npos;
  return content.at(pos + 1);
}

const c8& InputFile::peek_next(u64 n) {
  if (!is_open()) return npos;
  if (pos + n >= content.length()) return npos;
  return content.at(pos + n);
}

const c8& InputFile::peek_prev() {
  if (!is_open()) return npos;
  return content.at(pos - 1);
}

const c8& InputFile::peek_prev(u64 n) {
  if (!is_open()) return npos;
  return content.at(pos - n);
}

const c8& InputFile::prev() {
  if (!is_open()) return npos;

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

const c8& InputFile::prev(u64 n) {
  if (!is_open()) return npos;
  
  const c8* c = &npos;
  for (u64 i = 0; i < n; i++) {
    c = &prev();
  }
  return *c;
}

b8 InputFile::starts_with(std::string_view str) const {
  return std::strncmp(str.data(), &content.at(pos), str.length()) == 0;
}

std::string InputFile::get_line(u64 line) {
  if (!is_open() || line >= line_locations.size()) return "";
  
  u64 start = line_locations.at(line - 1);
  u64 end = line_locations.at(line);
  
  if (line != line_locations.size() - 1) {
    end--;
  }

  return content.substr(start, end - start);
}
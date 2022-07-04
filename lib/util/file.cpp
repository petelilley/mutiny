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

InputFile::const_iterator::const_iterator(const InputFile& file)
: file(file), iter(file.content.cbegin()) { }

InputFile::const_iterator::const_iterator(const InputFile& file, std::string::const_iterator iter)
: file(file), iter(iter) { 
  for (const u64& pos : file.line_locations) {
    if (iter >= file.content.cbegin() + pos) {
      line = file.line_locations.at(file.line_locations.size() - 1);
      column = iter - file.content.cbegin() - pos + 1;
      break;
    }
  }
}

InputFile::const_iterator::const_iterator(const const_iterator& other)
: file(other.file), iter(other.iter), line(other.line), column(other.column) { }

b8 InputFile::const_iterator::starts_with(std::string_view str) {
  return std::strncmp(str.data(), &**this, str.length()) == 0;
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

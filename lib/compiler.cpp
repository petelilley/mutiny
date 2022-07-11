#include <mutiny/mutiny.hpp>
#include <mutiny/compiler.hpp>
#include <mutiny/util/logger.hpp>
#include <mutiny/util/file.hpp>
#include <mutiny/translation_unit/translation_unit.hpp>
#include <mutiny/codegen/codegen.hpp>

using namespace mt;

Compiler::Compiler()
: log_out(Logger::Stream::STDOUT, Logger::OutputMode::AUTO),
  log_err(Logger::Stream::STDERR, Logger::OutputMode::AUTO),
  log_warn(Logger::Stream::STDERR, Logger::OutputMode::AUTO) { }

Compiler::~Compiler() = default;

void Compiler::setup(u32 argc, const char** argv) {
  std::vector<const char*> args(argv, argv + argc);
  decode_args(args);
  
  if (should_exit) return;
}

s32 Compiler::exec() {
  std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now(), end;
  
  do {
    if (should_exit) break;
    
    std::list<TranslationUnit> translation_units;
    std::vector<std::thread> translation_unit_threads;

    for (const std::filesystem::path& path : src_paths) {
      // Check if file is directory. Might do something with directories in the future, but right now it's an error.
      if (std::filesystem::is_directory(path)) {
        log_err << LogStyle::RED << LogStyle::BOLD << path.c_str() << ": " << LogStyle::CLEAR << LogStyle::BOLD << "Path is a directory.\n";
        should_exit = true;
        exit_code = ExitCode::INVALID_FILE;
        break;
      }
      // Check if file exists.
      if (!std::filesystem::is_regular_file(path)) {
        log_err << LogStyle::RED << LogStyle::BOLD << path.c_str() << ": " << LogStyle::CLEAR << LogStyle::BOLD << "Path does not exist.\n";
        should_exit = true;
        exit_code = ExitCode::INVALID_FILE;
        break;
      }

      // Translation units must be stored in a std::list instead of a std::vector because std::vectors may reallocate memory for the entire vector when adding new elements, making whatever elements that have been passed to threads become invalidated.
      translation_units.emplace_back(path, error_warnings);
      
      // Start a thread for the translation unit.
      translation_unit_threads.emplace_back([](TranslationUnit* tu) {
        tu->exec_lexer();
        if (tu->get_result() != TranslationUnit::Result::SUCCESS) return;

        tu->exec_parser();
        if (tu->get_result() != TranslationUnit::Result::SUCCESS) return;
      }, &translation_units.back());
    }

    // Join all the threads.
    for (std::thread& thread : translation_unit_threads) {
      thread.join();
    }
    
    if (should_exit) break;

    for (TranslationUnit& tu : translation_units) {
      // Show all the messages from the translation units.
      tu.dump_logs();
      
      switch (tu.get_result()) {
        case TranslationUnit::Result::SUCCESS:
        case TranslationUnit::Result::EMPTY_FILE:
          if (!error_warnings) break;
          // fall through
        case TranslationUnit::Result::INVALID_TOKENS:
          log_err << LogStyle::BOLD << LogStyle::RED << "Lexical analysis failed. Exiting now.\n" << LogStyle::CLEAR;
          should_exit = true;
          exit_code = ExitCode::INVALID_TOKENS;
          break;
        case TranslationUnit::Result::INVALID_SYNTAX:
          log_err << LogStyle::BOLD << LogStyle::RED << "Syntax analysis failed. Exiting now.\n" << LogStyle::CLEAR;
          should_exit = true;
          exit_code = ExitCode::INVALID_SYNTAX;
          break;
      }
    }

    if (should_exit) break;

    std::vector<ASTNode> ast_nodes;
    for (TranslationUnit& tu : translation_units) {
      std::optional<ASTNode> nd = tu.get_ast();
      if (nd) {
        ast_nodes.push_back(std::move(nd.value()));
      }
    }

    if (ast_nodes.empty()) break;

    ASTNode ast(std::move(ast_nodes));

    if (print_ast) {
      for (const ASTNode& nd : ast.get_children()) {
        nd.dump(log_out);
      }
    }

    // TODO: Semantic analysis.

    // End if the task is only to parse the source files.
    if (mode == Mode::PARSE) break;

    // Code generation.
    code_gen.exec(ast, optimize);

    if (mode == Mode::JIT) {
      // TODO: Run the generated code.
    }
    else {
      // TODO: Link the generated code.
    }

  } while (false);

  end = std::chrono::high_resolution_clock::now();

  u32 result = static_cast<u32>(exit_code);

  LogStyle color;
  if (result) color = LogStyle::RED;
  else color = LogStyle::GREEN;

  log_out << LogStyle::BOLD << color << "\nCompiler finished in " << static_cast<f64>(std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count()) / 1e9 << "s with exit code " << result << '\n' << LogStyle::CLEAR;

  return result;
}
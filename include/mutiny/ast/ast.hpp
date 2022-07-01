#pragma once

#include <mutiny/mutiny.hpp>
#include <mutiny/basic/source_loc.hpp>
#include <mutiny/util/logger.hpp>

namespace mt {

class ASTNode {
public:
  enum class Kind {
      GLOBAL_SCOPE = 0,       // Global scope.
      LOCAL_SCOPE,            // Local scope.
      
      FUNC_DECL,              // Function declaration.
      FUNC_DECL_PARAM_LIST,   // Function declaration parameter list.
      FUNC_DECL_PARAM,        // Function declaration parameter.

      TYPE,                   // Type.

      IDENTIFIER,             // Identifier.
  };
  
  // The union of possible AST node value types.
  using ValueType = std::variant<c8, std::string, u64, f128>;

  inline ASTNode(Kind kind, SourceLoc location, ValueType value = static_cast<c8>(0));

  inline ~ASTNode();

  /**
   * @brief Returns the kind of the node.
   * 
   * @return The kind of the node.
   */
  constexpr Kind get_kind() const;

  /**
   * @brief Returns the value of the node.
   * 
   * @tparam T The type of the value to retrieve.
   * @return The value of the node.
   */
  template<typename T>
  constexpr T get_value() const;

  /**
   * @brief Adds a child node to the node.
   * 
   * @param child The child node to add.
   */
  inline void add_child(ASTNode child);

  /**
   * @brief Dumps the node to the logger.
   * 
   * @param indent The indentation level.
   */
  void dump(Logger& log, u32 indent = 0) const;

private:
  Kind kind;
  SourceLoc location;
  ValueType value;

  std::vector<ASTNode> children;
};

} // namespace mt

#include <mutiny/ast/ast.ipp>
#include <mutiny/ast/ast.tpp>

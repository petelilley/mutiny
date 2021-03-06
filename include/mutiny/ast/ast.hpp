#pragma once

#include <mutiny/mutiny.hpp>
#include <mutiny/basic/source_loc.hpp>
#include <mutiny/util/logger.hpp>
#include <mutiny/syntax/operator.hpp>

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

      VAR_DECL,               // Variable declaration.

      EXPR,                   // Expression.
      FUNC_CALL,              // Function call.
      FUNC_CALL_PARAM_LIST,   // Function call parameter list.

      LABEL,                  // Label.
      IF_STMT,                // If statement.
      WHILE_STMT,             // While statement.
      RETURN_STMT,            // Return statement.
      GOTO_STMT,              // Goto statement.

      IDENTIFIER,             // Identifier.
      
      INT_LITERAL,            // Integer literal.
      FLOAT_LITERAL,          // Floating-point literal.
      STRING_LITERAL,         // String literal.
      CHAR_LITERAL,           // Character literal.

      PREFIX_OP,              // Prefix operator.
      SUFFIX_OP,              // Suffix operator.
      ARITH_OP,               // Arithmetic operator.
      ASGN_OP,                // Assignment operator.
      CMP_OP,                 // Comparison operator.
      LOG_OP,                 // Logical operator.
  };
  
  // The union of possible AST node value types.
  using ValueType = std::variant<c8, std::string, Operator, u64, f128>;

  inline ASTNode(Kind kind, SourceLoc location, ValueType value = static_cast<c8>(0));
  inline ASTNode(std::vector<ASTNode>&& children);
  inline ~ASTNode();

  /**
   * @brief Returns the kind of the node.
   * 
   * @return The kind of the node.
   */
  constexpr Kind get_kind() const;

  /**
   * @brief Returns the location of the node in the source file.
   * 
   * @return The location of the node in the source file.
   */
  inline SourceLoc get_location() const;

  /**
   * @brief Returns the value of the node.
   * 
   * @tparam T The type of the value to retrieve.
   * @return The value of the node.
   */
  template<typename T>
  constexpr T get_value() const;

  /**
   * @brief Sets the location of the node in the source file.
   * 
   * @param location The location of the node in the source file.
   */
  inline void set_location(SourceLoc location);

  /**
   * @brief Sets the value of the node.
   * 
   * @tparam T The type of the value to set.
   * @param The value to set.
   */
  template<typename T>
  constexpr void set_value(T value);

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

  /**
   * @brief Returns the children nodes.
   * 
   * @return The children nodes.
   */
  inline std::vector<ASTNode>& get_children();

  /**
   * @brief Returns children nodes that match the specified kind.
   * 
   * @tparam K The kind of nodes search for.
   * @return Children nodes that match the specified kind.
   */
  template<Kind K>
  inline std::vector<ASTNode*> get_children_of_kind();

  /**
   * @brief Returns a child node that matches the specified kind.
   * 
   * @tparam K The kind of node to search for.
   * @param index The preferred index in the list of children of the given kind.
   * @return The child node that matches the specified kind, nullptr if not found.
   */
  template<Kind K>
  inline ASTNode* get_child_of_kind(u32 index = 0);

private:
  Kind kind;
  SourceLoc location;
  ValueType value;

  std::vector<ASTNode> children;
};

} // namespace mt

#include <mutiny/ast/ast.ipp>
#include <mutiny/ast/ast.tpp>

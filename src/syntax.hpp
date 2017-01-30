/**
 * @file	syntax.hpp
 * @author	Chris Vig (chris@invictus.so)
 * @date	2017/01/28
 */

#pragma once

/* -- Includes -- */

#include <array>
#include <memory>

/* -- Types -- */

namespace regex
{

  /**
   * Enumeration of types of syntax nodes.
   */
  enum class syntax_node_type
  {
    literal,
    concatenation,
    alternation,
    optional,
    kleene,
    repeat,
  };

  /**
   * Abstract base class for types representing a node in a syntax tree.
   */
  class syntax_node
  {

    /* -- Public Methods -- */

  public:

    /** Returns the type of this syntax node. */
    virtual regex::syntax_node_type type() const = 0;

  };

  /**
   * Class representing a literal character note in a syntax tree.
   */
  class syntax_literal_node : public syntax_node
  {

    /* -- Lifecycle -- */

  public:

    /** Constructs a new `regex::syntax_literal_node` object for the specified character. */
    syntax_literal_node(char character)
      : m_character(character)
    { }

    /* -- Public Methods -- */

  public:

    /** Returns the type of this syntax node. */
    virtual regex::syntax_node_type type() const override
    {
      return syntax_node_type::literal;
    }

    /** Returns the character that this literal represents. */
    char character() const
    {
      return m_character;
    }

    /* -- Implementation -- */

  private:
    char m_character;
  };

  /**
   * Template for classes representing an internal syntax node.
   */
  template <regex::syntax_node_type NodeType, size_t NumChildren>
  class syntax_internal_node : public syntax_node
  {

    /* -- Types -- */

  public:

    /** Unique pointer to a child node. */
    using child_ptr = std::unique_ptr<const regex::syntax_node>;

    /** Array of children. */
    using child_array = std::array<const child_ptr, NumChildren>;

    /* -- Lifecycle -- */

  public:

    /** Constructs a new `regex::syntax_internal_node` with the specified children. */
    syntax_internal_node(child_array children)
      : m_children(std::move(children))
    { }

    /* -- Public Methods -- */

  public:

    /** Returns the type of this syntax node. */
    virtual regex::syntax_node_type type() const override
    {
      return NodeType;
    }

    /** Returns the child nodes of this syntax node. */
    const child_array& children() const
    {
      return m_children;
    }

    /* -- Implementation -- */

  private:

    child_array m_children;

  };

  /** Class for a syntax node representing a concatenation between two subexpressions. */
  using syntax_concatenation_node = regex::syntax_internal_node<regex::syntax_node_type::concatenation, 2>;

  /** Class for a syntax node representing a union between two subexpressions. */
  using syntax_alternation_node = regex::syntax_internal_node<regex::syntax_node_type::alternation, 2>;

  /** Class representing an optional closure over a subexpression. */
  using syntax_optional_node = regex::syntax_internal_node<regex::syntax_node_type::optional, 1>;

  /** Class representing a Kleene closure over a subexpression. */
  using syntax_kleene_node = regex::syntax_internal_node<regex::syntax_node_type::kleene, 1>;

  /** Class representing a repeat closure over a subexpression. */
  using syntax_repeat_node = regex::syntax_internal_node<regex::syntax_node_type::repeat, 1>;

}

/* -- Procedure Prototypes -- */

namespace regex
{

  /**
   * Prints the syntax tree rooted at the specified node.
   */
  void print_syntax_tree(const std::unique_ptr<regex::syntax_node>& root);

}

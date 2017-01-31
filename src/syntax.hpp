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

  /* -- Enumerations -- */

  /**
   * Enumeration of types of syntax nodes.
   */
  enum class syntax_node_type
  {
    literal,
    wildcard,
    concatenation,
    alternation,
    optional,
    kleene,
    repeat,
  };

  /* -- Base Type -- */

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

  /* -- Terminal Nodes -- */

  /**
   * Abstract base class for types representing a terminal node (leaf) in a syntax tree.
   */
  class syntax_terminal_node : public regex::syntax_node
  {

    /* -- Public Methods -- */

  public:

    /** Returns `true` if this node matches the specified character. */
    virtual bool matches_character(char ch) const = 0;

  };

  /**
   * Class representing a literal character note in a syntax tree.
   */
  class syntax_literal_node : public regex::syntax_terminal_node
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

    /** Returns `true` if this node matches the specified character. */
    virtual bool matches_character(char ch) const override
    {
      return (ch == m_character);
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
   * Class representing a wildcard character node in a syntax tree.
   */
  class syntax_wildcard_node : public regex::syntax_terminal_node
  {

    /* -- Public Methods -- */

  public:

    /** Returns the type of this syntax node. */
    virtual regex::syntax_node_type type() const override
    {
      return syntax_node_type::wildcard;
    }

    /** Returns `true` if this node matches the specified character. */
    virtual bool matches_character(char ch) const override
    {
      return true;
    }

  };

  /* -- Internal Nodes -- */

  /**
   * Template for classes representing an internal syntax node.
   */
  template <regex::syntax_node_type NodeType, size_t NumChildren>
  class syntax_internal_node : public regex::syntax_node
  {

    /* -- Types -- */

  public:

    /** Unique pointer to a child node. */
    using child_type = std::unique_ptr<const regex::syntax_node>;

    /** Array of children. */
    using children_type = std::array<const child_type, NumChildren>;

    /* -- Lifecycle -- */

  public:

    /** Constructs a new `regex::syntax_internal_node` with the specified children. */
    template <typename... TArgs>
    syntax_internal_node(TArgs&&... children)
      : m_children { std::forward<TArgs>(children)... }
    { }

    /* -- Public Methods -- */

  public:

    /** Returns the type of this syntax node. */
    virtual regex::syntax_node_type type() const override
    {
      return NodeType;
    }

    /** Returns the child nodes of this syntax node. */
    const children_type& children() const
    {
      return m_children;
    }

    /* -- Implementation -- */

  private:

    children_type m_children;

  };

  /** Class for a syntax node representing a concatenation between two subexpressions. */
  using syntax_concatenation_node =
    regex::syntax_internal_node<regex::syntax_node_type::concatenation, 2>;

  /** Class for a syntax node representing a union between two subexpressions. */
  using syntax_alternation_node =
    regex::syntax_internal_node<regex::syntax_node_type::alternation, 2>;

  /** Class representing an optional closure over a subexpression. */
  using syntax_optional_node =
    regex::syntax_internal_node<regex::syntax_node_type::optional, 1>;

  /** Class representing a Kleene closure over a subexpression. */
  using syntax_kleene_node =
    regex::syntax_internal_node<regex::syntax_node_type::kleene, 1>;

  /** Class representing a repeat closure over a subexpression. */
  using syntax_repeat_node =
    regex::syntax_internal_node<regex::syntax_node_type::repeat, 1>;

}

/* -- Procedure Prototypes -- */

namespace regex
{

  /**
   * Prints the syntax tree rooted at the specified node.
   */
  void print_syntax_tree(const std::unique_ptr<const regex::syntax_node>& root);

  /**
   * Returns a string for the specified `regex::syntax_node_type` enum.
   */
  const std::string& syntax_node_type_string(regex::syntax_node_type type);

}

/**
 * @file	syntax.hpp
 * @author	Chris Vig (chris@invictus.so)
 * @date	2017/01/28
 */

#pragma once

/* -- Includes -- */

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
   * Class representing a concatenation between two subexpressions.
   */
  class syntax_concatenation_node : public syntax_node
  {

    /* -- Lifecycle -- */

  public:

    /** Constructs a new `regex::syntax_concatenation_node` object with the specified subexpressions. */
    syntax_concatenation_node(std::unique_ptr<regex::syntax_node> sub_left,
                              std::unique_ptr<regex::syntax_node> sub_right)
      : m_sub_left(std::move(sub_left)),
        m_sub_right(std::move(sub_right))
    { }

    /* -- Public Methods -- */

    /** Returns the type of this syntax node. */
    virtual regex::syntax_node_type type() const override
    {
      return syntax_node_type::concatenation;
    }

    /** The sub_left-hand sub-node of this concatenation. */
    const std::unique_ptr<regex::syntax_node>& sub_left() const
    {
      return m_sub_left;
    }

    /** The sub_right-hand sub-node of this concatenation. */
    const std::unique_ptr<regex::syntax_node>& sub_right() const
    {
      return m_sub_right;
    }

    /* -- Implementation -- */

  private:
    std::unique_ptr<regex::syntax_node> m_sub_left;
    std::unique_ptr<regex::syntax_node> m_sub_right;
  };

  /**
   * Class representing a alternation between two subexpressions.
   */
  class syntax_alternation_node : public syntax_node
  {

    /* -- Lifecycle -- */

  public:

    /** Constructs a new `regex::syntax_alternation_node` object with the specified subexpressions. */
    syntax_alternation_node(std::unique_ptr<regex::syntax_node> sub_left,
                              std::unique_ptr<regex::syntax_node> sub_right)
      : m_sub_left(std::move(sub_left)),
        m_sub_right(std::move(sub_right))
    { }

    /* -- Public Methods -- */

    /** Returns the type of this syntax node. */
    virtual regex::syntax_node_type type() const override
    {
      return syntax_node_type::alternation;
    }

    /** The sub_left-hand sub-node of this alternation. */
    const std::unique_ptr<regex::syntax_node>& sub_left() const
    {
      return m_sub_left;
    }

    /** The sub_right-hand sub-node of this alternation. */
    const std::unique_ptr<regex::syntax_node>& sub_right() const
    {
      return m_sub_right;
    }

    /* -- Implementation -- */

  private:
    std::unique_ptr<regex::syntax_node> m_sub_left;
    std::unique_ptr<regex::syntax_node> m_sub_right;
  };

  /**
   * Class representing an optional closure node.
   */
  class syntax_optional_node : public syntax_node
  {

    /* -- Lifecycle -- */

  public:

    /** Constructs a new `regex::syntax_optional_node` object with the specified subexpression. */
    syntax_optional_node(std::unique_ptr<regex::syntax_node> sub)
      : m_sub(std::move(sub))
    { }

    /* -- Public Methods -- */

  public:

    /** Returns the type of this syntax node. */
    virtual regex::syntax_node_type type() const override
    {
      return syntax_node_type::optional;
    }

    /** Returns the sub-node for this optional closure node. */
    const std::unique_ptr<regex::syntax_node>& sub() const
    {
      return m_sub;
    }

    /* -- Implementation -- */

  private:
    std::unique_ptr<regex::syntax_node> m_sub;
  };

  /**
   * Class representing a Kleene closure node.
   */
  class syntax_kleene_node : public syntax_node
  {

    /* -- Lifecycle -- */

  public:

    /** Constructs a new `regex::syntax_kleene_node` object with the specified subexpression. */
    syntax_kleene_node(std::unique_ptr<regex::syntax_node> sub)
      : m_sub(std::move(sub))
    { }

    /* -- Public Methods -- */

  public:

    /** Returns the type of this syntax node. */
    virtual regex::syntax_node_type type() const override
    {
      return syntax_node_type::kleene;
    }

    /** Returns the sub-node for this Kleene closure node. */
    const std::unique_ptr<regex::syntax_node>& sub() const
    {
      return m_sub;
    }

    /* -- Implementation -- */

  private:
    std::unique_ptr<regex::syntax_node> m_sub;
  };

  /**
   * Class representing a repeat closure node.
   */
  class syntax_repeat_node : public syntax_node
  {

    /* -- Lifecycle -- */

  public:

    /** Constructs a new `regex::syntax_repeat_node` object with the specified subexpression. */
    syntax_repeat_node(std::unique_ptr<regex::syntax_node> sub)
      : m_sub(std::move(sub))
    { }

    /* -- Public Methods -- */

  public:

    /** Returns the type of this syntax node. */
    virtual regex::syntax_node_type type() const override
    {
      return syntax_node_type::repeat;
    }

    /** Returns the sub-node for this repeat closure node. */
    const std::unique_ptr<regex::syntax_node>& sub() const
    {
      return m_sub;
    }

    /* -- Implementation -- */

  private:
    std::unique_ptr<regex::syntax_node> m_sub;
  };

}

/* -- Procedure Prototypes -- */

namespace regex
{

  /**
   * Prints the syntax tree rooted at the specified node.
   */
  void print_syntax_tree(const std::unique_ptr<regex::syntax_node>& root);

}

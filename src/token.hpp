/**
 * @file	token.hpp
 * @author	Chris Vig (chris@invictus.so)
 * @date	2017/01/28
 */

#pragma once

/* -- Includes -- */

#include <string>

/* -- Types -- */

namespace regex
{

  /**
   * Enumeration of recognized token types.
   */
  enum class token_type
  {
    eof,
    literal,
    wildcard,
    quantifier,
    open_bracket,
    close_bracket,
    alternation_operator,
    optional_operator,
    kleene_operator,
    repeat_operator,
  };

  /**
   * Abstract base class for tokens.
   */
  class token
  {

    /* -- Public Methods -- */

  public:

    /** Returns the type of this token. */
    virtual regex::token_type type() const = 0;

    /** The position of this token in the input string. */
    virtual size_t position() const = 0;

  };

  /**
   * Template for classes representing a simple token.
   */
  template <token_type TokenType>
  class simple_token : public token
  {

    /* -- Lifecycle -- */

  public:

    /** Constructs a new `regex::simple_token` with the specified position. */
    simple_token(size_t position)
      : m_position(position)
    { }

    /* -- Public Methods -- */

  public:

    /** Returns the type of this token. */
    virtual regex::token_type type() const override
    {
      return TokenType;
    }

    /** Returns the position of this token in the input string. */
    virtual size_t position() const override
    {
      return m_position;
    }

    /* -- Implementation -- */

  private:

    size_t m_position;

  };

  /** Token class representing an EOF token. */
  using eof_token = simple_token<regex::token_type::eof>;

  /** Token class representing a wildcard token. */
  using wildcard_token = simple_token<regex::token_type::wildcard>;

  /** Token class representing an open bracket. */
  using open_bracket_token = simple_token<regex::token_type::open_bracket>;

  /** Token class representing a close bracket. */
  using close_bracket_token = simple_token<regex::token_type::close_bracket>;

  /** Token class representing an alternation operator. */
  using alternation_operator_token = simple_token<regex::token_type::alternation_operator>;

  /** Token class representing an optional closure operator. */
  using optional_operator_token = simple_token<regex::token_type::optional_operator>;

  /** Token class representing a Kleene closure operator. */
  using kleene_operator_token = simple_token<regex::token_type::kleene_operator>;

  /** Token class representing a repeat closure operator. */
  using repeat_operator_token = simple_token<regex::token_type::repeat_operator>;

  /**
   * Token class representing a literal character.
   */
  class literal_token : public regex::token
  {

    /* -- Lifecycle -- */

  public:

    /** Constructs a new `regex::literal_token` instance. */
    literal_token(char character, size_t position)
      : m_character(character),
        m_position(position)
    { }

    /* -- Public Methods -- */

  public:

    /** Returns the type of this token. */
    regex::token_type type() const override
    {
      return regex::token_type::literal;
    }

    /** Returns the literal character this token represents. */
    char character() const
    {
      return m_character;
    }

    /** Returns the position of this token. */
    size_t position() const override
    {
      return m_position;
    }

    /* -- Implementation -- */

  private:

    char m_character;
    size_t m_position;

  };

  /**
   * Token class representing a quantifier.
   */
  class quantifier_token : public regex::token
  {

    /* -- Lifecycle -- */

  public:

    /** Constructs a new instance matching the specified quantity only. */
    quantifier_token(size_t count)
      : m_min_count(count),
        m_max_count(count)
    { }

    /** Constructs a new instance matching quantities between `min_count` and `max_count`, inclusive. */
    quantifier_token(size_t min_count, size_t max_count)
      : m_min_count(min_count),
        m_max_count(max_count)
    { }

    /* -- Public Methods -- */

  public:

    /** Returns the type of this token. */
    regex::token_type type() const
    {
      return regex::token_type::quantifier;
    }

    /** The minimum number of repetitions for this token. */
    size_t min_count() const
    {
      return m_min_count;
    }

    /** The maximum number of repetitions for this token. */
    size_t max_count() const
    {
      return m_max_count;
    }

    /* -- Implementation -- */

  private:

    size_t m_min_count;
    size_t m_max_count;

  };

}

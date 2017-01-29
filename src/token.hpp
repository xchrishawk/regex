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
    character,
    open_bracket,
    close_bracket,
    union_operator,
    optional_operator,
    kleene_operator,
    repeat_operator,
  };

  /**
   * Class representing a token extracted from an input string.
   */
  class token
  {

    /* -- Lifecycle -- */

  public:

    /** Constructs a new `regex::token` instance with the specified type. */
    token(regex::token_type type)
      : m_type(type),
        m_begin(),
        m_end(),
        m_iterators_valid(false)
    { }

    /** Constructs a new `regex::token` instance with the specified values. */
    token(regex::token_type type,
          const std::string::const_iterator& begin,
          const std::string::const_iterator& end)
      : m_type(type),
        m_begin(begin),
        m_end(end),
        m_iterators_valid(true)
    { }

    /** Returns the type of this token. */
    regex::token_type type() const
    {
      return m_type;
    }

    /** Returns an iterator to the beginning position of this token. */
    std::string::const_iterator begin() const
    {
      return m_begin;
    }

    /** Returns an iterator to the end position of this token. */
    std::string::const_iterator end() const
    {
      return m_end;
    }

    /** Returns `true` if the `begin` and `end` iterators are valid. */
    bool iterators_valid() const
    {
      return m_iterators_valid;
    }

  private:
    token_type m_type;
    std::string::const_iterator m_begin;
    std::string::const_iterator m_end;
    bool m_iterators_valid;
  };

}

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
    alternation_operator,
    optional_operator,
    kleene_operator,
    repeat_operator,
  };

  /**
   * Class representing a token extracted from an input string.
   */
  template <typename TString>
  class basic_token
  {

    /* -- Lifecycle -- */

  public:

    /** Constructs a new `regex::token` instance with the specified type. */
    basic_token(regex::token_type type)
      : m_type(type),
        m_begin(),
        m_end(),
        m_position(0),
        m_iterators_valid(false)
    { }

    /** Constructs a new `regex::token` instance with the specified values. */
    basic_token(regex::token_type type,
                const typename TString::const_iterator& begin,
                const typename TString::const_iterator& end,
                const typename TString::const_iterator::difference_type position)
      : m_type(type),
        m_begin(begin),
        m_end(end),
        m_position(position),
        m_iterators_valid(true)
    { }

    /** Returns the type of this token. */
    auto type() const
    {
      return m_type;
    }

    /** Returns an iterator to the beginning position of this token. */
    auto begin() const
    {
      return m_begin;
    }

    /** Returns an iterator to the end position of this token. */
    auto end() const
    {
      return m_end;
    }

    /** The position of this token in the input string. */
    auto position() const
    {
      return m_position;
    }

    /** Returns `true` if the `begin` and `end` iterators are valid. */
    auto iterators_valid() const
    {
      return m_iterators_valid;
    }

  private:
    token_type m_type;
    typename TString::const_iterator m_begin;
    typename TString::const_iterator m_end;
    typename TString::const_iterator::difference_type m_position;
    bool m_iterators_valid;
  };

  /** Specialization of `regex::basic_token` for `std::string`. */
  using token = regex::basic_token<std::string>;

}

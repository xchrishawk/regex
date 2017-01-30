/**
 * @file	syntax_analyzer.hpp
 * @author	Chris Vig (chris@invictus.so)
 * @date	2017/01/28
 */

#pragma once

/* -- Includes -- */

#include <memory>
#include <stdexcept>
#include <vector>

#include "syntax.hpp"
#include "token.hpp"

/* -- Procedure Prototypes -- */

namespace regex
{

  /**
   * Class representing an exception thrown for a syntax error.
   */
  class syntax_error : public std::runtime_error
  {
  public:

    /** Constructs a new `regex::syntax_error` instance with the specified message. */
    syntax_error(const std::string& message)
      : std::runtime_error(message)
    { }

  };

  /**
   * Class reponsible for parsing a regular expression syntax tree from a stream of tokens.
   */
  class syntax_analyzer
  {

    /* -- Lifecycle -- */

  public:

    /** Constructs a new `regex::syntax_analyzer` instance using the specified tokens. */
    syntax_analyzer(std::vector<regex::token> tokens);

    /** Destructor. */
    ~syntax_analyzer();

    /* -- Public Methods -- */

  public:

    /**
     * Parses a regular expression using this syntax analyzer's token list.
     *
     * @exception regex::syntax_error
     * Thrown if a syntax error is encountered.
     */
    std::unique_ptr<const regex::syntax_node> parse_regex();

    /* -- Implementation -- */

  private:

    struct implementation;
    std::unique_ptr<implementation> impl;

  };

}

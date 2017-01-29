/**
 * @file	lexical_analyzer.hpp
 * @author	Chris Vig (chris@invictus.so)
 * @date	2017/01/28
 */

#pragma once

/* -- Includes -- */

#include <memory>
#include <string>
#include <vector>

#include "token.hpp"

/* -- Types -- */

namespace regex
{

  /**
   * Lexical analyzer class for extracting tokens from a regular expression.
   */
  class lexical_analyzer
  {

    /* -- Lifecycle -- */

  public:

    /** Constructs a new instance with the specified input. */
    lexical_analyzer(const std::string& input);

    /** Destructor. */
    ~lexical_analyzer();

    /* -- Public Methods -- */

  public:

    /** Extracts all tokens from the input string and return them as a vector. */
    std::vector<regex::token> all_tokens();

    /** Extracts the next token from the input string. */
    regex::token next_token();

    /** Peeks at the next token without removing it from the input string. */
    regex::token peek_token();

    /* -- Implementation -- */

  private:

    struct implementation;
    std::unique_ptr<implementation> impl;

  };

}

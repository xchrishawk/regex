/**
 * @file	parser.hpp
 * @author	Chris Vig (chris@invictus.so)
 * @date	2017/01/28
 */

#pragma once

/* -- Includes -- */

#include <memory>
#include <vector>

#include "syntax.hpp"
#include "token.hpp"

/* -- Procedure Prototypes -- */

namespace regex
{

  /**
   * Class reponsible for parsing a regular expression syntax tree from a stream of tokens.
   */
  class parser
  {

    /* -- Lifecycle -- */

  public:

    /** Constructs a new `regex::parser` instance using the specified tokens. */
    parser(std::vector<regex::token> tokens);

    /** Destructor. */
    ~parser();

    /* -- Public Methods -- */

  public:

    /** Parses a regular expression using this parser's token list. */
    std::unique_ptr<regex::syntax_node> parse_regex();

    /* -- Implementation -- */

  private:

    struct implementation;
    std::unique_ptr<implementation> impl;

  };

}

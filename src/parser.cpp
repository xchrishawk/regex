/**
 * @file	parser.cpp
 * @author	Chris Vig (chris@invictus.so)
 * @date	2017/01/28
 */

/* -- Includes -- */

#include <cassert>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "lexical_analyzer.hpp"
#include "parser.hpp"
#include "syntax.hpp"
#include "token.hpp"

/* -- Namespaces -- */

using namespace std;
using namespace regex;

/* -- Types -- */

struct parser::implementation
{

  /* -- Fields -- */

  vector<token> tokens;
  vector<token>::const_iterator it;

  /* -- Methods -- */

  /** Parses a regular expression. */
  unique_ptr<syntax_node> parse_regex()
  {
    auto expr = parse_expr();
    switch (it->type())
    {
    case token_type::union_operator:
    {
      it++;
      auto regex = parse_regex();
      return make_unique<syntax_alternation_node>(move(expr), move(regex));
    }

    default:
      return expr;
    }
  }

  /** Parses an expression. */
  unique_ptr<syntax_node> parse_expr()
  {
    auto subexpr = parse_subexpr();
    switch (it->type())
    {
    case token_type::open_bracket:
    case token_type::character:
    {
      // we can only start a new concatenation on an open bracket or character
      auto expr = parse_expr();
      return make_unique<syntax_concatenation_node>(move(subexpr), move(expr));
    }

    default:
      return subexpr;
    }
  }

  /** Parses a subexpression. */
  unique_ptr<syntax_node> parse_subexpr()
  {
    auto atom = parse_atom();
    switch (it->type())
    {
    case token_type::optional_operator:
      it++;
      return make_unique<syntax_optional_node>(move(atom));

    case token_type::kleene_operator:
      it++;
      return make_unique<syntax_kleene_node>(move(atom));

    case token_type::repeat_operator:
      it++;
      return make_unique<syntax_repeat_node>(move(atom));

    default:
      return atom;
    }
  }

  /** Parses an atom. */
  unique_ptr<syntax_node> parse_atom()
  {
    switch (it->type())
    {
    case token_type::character:
      return parse_literal();

    case token_type::open_bracket:
    {
      it++;
      auto subexpr = parse_regex();
      if (it->type() != token_type::close_bracket)
        throw runtime_error("Expected close bracket!");
      it++;
      return subexpr;
    }

    default:
      throw runtime_error("Expected atom!");
    }
  }

  /** Parses a literal. */
  unique_ptr<syntax_node> parse_literal()
  {
    switch (it->type())
    {
    case token_type::character:
    {
      auto node = make_unique<syntax_literal_node>(*it->begin());
      it++;
      return move(node);
    }

    default:
      throw runtime_error("Expected literal!");
    }
  }

};

/* -- Procedures -- */

parser::parser(vector<token> tokens)
  : impl(make_unique<implementation>())
{
  impl->tokens = move(tokens);
  impl->it = impl->tokens.cbegin();
}

parser::~parser() = default;

unique_ptr<regex::syntax_node> parser::parse_regex()
{
  return impl->parse_regex();
}

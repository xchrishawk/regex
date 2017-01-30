/**
 * @file	syntax_analyzer.cpp
 * @author	Chris Vig (chris@invictus.so)
 * @date	2017/01/28
 */

/* -- Includes -- */

#include <cassert>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

#include "lexical_analyzer.hpp"
#include "syntax.hpp"
#include "syntax_analyzer.hpp"
#include "token.hpp"

/* -- Namespaces -- */

using namespace std;
using namespace regex;

/* -- Types -- */

struct syntax_analyzer::implementation
{

  /* -- Fields -- */

  vector<token> tokens;
  vector<token>::const_iterator it;

  /* -- Methods -- */

  /** Parses a regular expression. */
  unique_ptr<const syntax_node> parse_regex()
  {
    auto expr = parse_expr();
    switch (it->type())
    {
    case token_type::alternation_operator:
    {
      it++;
      auto regex = parse_regex();
      return make_unique<const syntax_alternation_node>(move(expr), move(regex));
    }

    default:
      return expr;
    }
  }

  /** Parses an expression. */
  unique_ptr<const syntax_node> parse_expr()
  {
    auto subexpr = parse_subexpr();
    switch (it->type())
    {
    case token_type::open_bracket:
    case token_type::character:
    {
      // we can only start a new concatenation on an open bracket or character
      auto expr = parse_expr();
      return make_unique<const syntax_concatenation_node>(move(subexpr), move(expr));
    }

    default:
      return subexpr;
    }
  }

  /** Parses a subexpression. */
  unique_ptr<const syntax_node> parse_subexpr()
  {
    auto atom = parse_atom();
    switch (it->type())
    {
    case token_type::optional_operator:
      it++;
      return make_unique<const syntax_optional_node>(move(atom));

    case token_type::kleene_operator:
      it++;
      return make_unique<const syntax_kleene_node>(move(atom));

    case token_type::repeat_operator:
      it++;
      return make_unique<const syntax_repeat_node>(move(atom));

    default:
      return atom;
    }
  }

  /** Parses an atom. */
  unique_ptr<const syntax_node> parse_atom()
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
        throw_syntax_error(it->position(), "Expected close bracket.");
      it++;
      return subexpr;
    }

    default:
      throw_syntax_error(it->position(), "Expected atom.");
    }
  }

  /** Parses a literal. */
  unique_ptr<const syntax_node> parse_literal()
  {
    switch (it->type())
    {
    case token_type::character:
    {
      auto node = make_unique<const syntax_literal_node>(*it->begin());
      it++;
      return move(node);
    }

    default:
      throw_syntax_error(it->position(), "Expected literal character.");
    }
  }

  /** Throws a syntax error. */
  [[noreturn]] static void throw_syntax_error(size_t position, const string& error_message)
  {
    ostringstream message;
    message << "Syntax error at position " << position << ". " << error_message;
    throw syntax_error(message.str());
  }

};

/* -- Procedures -- */

syntax_analyzer::syntax_analyzer(vector<token> tokens)
  : impl(make_unique<implementation>())
{
  impl->tokens = move(tokens);
  impl->it = impl->tokens.cbegin();
}

syntax_analyzer::~syntax_analyzer() = default;

unique_ptr<const syntax_node> syntax_analyzer::parse_regex()
{
  auto regex = impl->parse_regex();
  if (impl->it->type() != token_type::eof)
    implementation::throw_syntax_error(impl->it->position(), "Unparseable tokens at end of string.");
  return regex;
}

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

  vector<unique_ptr<const token>> tokens;
  vector<unique_ptr<const token>>::const_iterator it;

  /* -- Methods -- */

  /** Parses a regular expression. */
  unique_ptr<const syntax_node> parse_regex()
  {
    auto expr = parse_expr();
    switch (next_token_type())
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
    switch (next_token_type())
    {
    case token_type::open_bracket:
    case token_type::literal:
    case token_type::wildcard:
    {
      // we can only start a new concatenation on an open bracket, literal, or wildcard
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
    switch (next_token_type())
    {
    case token_type::optional_operator:
      skip_next_token();
      return make_unique<const syntax_optional_node>(move(atom));

    case token_type::kleene_operator:
      skip_next_token();
      return make_unique<const syntax_kleene_node>(move(atom));

    case token_type::repeat_operator:
      skip_next_token();
      return make_unique<const syntax_repeat_node>(move(atom));

    default:
      return atom;
    }
  }

  /** Parses an atom. */
  unique_ptr<const syntax_node> parse_atom()
  {
    switch (next_token_type())
    {
    case token_type::literal:
      return parse_literal();

    case token_type::wildcard:
      return parse_wildcard();

    case token_type::open_bracket:
    {
      skip_next_token();
      auto subexpr = parse_regex();
      if (next_token_type() != token_type::close_bracket)
        throw_syntax_error(next_token_position(), "Expected close bracket.");
      skip_next_token();
      return subexpr;
    }

    default:
      throw_syntax_error(next_token_position(), "Expected atom.");
    }
  }

  /** Parses a literal. */
  unique_ptr<const syntax_node> parse_literal()
  {
    switch (next_token_type())
    {
    case token_type::literal:
    {
      auto literal_token = next_token<class literal_token>();
      auto node = make_unique<const syntax_literal_node>(literal_token->character());
      skip_next_token();
      return move(node);
    }

    default:
      throw_syntax_error(next_token_position(), "Expected literal character.");
    }
  }

  /** Parses a wildcard. */
  unique_ptr<const syntax_node> parse_wildcard()
  {
    switch (next_token_type())
    {
    case token_type::wildcard:
    {
      auto node = make_unique<const syntax_wildcard_node>();
      skip_next_token();
      return move(node);
    }

    default:
      throw_syntax_error(next_token_position(), "Expected wildcard.");
    }
  }

  /** Skips the current token. */
  void skip_next_token()
  {
    it++;
  }

  /** Returns the type of the next token. */
  token_type next_token_type() const
  {
    return (*it)->type();
  }

  /** Returns the position of the next token. */
  size_t next_token_position() const
  {
    return (*it)->position();
  }

  /** Gets a reference to the next token as the specified type. */
  template <typename TToken>
  const TToken* next_token() const
  {
    auto ptr = dynamic_cast<const TToken*>(it->get());
    assert(ptr != nullptr);
    return ptr;
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

syntax_analyzer::syntax_analyzer(vector<unique_ptr<const token>> tokens)
  : impl(make_unique<implementation>())
{
  impl->tokens = move(tokens);
  impl->it = impl->tokens.cbegin();
}

syntax_analyzer::~syntax_analyzer() = default;

unique_ptr<const syntax_node> syntax_analyzer::parse_regex()
{
  auto regex = impl->parse_regex();
  if (impl->next_token_type() != token_type::eof)
    implementation::throw_syntax_error(impl->next_token_position(), "Unparseable tokens at end of string.");
  return regex;
}

/**
 * @file	lexical_analyzer.cpp
 * @author	Chris Vig (chris@invictus.so)
 * @date	2017/01/28
 */

/* -- Includes -- */

#include <memory>
#include <string>
#include <vector>

#include "lexical_analyzer.hpp"
#include "token.hpp"

/* -- Namespaces -- */

using namespace std;
using namespace regex;

/* -- Types -- */

struct lexical_analyzer::implementation
{

  /* -- Constructor -- */

  implementation(const std::string& input)
    : input(input),
      position(input.cbegin())
  { }

  /* -- Fields -- */

  const std::string& input;
  std::string::const_iterator position;

};

/* -- Procedures -- */

lexical_analyzer::lexical_analyzer(const std::string& input)
  : impl(make_unique<implementation>(input))
{
}

lexical_analyzer::~lexical_analyzer() = default;

vector<token> lexical_analyzer::all_tokens()
{
  vector<token> tokens;
  while (true)
  {
    token tok = next_token();
    if (tok.type() == token_type::eof)
      break;
    tokens.push_back(tok);
  }
  return tokens;
}

token lexical_analyzer::next_token()
{
  auto tok = peek_token();
  if (tok.iterators_valid())
    impl->position = tok.end();
  return tok;
}

token lexical_analyzer::peek_token()
{
  // return EOF if we're out of input
  if (impl->position == impl->input.cend())
    return token(token_type::eof);

  auto search = impl->position;
  token_type type;

  switch (*search)
  {
  case '(':
    type = token_type::open_bracket;
    break;

  case ')':
    type = token_type::close_bracket;
    break;

  case '|':
    type = token_type::union_operator;
    break;

  case '?':
    type = token_type::optional_operator;
    break;

  case '*':
    type = token_type::kleene_operator;
    break;

  case '+':
    type = token_type::repeat_operator;
    break;

  default:
    type = token_type::character;
    break;
  }

  search++;
  return token(type, impl->position, search);
}

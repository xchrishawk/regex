/**
 * @file	lexical_analyzer.cpp
 * @author	Chris Vig (chris@invictus.so)
 * @date	2017/01/28
 */

/* -- Includes -- */

#include <iterator>
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

vector<unique_ptr<const token>> lexical_analyzer::all_tokens()
{
  vector<unique_ptr<const token>> tokens;
  while (true)
  {
    auto tok = next_token();
    tokens.push_back(move(tok));
    if (tokens.back()->type() == token_type::eof)
      break;
  }
  return tokens;
}

unique_ptr<const token> lexical_analyzer::next_token()
{
  // skip a character
  auto skip = [this] () -> void {
    this->impl->position++;
  };

  // get the current character
  auto get_character = [this] () -> char {
    return *this->impl->position;
  };

  // get current position
  auto get_position = [this] () -> size_t {
    return distance(this->impl->input.cbegin(), this->impl->position);
  };

  // return EOF if we're out of input
  if (impl->position == impl->input.cend())
    return make_unique<eof_token>(get_position());

  switch (*impl->position)
  {

  case '.':
  {
    auto position = get_position();
    skip();
    return make_unique<wildcard_token>(position);
  }

  case '(':
  {
    auto position = get_position();
    skip();
    return make_unique<open_bracket_token>(position);
  }

  case ')':
  {
    auto position = get_position();
    skip();
    return make_unique<close_bracket_token>(position);
  }

  case '|':
  {
    auto position = get_position();
    skip();
    return make_unique<alternation_operator_token>(position);
  }

  case '?':
  {
    auto position = get_position();
    skip();
    return make_unique<optional_operator_token>(position);
  }

  case '*':
  {
    auto position = get_position();
    skip();
    return make_unique<kleene_operator_token>(position);
  }

  case '+':
  {
    auto position = get_position();
    skip();
    return make_unique<repeat_operator_token>(position);
  }

  default:
  {
    auto character = get_character();
    auto position = get_position();
    skip();
    return make_unique<literal_token>(character, position);
  }

  }
}

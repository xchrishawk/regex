/**
 * @file	main.cpp
 * @author	Chris Vig (chris@invictus.so)
 * @date	2017/01/28
 */

/* -- Includes -- */

#include <iostream>
#include <string>

#include "lexical_analyzer.hpp"
#include "token.hpp"

/* -- Namespaces -- */

using namespace std;
using namespace regex;

/* -- Procedures -- */

int main(int argc, char** argv)
{
  const string regex = "(a)bc";
  lexical_analyzer lex(regex);

  for (int idx = 0; idx < 10; idx++)
  {
    auto tok = lex.peek_token();
    cout << static_cast<int>(tok.type()) << endl;
    if (tok.iterators_valid())
      cout << "  " << string(tok.begin(), tok.end()) << endl;
  }

  return 0;
}

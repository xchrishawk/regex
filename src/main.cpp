/**
 * @file	main.cpp
 * @author	Chris Vig (chris@invictus.so)
 * @date	2017/01/28
 */

/* -- Includes -- */

#include <iostream>
#include <string>

#include "lexical_analyzer.hpp"
#include "parser.hpp"
#include "syntax.hpp"

/* -- Namespaces -- */

using namespace std;
using namespace regex;

/* -- Procedures -- */

int main(int argc, char** argv)
{
  static const string INPUT = "(chris|hannah)";
  lexical_analyzer lex(INPUT);
  parser parse(lex.all_tokens());

  auto regex = parse.parse_regex();
  print_syntax_tree(regex);

  return 0;
}

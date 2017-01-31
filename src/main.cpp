/**
 * @file	main.cpp
 * @author	Chris Vig (chris@invictus.so)
 * @date	2017/01/28
 */

/* -- Includes -- */

#include <exception>
#include <iostream>
#include <string>

#include "lexical_analyzer.hpp"
#include "syntax.hpp"
#include "syntax_analyzer.hpp"

/* -- Namespaces -- */

using namespace std;
using namespace regex;

/* -- Procedures -- */

int main(int argc, char** argv)
{
  try
  {
    static const string INPUT = "\\?\\\\\\*\\++a?\\?";

    cout << "Regex: " << INPUT << endl;

    lexical_analyzer lex(INPUT);
    syntax_analyzer parse(lex.all_tokens());

    auto regex = parse.parse_regex();
    print_syntax_tree(regex);

    return 0;
  }
  catch (const exception& ex)
  {
    cerr << ex.what() << endl;
    return 1;
  }
}

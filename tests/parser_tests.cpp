/**
 * @file	parser_tests.cpp
 * @author	Chris Vig (chris@invictus.so)
 * @date	2017/01/28
 */

/* -- Includes -- */

#include <string>
#include <vector>
#include <gtest/gtest.h>

#include "lexical_analyzer.hpp"
#include "parser.hpp"
#include "syntax.hpp"
#include "token.hpp"

/* -- Namespaces -- */

using namespace std;
using namespace testing;
using namespace regex;

/* -- Test Cases -- */

/**
 * Unit test for the `regex::lexical_analyzer` class.
 */
class ParserTests : public Test
{
protected:

  /** Parses a syntax tree from the specified input. */
  std::unique_ptr<syntax_node> syntax_tree(const std::string& input)
  {
    lexical_analyzer lex(input);
    auto tokens = lex.all_tokens();
    parser parse(tokens);
    return parse.parse_regex();
  }

};

TEST_F(ParserTests, PlainRegex)
{
  auto root = syntax_tree("abc");

  ASSERT_EQ(root->type(), syntax_node_type::concatenation);
  auto root_concat = dynamic_cast<syntax_concatenation_node*>(root.get());
}

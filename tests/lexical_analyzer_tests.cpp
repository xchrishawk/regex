/**
 * @file	lexical_analyzer_tests.cpp
 * @author	Chris Vig (chris@invictus.so)
 * @date	2017/01/28
 */

/* -- Includes -- */

#include <string>
#include <vector>
#include <gtest/gtest.h>

#include "lexical_analyzer.hpp"
#include "token.hpp"

/* -- Namespaces -- */

using namespace std;
using namespace testing;
using namespace regex;

/* -- Test Cases -- */

/**
 * Unit test for the `regex::lexical_analyzer` class.
 */
class LexicalAnalyzerTests : public Test
{
protected:

  /** Expect a single token to be extracted from the specified string. */
  void expect_single_token(const std::string& input, const token_type type)
  {
    lexical_analyzer lex(input);

    token tok = lex.next_token();
    EXPECT_EQ(tok.type(), type);
    EXPECT_EQ(tok.begin(), input.cbegin());
    EXPECT_EQ(tok.end(), input.cend());

    expect_eof(lex);
  }

  /** Extract the next token from `lex` and verify it is an `EOF` token. */
  void expect_eof(lexical_analyzer& lex)
  {
    token tok = lex.next_token();
    EXPECT_EQ(tok.type(), token_type::eof);
  }

};

/** Verify that the `regex::lexical_analyzer` class extracts EOF tokens. */
TEST_F(LexicalAnalyzerTests, ExtractsEOFToken)
{
  lexical_analyzer lex("");
  expect_eof(lex);
}

/** Verify that the `regex::lexical_analyzer` class extracts open bracket tokens. */
TEST_F(LexicalAnalyzerTests, ExtractsOpenBracketToken)
{
  expect_single_token("(", token_type::open_bracket);
}

/** Verify that the `regex::lexical_analyzer` class extracts close bracket tokens. */
TEST_F(LexicalAnalyzerTests, ExtractsCloseBracketToken)
{
  expect_single_token(")", token_type::close_bracket);
}

/** Verify that the `regex::lexical_analyzer` class extracts union operator tokens. */
TEST_F(LexicalAnalyzerTests, ExtractsUnionOperatorToken)
{
  expect_single_token("|", token_type::union_operator);
}

/** Verify that the `regex::lexical_analyzer` class extracts optional operator tokens. */
TEST_F(LexicalAnalyzerTests, ExtractsOptionalOperatorToken)
{
  expect_single_token("?", token_type::optional_operator);
}

/** Verify that the `regex::lexical_analyzer` class extracts Kleene operator tokens. */
TEST_F(LexicalAnalyzerTests, ExtractsKleeneOperatorToken)
{
  expect_single_token("*", token_type::kleene_operator);
}

/** Verify that the `regex::lexical_analyzer` class extracts repeat operator tokens. */
TEST_F(LexicalAnalyzerTests, ExtractsRepeatOperatorToken)
{
  expect_single_token("+", token_type::repeat_operator);
}

/** Verify that the `regex::lexical_analyzer` class extracts a vector of all tokens. */
TEST_F(LexicalAnalyzerTests, ExtractsAllTokens)
{
  static const string INPUT = ")(+*";
  lexical_analyzer lex(INPUT);
  vector<token> tokens = lex.all_tokens();

  ASSERT_EQ(tokens.size(), 4);

  EXPECT_EQ(tokens[0].type(), token_type::close_bracket);
  EXPECT_EQ(tokens[0].begin(), INPUT.cbegin());
  EXPECT_EQ(tokens[0].end(), INPUT.cbegin() + 1);

  EXPECT_EQ(tokens[1].type(), token_type::open_bracket);
  EXPECT_EQ(tokens[1].begin(), INPUT.cbegin() + 1);
  EXPECT_EQ(tokens[1].end(), INPUT.cbegin() + 2);

  EXPECT_EQ(tokens[2].type(), token_type::repeat_operator);
  EXPECT_EQ(tokens[2].begin(), INPUT.cbegin() + 2);
  EXPECT_EQ(tokens[2].end(), INPUT.cbegin() + 3);

  EXPECT_EQ(tokens[3].type(), token_type::kleene_operator);
  EXPECT_EQ(tokens[3].begin(), INPUT.cbegin() + 3);
  EXPECT_EQ(tokens[3].end(), INPUT.cend());
}

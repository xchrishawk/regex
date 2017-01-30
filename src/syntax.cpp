/**
 * @file	syntax.cpp
 * @author	Chris Vig (chris@invictus.so)
 * @date	2017/01/28
 */

/* -- Includes -- */

#include <cassert>
#include <iostream>
#include <memory>
#include <string>

#include "syntax.hpp"

/* -- Namespaces -- */

using namespace std;
using namespace regex;

/* -- Private Procedures -- */

namespace
{

  /** Recursively prints a syntax tree. */
  void recursive_print_syntax_tree(const unique_ptr<const syntax_node>& root, int indentation)
  {
    for (int idx = 0; idx < indentation; idx++)
      cout << "  ";

    auto print_internal = [indentation] (auto node) {
      assert(node != nullptr);
      cout << syntax_node_type_string(node->type()) << endl;
      for (const auto& child : node->children())
        recursive_print_syntax_tree(child, indentation + 1);
    };

    switch (root->type())
    {
    case syntax_node_type::literal:
    {
      auto literal_node = dynamic_cast<const syntax_literal_node*>(root.get());
      assert(literal_node != nullptr);
      cout << "Literal: " << literal_node->character() << endl;
      break;
    }

    case syntax_node_type::concatenation:
      print_internal(dynamic_cast<const syntax_concatenation_node*>(root.get()));
      break;

    case syntax_node_type::alternation:
      print_internal(dynamic_cast<const syntax_alternation_node*>(root.get()));
      break;

    case syntax_node_type::optional:
      print_internal(dynamic_cast<const syntax_optional_node*>(root.get()));
      break;

    case syntax_node_type::kleene:
      print_internal(dynamic_cast<const syntax_kleene_node*>(root.get()));
      break;

    case syntax_node_type::repeat:
      print_internal(dynamic_cast<const syntax_repeat_node*>(root.get()));
      break;
    }
  }

}

/* -- Procedures -- */

void regex::print_syntax_tree(const unique_ptr<const syntax_node>& root)
{
  recursive_print_syntax_tree(root, 0);
}

const string& regex::syntax_node_type_string(syntax_node_type type)
{
  static const string STRING_LITERAL 		= "Literal";
  static const string STRING_CONCATENATION	= "Concatenation";
  static const string STRING_ALTERNATION	= "Alternation";
  static const string STRING_OPTIONAL		= "Optional";
  static const string STRING_KLEENE		= "Kleene";
  static const string STRING_REPEAT		= "Repeat";
  static const string STRING_DEFAULT		= "Unknown";

  switch (type)
  {
  case syntax_node_type::literal:		return STRING_LITERAL;
  case syntax_node_type::concatenation:		return STRING_CONCATENATION;
  case syntax_node_type::alternation:		return STRING_ALTERNATION;
  case syntax_node_type::optional:		return STRING_OPTIONAL;
  case syntax_node_type::kleene:		return STRING_KLEENE;
  case syntax_node_type::repeat:		return STRING_REPEAT;
  default:					return STRING_DEFAULT;
  }
}

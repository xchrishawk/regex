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
  void recursive_print_syntax_tree(const std::unique_ptr<regex::syntax_node>& root, int indentation)
  {
    for (int idx = 0; idx < indentation; idx++)
      cout << "  ";

    switch (root->type())
    {
    case syntax_node_type::literal:
    {
      syntax_literal_node* literal_node = dynamic_cast<syntax_literal_node*>(root.get());
      assert(literal_node != nullptr);
      cout << "Literal: " << literal_node->character() << endl;
      break;
    }

    case syntax_node_type::concatenation:
    {
      syntax_concatenation_node* concat_node = dynamic_cast<syntax_concatenation_node*>(root.get());
      assert(concat_node != nullptr);
      cout << "Concatenation" << endl;
      recursive_print_syntax_tree(concat_node->sub_left(), indentation + 1);
      recursive_print_syntax_tree(concat_node->sub_right(), indentation + 1);
      break;
    }

    case syntax_node_type::alternation:
    {
      syntax_alternation_node* altern_node = dynamic_cast<syntax_alternation_node*>(root.get());
      assert(altern_node != nullptr);
      cout << "Alternation" << endl;
      recursive_print_syntax_tree(altern_node->sub_left(), indentation + 1);
      recursive_print_syntax_tree(altern_node->sub_right(), indentation + 1);
      break;
    }

    case syntax_node_type::optional:
    {
      syntax_optional_node* optional_node = dynamic_cast<syntax_optional_node*>(root.get());
      assert(optional_node != nullptr);
      cout << "Optional" << endl;
      recursive_print_syntax_tree(optional_node->sub(), indentation + 1);
      break;
    }

    case syntax_node_type::kleene:
    {
      syntax_kleene_node* kleene_node = dynamic_cast<syntax_kleene_node*>(root.get());
      assert(kleene_node != nullptr);
      cout << "Kleene" << endl;
      recursive_print_syntax_tree(kleene_node->sub(), indentation + 1);
      break;
    }

    case syntax_node_type::repeat:
    {
      syntax_repeat_node* repeat_node = dynamic_cast<syntax_repeat_node*>(root.get());
      assert(repeat_node != nullptr);
      cout << "Repeat" << endl;
      recursive_print_syntax_tree(repeat_node->sub(), indentation + 1);
      break;
    }

    default:
    {
      cout << "Unknown node type" << endl;
      break;
    }
    }
  }

}

/* -- Procedures -- */

void regex::print_syntax_tree(const std::unique_ptr<regex::syntax_node>& root)
{
  recursive_print_syntax_tree(root, 0);
}

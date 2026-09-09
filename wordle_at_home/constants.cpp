#include "constants.h"
#include <algorithm>
#include <fstream>
#include <list>

struct Node {
  std::unordered_map<wchar_t, Node *> children;
  Node *parent = nullptr;
  wchar_t ch = L'\0';

  std::wstring traceback() const {
    if (parent) {
      return parent->traceback() + ch;
    }
    return L"";
  }
};

Constants::Constants() {
  // Read file line-by-line to avoid loading the whole file into memory
  std::wifstream german("german.txt");
  german.imbue(std::locale(""));

  std::list<Node> tree = {{}}; // keep nodes in a list so pointers remain stable

  std::wstring line;
  while (std::getline(german, line)) {
    Node *current = &tree.front();
    int newNodeCount = 0;
    int nodeCount = 0;

    for (wchar_t c : line) {
      if (!current->children.count(c)) {
        tree.emplace_back();
        tree.back().parent = current;
        tree.back().ch = c;
        current->children[c] = &tree.back();
        ++newNodeCount;
      }
      ++nodeCount;
      current = current->children[c];
    }

    if (newNodeCount > 4 || newNodeCount == nodeCount) {
      words.push_back(current->traceback());
    }
  }

  std::sort(words.begin(), words.end(),
            [](const auto &a, const auto &b) { return a.size() < b.size(); });
}
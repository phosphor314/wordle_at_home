#include "constants.h"
#include <fstream>
#include <list>

struct Node{
	std::unordered_map<wchar_t, Node*> children;
	Node* parent = nullptr;
	
	std::wstring traceback() const{
		if (parent){
			return parent->traceback() + std::find_if(parent->children.begin(), parent->children.end(), [this](const auto& x){
				return x.second == this;
			})->first;
		}
		return L"";
	}
};

Constants::Constants(){
    std::wifstream german;
	german.imbue(std::locale(""));
	german.open("german.txt", std::ios_base::ate);
	std::vector<wchar_t> content;
	content.resize(german.tellg());
	german.seekg(0);
	german.read(content.data(), content.size());
	
	std::list<Node> tree = {{}};
	
	Node* current = &tree.front();
	int newNodeCount = 0;
	int nodeCount = 0;
	
	for (wchar_t c : content) {
		if (c == L'\n') {
			if (newNodeCount > 4 || newNodeCount == nodeCount){
				words.push_back(current->traceback());
			}
			current = &tree.front();
			newNodeCount = 0;
			nodeCount = 0;
		}
		else {
			if (!current->children.count(c)){
				tree.emplace_back();
				tree.back().parent = current;
				current->children[c] = &tree.back();
				++newNodeCount;
			}
			++nodeCount;
			current = current->children[c];
		}
	}
	
	std::sort(words.begin(), words.end(), [](const auto& a, const auto& b){
		return a.size() < b.size();
	});
}
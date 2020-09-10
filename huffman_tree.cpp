#include "huffman_tree.h"

HuffmanTree::HuffmanTree(const std::vector<unsigned int>& counts) {

	std::vector<HuffmanPtr> node_list;
	for (size_t i = 0; i < counts.size(); i++) {
		if (counts[i] > 0) {
			HuffmanPtr node = new HuffmanNode();
			node->val = counts[i];
			node->byte = i;
			node_list.push_back(node);
		}
	}
	std::sort(node_list.begin(), node_list.end(),
		[](const HuffmanPtr& lhs, const HuffmanPtr& rhs) {
			return lhs->val > rhs->val;
		});

	while (node_list.size() > 1) {

		HuffmanPtr node = new HuffmanNode();

		node->right = node_list.back();
		node_list.pop_back();
		node->left = node_list.back();
		node_list.pop_back();

		node->val = node->left->val + node->right->val;
		node->left->parent = node->right->parent = node;

		decltype(node_list)::iterator pos = std::lower_bound(node_list.begin(), node_list.end(), node,
			[](const HuffmanPtr& lhs, const HuffmanPtr& rhs) {
				return lhs->val >= rhs->val;
			});
	
		node_list.insert(pos, node);
	}

	root = node_list.front();
}

HuffmanTree::~HuffmanTree() {

	if (root == nullptr) {
		return;
	}
	std::queue<HuffmanPtr> q;
	q.push(root);
	while (!q.empty()) {
		HuffmanPtr node = q.front();
		if (node->left)
			q.push(node->left);
		if (node->right)
			q.push(node->right);
		delete node;
		node = nullptr;
		q.pop();
	}
}

std::vector<std::string> HuffmanTree::get_codes() const {

	std::vector<std::string> ans(kBytesSize);
	std::queue<HuffmanPtr> q;
	q.push(root);

	while (!q.empty()) {
		HuffmanPtr node = q.front();
		if (node->left == nullptr && node->right == nullptr) {
			ans[node->byte] = node->codes;
		} else {
			node->left->codes = node->codes + "0";
			node->right->codes = node->codes + "1";
			q.push(node->left);
			q.push(node->right);
		}
		q.pop();
	}
	return ans;
}

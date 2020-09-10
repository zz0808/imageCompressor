#pragma once

#include <algorithm>
#include <queue>
#include "bytes_map.h"
struct HuffmanNode {
	unsigned int val; // Ȩ��
	std::string codes; // ����ֵ
	unsigned char byte; // Ҷ�ӽڵ��Ӧ���ֽ�ֵ
	HuffmanNode* parent = nullptr;
	HuffmanNode* left = nullptr, * right = nullptr;
};
typedef struct HuffmanNode* HuffmanPtr;

class HuffmanTree {
public:
	HuffmanTree(const std::vector<unsigned int>& counts );
	~HuffmanTree();
	HuffmanTree() {

	}
	HuffmanPtr get_root() const {
		return root;
	}

	std::vector<std::string> get_codes() const;

private:
	HuffmanPtr root;
};



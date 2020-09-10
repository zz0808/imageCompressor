#pragma once

#include "bytes_map.h"
#include "huffman_tree.h"

class BytesMap;
class HuffmanTree;

class Compressor {
public:
	Compressor();
	~Compressor();

	// �⡢ѹ��
	bool Compress(const std::string& filepath);
	bool Decompress(const std::string& filepath);

	// �����ѹ�ļ���Ϣ 
	void PrintInfo(const std::string& filepath);

private:
	std::unique_ptr<BytesMap> map;
	std::unique_ptr<HuffmanTree> tree;

	// ��ȡ�б��ļ���ѹ���ļ��ͽ�ѹ�ļ���·��
	std::string get_map_path(const std::string& filepath);
	std::string get_compress_path(const std::string& filepath);
	std::string get_decompress_path(const std::string& filepath);

	// ���桢��ȡ�б��ļ�
	bool ReadMap(const std::string& filepath);
	bool SaveMap(const std::string& filepath);
};

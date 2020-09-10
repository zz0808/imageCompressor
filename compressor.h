#pragma once

#include "bytes_map.h"
#include "huffman_tree.h"

class BytesMap;
class HuffmanTree;

class Compressor {
public:
	Compressor();
	~Compressor();

	// 解、压缩
	bool Compress(const std::string& filepath);
	bool Decompress(const std::string& filepath);

	// 输出解压文件信息 
	void PrintInfo(const std::string& filepath);

private:
	std::unique_ptr<BytesMap> map;
	std::unique_ptr<HuffmanTree> tree;

	// 获取列表文件、压缩文件和解压文件的路径
	std::string get_map_path(const std::string& filepath);
	std::string get_compress_path(const std::string& filepath);
	std::string get_decompress_path(const std::string& filepath);

	// 保存、读取列表文件
	bool ReadMap(const std::string& filepath);
	bool SaveMap(const std::string& filepath);
};

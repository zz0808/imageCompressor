#pragma once
#include "file_helper.h"
#include <vector>
#include <string>

const unsigned short kBytesSize = 256;

class BytesMap {
public:
	BytesMap() : bytes(kBytesSize) {}
	
	bool LoadFile(const std::string& filepath);
	~BytesMap();
	std::vector<std::string> get_codes() const;
	void set_codes(const std::vector<std::string>& codes);

	std::vector<unsigned int> get_counts() const;
	void set_counts(const std::vector<unsigned int>& counts);

private:
	std::vector<std::pair<unsigned int, std::string>> bytes;
};

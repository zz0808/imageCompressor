#include "bytes_map.h"

bool BytesMap::LoadFile(const std::string& filepath) {

	FilePtr fptr(filepath.c_str(), "rb");
	if (!is_file_open_success(fptr)) {
		std::string err_msg = "Error " + filepath + " open failed\n";
		perror(err_msg.c_str());
		return false;
	}

	unsigned char ch = fgetc(fptr.get());

	while (!feof(fptr.get())) {
		++bytes[ch].first;
		ch = fgetc(fptr.get());
	}

	return true;
}

std::vector<std::string> BytesMap::get_codes() const {

	std::vector<std::string> ans(kBytesSize);
	for (size_t i = 0; i < bytes.size(); i++) {
		ans[i] = bytes[i].second;
	}
	return ans;
}

void BytesMap::set_codes(const std::vector<std::string>& codes) {

	int n = codes.size() < bytes.size() ? codes.size() : bytes.size();

	for (size_t i = 0; i < n; i++) {
		bytes[i].second = codes[i];
	}
}

std::vector<unsigned int> BytesMap::get_counts() const {

	std::vector<unsigned int> ans(kBytesSize);

	for (size_t i = 0; i < bytes.size(); i++) {
		ans[i] = bytes[i].first;
	}
	return ans;
}

void BytesMap::set_counts(const std::vector<unsigned int>& counts) {

	int n = counts.size() < bytes.size() ? counts.size() : bytes.size();
	for (size_t i = 0; i < n; i++) {
		bytes[i].first = counts[i];
	}
}

BytesMap::~BytesMap() {

}


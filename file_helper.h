#pragma once
#include <iostream>

// �ļ�����
class FilePtr : public std::unique_ptr<FILE, decltype(std::fclose)*> {
public:
	FilePtr(const char* filename, const char* mode) : std::unique_ptr<FILE, decltype(std::fclose)*>(nullptr, std::fclose) {

		fopen_s(&f, filename, mode);
		this->reset(f);
	}
	~FilePtr() {

	}
private:
	FILE* f;
};


// �ж��ļ��Ƿ�򿪳ɹ�
inline bool is_file_open_success(const FilePtr& fptr ) {
	
	if (!fptr.get()) {
		return false;
	}

	return true;
 }

#define  _CRT_SECURE_NO_WARNINGS
#include "compressor.h"

Compressor::Compressor()
	:  map(std::make_unique<BytesMap>()), tree(std::make_unique<HuffmanTree>()) {

}

bool Compressor::Compress(const std::string& filepath) {

	map->LoadFile(filepath);
	tree = std::make_unique<HuffmanTree>(map->get_counts());
	map->set_codes(tree->get_codes());

	// 打开文件流
	std::string compress_path = std::move(get_compress_path(filepath));
	FilePtr fcom(compress_path.c_str(), "wb"), fsrc(filepath.c_str(), "rb");
	if (!is_file_open_success(fcom) || !is_file_open_success(fsrc))
		return false;

	std::string code;
	unsigned int pos = 0;
	unsigned char new_byte = 0;
	unsigned char src_byte = fgetc(fsrc.get());
	std::vector<std::string> codes = std::move(map->get_codes());
	while (!feof(fsrc.get())) {
		code = codes[static_cast<unsigned int>(src_byte)];
		for (size_t i = 0; i < code.length(); i++) {
			new_byte <<= 1;
			new_byte |= (code[i] == '1') ? 1 : 0;
			++pos;
			if (pos == 8) {
				fputc(new_byte, fcom.get());
				pos = 0;
				new_byte = 0;
			}
		}
		//读取新的字节
		src_byte = fgetc(fsrc.get());
	}

	if (pos) {
		new_byte <<= (8 - pos);
		fputc(new_byte, fcom.get());
	}

	SaveMap(get_map_path(filepath));

	printf("%s %s %db\n", "压缩文件创建：", compress_path.c_str(), ftell(fcom.get()));

	return true;
}

bool Compressor::Decompress(const std::string& filepath) {

	ReadMap(get_map_path(filepath));
	tree = std::make_unique<HuffmanTree>(map->get_counts());
	map->set_codes(tree->get_codes());

	std::string decompress_path = get_decompress_path(filepath), compress_path = get_compress_path(filepath);
	FilePtr fdecom(decompress_path.c_str(), "wb"), fcom(compress_path.c_str(), "rb");
	if (!is_file_open_success(fdecom) || !is_file_open_success(fcom)) {
		return false;
	}

	HuffmanPtr root = tree->get_root(), cur = tree->get_root();
	unsigned int pos = 0;
	unsigned int char_count = root->val;


	unsigned char com_byte = fgetc(fcom.get());
	while (char_count && !feof(fcom.get()) ) {
		//到叶子节点说明找到编码对应的源字节，写入解压文件中
		if (cur->left == nullptr && cur->right == nullptr) {
			fputc(cur->byte, fdecom.get());
			--char_count;
			cur = root;
		} else {
			if (com_byte & (1 << (7 - pos))) {
				cur = cur->right;
			} else {
				cur = cur->left;
			}
			++pos;
			if (pos == 8) {
				pos = 0;
				com_byte = fgetc(fcom.get());
			}
		}
	}
	printf("%s %s %db\n", "解压缩文件创建：", decompress_path.c_str(), ftell(fdecom.get()));
	return true;
}

void Compressor::PrintInfo(const std::string& filepath) {

	std::string compress_path = std::move(get_compress_path(filepath));
	std::string decompress_path = std::move(get_decompress_path(filepath));
	FilePtr fsrc(filepath.c_str(), "rb"), fcom(compress_path.c_str(), "rb"), fdecom(decompress_path.c_str(), "rb");
	if (!is_file_open_success(fsrc) || !is_file_open_success(fcom)
		|| !is_file_open_success(fdecom)) {
		return;
	}

	fseek(fsrc.get(), 0L, SEEK_END);
	fseek(fcom.get(), 0L, SEEK_END);
	fseek(fdecom.get(), 0L, SEEK_END);

	std::cout << "源文件大小      :" << ftell(fsrc.get()) << std::endl;
	std::cout << "压缩文件大小    :" << ftell(fcom.get()) << std::endl;
	std::cout << "解压缩文件大小  :" << ftell(fdecom.get()) << std::endl;
	
	return;
}

std::string Compressor::get_map_path(const std::string& filepath) {
	return filepath + ".map";
}

std::string Compressor::get_compress_path(const std::string& filepath) {
	return filepath + ".compress";
}

std::string Compressor::get_decompress_path(const std::string& filepath) {

	size_t decom_pos = filepath.rfind('\\');
	std::string decompress_path = filepath;
	decompress_path.insert(decom_pos + 1, "decompress_");
	return decompress_path;
}

bool Compressor::ReadMap(const std::string& mappath) {

	FilePtr fmap(mappath.c_str(), "rb");
	if (!is_file_open_success(fmap))
		return false;
	std::string line;
	unsigned char ch = 0, byte_type = 0;
	bool is_special_byte = false;
	std::vector<unsigned int>counts(kBytesSize);

	while (true) {
		ch = fgetc(fmap.get());
		if (feof(fmap.get()))
			break;
		//处理特殊字符
		if (ch == '\0' || ch == '\n') {
			is_special_byte = true;
			byte_type = ch;
			ch = fgetc(fmap.get());
		}
		// 读取一行
		while (!feof(fmap.get()) && ch != '\n') {
			line += ch;
			ch = fgetc(fmap.get());
		}
		if (!line.empty()) {
			if (is_special_byte) { // 特殊字符
				counts[static_cast<unsigned int>(byte_type)] = atoi(line.substr(1).c_str());
			} else {
				byte_type = line[0];
				counts[static_cast<unsigned int>(byte_type)] = atoi(line.substr(2).c_str());
			}
			line.clear();
		}
		is_special_byte = false;
	}
	map->set_counts(counts);
	return true;
}

bool Compressor::SaveMap(const std::string& mappath) {

	std::vector<unsigned int> map_data = std::move(map->get_counts());
	//打开文件
	FilePtr fmap(mappath.c_str(), "wb");
	if (!is_file_open_success(fmap))
		return false;
	std::string line;
	char buff[128];

	for (size_t i = 0; i < map_data.size(); i++) {
		if (map_data[i]) {

			if (i == 0) {
				unsigned char zero = 0;
				fputc(zero, fmap.get());
			} else {
				line += static_cast<unsigned char>(i);
			}
			line += ',';
			line += _itoa(map_data[i], buff, 10);
			line += '\n';
			fputs(line.c_str(), fmap.get());
			line.clear();
		}
	}
	printf("%s\t%s %dKb\n", "Map文件创建: ", mappath.c_str(), ftell(fmap.get()));
	return true;
}

Compressor::~Compressor() {

}

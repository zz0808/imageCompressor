#include "compressor.h"
using namespace std;

int main() {

	string filepath;
	cout << "´ýÑ¹ËõÍ¼ÏñÂ·¾¶£º" << endl;
	cin >> filepath;

	Compressor co;
	co.Compress(filepath);
	co.Decompress(filepath);
	cout << endl;
	co.PrintInfo(filepath);

	return 0;
}
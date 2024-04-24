#include <iostream>
#include "../headers/AffineCoder.h"
#include "../headers/FilesUtil.h"

void prepareInput(string &line, const string &input, const string &tmpLocation);

void cipherInput(enc::AffineCoder& coder, const string& tmpLocation);

int main() {
	string line, input = "../data/in.txt", tmp = "../data/tmp.txt";
	enc::AffineCoder coder = enc::AffineCoder();

	prepareInput(line, input, tmp);
	cipherInput(coder, tmp);
}


void prepareInput(string &line, const string &input, const string &tmpLocation) {
	auto if_stream = files::getInputHandle(input.c_str());
	auto of_stream = files::getOutputHandle(tmpLocation.c_str());

	while (getline(if_stream, line)) {
		const string sanitized_in = enc::AffineCoder::sanitize(line);
		of_stream << sanitized_in << endl;
	}
	of_stream.close();
	if_stream.close();
}

void cipherInput(enc::AffineCoder& coder, const string& tmpLocation) {
	string sanitized_in;
	string cipherResultLocation = "../result/ciphered.txt";
	const map<char, char> &keyMap = coder.randomizeKeyMap();
	
	coder.setKeysMap(keyMap);
	
	auto if_stream = files::getInputHandle(tmpLocation.c_str());
	auto of_stream = files::getOutputHandle(cipherResultLocation.c_str());

	while (getline(if_stream, sanitized_in)) {
		const string ciphered_in = coder.encode(sanitized_in);
		of_stream << ciphered_in << endl;
	}

	of_stream.close();
	if_stream.close();

	cout << "Ciphering input completed with keys:" << endl;
	for (const auto &[k, v]: coder.getKeysMap()) {
		cout << "Key:" << k << " -> Val:" << v << endl;
	}
}
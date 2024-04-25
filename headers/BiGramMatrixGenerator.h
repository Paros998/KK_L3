//
// Created by part4 on 25.04.2024.
//

#ifndef KK_L3_BIGRAMMATRIXGENERATOR_H
#define KK_L3_BIGRAMMATRIXGENERATOR_H

#include <vector>
#include <string>
#include <iostream>

#include "FilesUtil.h"

using namespace std;

namespace matrix {
	const int EN_ALPHABET_SIZE = 26;
	const int EN_FIRST_LETTER_ASCII = 'A';

	static vector<vector<int>> generateBiGramMatrix(const string &inFile) {
		vector<vector<int>> matrix(EN_ALPHABET_SIZE, vector<int>(EN_ALPHABET_SIZE, 0));

		auto if_stream = files::getInputHandle(inFile.c_str());

		if (!if_stream.is_open()) {
			cerr << "Cannot read from input file:" << inFile << endl;
		}

		size_t i;
		string line;
		while (getline(if_stream, line) && !line.empty()) {
			line = enc::AffineCoder::sanitize(line);
			for (i = 0; i < line.size() - 1; ++i)
				if (std::isalpha(line[i]) && std::isalpha(line[i + 1])) {
					int row = line[i] - EN_FIRST_LETTER_ASCII;
					int col = line[i + 1] - EN_FIRST_LETTER_ASCII;
					matrix[row][col]++;
				}
		}

		if_stream.close();
		return matrix;
	}
}

#endif //KK_L3_BIGRAMMATRIXGENERATOR_H

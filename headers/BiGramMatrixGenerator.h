//
// Created by part4 on 25.04.2024.
//

#ifndef KK_L3_BIGRAMMATRIXGENERATOR_H   // Header guard to prevent multiple inclusions
#define KK_L3_BIGRAMMATRIXGENERATOR_H

#include <vector>   // Include necessary header for std::vector
#include <string>   // Include necessary header for std::string
#include <iostream> // Include necessary header for input/output operations

#include "FilesUtil.h"  // Include necessary header for file utilities
// (the implementation of this header is not provided)

using namespace std;

namespace matrix {
	const int EN_ALPHABET_SIZE = 26;   // Constant representing the size of the English alphabet
	const int EN_FIRST_LETTER_ASCII = 'A'; // Constant representing the ASCII value of the first letter ('A')

	// Function to generate a bigram matrix from an input file
	static vector<vector<int>> generateBiGramMatrix(const string &inFile) {
		// Initialize a 2D vector (matrix) to store bigram frequencies
		vector<vector<int>> matrix(EN_ALPHABET_SIZE, vector<int>(EN_ALPHABET_SIZE, 0));

		// Open the input file for reading
		auto if_stream = files::getInputHandle(inFile.c_str());

		// Check if the input file is open
		if (!if_stream.is_open()) {
			// Print an error message if the file cannot be opened
			cerr << "Cannot read from input file:" << inFile << endl;
		}

		// Declare variables for iteration and storing input lines
		size_t i;
		string line;

		// Read lines from the input file until EOF or an empty line is encountered
		while (getline(if_stream, line) && !line.empty()) {
			// Sanitize the input line using the AffineCipher::sanitize function (not implemented here)
			line = enc::AffineCoder::sanitize(line);

			// Iterate over the characters in the sanitized line to count bigrams
			for (i = 0; i < line.size() - 1; ++i) {
				// Check if both characters are alphabetic
				if (std::isalpha(line[i]) && std::isalpha(line[i + 1])) {
					// Calculate the row and column indices in the matrix based on ASCII values
					int row = line[i] - EN_FIRST_LETTER_ASCII;
					int col = line[i + 1] - EN_FIRST_LETTER_ASCII;

					// Increment the corresponding element in the matrix
					matrix[row][col]++;
				}
			}
		}

		// Close the input file
		if_stream.close();

		// Return the generated bigram matrix
		return matrix;
	}
}

#endif //KK_L3_BIGRAMMATRIXGENERATOR_H   // End of header guard

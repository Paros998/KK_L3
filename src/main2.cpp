#include <iostream>
#include "../headers/AffineCoder.h"
#include "../headers/BiGramMatrixGenerator.h"

const std::string BEST_PERMUTATION_KEY = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

void shuffleTwoSignsInPermutation(std::string &permutation) {
	int firstIndex = enc::AffineCoder::randomInRange(0, matrix::EN_ALPHABET_SIZE - 1);
	int secondIndex = enc::AffineCoder::randomInRange(0, matrix::EN_ALPHABET_SIZE - 1);
	swap(permutation[firstIndex], permutation[secondIndex]);
}

double calculateProbability(const vector<vector<int>> &referenceM, const vector<vector<int>> &cipheredM,
                            const std::string &permutationKey) {
	double probability = 1.0;
	for (int i = 0; i < matrix::EN_ALPHABET_SIZE; ++i) {
		for (int j = 0; j < matrix::EN_ALPHABET_SIZE; ++j) {
			const int index1 = permutationKey[i] - matrix::EN_FIRST_LETTER_ASCII;
			const int index2 = permutationKey[j] - matrix::EN_FIRST_LETTER_ASCII;
			probability *= std::pow(cipheredM[index1][index2] + 1, referenceM[i][j]);
		}
	}
	return probability;
}

std::string breakCipher(const std::string &referenceFile, const std::string &cipheredFile, const int iterations) {
	auto referenceMatrix = matrix::generateBiGramMatrix(referenceFile);
	auto cipherMatrix = matrix::generateBiGramMatrix(cipheredFile);
	double bestProbability = calculateProbability(referenceMatrix, cipherMatrix, BEST_PERMUTATION_KEY);

	std::string resolvedKey = BEST_PERMUTATION_KEY;
	for (int i = 0; i < iterations; ++i) {
		std::string currentKey = resolvedKey;
		shuffleTwoSignsInPermutation(currentKey);
		auto permutationProbability = calculateProbability(referenceMatrix, cipherMatrix, currentKey);

		if (permutationProbability > bestProbability) {
			resolvedKey = currentKey;
			bestProbability = permutationProbability;
		} else {
			// WTF
		}
	}

	return resolvedKey;
}

std::string decryptWithPermutation(const std::string &in, const std::string &permutation) {
	std::string result;
	for (auto c: in) {
		if (std::isalpha(c)) {
			result += static_cast<char>(matrix::EN_FIRST_LETTER_ASCII + permutation.find(c));
		}
	}
	return result;
}

int main() {
	std::string reference = "../data/in.txt", ciphered = "../result/ciphered.txt", result = "../result/deciphered.txt",
	resultKey = "../result/deciphered_key.txt";

	auto key = breakCipher(reference, ciphered, 10000);
	std::cout << "Found key: " << key << std::endl << "Starting decryption" << endl;
	auto of_stream = files::getOutputHandle(resultKey.c_str());
	if (of_stream.is_open()) {
		of_stream << key;
	}
	of_stream.close();

	auto if_stream = files::getInputHandle(ciphered.c_str());
	of_stream = files::getOutputHandle(result.c_str());

	if (!if_stream.is_open() || !of_stream.is_open()) {
		std::cerr << "Cannot decrypt, I/O error" << endl;
	}

	std::string line;
	while (getline(if_stream, line)) {
		auto decipheredIn = decryptWithPermutation(line, key);
		of_stream << decipheredIn << endl;
	}

	std::cout << "Decryption completed" << endl;
	of_stream.close();
	if_stream.close();
}
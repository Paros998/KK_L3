#include <iostream>
#include "../headers/AffineCoder.h" // Include the header file for the AffineCoder class
#include "../headers/BiGramMatrixGenerator.h" // Include the header file for the BiGramMatrixGenerator namespace

const std::string BEST_PERMUTATION_KEY = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"; // Define the best permutation key as a constant

// Function to shuffle two characters in a permutation string
void shuffleTwoSignsInPermutation(std::string &permutation) {
	int firstIndex = enc::AffineCoder::randomInRange(0, matrix::EN_ALPHABET_SIZE - 1); // Generate a random index within the alphabet size
	int secondIndex = enc::AffineCoder::randomInRange(0, matrix::EN_ALPHABET_SIZE - 1); // Generate another random index within the alphabet size
	std::swap(permutation[firstIndex], permutation[secondIndex]); // Swap the characters at the two random indices in the permutation string
}

// Function to calculate the probability of a permutation based on reference and ciphered bi-gram matrices
double calculateProbability(const vector<vector<int>> &referenceM, const vector<vector<int>> &cipheredM,
                            const std::string &permutationKey) {
	double probability = 1.0; // Initialize the probability to 1.0
	for (int i = 0; i < matrix::EN_ALPHABET_SIZE; ++i) { // Iterate over rows of the matrices
		for (int j = 0; j < matrix::EN_ALPHABET_SIZE; ++j) { // Iterate over columns of the matrices
			const int index1 = permutationKey[i] - matrix::EN_FIRST_LETTER_ASCII; // Calculate the index in the permutation key
			const int index2 = permutationKey[j] - matrix::EN_FIRST_LETTER_ASCII; // Calculate the index in the permutation key
			probability *= std::pow(cipheredM[index1][index2] + 1, referenceM[i][j]); // Update the probability based on the counts in the matrices
		}
	}
	return probability; // Return the calculated probability
}

// Function to break the cipher using the reference and ciphered bi-gram matrices and iterations for shuffling
std::string breakCipher(const std::string &referenceFile, const std::string &cipheredFile, const int iterations) {
	auto referenceMatrix = matrix::generateBiGramMatrix(referenceFile); // Generate the bi-gram matrix for the reference text
	auto cipherMatrix = matrix::generateBiGramMatrix(cipheredFile); // Generate the bi-gram matrix for the ciphered text
	double bestProbability = calculateProbability(referenceMatrix, cipherMatrix, BEST_PERMUTATION_KEY); // Calculate the probability for the best permutation key

	std::string resolvedKey = BEST_PERMUTATION_KEY; // Initialize the resolved key with the best permutation key
	for (int i = 0; i < iterations; ++i) { // Iterate for the specified number of iterations
		std::string currentKey = resolvedKey; // Initialize the current key with the resolved key
		shuffleTwoSignsInPermutation(currentKey); // Shuffle two characters in the current key
		auto permutationProbability = calculateProbability(referenceMatrix, cipherMatrix, currentKey); // Calculate the probability for the current permutation key

		if (permutationProbability > bestProbability) { // If the probability for the current permutation is higher than the best probability
			resolvedKey = currentKey; // Update the resolved key with the current key
			bestProbability = permutationProbability; // Update the best probability with the current probability
		}
	}

	return resolvedKey; // Return the resolved key
}

// Function to decrypt the input string using a given permutation key
std::string decryptWithPermutation(const std::string &in, const std::string &permutation) {
	std::string result; // Initialize the result string
	for (auto c: in) { // Iterate over characters in the input string
		if (std::isalpha(c)) { // If the character is alphabetic
			result += static_cast<char>(matrix::EN_FIRST_LETTER_ASCII + permutation.find(c)); // Append the corresponding decrypted character to the result string
		}
	}
	return result; // Return the decrypted result string
}

// Main function
int main() {
	std::string reference = "../data/in.txt", ciphered = "../result/ciphered.txt", result = "../result/deciphered.txt", // File paths for reference, ciphered, and result files
	resultKey = "../result/deciphered_key.txt"; // File path for the deciphered key

	auto key = breakCipher(reference, ciphered, 10000); // Break the cipher and obtain the key
	std::cout << "Found key: " << key << std::endl << "Starting decryption" << std::endl; // Output the found key and start decryption process

	auto of_stream = files::getOutputHandle(resultKey.c_str()); // Open output file stream for the deciphered key
	if (of_stream.is_open()) { // If the output file stream is open
		of_stream << key; // Write the key to the output file
	}
	of_stream.close(); // Close the output file stream

	auto if_stream = files::getInputHandle(ciphered.c_str()); // Open input file stream for the ciphered text
	of_stream = files::getOutputHandle(result.c_str()); // Open output file stream for the deciphered text

	if (!if_stream.is_open() || !of_stream.is_open()) { // If either input or output file streams are not open
		std::cerr << "Cannot decrypt, I/O error" << std::endl; // Output an error message
	}

	std::string line; // Variable to store each line of input text
	while (getline(if_stream, line)) { // Read each line from the input file
		auto decipheredIn = decryptWithPermutation(line, key); // Decrypt the line using the obtained key
		of_stream << decipheredIn << std::endl; // Write the decrypted line to the output file
	}

	std::cout << "Decryption completed" << std::endl; // Output message indicating completion
	of_stream.close(); // Close the output file stream
	if_stream.close(); // Close the input file stream

	return 0; // Return 0 to indicate successful execution
}

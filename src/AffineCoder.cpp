//
// Created by part4 on 24.04.2024.
//

#include "../headers/AffineCoder.h"  // Include the corresponding header file

namespace enc {
	// Function to encode a character using the keys map
	char AffineCoder::encodeChar(const char in) const {
		// Find the character in the keys map
		const auto find = this->keys_map_.find(in);
		// If the character is not found, return the original character
		if (find->second == 0) return in;
		// Otherwise, return the encoded character
		return find->second;
	}

	// Function to decode a character using the keys map
	char AffineCoder::decodeChar(const char in) const {
		// Iterate through the keys map
		for (auto [key, value]: this->keys_map_) {
			// If the value matches the input character, return the corresponding key
			if (value == in) return key;
		}
		// If the character is not found in the keys map, return the original character
		return in;
	}

	// Function to generate a random integer in the specified range
	int AffineCoder::randomInRange(int min, int max) {
		// Seed the random number generator
		std::random_device rd;
		std::mt19937 eng(rd());
		// Define the range for the distribution
		std::uniform_int_distribution<> distribution(min, max);

		// Generate and return the random number
		return distribution(eng);
	}

	// Function to generate a random keys map
	map<char, char> AffineCoder::randomizeKeyMap() {
		char randomSign;
		map<char, char> keysMap = map<char, char>();
		vector<int> lettersUsed;

		// Iterate through the letters
		for (const auto c: letters) {
			// Generate a random sign
			randomSign = -1;
			do {
				randomSign = static_cast<char>(randomInRange(65, 90));
			} while (binary_search(lettersUsed.begin(), lettersUsed.end(), static_cast<int>(randomSign)));

			// Insert the pair into the keys map and mark the letter as used
			keysMap.insert({c, randomSign});
			lettersUsed.push_back(static_cast<int>(randomSign));
			sort(lettersUsed.begin(), lettersUsed.end());
		}
		return keysMap;
	}

	// Function to sanitize a string (convert lowercase to uppercase)
	string AffineCoder::sanitize(const string &in) {
		string sanitized_in;

		// Iterate through the input string
		for (const auto c: in) {
			// Convert lowercase letters to uppercase
			if (c >= 97 && c <= 122) {
				sanitized_in += static_cast<char>(std::toupper(c));
				continue;
			}

			// Keep uppercase letters unchanged
			if (c >= 65 && c <= 90) {
				sanitized_in += c;
			}
		}

		return sanitized_in;  // Return the sanitized string
	}

	// Function to encode a string using the keys map
	string AffineCoder::encode(const string &in) {
		string out;
		// Iterate through the input string
		for (const auto c: in) {
			// Encode each character and append it to the output string
			const char encoded_char = encodeChar(c);
			out += encoded_char;
		}
		return out;  // Return the encoded string
	}

	// Function to decode a string using the keys map
	string AffineCoder::decode(const string &in) {
		string out;
		// Iterate through the input string
		for (const auto c: in) {
			// Decode each character and append it to the output string
			const char decoded_char = decodeChar(c);
			out += decoded_char;
		}
		return out;  // Return the decoded string
	}
}

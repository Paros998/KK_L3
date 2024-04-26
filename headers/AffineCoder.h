//
// Created by part4 on 24.04.2024.
//

#ifndef KK_L3_AFFINECODER_H   // Header guard to prevent multiple inclusions
#define KK_L3_AFFINECODER_H

#include <map>      // Include necessary header for std::map
#include <string>   // Include necessary header for std::string
#include <random>   // Include necessary header for random number generation
#include <vector>   // Include necessary header for std::vector
#include <algorithm>    // Include necessary header for std::generate_n

using namespace std;

namespace enc {
	class AffineCoder {
		// Private member variables
		map<char, char> keys_map_{};    // Map to store character substitution keys
		vector<char> letters;   // Vector to store English alphabet characters

		// Private member functions for character encoding and decoding
		[[nodiscard]] char encodeChar(char in) const;
		[[nodiscard]] char decodeChar(char in) const;

	public:
		// Static member function to generate a random integer within a range
		static int randomInRange(int min, int max);

		// Static member function to sanitize input text (not implemented in this header)
		static string sanitize(const string &in);

		// Constructor to initialize the English alphabet characters
		AffineCoder() {
			for (int i = 65; i <= 90; i++) {
				letters.push_back(static_cast<char>(i));
			}
		};

		// Destructor (default)
		~AffineCoder() = default;

		// Getter function to retrieve the keys map
		[[nodiscard]] const map<char, char> &getKeysMap() const {
			return keys_map_;
		}

		// Setter function to set the keys map
		void setKeysMap(const map<char, char> &keysMap) {
			keys_map_ = keysMap;
		}

		// Function to encode input text using the Affine cipher
		string encode(const string &in);

		// Function to decode input text using the Affine cipher
		string decode(const string &in);

		// Function to generate a random key map for the Affine cipher
		map<char, char> randomizeKeyMap();
	};
}

#endif //KK_L3_AFFINECODER_H   // End of header guard

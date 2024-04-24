//
// Created by part4 on 24.04.2024.
//

#include "../headers/AffineCoder.h"

namespace enc {
	char AffineCoder::encodeChar(const char in) const {
		const auto find = this->keys_map_.find(in);
		if (find->second == 0) return in;
		return find->second;
	}

	char AffineCoder::decodeChar(const char in) const {
		for (auto [key, value]: this->keys_map_) {
			if (value == in) return key;
		}
		return in;
	}

	int AffineCoder::randomInRange(int min, int max) {
		std::random_device rd;
		std::mt19937 eng(rd()); // Seed the generator
		std::uniform_int_distribution<> distribution(min, max); // Define the range

		return distribution(eng); // Generate the random number
	}

	map<char, char> AffineCoder::randomizeKeyMap() {
		char randomSign;
		map<char, char> keysMap = map<char, char>();
		vector<int> lettersUsed;

		for (const auto c: letters) {
			randomSign = -1;
			do {
				randomSign = static_cast<char>(randomInRange(65, 90));
			} while (binary_search(lettersUsed.begin(), lettersUsed.end(), static_cast<int>(randomSign)));

			keysMap.insert({c, randomSign});
			lettersUsed.push_back(static_cast<int>(randomSign));
			sort(lettersUsed.begin(), lettersUsed.end());
		}
		return keysMap;
	}

	string AffineCoder::sanitize(const string &in) {
		string sanitized_in;

		for (const auto c: in) {
			if (c >= 97 && c <= 122) {
				sanitized_in += static_cast<char>(std::toupper(c));
				continue;
			}

			if (c >= 65 && c <= 90) {
				sanitized_in += c;
			}
		}

		return sanitized_in;
	}

	string AffineCoder::encode(const string &in) {
		string out;
		for (const auto c: in) {
			const char encoded_char = encodeChar(c);
			out += encoded_char;
		}
		return out;
	}

	string AffineCoder::decode(const string &in) {
		string out;
		for (const auto c: in) {
			const char decoded_char = decodeChar(c);
			out += decoded_char;
		}
		return out;
	}
}
//
// Created by part4 on 24.04.2024.
//

#ifndef KK_L3_AFFINECODER_H
#define KK_L3_AFFINECODER_H

#include <map>
#include <string>
#include <random>
#include <vector>
#include <algorithm>

using namespace std;

namespace enc {
	class AffineCoder {
		static int randomInRange(int min, int max);

		map<char, char> keys_map_{};
		vector<char> letters;

		[[nodiscard]] char encodeChar(char in) const;

		[[nodiscard]] char decodeChar(char in) const;

	public:
		static string sanitize(const string &in);

		AffineCoder() {
			for (int i = 65; i <= 90; i++) {
				letters.push_back(static_cast<char>(i));
			}
		};

		~AffineCoder() = default;

		[[nodiscard]] const map<char, char> &getKeysMap() const {
			return keys_map_;
		}

		void setKeysMap(const map<char, char> &keysMap) {
			keys_map_ = keysMap;
		}

		string encode(const string &in);

		string decode(const string &in);

		map<char, char> randomizeKeyMap();
	};
}


#endif //KK_L3_AFFINECODER_H

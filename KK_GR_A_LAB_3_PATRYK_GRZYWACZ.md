# Kryptografia i kryptoanaliza
## Laboratorium 3
### Grupa 1IZ22A
### Autor: Patryk Grzywacz

### Zadanie 1

Zrealizować program implementujący podstawieniowy algorytm szyfrowania.
1. Wybrać fragment tekstu w języku angielskim.
2. Usunąć z niego wszystkie znaki nie będące literami (ograniczenie do 26 liter alfabetu łacińskiego).
3. Zaszyfrować tekst używając wybranego w sposób losowy klucza (tablicy podstawień): permutacji π

### Zadanie 2

Mając do dyspozycji, otrzymany w ramach pierwszego zadania szyfrogram, dokonać ataku na zaimplementowany kryptosystem wykorzystując Algorytm 1:

Opis algorytmu - postać ogólna:

• Rozpocznij od początkowego przypuszczenia πˆ dla permutacji dekodowania;

• Dla πˆ oblicz wiarygodność: Pl (ˆπ) na zaszyfrowanym tekście;

• Powtórz następujące kroki dla wystarczającej liczby iteracji:

– Zamień losowo πˆ poprzez zamianę dwóch symboli z permutacji πˆ; nowa permutacja jest oznaczana
jako πˆ′

– Oblicz wiarygodność dla πˆ′: Pl (ˆπ ′)

Uwagi:

1. πˆ - permutacja klucza (tablicy podstawień).
2. Pl (ˆπ) - funkcja wiarygodności zdefiniowana: Pl (ˆπ) = Q i,j (Mi,j )Mˆ i,j .

   • M - macierz bigramów utworzona na bazie tekstu referencyjnego, Mi,j - liczba wystąpień pary(i, j) w tekście referencyjnym.

   • Mˆ - macierz bigramów utworzona na bazie szyfrogramu, Mˆ i,j - liczba wystąpień pary (i, j) w szyfrogramie.

3. Dla danej permutacji πˆ, rozkład prawdopodobieństwa qπˆπˆ ′ : ˆπ ′ ∈ χ jest zdefiniowany w następujący sposób:

   • qπˆπˆ ′ = 1 262 jeśli przejście od πˆ do πˆ ′ może być dokonane przez losową zamianę dwóch wartości, oraz

   • qπˆπˆ ′ = 0 w przeciwnym przypadku.

gdzie: χ to przestrzeń stanów wszystkich możliwych kluczy, które odpowiadają wszystkim możliwym
permutacjom, w rozważanym przypadku wszystkich możliwych permutacji jest 26!.

4. Algorytm, na bazie funkcji wiarygodności, wygeneruje sekwencję kluczy deszyfrujących: {Xt: t = 0, . . . , T}.

6. Jeśli U ({1, 2, . . . , 26}) oznacza dyskretny rozkład jednostajny na liczbach całkowitych od 1 do 26, to
   ostatecznie implementacja algorytmu dla szyfru podstawieniowego przyjmuje postać:

Algorithm 1 MH

```
1: t ← 0

2: X0 ← πˆ0

3: for t = 1, . . . , T do

4:      dla Xt ← πˆ
        
5:      wygeneruj i, j ∼ U({1, 2, . . . , 26}) ▷ ∼ znaczy ma rozkład
        
6:      wygeneruj πˆ ′ ▷ zamieniając znaki na pozycjach i oraz j w kluczu πˆ
        
7:      ρ(ˆπ, πˆ ′) ← Pl(ˆπ ′)Pl(π)▷ ρ - prawdopodobieństwo akceptacji
        
8:      wygeneruj u ∼ U([0, 1])
        
9:      if u ≤ ρ(ˆπ, πˆ ′) then
        
10:      Xt+1 ← πˆ ′
        
11:      else
        
12:      Xt+1 ← πˆ
        
13:      end if

14: end for
```

### Kody źródłowe - headery klas

* AffineCoder.h

``` C++
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

```

* BiGramMatrixGenerator.h

``` C++
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

```

* FilesUtil.h

``` C++
//
// Created by part4 on 24.04.2024.
//

#ifndef KK_L3_FILESUTIL_H   // Header guard to prevent multiple inclusions
#define KK_L3_FILESUTIL_H

#include <fstream>  // Include necessary header for file input/output operations

using namespace std;

namespace files {
	// Function to get an input file handle
	std::ifstream getInputHandle(const char *file) {
		// Return an input file stream object associated with the specified file
		return ifstream(file);
	}

	// Function to get an output file handle
	std::ofstream getOutputHandle(const char *file) {
		// Return an output file stream object associated with the specified file
		return ofstream(file);
	}
}

#endif //KK_L3_FILESUTIL_H   // End of header guard

```

### Kody źródłowe - źródła klas

* AffineCoder.cpp

``` C++
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

```

* main1.cpp - Kod wykonawczy dla zadania 1

``` C++
#include <iostream>
#include "../headers/AffineCoder.h" // Include the header file for the AffineCoder class
#include "../headers/FilesUtil.h" // Include the header file for the FilesUtil namespace

// Function to prepare the input data by sanitizing each line and writing it to a temporary file
void prepareInput(string &line, const string &input, const string &tmpLocation) {
	auto if_stream = files::getInputHandle(input.c_str()); // Open input file stream
	auto of_stream = files::getOutputHandle(tmpLocation.c_str()); // Open output file stream for temporary file

	// Read each line from the input file
	while (getline(if_stream, line)) {
		const string sanitized_in = enc::AffineCoder::sanitize(line); // Sanitize the line
		of_stream << sanitized_in << endl; // Write the sanitized line to the temporary file
	}

	of_stream.close(); // Close the output file stream
	if_stream.close(); // Close the input file stream
}

// Function to apply an affine cipher to the input data and save the ciphered result and key to files
void cipherInput(enc::AffineCoder& coder, const string& tmpLocation) {
	string sanitized_in;
	string cipherResultLocation = "../result/ciphered.txt"; // File path for the ciphered result
	string cipherResultKeyLocation = "../result/ciphered_key.txt"; // File path for the ciphered key
	const map<char, char> &keyMap = coder.randomizeKeyMap(); // Generate a random key map

	coder.setKeysMap(keyMap); // Set the keys map in the AffineCoder object

	auto if_stream = files::getInputHandle(tmpLocation.c_str()); // Open input file stream for temporary file
	auto of_stream = files::getOutputHandle(cipherResultLocation.c_str()); // Open output file stream for ciphered result

	// Read each line from the temporary file
	while (getline(if_stream, sanitized_in)) {
		const string ciphered_in = coder.encode(sanitized_in); // Encode the sanitized line using the AffineCoder
		of_stream << ciphered_in << endl; // Write the ciphered line to the ciphered result file
	}

	of_stream.close(); // Close the output file stream
	if_stream.close(); // Close the input file stream

	cout << "Ciphering input completed with keys:" << endl; // Output message indicating completion

	of_stream = files::getOutputHandle(cipherResultKeyLocation.c_str()); // Open output file stream for ciphered key

	// Write the key map to the ciphered key file and output each key-value pair
	for (const auto &[k, v]: coder.getKeysMap()) {
		cout << "Key:" << k << " -> Val:" << v << endl; // Output key-value pair
		of_stream << v; // Write the value (key) to the ciphered key file
	}

	of_stream.close(); // Close the output file stream
}

// Main function
int main() {
	string line, input = "../data/in.txt", tmp = "../data/tmp.txt"; // File paths for input and temporary files
	enc::AffineCoder coder = enc::AffineCoder(); // Create an AffineCoder object

	// Prepare the input data by sanitizing and writing it to a temporary file
	prepareInput(line, input, tmp);
	// Apply the affine cipher to the prepared input and save the result and key to files
	cipherInput(coder, tmp);

	return 0; // Return 0 to indicate successful execution
}

```

* main2.cpp - Kod wykonawczy dla zadania 2

``` C++
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

```

### Wyniki

Do testów zadań użyto następującego tekstu referencyjnego
```
She stood by the window, gazing out at the city below as the sun dipped low on the horizon. It had been a long day at work, but the weariness melted away as she heard the familiar sound of his footsteps approaching. With a soft smile playing on her lips, she turned to face him, her heart skipping a beat at the sight of him.

"Hey," he said, his voice warm and filled with affection as he crossed the room to wrap her in his arms. "Rough day?"

She nodded, leaning into his embrace, finding solace in his presence. "But seeing you makes it all better."

They stood there for a moment, lost in each other's arms, the world outside fading into insignificance. In that moment, nothing else mattered but the love they shared.

The night was still, the air heavy with anticipation as Detective Miller surveyed the crime scene. A chill ran down his spine as he studied the scene before him – the victim lying motionless on the ground, surrounded by a pool of blood that glistened in the moonlight.

Something didn't add up. There were no signs of struggle, no clues left behind to hint at the perpetrator. It was as if the killer had vanished into thin air, leaving nothing but questions in their wake.

Miller narrowed his eyes, his mind racing as he pieced together the puzzle before him. This was no ordinary case; this was a challenge unlike any he had faced before. But he was determined to crack it, to bring justice to the victim and closure to their loved ones.

The darkness pressed in around her as she navigated the labyrinthine corridors of the abandoned asylum. Every sound echoed ominously, sending shivers down her spine as she pressed on, her heart pounding in her chest.

She knew the risks of coming here alone, but the lure of the unknown was too strong to resist. There were secrets hidden within these walls, secrets that begged to be uncovered, no matter the cost.

As she rounded a corner, a figure loomed before her, their eyes gleaming in the darkness. She froze, her breath catching in her throat as fear gripped her heart. But she refused to back down, steeling herself for whatever lay ahead.

The forest was alive with magic, the air humming with the whispers of ancient spells and enchantments. Creatures of myth and legend darted through the underbrush, their eyes alight with curiosity as they watched the lone traveler making her way through the dense foliage.

She was on a quest, driven by a sense of purpose that burned bright within her soul. With each step, she drew closer to her destiny, guided by the wisdom of the stars and the guidance of her trusty companions.

But danger lurked around every corner, testing her resolve and pushing her to the limits of her strength. Yet she pressed on, fueled by courage and determination, knowing that the fate of the world rested on her shoulders.

These longer examples offer a glimpse into the immersive worlds and captivating narratives often found in light literature.
```

#### Zadanie 1

Uruchomienie aplikacji main z exe `KK_L3_1`

Daje rezultat:

```
Ciphering input completed with keys:
Key:A -> Val:L
Key:B -> Val:C
Key:C -> Val:D
Key:D -> Val:E
Key:E -> Val:H
Key:F -> Val:P
Key:G -> Val:F
Key:H -> Val:Y
Key:I -> Val:Q
Key:J -> Val:J
Key:K -> Val:K
Key:L -> Val:O
Key:M -> Val:M
Key:N -> Val:R
Key:O -> Val:Z
Key:P -> Val:X
Key:Q -> Val:S
Key:R -> Val:A
Key:S -> Val:I
Key:T -> Val:G
Key:U -> Val:U
Key:V -> Val:N
Key:W -> Val:B
Key:X -> Val:W
Key:Y -> Val:T
Key:Z -> Val:V

Process finished with exit code 0
```

i szyfrogram:

```
IYHIGZZECTGYHBQREZBFLVQRFZUGLGGYHDQGTCHOZBLIGYHIUREQXXHEOZBZRGYHYZAQVZRQGYLECHHRLOZRFELTLGBZAKCUGGYHBHLAQRHIIMHOGHELBLTLIIYHYHLAEGYHPLMQOQLAIZUREZPYQIPZZGIGHXILXXAZLDYQRFBQGYLIZPGIMQOHXOLTQRFZRYHAOQXIIYHGUARHEGZPLDHYQMYHAYHLAGIKQXXQRFLCHLGLGGYHIQFYGZPYQM

YHTYHILQEYQINZQDHBLAMLREPQOOHEBQGYLPPHDGQZRLIYHDAZIIHEGYHAZZMGZBALXYHAQRYQILAMIAZUFYELT

IYHRZEEHEOHLRQRFQRGZYQIHMCALDHPQREQRFIZOLDHQRYQIXAHIHRDHCUGIHHQRFTZUMLKHIQGLOOCHGGHA

GYHTIGZZEGYHAHPZALMZMHRGOZIGQRHLDYZGYHAILAMIGYHBZAOEZUGIQEHPLEQRFQRGZQRIQFRQPQDLRDHQRGYLGMZMHRGRZGYQRFHOIHMLGGHAHECUGGYHOZNHGYHTIYLAHE

GYHRQFYGBLIIGQOOGYHLQAYHLNTBQGYLRGQDQXLGQZRLIEHGHDGQNHMQOOHAIUANHTHEGYHDAQMHIDHRHLDYQOOALREZBRYQIIXQRHLIYHIGUEQHEGYHIDHRHCHPZAHYQMGYHNQDGQMOTQRFMZGQZROHIIZRGYHFAZUREIUAAZUREHECTLXZZOZPCOZZEGYLGFOQIGHRHEQRGYHMZZROQFYG

IZMHGYQRFEQERGLEEUXGYHAHBHAHRZIQFRIZPIGAUFFOHRZDOUHIOHPGCHYQREGZYQRGLGGYHXHAXHGALGZAQGBLILIQPGYHKQOOHAYLENLRQIYHEQRGZGYQRLQAOHLNQRFRZGYQRFCUGSUHIGQZRIQRGYHQABLKH

MQOOHARLAAZBHEYQIHTHIYQIMQREALDQRFLIYHXQHDHEGZFHGYHAGYHXUVVOHCHPZAHYQMGYQIBLIRZZAEQRLATDLIHGYQIBLILDYLOOHRFHUROQKHLRTYHYLEPLDHECHPZAHCUGYHBLIEHGHAMQRHEGZDALDKQGGZCAQRFJUIGQDHGZGYHNQDGQMLREDOZIUAHGZGYHQAOZNHEZRHI

GYHELAKRHIIXAHIIHEQRLAZUREYHALIIYHRLNQFLGHEGYHOLCTAQRGYQRHDZAAQEZAIZPGYHLCLREZRHELITOUMHNHATIZUREHDYZHEZMQRZUIOTIHREQRFIYQNHAIEZBRYHAIXQRHLIIYHXAHIIHEZRYHAYHLAGXZUREQRFQRYHADYHIG

IYHKRHBGYHAQIKIZPDZMQRFYHAHLOZRHCUGGYHOUAHZPGYHURKRZBRBLIGZZIGAZRFGZAHIQIGGYHAHBHAHIHDAHGIYQEEHRBQGYQRGYHIHBLOOIIHDAHGIGYLGCHFFHEGZCHURDZNHAHERZMLGGHAGYHDZIG

LIIYHAZUREHELDZARHALPQFUAHOZZMHECHPZAHYHAGYHQAHTHIFOHLMQRFQRGYHELAKRHIIIYHPAZVHYHACAHLGYDLGDYQRFQRYHAGYAZLGLIPHLAFAQXXHEYHAYHLAGCUGIYHAHPUIHEGZCLDKEZBRIGHHOQRFYHAIHOPPZABYLGHNHAOLTLYHLE

GYHPZAHIGBLILOQNHBQGYMLFQDGYHLQAYUMMQRFBQGYGYHBYQIXHAIZPLRDQHRGIXHOOILREHRDYLRGMHRGIDAHLGUAHIZPMTGYLREOHFHREELAGHEGYAZUFYGYHUREHACAUIYGYHQAHTHILOQFYGBQGYDUAQZIQGTLIGYHTBLGDYHEGYHOZRHGALNHOHAMLKQRFYHABLTGYAZUFYGYHEHRIHPZOQLFH

IYHBLIZRLSUHIGEAQNHRCTLIHRIHZPXUAXZIHGYLGCUARHECAQFYGBQGYQRYHAIZUOBQGYHLDYIGHXIYHEAHBDOZIHAGZYHAEHIGQRTFUQEHECTGYHBQIEZMZPGYHIGLAILREGYHFUQELRDHZPYHAGAUIGTDZMXLRQZRI

CUGELRFHAOUAKHELAZUREHNHATDZARHAGHIGQRFYHAAHIZONHLREXUIYQRFYHAGZGYHOQMQGIZPYHAIGAHRFGYTHGIYHXAHIIHEZRPUHOHECTDZUALFHLREEHGHAMQRLGQZRKRZBQRFGYLGGYHPLGHZPGYHBZAOEAHIGHEZRYHAIYZUOEHAI

GYHIHOZRFHAHWLMXOHIZPPHALFOQMXIHQRGZGYHQMMHAIQNHBZAOEILREDLXGQNLGQRFRLAALGQNHIZPGHRPZUREQROQFYGOQGHALGUAH

```

#### Zadanie 2

Uruchomienie aplikacji main z exe `KK_L3_2` dla 10000 iteracji

Daje różne rezultaty, np.:

```
Found key: LCDEHPFYQJKOMRZXWAIGUSBNTV
Starting decryption
Decryption completed

Process finished with exit code 0
```

i tekst odszyfrowany:

```
SHESTOODBYTHEWINDOWGAZINGOUTATTHECITYBELOWASTHESUNDIPPEDLOWONTHEHORIZONITHADBEENALONGDAYATWORKBUTTHEWEARINESSMELTEDAWAYASSHEHEARDTHEFAMILIARSOUNDOFHISFOOTSTEPSAPPROACHINGWITHASOFTSMILEPLAYINGONHERLIPSSHETURNEDTOFACEHIMHERHEARTSKIPPINGABEATATTHESIGHTOFHIM

HEYHESAIDHISXOICEWARMANDFILLEDWITHAFFECTIONASHECROSSEDTHEROOMTOWRAPHERINHISARMSROUGHDAY

SHENODDEDLEANINGINTOHISEMBRACEFINDINGSOLACEINHISPRESENCEBUTSEEINGYOUMAKESITALLBETTER

THEYSTOODTHEREFORAMOMENTLOSTINEACHOTHERSARMSTHEWORLDOUTSIDEFADINGINTOINSIGNIFICANCEINTHATMOMENTNOTHINGELSEMATTEREDBUTTHELOXETHEYSHARED

THENIGHTWASSTILLTHEAIRHEAXYWITHANTICIPATIONASDETECTIXEMILLERSURXEYEDTHECRIMESCENEACHILLRANDOWNHISSPINEASHESTUDIEDTHESCENEBEFOREHIMTHEXICTIMLYINGMOTIONLESSONTHEGROUNDSURROUNDEDBYAPOOLOFBLOODTHATGLISTENEDINTHEMOONLIGHT

SOMETHINGDIDNTADDUPTHEREWERENOSIGNSOFSTRUGGLENOCLUESLEFTBEHINDTOHINTATTHEPERPETRATORITWASASIFTHEKILLERHADXANISHEDINTOTHINAIRLEAXINGNOTHINGBUTVUESTIONSINTHEIRWAKE

MILLERNARROWEDHISEYESHISMINDRACINGASHEPIECEDTOGETHERTHEPUZZLEBEFOREHIMTHISWASNOORDINARYCASETHISWASACHALLENGEUNLIKEANYHEHADFACEDBEFOREBUTHEWASDETERMINEDTOCRACKITTOBRINGJUSTICETOTHEXICTIMANDCLOSURETOTHEIRLOXEDONES

THEDARKNESSPRESSEDINAROUNDHERASSHENAXIGATEDTHELABYRINTHINECORRIDORSOFTHEABANDONEDASYLUMEXERYSOUNDECHOEDOMINOUSLYSENDINGSHIXERSDOWNHERSPINEASSHEPRESSEDONHERHEARTPOUNDINGINHERCHEST

SHEKNEWTHERISKSOFCOMINGHEREALONEBUTTHELUREOFTHEUNKNOWNWASTOOSTRONGTORESISTTHEREWERESECRETSHIDDENWITHINTHESEWALLSSECRETSTHATBEGGEDTOBEUNCOXEREDNOMATTERTHECOST

ASSHEROUNDEDACORNERAFIGURELOOMEDBEFOREHERTHEIREYESGLEAMINGINTHEDARKNESSSHEFROZEHERBREATHCATCHINGINHERTHROATASFEARGRIPPEDHERHEARTBUTSHEREFUSEDTOBACKDOWNSTEELINGHERSELFFORWHATEXERLAYAHEAD

THEFORESTWASALIXEWITHMAGICTHEAIRHUMMINGWITHTHEWHISPERSOFANCIENTSPELLSANDENCHANTMENTSCREATURESOFMYTHANDLEGENDDARTEDTHROUGHTHEUNDERBRUSHTHEIREYESALIGHTWITHCURIOSITYASTHEYWATCHEDTHELONETRAXELERMAKINGHERWAYTHROUGHTHEDENSEFOLIAGE

SHEWASONAVUESTDRIXENBYASENSEOFPURPOSETHATBURNEDBRIGHTWITHINHERSOULWITHEACHSTEPSHEDREWCLOSERTOHERDESTINYGUIDEDBYTHEWISDOMOFTHESTARSANDTHEGUIDANCEOFHERTRUSTYCOMPANIONS

BUTDANGERLURKEDAROUNDEXERYCORNERTESTINGHERRESOLXEANDPUSHINGHERTOTHELIMITSOFHERSTRENGTHYETSHEPRESSEDONFUELEDBYCOURAGEANDDETERMINATIONKNOWINGTHATTHEFATEOFTHEWORLDRESTEDONHERSHOULDERS

THESELONGEREQAMPLESOFFERAGLIMPSEINTOTHEIMMERSIXEWORLDSANDCAPTIXATINGNARRATIXESOFTENFOUNDINLIGHTLITERATURE

```

co generuje prawie identyczny tekst referencyjny.
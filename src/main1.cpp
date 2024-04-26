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

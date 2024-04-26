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

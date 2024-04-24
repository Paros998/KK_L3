//
// Created by part4 on 24.04.2024.
//


#ifndef KK_L3_FILESUTIL_H
#define KK_L3_FILESUTIL_H

#include <fstream>

using namespace std;

namespace files {
	std::ifstream getInputHandle(const char *file) {
		return ifstream(file);
	}

	std::ofstream getOutputHandle(const char *file) {
		return ofstream(file);
	}
}

#endif //KK_L3_FILESUTIL_H
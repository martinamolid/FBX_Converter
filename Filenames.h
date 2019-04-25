#pragma once

#include <string>

//#define BINARY_FILE = "xTestBinary.bin";
//#define ASCII_FILE = "xTestAscii.txt";

#define NAME_SIZE 256

const std::string ASCII_FILE = "xTestAscii.txt";
const std::string BINARY_FILE = "xTestBinary.bin";
const std::string IN_FBX_FILEPATH = "twoConnorCubes.fbx";

//class Filenames {
//public:
//	Filenames();
//	~Filenames();
//
//	std::string getBinFilename() const;
//	std::string getAsciiFilename() const;
//private:
//	std::string binaryFile;
//	std::string asciiFile;
//};
//
//Filenames::Filenames() {
//	binaryFile = "xTestBinary.bin";
//	asciiFile = "xTestAscii.txt";
//}
//
//Filenames::~Filenames() {
//
//}
//
//std::string Filenames::getBinFilename() const {
//	return binaryFile;
//}
//
//std::string Filenames::getAsciiFilename() const {
//	return asciiFile;
//}
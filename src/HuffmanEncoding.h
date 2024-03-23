/*
 *  Created on: Dec 22, 2020
 *      Author: kompalli
 */

#ifndef HUFFMANENCODING_H_
#define HUFFMANENCODING_H_

#include <iostream>
#include <fstream>
#include <limits.h>
#include <math.h>
#include <unistd.h>
#include "util/GetMemUsage.h"
#include "util/LogManager.h"

class HuffmanEncoding{

public:
	/**
	 * Given an input text file, obtain frequencies of alphabets and generate HuffmanCode.
	 *
	 * @param trainFilePath Path of the input file.
	 * @param resultFilePath Path of the output Huffman code file.
	 *
	 * If the input file cannot be read throw an error of type ios_base::failure
	 * If the output file cannot be generated, then throw an error of type ios_base::failure
	 */
	static void generateAlphabetCode(char* trainFilePath, char* resultFilePath);


	/**
	 * Given an input text file and a file contain the HuffmanCode for alphabets, generate
	 * the encoded file
	 *
	 * @param testASCIIFilePath Path of the input file.
	 * @param huffmanCodeFilePath Path of the alphabet Huffman code file.
	 * @param resultFilePath Path of the output encoded file.
	 *
	 * If the input file cannot be read throw an error of type ios_base::failure
	 * If the output file cannot be generated, then throw an error of type ios_base::failure
	 */
	static void encodeText(char* testASCIIFilePath, char* huffmanCodeFilePath, char* resultFilePath);

	/**
	 * Given an input encoded file and a file contain the HuffmanCode for alphabets, generate
	 * the decoded file
	 *
	 * @param testEncodedFilePath Path of the input encoded file.
	 * @param huffmanCodeFilePath Path of the alphabet Huffman code file.
	 * @param resultFilePath Path of the output decoded file.
	 *
	 * If the input file cannot be read throw an error of type ios_base::failure
	 * If the output file cannot be generated, then throw an error of type ios_base::failure
	 */
	static void decodeText(char* testEncodedFilePath, char* huffmanCodeFilePath, char* resultFilePath);

};

#endif /* HUFFMANENCODING_H_ */

// homework.cpp
#include "homework.h"
#include <cstdio>
#include <cstring>

int main(int argc, char **argv)
{
	LogManager::resetLogFile();
	LogManager::writePrintfToLog(LogManager::Level::Status, "main", "In main file.");
	printf("Usage:\n\n");
	printf("./homework testCodeGeneration trainFilePath\n\n");
	printf("./homework testEncoding testASCIIFilePath huffmanCodeFilePath\n\n");
	printf("./homework testDecoding testEncodedFilePath huffmanCodeFilePath\n\n");

	if (argc < 2)
		return 0;

	int peakMem1 = getPeakRSS();
	int currMem1 = getCurrentRSS();
	printf("peakRSS = %d, currMem=%d\n", peakMem1, currMem1);
	printf("sizeof(bool) = %d \n", (int)sizeof(bool));
	printf("sizeof(int) = %d \n", (int)sizeof(int));
	printf("sizeof(char) = %d \n", (int)sizeof(char));
	printf("INT_MIN = %d, INT_MAX =%d\n", INT_MIN, INT_MAX);

	auto start = std::chrono::high_resolution_clock::now();

	if (strncmp(argv[1], "testCodeGeneration", 18) == 0)
	{
		// Print all command-line arguments
		for (int i = 0; i < argc; ++i)
		{
			printf("argv[%d]: %s\n", i, argv[i]);
		}
		char inputTrainFilePath[1024], outputHuffmanCodePath[1024];
		strncpy(inputTrainFilePath, argv[2], sizeof(inputTrainFilePath) - 1);
		inputTrainFilePath[sizeof(inputTrainFilePath) - 1] = '\0';
		snprintf(outputHuffmanCodePath, sizeof(outputHuffmanCodePath), "%s.huffman.txt", argv[2]);

		HuffmanEncoding::generateAlphabetCode(inputTrainFilePath, outputHuffmanCodePath);
	}
	else if (strncmp(argv[1], "testEncoding", 12) == 0)
	{
		char testASCIIFilePath[1024], huffmanCodeFilePath[1024], outFile[1024];
		strncpy(testASCIIFilePath, argv[2], sizeof(testASCIIFilePath) - 1);
		testASCIIFilePath[sizeof(testASCIIFilePath) - 1] = '\0';
		strncpy(huffmanCodeFilePath, argv[3], sizeof(huffmanCodeFilePath) - 1);
		huffmanCodeFilePath[sizeof(huffmanCodeFilePath) - 1] = '\0';
		snprintf(outFile, sizeof(outFile), "%s.encode.txt", testASCIIFilePath);

		HuffmanEncoding::encodeText(testASCIIFilePath, huffmanCodeFilePath, outFile);
	}
	else if (strncmp(argv[1], "testDecoding", 12) == 0)
	{
		char testEncodedFilePath[1024], huffmanCodeFilePath[1024], outFile[1024];
		strncpy(testEncodedFilePath, argv[2], sizeof(testEncodedFilePath) - 1);
		testEncodedFilePath[sizeof(testEncodedFilePath) - 1] = '\0';
		strncpy(huffmanCodeFilePath, argv[3], sizeof(huffmanCodeFilePath) - 1);
		huffmanCodeFilePath[sizeof(huffmanCodeFilePath) - 1] = '\0';
		snprintf(outFile, sizeof(outFile), "%s.ascii.txt", testEncodedFilePath);

		HuffmanEncoding::decodeText(testEncodedFilePath, huffmanCodeFilePath, outFile);
	}

	auto stop = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
	printf("run time =%d microseconds\n", static_cast<int>(duration.count()));

	int peakMem2 = getPeakRSS();
	int currMem2 = getCurrentRSS();
	printf("peakRSS = %d, currMem=%d\n", peakMem2, currMem2);
	printf("Diff peakRSS = %d, currMem=%d\n", peakMem2 - peakMem1, currMem2 - currMem1);

	return 0;
}
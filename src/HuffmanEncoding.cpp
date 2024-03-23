/*
 *  Created on: Dec 22, 2020
 *      Author: kompalli
 */
#include "HuffmanEncoding.h"
#include <iomanip>
#include <iostream>
struct Node
{
    char character;
    int count;
    Node *left;
    Node *right;
};

// Function to create a new node
Node *createNode(char character, int count)
{
    Node *newNode = new Node();
    newNode->character = character;
    newNode->count = count;
    newNode->left = nullptr;
    newNode->right = nullptr;
    return newNode;
}

// Function to traverse the Huffman tree and assign codes
void traverseAndPrintCodes(Node *root, std::string code, std::ofstream &outputFile, int total)
{
    if (root == nullptr)
        return;

    // If the node is a leaf node, print the character, probability, and code
    if (!root->left && !root->right)
        outputFile << "\"" << (root->character == '\n' ? "\\n" : std::string(1, root->character)) << "\""
                                                                                                     " \""
                   << std::fixed << std::setprecision(6) << static_cast<double>(root->count) / total << "\""
                                                                                                        " \""
                   << code << "\"\n";

    // Traverse left and append "0" to the code
    traverseAndPrintCodes(root->left, code + "0", outputFile, total);

    // Traverse right and append "1" to the code
    traverseAndPrintCodes(root->right, code + "1", outputFile, total);
}

// Function to build the Huffman tree
Node *buildHuffmanTree(int count[], int numCharacters)
{
    // Create leaf nodes for characters with non-zero frequencies
    Node *nodes[numCharacters * 2];
    int nodeCount = 0;
    for (int i = 0; i < numCharacters; ++i)
    {
        if (count[i] > 0)
        {
            nodes[nodeCount++] = createNode(static_cast<char>(i), count[i]);
        }
    }

    // Construct the Huffman tree
    while (nodeCount > 1)
    {
        int min1 = 0;
        int min2 = 1;
        if (nodes[min1]->count > nodes[min2]->count)
            std::swap(min1, min2);

        for (int i = 2; i < nodeCount; ++i)
        {
            if (nodes[i]->count < nodes[min1]->count)
            {
                min2 = min1;
                min1 = i;
            }
            else if (nodes[i]->count < nodes[min2]->count)
            {
                min2 = i;
            }
        }

        Node *parent = createNode('\0', nodes[min1]->count + nodes[min2]->count);
        parent->left = nodes[min1];
        parent->right = nodes[min2];
        nodes[min1] = parent;
        nodes[min2] = nodes[--nodeCount];
    }

    // Return the root of the Huffman tree
    return nodes[0];
}

void HuffmanEncoding::generateAlphabetCode(char *trainFilePath, char *resultFilePath)
{
    // Open input file
    std::ifstream inputFile(trainFilePath);
    if (!inputFile)
    {
        std::cerr << "Error: Unable to open input file.\n";
        return;
    }

    // Initialize count array for ASCII characters
    const int numCharacters = 128; // ASCII range
    int count[numCharacters] = {0};

    char ch;
    int total = 0;
    while (inputFile.get(ch))
    {
        // Check if character is printable or newline
        if (std::isprint(ch) || ch == '\n')
        {
            count[ch]++;
            total++;
        }
    }
    inputFile.close();

    // Open output file
    std::ofstream outputFile(resultFilePath);
    if (!outputFile)
    {
        std::cerr << "Error: Unable to open output file.\n";
        return;
    }

    // Build Huffman tree
    Node *root = buildHuffmanTree(count, numCharacters);

    // Generate Huffman codes and print them to the output file
    traverseAndPrintCodes(root, "", outputFile, total);

    // Close the output file
    outputFile.close();
}

void extractInformation(const std::string &line, std::string &character, std::string &prob, std::string &code)
{
    std::istringstream iss(line);
    std::string temp;

    // Extract character (inside double quotes)
    std::getline(iss, temp, '"'); // Skip opening double quote
    std::getline(iss, temp, '"'); // Read character
    character = temp;             // Assuming character is a single character

    // Extract probability (inside double quotes)
    std::getline(iss, temp, '"'); // Skip space and opening double quote
    std::getline(iss, prob, '"'); // Read probability

    // Extract code (inside double quotes)
    std::getline(iss, temp, '"'); // Skip space and opening double quote
    std::getline(iss, code, '"'); // Read code
}

void HuffmanEncoding::encodeText(char *testASCIIFilePath, char *huffmanCodeFilePath, char *resultFilePath)
{
    // Open Huffman code file
    std::ifstream huffmanCodeFile(huffmanCodeFilePath);
    if (!huffmanCodeFile)
    {
        std::cerr << "Error: Unable to open Huffman code file.\n";
        return;
    }

    // Create arrays to store characters and their corresponding codes
    const int MAX_CHARACTERS = 128; // ASCII range
    std::string characters[MAX_CHARACTERS];
    std::string codes[MAX_CHARACTERS];
    int index = 0;

    // Read Huffman codes from file
    std::string line;
    while (std::getline(huffmanCodeFile, line))
    {
        std::string character, prob, code;
        extractInformation(line, character, prob, code);
        characters[index] = character;
        codes[index] = code; // Extract Huffman code
        // std::cout<<characters[index]<<" "<<codes[index]<<std::endl;
        index++;
    }
    huffmanCodeFile.close();

    // Open input text file
    std::ifstream inputFile(testASCIIFilePath);
    if (!inputFile)
    {
        std::cerr << "Error: Unable to open input text file.\n";
        return;
    }

    // Open output encoded file
    std::ofstream outputFile(resultFilePath);
    if (!outputFile)
    {
        std::cerr << "Error: Unable to open output encoded file.\n";
        inputFile.close();
        return;
    }

    // Encode the text using Huffman codes
    char c;
    while (inputFile.get(c))
    {
        std::string ch(1, c);
        if (ch == "\n")
        {
            for (int i = 0; i < index; ++i)
            {
                if (characters[i] == "\\n")
                {
                    outputFile << codes[i];
                    break;
                }
            }
          
        }
        
        else
        {
            // Write encoded character
            bool found = false;
            for (int i = 0; i < index; ++i)
            {
                if (characters[i] == ch)
                {
                    outputFile << codes[i];
                    found = true;
                    break;
                }
            }
            if (!found)
            {
                std::cerr << "Error: Huffman code not found for character '" << ch << "'.\n";
                inputFile.close();
                outputFile.close();
                return;
            }
        }
    }

    // Close input and output files
    inputFile.close();
    outputFile.close();
}

// void HuffmanEncoding::decodeText(char *testEncodedFilePath, char *huffmanCodeFilePath, char *resultFilePath)
// {

// }
#include <cstring> // Include cstring for string operations
#include <fstream>

const int MAX_CODE_LENGTH = 32; // Maximum length of Huffman code

struct HuffmanNode
{
    char character;
    HuffmanNode *left;
    HuffmanNode *right;
};

// Function to create a new Huffman node
HuffmanNode *createHuffmanNode(char character)
{
    HuffmanNode *newNode = new HuffmanNode();
    newNode->character = character;
    newNode->left = nullptr;
    newNode->right = nullptr;
    return newNode;
}

// Function to decode the encoded text using Huffman codes
void HuffmanEncoding::decodeText(char *testEncodedFilePath, char *huffmanCodeFilePath, char *resultFilePath)
{
    // Read Huffman codes from huffman.txt and store them in arrays
    std::ifstream huffmanCodeFile(huffmanCodeFilePath);
    if (!huffmanCodeFile)
    {
        std::cerr << "Error: Unable to open Huffman code file.\n";
        return;
    }

    const int MAX_CHARACTERS = 128; // ASCII range
    char characters[MAX_CHARACTERS];
    char codes[MAX_CHARACTERS][MAX_CODE_LENGTH];
    int index = 0;

    std::string line;
    while (std::getline(huffmanCodeFile, line))
    {
        std::string character, prob, code;
        extractInformation(line, character, prob, code);
        characters[index] = character[0];
        std::strcpy(codes[index], code.c_str());
        index++;
    }
    huffmanCodeFile.close();

    // Decode the encoded text using Huffman codes
    std::ifstream inputFile(testEncodedFilePath);
    if (!inputFile)
    {
        std::cerr << "Error: Unable to open encoded text file.\n";
        return;
    }

    std::ofstream outputFile(resultFilePath);
    if (!outputFile)
    {
        std::cerr << "Error: Unable to create output file.\n";
        inputFile.close();
        return;
    }

    char currentCode[MAX_CODE_LENGTH];
    int codeLength = 0;
    HuffmanNode *root = nullptr;
    HuffmanNode *currentNode = nullptr;

    while (inputFile.peek() != EOF)
    {
        char bit;
        inputFile.get(bit);

        currentCode[codeLength++] = bit;
        currentCode[codeLength] = '\0'; // Null terminate the string

        for (int i = 0; i < index; ++i)
        {
            if (std::strcmp(currentCode, codes[i]) == 0)
            {
                outputFile << characters[i]; // Write decoded character to output file
                codeLength = 0;              // Reset codeLength
                break;
            }
        }
    }

    // Close input and output files
    inputFile.close();
    outputFile.close();
}
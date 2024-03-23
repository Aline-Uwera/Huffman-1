#include <cstdio>
#include "HuffmanEncoding.h"
#include <iomanip>
#include <string>

struct Node
{
    char character;
    int count;
    Node *left;
    Node *right;
};
Node *createNode(char character, int count)
{
    Node *newNode = new Node();
    newNode->character = character;
    newNode->left = nullptr;
    newNode->right = nullptr;
    newNode->count = count;
    return newNode;
}

Node *huffmanTree(int characterFrequencies[], int numCharacters)
{
    Node *nodes[numCharacters * 2];
    int nodeCount = 0;
    for (int i = 0; i < numCharacters; ++i)
    {
        if (characterFrequencies[i] > 0)
        {
            nodes[nodeCount++] = createNode(static_cast<char>(i), characterFrequencies[i]);
        }
    }

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

    return nodes[0];
}

void traverse(Node *root, std::string code, FILE *outputFile, int totalFrequency)
{
    if (root == nullptr)
        return;

    if (!root->left && !root->right)
    {
        fprintf(outputFile, "\"%s\" \"%f\" \"%s\"\n", (root->character == '\n' ? "\\n" : std::string(1, root->character)).c_str(),
                static_cast<double>(root->count) / totalFrequency, code.c_str());
    }

    traverse(root->left, code + "0", outputFile, totalFrequency);
    traverse(root->right, code + "1", outputFile, totalFrequency);
}


void HuffmanEncoding::generateAlphabetCode(char *trainFilePath, char *resultFilePath)
{
    FILE *inputFile = fopen(trainFilePath, "r");
    if (!inputFile)
    {
        std::cerr << "Error: Unable to open input file.\n";
        return;
    }

    const int numCharacters = 128; // ASCII range
    int count[numCharacters] = {0};

    char ch;
    int totalFrequency = 0;
    while ((ch = fgetc(inputFile)) != EOF)
    {
        if (std::isprint(ch) || ch == '\n')
        {
            count[ch]++;
            totalFrequency++;
        }
    }
    fclose(inputFile);

    FILE *outputFile = fopen(resultFilePath, "w");
    if (!outputFile)
    {
        std::cerr << "Error: Unable to open output file.\n";
        return;
    }

    Node *root = huffmanTree(count, numCharacters);

    traverse(root, "", outputFile, totalFrequency);

    fclose(outputFile);
}

void parseLine(const char *line, char *character, char *prob, char *code)
{
    const char *charStart = strchr(line, '"');
    if (!charStart)
        return; 

    const char *charEnd = strchr(charStart + 1, '"');
    if (!charEnd)
        return; 

    strncpy(character, charStart + 1, charEnd - charStart - 1);
    character[charEnd - charStart - 1] = '\0';

    const char *probStart = strchr(charEnd + 1, '"');
    if (!probStart)
        return; 

    const char *probEnd = strchr(probStart + 1, '"');
    if (!probEnd)
        return; 

    strncpy(prob, probStart + 1, probEnd - probStart - 1);
    prob[probEnd - probStart - 1] = '\0';

    const char *codeStart = strchr(probEnd + 1, '"');
    if (!codeStart)
        return; 

    const char *codeEnd = strchr(codeStart + 1, '"');
    if (!codeEnd)
        return; 

    strncpy(code, codeStart + 1, codeEnd - codeStart - 1);
    code[codeEnd - codeStart - 1] = '\0';
}

void HuffmanEncoding::encodeText(char *testASCIIFilePath, char *huffmanCodeFilePath, char *resultFilePath)
{
    FILE *huffmanCodeFile = fopen(huffmanCodeFilePath, "r");
    if (!huffmanCodeFile)
    {
        std::cerr << "Error: Unable to open Huffman code file.\n";
        return;
    }

    const int MAX_CHARACTERS = 128; // ASCII range
    char characters[MAX_CHARACTERS][256];
    char codes[MAX_CHARACTERS][256];
    int index = 0;

    char line[256];
    while (fgets(line, sizeof(line), huffmanCodeFile))
    {
        char character[256], prob[256], code[256];
        parseLine(line, character, prob, code);
        strcpy(characters[index], character);
        strcpy(codes[index], code); 
        index++;
    }
    fclose(huffmanCodeFile);

    FILE *inputFile = fopen(testASCIIFilePath, "r");
    if (!inputFile)
    {
        std::cerr << "Error: Unable to open input text file.\n";
        return;
    }

    FILE *outputFile = fopen(resultFilePath, "w");
    if (!outputFile)
    {
        std::cerr << "Error: Unable to open output encoded file.\n";
        fclose(inputFile);
        return;
    }

    int c;
    while ((c = fgetc(inputFile)) != EOF)
    {
        if (c == '\n')
        {
            for (int i = 0; i < index; ++i)
            {
                if (strcmp(characters[i], "\\n") == 0)
                {
                    fprintf(outputFile, "%s", codes[i]);
                    break;
                }
            }
        }
        else
        {
            bool found = false;
            for (int i = 0; i < index; ++i)
            {
                if (characters[i][0] == c && characters[i][1] == '\0')
                {
                    fprintf(outputFile, "%s", codes[i]);
                    found = true;
                    break;
                }
            }
            if (!found)
            {
                std::cerr << "Error: Huffman code not found for character '" << (char)c << "'.\n";
                fclose(inputFile);
                fclose(outputFile);
                return;
            }
        }
    }

    fclose(inputFile);
    fclose(outputFile);
}

struct TrieNode
{
    char data;
    TrieNode *children[2]; 
    bool isLeaf;

    TrieNode(char c) : data(c), isLeaf(false)
    {
        children[0] = nullptr;
        children[1] = nullptr;
    }
};

class HuffmanDecoder
{
private:
    TrieNode *root;

    void insert(const char *code, char character)
    {
        TrieNode *current = root;
        for (int i = 0; code[i] != '\0'; ++i)
        {
            int index = (code[i] == '0') ? 0 : 1;
            if (!current->children[index])
            {
                current->children[index] = new TrieNode('\0');
            }
            current = current->children[index];
        }
        current->data = character;
        current->isLeaf = true;
    }

    void decodeText(FILE *encodedFile, FILE *outputFile)
    {
        TrieNode *current = root;
        char ch;
        while ((ch = fgetc(encodedFile)) != EOF)
        {
            int bit = (ch == '0') ? 0 : 1;
            current = current->children[bit];
            if (current->isLeaf)
            {
                if (current->data == '\\')
                {                              
                    fprintf(outputFile, "\n"); 
                }
                else
                {
                    fprintf(outputFile, "%c", current->data); 
                }
                current = root; 
            }
        }
    }

public:
    HuffmanDecoder()
    {
        root = new TrieNode('\0');
    }

    ~HuffmanDecoder()
    {
        deleteTrie(root);
    }

    void buildTrie(char *huffmanCodeFilePath)
    {
        FILE *huffmanCodeFile = fopen(huffmanCodeFilePath, "r");
        if (!huffmanCodeFile)
        {
            std::cerr << "Error: Unable to open Huffman code file.\n";
            return;
        }

        char lineBuffer[256]; 
        while (fgets(lineBuffer, sizeof(lineBuffer), huffmanCodeFile))
        {
            char character[128], probability[128], code[128];
            sscanf(lineBuffer, "\"%127[^\"]\" \"%127[^\"]\" \"%127[^\"]\"", character, probability, code);
            insert(code, character[0]);
        }
        fclose(huffmanCodeFile);
    }

    void decodeText(char *testEncodedFilePath, char *resultFilePath)
    {
        FILE *encodedFile = fopen(testEncodedFilePath, "r");
        if (!encodedFile)
        {
            std::cerr << "Error: Unable to open input encoded file.\n";
            return;
        }

        FILE *outputFile = fopen(resultFilePath, "w");
        if (!outputFile)
        {
            std::cerr << "Error: Unable to open output decoded file.\n";
            fclose(encodedFile);
            return;
        }

        decodeText(encodedFile, outputFile);

        fclose(encodedFile);
        fclose(outputFile);
    }

private:
    void deleteTrie(TrieNode *node)
    {
        if (!node)
            return;
        for (int i = 0; i < 2; ++i)
        {
            if (node->children[i])
                deleteTrie(node->children[i]);
        }
        delete node;
    }
};

void HuffmanEncoding::decodeText(char *testEncodedFilePath, char *huffmanCodeFilePath, char *resultFilePath)
{
    HuffmanDecoder decoder;
    decoder.buildTrie(huffmanCodeFilePath);
    decoder.decodeText(testEncodedFilePath, resultFilePath);
}

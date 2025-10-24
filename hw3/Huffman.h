#pragma once

#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>

// 定義 Huffman 樹節點
struct Node {
    unsigned char data; // 存放 ASCII 字元 (0-255)
    int freq;           // 頻率
    Node *left, *right; // 左右子節點

    // 葉子節點的構造函式
    Node(unsigned char d, int f) : data(d), freq(f), left(nullptr), right(nullptr) {}

    // 內部節點的構造函式
    Node(int f, Node* l, Node* r) : data('\0'), freq(f), left(l), right(r) {}

    // 解構函式 (用於釋放記憶體)
    ~Node() {
        delete left;
        delete right;
    }
};

// 比較器：用於在建樹時將頻率最低的節點放在前面
struct CompareNode {
    bool operator()(const Node* a, const Node* b) const {
        return a->freq > b->freq; // Min-Heap 行為: 頻率小的優先
    }
};

class Huffman {
private:
    Node* root = nullptr;
    std::map<unsigned char, std::string> codes;
    // 使用 vector 模擬 Min-Heap (簡單實現，非標準的 std::priority_queue)
    std::vector<Node*> minHeap; 

    // 內部函數：計算檔案中每個字元的頻率
    std::map<unsigned char, int> calculateFrequency(std::ifstream& inputFile);

    // 內部函數：建構 Huffman 樹 (使用 Min-Heap 邏輯)
    void buildTree(const std::map<unsigned char, int>& freqMap);

    // 內部函數：遞迴地產生 Huffman 編碼
    void generateCodes(Node* node, std::string code);

    // 內部函數：序列化 (寫入) Huffman 樹/Header 到檔案
    void writeHeader(std::ofstream& outputFile);

    // 內部函數：反序列化 (讀取) Huffman 樹/Header
    void readHeader(std::ifstream& inputFile);

    // 內部函數：釋放 Huffman 樹所佔用的記憶體
    void destroyTree(Node* node);

    // 內部函數：Min-Heap 的基本操作 (手動實現)
    void pushHeap(Node* node);
    Node* popHeap();

public:
    Huffman() = default;
    ~Huffman();

    // 核心功能：壓縮
    bool compress(const std::string& inputFile, const std::string& outputFile);

    // 核心功能：解壓縮
    bool decompress(const std::string& inputFile, const std::string& outputFile);
};
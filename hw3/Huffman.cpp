#include "Huffman.h"

// ---------------------- Min-Heap 模擬操作 (簡易實現) ----------------------
void Huffman::pushHeap(Node* node) {
    minHeap.push_back(node);
    std::push_heap(minHeap.begin(), minHeap.end(), CompareNode());
}

Node* Huffman::popHeap() {
    if (minHeap.empty()) return nullptr;
    std::pop_heap(minHeap.begin(), minHeap.end(), CompareNode());
    Node* top = minHeap.back();
    minHeap.pop_back();
    return top;
}
// --------------------------------------------------------------------------

Huffman::~Huffman() {
    // 釋放 Huffman 樹所佔用的記憶體 (防止記憶體洩漏)
    // destroyTree(root); // Node 的解構函式已包含遞迴釋放
    delete root; // 呼叫 root 的解構函式會遞迴釋放整個樹
}

std::map<unsigned char, int> Huffman::calculateFrequency(std::ifstream& inputFile) {
    std::map<unsigned char, int> freqMap;
    unsigned char byte;
    
    // 讀取檔案直到結束
    while (inputFile.read(reinterpret_cast<char*>(&byte), 1)) {
        freqMap[byte]++;
    }
    inputFile.clear();             // 清除 EOF 標誌
    inputFile.seekg(0, std::ios::beg); // 將檔案指標移回開頭 (用於後續的編碼)
    
    return freqMap;
}

void Huffman::buildTree(const std::map<unsigned char, int>& freqMap) {
    // 1. 將葉子節點加入 Min-Heap
    for (const auto& pair : freqMap) {
        pushHeap(new Node(pair.first, pair.second));
    }

    // 2. 重複合併頻率最小的兩個節點
    while (minHeap.size() > 1) {
        Node* left = popHeap();
        Node* right = popHeap();
        
        // 創建一個新的內部節點，頻率為兩者之和
        Node* parent = new Node(left->freq + right->freq, left, right);
        pushHeap(parent);
    }

    // 3. 佇列中剩下的唯一節點即為根節點
    if (!minHeap.empty()) {
        root = popHeap();
    }
}

void Huffman::generateCodes(Node* node, std::string code) {
    if (!node) return;

    // 找到葉子節點
    if (!node->left && !node->right) {
        codes[node->data] = code;
        return;
    }

    // 遞迴：左子節點為 '0'，右子節點為 '1'
    generateCodes(node->left, code + "0");
    generateCodes(node->right, code + "1");
}

void Huffman::writeHeader(std::ofstream& outputFile) {
    // 1. 寫入編碼總數 (Header 的大小)
    size_t codeCount = codes.size();
    outputFile.write(reinterpret_cast<const char*>(&codeCount), sizeof(codeCount));
    
    // 2. 寫入每個 (字元, 編碼長度, 編碼) 三元組
    for (const auto& pair : codes) {
        unsigned char charData = pair.first;
        const std::string& code = pair.second;
        
        // 寫入字元 (1 byte)
        outputFile.write(reinterpret_cast<const char*>(&charData), 1);
        
        // 寫入編碼長度 (1 byte)
        unsigned char codeLength = static_cast<unsigned char>(code.length());
        outputFile.write(reinterpret_cast<const char*>(&codeLength), 1);
        
        // 寫入編碼位元
        unsigned char buffer = 0;
        int bitCount = 0;
        
        for (char bit : code) {
            buffer <<= 1;
            if (bit == '1') {
                buffer |= 1;
            }
            bitCount++;
            
            if (bitCount == 8) {
                outputFile.write(reinterpret_cast<const char*>(&buffer), 1);
                buffer = 0;
                bitCount = 0;
            }
        }
        // 寫入剩餘的位元
        if (bitCount > 0) {
            buffer <<= (8 - bitCount); // 補齊
            outputFile.write(reinterpret_cast<const char*>(&buffer), 1);
        }
    }
}

void Huffman::readHeader(std::ifstream& inputFile) {
    size_t codeCount = 0;
    // 1. 讀取編碼總數
    if (!inputFile.read(reinterpret_cast<char*>(&codeCount), sizeof(codeCount))) {
        throw std::runtime_error("Error reading code count from file header.");
    }
    
    // 2. 讀取每個 (字元, 編碼長度, 編碼) 三元組並重建樹
    for (size_t i = 0; i < codeCount; ++i) {
        unsigned char charData;
        unsigned char codeLength;
        
        // 讀取字元和長度
        if (!inputFile.read(reinterpret_cast<char*>(&charData), 1) || 
            !inputFile.read(reinterpret_cast<char*>(&codeLength), 1)) 
        {
            throw std::runtime_error("Error reading char data or code length.");
        }
        
        std::string code = "";
        unsigned char buffer = 0;
        int bitsToRead = codeLength;
        int byteIndex = 0; // 當前讀取的位元在當前 byte 中的位置
        
        // 循環讀取編碼的位元
        while (bitsToRead > 0) {
            // 需要讀取新的一個 byte
            if (byteIndex == 0) {
                if (!inputFile.read(reinterpret_cast<char*>(&buffer), 1)) {
                    throw std::runtime_error("Error reading code byte.");
                }
            }
            
            int currentBit = 7 - byteIndex;
            // 提取最高位 (最左邊的位元)
            char bitChar = (buffer & (1 << currentBit)) ? '1' : '0';
            code += bitChar;
            
            byteIndex = (byteIndex + 1) % 8;
            bitsToRead--;
        }
        
        // 3. 將編碼插入樹中 (從根開始，根據 '0'/'1' 走左/右)
        if (!root) {
            root = new Node('\0', 0); // 如果根節點不存在，先建立它
        }
        Node* current = root;
        for (char bit : code) {
            if (bit == '0') {
                if (!current->left) {
                    current->left = new Node('\0', 0);
                }
                current = current->left;
            } else { // bit == '1'
                if (!current->right) {
                    current->right = new Node('\0', 0);
                }
                current = current->right;
            }
        }
        current->data = charData; // 抵達葉子節點，存入字元
    }
}

// ------------------------- 核心功能 -------------------------

bool Huffman::compress(const std::string& inputFile, const std::string& outputFile) {
    std::ifstream ifs(inputFile, std::ios::binary);
    std::ofstream ofs(outputFile, std::ios::binary);
    if (!ifs.is_open() || !ofs.is_open()) {
        std::cerr << "Error: Cannot open files for compression." << std::endl;
        return false;
    }
    
    try {
        // 1. 計算頻率 (第一次讀取)
        std::map<unsigned char, int> freqMap = calculateFrequency(ifs);
        if (freqMap.empty()) {
            std::cerr << "Error: Input file is empty or cannot be read." << std::endl;
            return false;
        }

        // 2. 建構 Huffman 樹
        buildTree(freqMap);
        
        // 3. 產生編碼
        generateCodes(root, "");
        
        // 4. 寫入 Header (編碼表)
        writeHeader(ofs);
        
        // 5. 寫入原始檔案大小 (用於解壓縮停止)
        long long originalSize = freqMap.empty() ? 0 : root->freq;
        ofs.write(reinterpret_cast<const char*>(&originalSize), sizeof(originalSize));

        // 6. 寫入編碼資料 (第二次讀取)
        unsigned char byte;
        unsigned char buffer = 0;
        int bitCount = 0;
        
        while (ifs.read(reinterpret_cast<char*>(&byte), 1)) {
            const std::string& code = codes.at(byte);
            
            for (char bit : code) {
                buffer <<= 1;
                if (bit == '1') {
                    buffer |= 1;
                }
                bitCount++;
                
                if (bitCount == 8) {
                    ofs.write(reinterpret_cast<const char*>(&buffer), 1);
                    buffer = 0;
                    bitCount = 0;
                }
            }
        }
        
        // 寫入剩餘的位元
        if (bitCount > 0) {
            buffer <<= (8 - bitCount); // 補齊
            ofs.write(reinterpret_cast<const char*>(&buffer), 1);
        }
        
        std::cout << "Compression successful. Original Size: " << originalSize << " bytes." << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "Compression error: " << e.what() << std::endl;
        return false;
    }
    
    ifs.close();
    ofs.close();
    return true;
}


bool Huffman::decompress(const std::string& inputFile, const std::string& outputFile) {
    std::ifstream ifs(inputFile, std::ios::binary);
    std::ofstream ofs(outputFile, std::ios::binary);
    if (!ifs.is_open() || !ofs.is_open()) {
        std::cerr << "Error: Cannot open files for decompression." << std::endl;
        return false;
    }

    try {
        // 1. 讀取 Header (重建 Huffman 樹)
        readHeader(ifs);
        if (!root) {
            std::cerr << "Error: Cannot rebuild Huffman tree from header." << std::endl;
            return false;
        }

        // 2. 讀取原始檔案大小
        long long originalSize = 0;
        if (!ifs.read(reinterpret_cast<char*>(&originalSize), sizeof(originalSize))) {
            throw std::runtime_error("Error reading original file size.");
        }
        
        // 3. 讀取編碼資料並解碼
        long long decodedCount = 0;
        unsigned char buffer;
        Node* current = root;
        
        while (ifs.read(reinterpret_cast<char*>(&buffer), 1) && decodedCount < originalSize) {
            // 逐位元讀取
            for (int i = 7; i >= 0 && decodedCount < originalSize; --i) {
                int bit = (buffer >> i) & 1; // 提取位元
                
                if (bit == 0) {
                    current = current->left;
                } else {
                    current = current->right;
                }
                
                // 檢查是否到達葉子節點
                if (!current->left && !current->right) {
                    // 寫入解碼後的字元
                    ofs.write(reinterpret_cast<const char*>(&current->data), 1);
                    decodedCount++;
                    current = root; // 重設回根節點
                }
            }
        }

        if (decodedCount != originalSize) {
            std::cerr << "Warning: Decoded size mismatch. Expected: " 
                      << originalSize << ", Actual: " << decodedCount << std::endl;
        }
        
        std::cout << "Decompression successful. Decoded Size: " << decodedCount << " bytes." << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "Decompression error: " << e.what() << std::endl;
        return false;
    }
    
    ifs.close();
    ofs.close();
    return true;
}
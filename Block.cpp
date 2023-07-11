/*
 *  Copyright (c) (2023) SPHINX_ORG
 *  Authors:
 *    - (C kusuma) <thekoesoemo@gmail.com>
 *      GitHub: (https://github.com/chykusuma)
 *  Contributors:
 *    - (Contributor 1) <email1@example.com>
 *      Github: (https://github.com/yourgit)
 *    - (Contributor 2) <email2@example.com>
 *      Github: (https://github.com/yourgit)
 */


/////////////////////////////////////////////////////////////////////////////////////////////////////////
// This code appears to be a C++ program that implements various classes and namespaces related to blockchain functionality.

// Namespace SPHINXVerify:

  // The SPHINX_PublicKey class represents a public key used for verifying the signature of a block.
  // The class has a public member variable publicKey of type std::string to store the public key.
  // It also has a constructor that takes a std::string key as input and initializes the publicKey member variable.
  // The verifySignature function takes a block hash, a signature, and a SPHINX_PublicKey object as input parameters.
  // This function verifies the signature of the block using the provided signature and public key.
  // It calls the Crypto::verify function, passing the block hash, signature, and public key as arguments, and returns a boolean value indicating whether the verification was successful.

// Namespace SPHINXHash:
  // This namespace contains a declaration for the SPHINX_256 function, which calculates the SPHINX-256 hash of the input data.
  // The function takes a std::string data as input and returns a std::string representing the calculated hash.

// Namespace SPHINX_Chain:
  // The Chain class represents a blockchain.
  // It provides a member function addBlock that takes a SPHINXMerkleBlock::MerkleBlock object as input.
  // This function is responsible for adding a block to the chain. The implementation details of this function are not provided in the given code.

// Namespace SPHINXDb:
  // The DistributedDb class represents a distributed database.
  // It has two member functions: saveData and loadData.
  // The saveData function takes a std::string data and a block hash as input.
  // This function saves the provided data with the specific block hash in the distributed database.
  // The loadData function takes a block ID as input and returns the corresponding data from the distributed database.

// Namespace SPHINXMerkleBlock:
  // The MerkleBlock class represents a Merkle block in a blockchain.
  // It provides a member function constructMerkleTree that constructs the Merkle tree from a vector of SPHINXTrx::Transaction objects.
  // The function recursively splits the transactions into halves and calculates the SPHINX-256 hash of the concatenated left and right roots until it reaches the base case of a single transaction.
  // The verifyMerkleRoot function takes a Merkle root and a vector of SPHINXTrx::Transaction objects as input.
  // It calculates the constructed Merkle root using the constructMerkleTree function and compares it with the provided Merkle root to verify its validity.

// Namespace SPHINXBlock:
  // The Block class represents a block in the blockchain.
  // It has member variables such as previousHash_, blockHeight_, nonce_, difficulty_, timestamp_, transactions_, merkleRoot_, and signature_ to store various block-related information.
  // The constructor of the Block class takes the previous hash as input and initializes the member variables with default values.
  // The addTransaction function adds a transaction to the block by appending it to the transactions_ vector.
  // The calculateBlockHash function calculates the hash of the block by concatenating the previous hash, Merkle root, timestamp, nonce, and transactions, and solving the nonce using the Proof-of-Work algorithm from SPHINXPoW::solveNonce.
  // The calculateMerkleRoot function calculates the Merkle root of the block by calling the constructMerkleTree function from the SPHINXMerkleBlock namespace.
  // The getBlockHash function returns the calculated hash of the block.
  // The verifyBlock function verifies the block's signature and Merkle root.
  // The verifySignature function calculates the block's hash and verifies its signature using the provided public key.
  // The setBlockHeight and getBlockHeight functions are used to set and retrieve the block's height, respectively.
  // The getTransactionCount function returns the number of transactions in the block.
  // The isValid function checks if the block is valid based on the transaction count and timestamp.
  // The setBlockchain function sets the blockchain pointer to the provided blockchain object.
  // The addToBlockchain function adds the block to the blockchain if the blockchain pointer is valid.
  // The toJson function converts the block object to JSON format.
  // The fromJson function parses the JSON object and assigns values to the corresponding member variables.
  // The save function saves the block to a file in JSON format.
  // The load function loads a block from a file.
  // The saveToDatabase function saves the block data to the distributed database using the saveData function from the SPHINXDb namespace.
  // The loadFromDatabase function loads a block from the distributed database using the loadData function from the SPHINXDb namespace.

// The code implements classes and functions for block management, Merkle tree construction and verification, signature verification, hash generation, and saving/loading block data to a distributed database. It provides the necessary functionality to work with blocks in a blockchain system.
/////////////////////////////////////////////////////////////////////////////////////////////////////////



#include <stdexcept>
#include <fstream> 
#include <iostream>
#include <ctime>
#include <string>
#include <vector>

#include "Block.hpp"
#include "Hash.hpp"
#include "Sign.hpp"
#include "json.hpp"
#include "MerkleBlock.hpp"
#include "Chain.hpp"
#include "PoW.hpp"
#include "db.hpp"
#include "Verify.hpp"


using json = nlohmann::json;


namespace SPHINXVerify {
    class SPHINX_PublicKey {
    public:
        std::string publicKey;  // Placeholder for the public key

        // Constructor
        SPHINX_PublicKey(const std::string& key) : publicKey(key) {}
    };

    // Function to verify the signature of a block
    bool verifySignature(const std::string& blockHash, const std::string& signature, const SPHINX_PublicKey& publicKey) {
        // Verify the signature of the block using the provided signature and public key
        bool verified = Crypto::verify(blockHash, signature, publicKey.publicKey);
        return verified;
    }
}

namespace SPHINXHash {
    // Function to calculate the SPHINX-256 hash of data
    std::string SPHINX_256(const std::string& data);
}

namespace SPHINX_Chain {
    class Chain {
    public:
        // Function to add a block to the chain
        void addBlock(const SPHINXMerkleBlock::MerkleBlock& block);
    };
}

namespace SPHINXDb {
    class DistributedDb {
    public:
        // Function to save data with a specific block hash
        void saveData(const std::string& data, const std::string& blockHash);

        // Function to load data for a given block ID
        std::string loadData(const std::string& blockId);
    };
}

namespace SPHINXMerkleBlock {
    class MerkleBlock {
    public:
        // Function to construct the Merkle tree
        std::string constructMerkleTree(const std::vector<SPHINXTrx::Transaction>& signedTransactions) const {
            // If there are no signed transactions, return an empty string
            if (signedTransactions.empty()) {
                return "";
            }

            // If there is only one signed transaction, return its SPHINX-256 hash
            if (signedTransactions.size() == 1) {
                return SPHINXHash::SPHINX_256(signedTransactions[0].transaction);
            }

            // Split the signed transactions into two halves
            size_t mid = signedTransactions.size() / 2;
            std::vector<SPHINXTrx::Transaction> leftTransactions(signedTransactions.begin(), signedTransactions.begin() + mid);
            std::vector<SPHINXTrx::Transaction> rightTransactions(signedTransactions.begin() + mid, signedTransactions.end());

            // Recursively construct the Merkle tree for the left and right transactions
            std::string leftRoot = constructMerkleTree(leftTransactions);
            std::string rightRoot = constructMerkleTree(rightTransactions);

            // Concatenate the left and right roots and calculate the SPHINX-256 hash
            return SPHINXHash::SPHINX_256(leftRoot + rightRoot);
        }

        // Function to verify the Merkle root
        bool MerkleBlock::verifyMerkleRoot(const std::string& merkleRoot, const std::vector<SPHINXTrx::Transaction>& signedTransactions) const {
        // Base case: If there are no signed transactions, the Merkle root should be an empty string
        if (signedTransactions.empty()) {
            return merkleRoot.empty();
        }

        // Calculate the constructed Merkle root using the signed transactions
        std::string constructedRoot = constructMerkleTree(signedTransactions);

        // Compare the constructed Merkle root with the provided Merkle root
        return (constructedRoot == merkleRoot);
        }

        std::string MerkleBlock::constructMerkleTree(const std::vector<SPHINXTrx::Transaction>& signedTransactions) const {
            // Base case: If there are no signed transactions, return an empty string
            if (signedTransactions.empty()) {
                return "";
            }

            // Base case: If there is only one signed transaction, return its hash as the Merkle tree root
            if (signedTransactions.size() == 1) {
                return SPHINXMerkleBlock::SPHINXHash::SPHINX_256(signedTransactions[0].transaction);
            }

            // Recursive case: Divide the signed transactions into two halves
            size_t mid = signedTransactions.size() / 2;
            std::vector<SPHINXTrx::Transaction> leftTransactions(signedTransactions.begin(), signedTransactions.begin() + mid);
            std::vector<SPHINXTrx::Transaction> rightTransactions(signedTransactions.begin() + mid, signedTransactions.end());

            // Recursively construct the Merkle tree for the left and right subtrees
            std::string leftRoot = constructMerkleTree(leftTransactions);
            std::string rightRoot = constructMerkleTree(rightTransactions);

            // Combine the left and right roots by hashing them together
            return SPHINXMerkleBlock::SPHINXHash::SPHINX_256(leftRoot + rightRoot);
        }
    };
}

namespace SPHINXBlock {
    Block::Block(const std::string& previousHash)
        : previousHash_(previousHash), blockHeight_(0), nonce_(0), difficulty_(0) {
        timestamp_ = std::time(nullptr);
    }

    std::string Block::calculateBlockHash() const {
        std::string dataToHash = previousHash_ + merkleRoot_ + std::to_string(timestamp_) + std::to_string(nonce_);
        for (const std::string& transaction : transactions_) {
            dataToHash += transaction;
        }

        // Solve the nonce using the Proof-of-Work algorithm from PoW.hpp
        int difficulty = 4; // Specify the desired difficulty level
        std::string blockHash = SPHINXPoW::solveNonce(dataToHash, difficulty);

        return blockHash;
    }

    void Block::addTransaction(const std::string& transaction) {
        transactions_.push_back(transaction);
    }

    std::string Block::calculateMerkleRoot() const {
        return merkleBlock_.constructMerkleTree(transactions_);
    }

    std::string Block::getBlockHash() const {
        return calculateBlockHash();
    }

    bool Block::verifyBlock(const SPHINXVerify::SPHINX_PublicKey& publicKey) const {
        // Verify the block's signature and Merkle root
        return verifySignature(publicKey) && merkleBlock_.verifyMerkleRoot(merkleRoot_, transactions_);
    }

    bool Block::verifySignature(const SPHINXVerify::SPHINX_PublicKey& publicKey) const {
        // Calculate the block's hash and verify its signature using the provided public key
        std::string blockHash = calculateBlockHash();
        return SPHINXVerify::verifySignature(blockHash, signature_, publicKey);
    }

    void Block::setBlockHeight(uint32_t height) {
        // Set the block's height
        blockHeight_ = height;
    }

    uint32_t Block::getBlockHeight() const {
        // Get the block's height
        return blockHeight_;
    }

    uint32_t Block::getTransactionCount() const {
        // Get the number of transactions in the block
        return transactions_.size();
    }

    bool Block::isValid() const {
        // Check if the block is valid based on transaction count and timestamp
        return (transactions_.size() <= MAX_BLOCK_SIZE) && (timestamp_ <= std::time(nullptr) + MAX_TIMESTAMP_OFFSET);
    }

    void Block::setBlockchain(SPHINX_Chain::Chain* blockchain) {
        // Set the blockchain pointer to the provided blockchain object
        blockchain_ = blockchain;
    }

    void Block::addToBlockchain() {
        if (blockchain_) {
            // Add the block to the blockchain (if the blockchain pointer is valid)
            blockchain_->addBlock(merkleBlock_);
        }
    }

    // Block headers
    json Block::toJson() const {
        // Convert the block object to JSON format
        json blockJson;

        blockJson["previousHash"] = previousHash_;     // Store the previous hash in the JSON object
        blockJson["merkleRoot"] = merkleRoot_;         // Store the Merkle root in the JSON object
        blockJson["signature"] = signature_;           // Store the signature in the JSON object
        blockJson["blockHeight"] = blockHeight_;       // Store the block height in the JSON object
        blockJson["timestamp"] = timestamp_;           // Store the timestamp in the JSON object
        blockJson["nonce"] = nonce_;                   // Store the nonce in the JSON object
        blockJson["difficulty"] = difficulty_;         // Store the difficulty in the JSON object

        json transactionsJson = json::array();
        for (const std::string& transaction : transactions_) {
            transactionsJson.push_back(transaction);   // Store each transaction in the JSON array
        }
        blockJson["transactions"] = transactionsJson;  // Store the transactions array in the JSON object

        return blockJson;                              // Return the JSON object
    }

    void Block::fromJson(const json& blockJson) {
        // Parse the JSON object and assign values to the corresponding member variables
        previousHash_ = blockJson["previousHash"].get<std::string>();     // Retrieve the previous hash from the JSON object
        merkleRoot_ = blockJson["merkleRoot"].get<std::string>();         // Retrieve the Merkle root from the JSON object
        signature_ = blockJson["signature"].get<std::string>();           // Retrieve the signature from the JSON object
        blockHeight_ = blockJson["blockHeight"].get<uint32_t>();          // Retrieve the block height from the JSON object
        timestamp_ = blockJson["timestamp"].get<std::time_t>();           // Retrieve the timestamp from the JSON object
        nonce_ = blockJson["nonce"].get<uint32_t>();                      // Retrieve the nonce from the JSON object
        difficulty_ = blockJson["difficulty"].get<uint32_t>();            // Retrieve the difficulty from the JSON object

        transactions_.clear();
        const json& transactionsJson = blockJson["transactions"];
        for (const auto& transactionJson : transactionsJson) {
            transactions_.push_back(transactionJson.get<std::string>());  // Retrieve each transaction from the JSON array
        }
    }

    bool Block::save(const std::string& filename) const {
        // Convert the block object to JSON format
        json blockJson = toJson();
        
        // Open the output file stream
        std::ofstream outputFile(filename);
        if (outputFile.is_open()) {
            // Write the JSON data to the file with indentation
            outputFile << blockJson.dump(4);
            outputFile.close();
            return true; // Return true to indicate successful save
        }
        return false; // Return false to indicate failed save
    }

    Block Block::load(const std::string& filename) {
        // Open the input file stream
        std::ifstream inputFile(filename);
        if (inputFile.is_open()) {
            // Parse the JSON data from the file
            json blockJson;
            inputFile >> blockJson;
            inputFile.close();

            // Create a new block object and initialize it from the parsed JSON
            Block loadedBlock;
            loadedBlock.fromJson(blockJson);
            return loadedBlock; // Return the loaded block
        }
        throw std::runtime_error("Failed to load block from file: " + filename); // Throw an exception if the file could not be opened
    }

    bool Block::saveToDatabase(SPHINXDb::DistributedDb& distributedDb) const {
        // Convert the block object to JSON format
        json blockJson = toJson();

        // Get the block hash as the database key
        std::string blockId = getBlockHash();
        
        // Convert the JSON data to a string
        std::string blockData = blockJson.dump();

        // Save the block data to the distributed database
        distributedDb.saveData(blockData, blockId);

        return true;
    }

    Block Block::loadFromDatabase(const std::string& blockId, SPHINXDb::DistributedDb& distributedDb) {
        std::string blockData = distributedDb.loadData(blockId); // Load the block data from the distributed database
        json blockJson = json::parse(blockData); // Parse the JSON string

        Block loadedBlock;
        loadedBlock.fromJson(blockJson); // Initialize the block from the JSON
        return loadedBlock;
    }
} // namespace SPHINXBlock

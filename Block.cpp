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
// This code appears to be a C++ program that implements various classes and namespaces related to SPHINX blockchain project for functionality.

// The Block class:
    // The class has private member variables that store block-related information, such as the previous block's hash, Merkle root, signature, block height, timestamp, nonce, mining difficulty, list of transactions, a pointer to the blockchain, and a reference to the list of checkpoint blocks.

// Constructors:
    // The class has two constructors: one without the checkpointBlocks parameter and one with it. Both constructors initialize some member variables and set the timestamp to the current time.

// Member Functions:
    // calculateBlockHash(): This function is not fully implemented in the provided code. It is intended to calculate and return the hash of the block using various block-related information.
    // addTransaction(const std::string& transaction): This function adds a transaction to the block's list of transactions.
    // calculateMerkleRoot() const: This function calls the constructMerkleTree() function from the SPHINXMerkleBlock namespace to calculate and return the Merkle root hash of the transactions included in the block.
    // signMerkleRoot(const SPHINXPrivKey& privateKey, const std::string& merkleRoot): This function is intended to sign the Merkle root using the provided private key (SPHINXPrivKey) and return the signature. The actual signing logic is assumed to be available in the library.
    // storeMerkleRootAndSignature(const std::string& merkleRoot, const std::string& signature): This function is not fully implemented and is intended to store the Merkle root and signature in the block header. The actual storage implementation is missing.
    // getBlockHash() const: This function calls calculateBlockHash() to retrieve the hash of the block.
    // verifyBlock(const SPHINXMerkleBlock::SPHINXPubKey& publicKey) const: This function is intended to verify the block's signature and Merkle root using the provided public key (SPHINXPubKey). The actual verification logic is not fully implemented.
    // verifySignature(const std::string& blockHash, const std::string& signature, const SPHINXMerkleBlock::SPHINXPubKey& publicKey) const: This function is intended to implement the verification logic for the block signature using the provided block hash, signature, and public key. The actual implementation is missing.
    // verifySignature(const SPHINXVerify::SPHINXPubKey& publicKey) const: This function calculates the block hash and then calls the verifySignature() function mentioned above to verify the block signature using the calculated hash, the block's stored signature, and the provided public key.
    // mineBlock(uint32_t difficulty): This function attempts to mine the block by incrementing the nonce_ value and recalculating the block hash until a hash meeting the specified mining difficulty (number of leading zeros) is found. The mining process is simulated by repeatedly recalculating the hash without adjusting the timestamp or other mining parameters.
    // Setters and Getters: Functions to set and get various member variables of the Block class.

// JSON Serialization Functions:
    // toJson() const: This function converts the Block object into a JSON object, representing block-related data.
    // fromJson(const nlohmann::json& blockJson): This function parses a JSON object and sets the Block object's member variables accordingly.
    // save(const std::string& filename) const: This function saves the Block object's data as a JSON object in a file with the specified filename.
    // load(const std::string& filename): This static function loads a Block object from a JSON file with the specified filename.

// Database Integration Functions:
    // saveToDatabase(SPHINXDb::DistributedDb& distributedDb) const: This function converts the Block object to a JSON object and saves it to a distributed database (SPHINXDb::DistributedDb) using the block's hash as the database key.
    // loadFromDatabase(const std::string& blockId, SPHINXDb::DistributedDb& distributedDb): This static function loads a Block object from the distributed database using the provided block hash (blockId) and the database object.

// Interactions:
    // The "MerkleBlock" class is used to construct and verify the Merkle tree for the transactions included in a block. It is called from the "Block" class to calculate and set the Merkle root of the block.
    // The "Block" class interacts with the "MerkleBlock" class to obtain the Merkle root, which is then used to sign the block with SPHINCS+ private key. The signed block is then stored, and the Merkle root and signature are added to the block's header.
    // When verifying the block's integrity, the "Block" class calls functions from the "MerkleBlock" class to verify the signature and Merkle root against the transactions.
    
// The code represents a simplified implementation of a blockchain system with functionality related to block creation, verification, mining, Merkle tree construction, and database interaction.
/////////////////////////////////////////////////////////////////////////////////////////////////////////



#include <stdexcept>
#include <fstream> 
#include <iostream>
#include <ctime>
#include <string>
#include <vector>
#include <array>

#include "Block.hpp"
#include "Hash.hpp"
#include "Sign.hpp"
#include "json.hpp"
#include "MerkleBlock.hpp"
#include "Transaction.hpp"
#include "Chain.hpp"
#include "PoW.hpp"
#include "db.hpp"
#include "Verify.hpp"
#include "PoW.hpp"
#include "Key.hpp"


using json = nlohmann::json;

namespace SPHINXHash {
    std::string SPHINX_256(const std::string& data); // Function declaration for SPHINX_256
}

namespace SPHINXMerkleBlock {
    class MerkleBlock; // Forward declaration of the MerkleBlock class

    // Correct the constructMerkleTree function declaration
    std::string constructMerkleTree(const std::vector<SPHINXTrx::Transaction>& signedTransactions);
}

namespace SPHINXBlock {
    class Block {
    private:
        // Private member variables
        std::string previousHash_;               // The hash of the previous block in the blockchain
        std::string merkleRoot_;                 // The Merkle root hash of the transactions in the block
        std::string signature_;                  // The signature of the block
        uint32_t blockHeight_;                   // The position of the block within the blockchain
        std::time_t timestamp_;                  // The time when the block was created
        uint32_t nonce_;                         // A random value used in the mining process to find a valid block hash
        uint32_t difficulty_;                    // A measure of how hard it is to find a valid block hash (mining difficulty)
        std::vector<std::string> transactions_;  // The list of transactions included in the block
        SPHINXChain::Chain* blockchain_;         // A pointer to the blockchain (assuming SPHINXChain::Chain is a class)
        const std::vector<std::string>& checkpointBlocks_; // Reference to the list of checkpoint blocks

    public:
        static const uint32_t MAX_BLOCK_SIZE = 1000;       // Maximum allowed block size in number of transactions
        static const uint32_t MAX_TIMESTAMP_OFFSET = 600;  // Maximum allowed timestamp difference from current time

        // Constructor without checkpointBlocks parameter
        Block(const std::string& previousHash)
            : previousHash_(previousHash), blockHeight_(0), nonce_(0), difficulty_(0) {
            timestamp_ = std::time(nullptr); // Set the timestamp to the current time
        }

        // Constructor with the addition of checkpointBlocks parameter
        Block(const std::string& previousHash, const std::vector<std::string>& checkpointBlocks)
            : previousHash_(previousHash), checkpointBlocks_(checkpointBlocks), blockHeight_(0), nonce_(0), difficulty_(0) {
            timestamp_ = std::time(nullptr); // Set the timestamp to the current time
        }

        std::string Block::calculateBlockHash() const {
            // Implementation of calculateBlockHash function
            // Add your code here
        }

        void Block::addTransaction(const std::string& transaction) {
            transactions_.push_back(transaction);
        }

        std::string Block::calculateMerkleRoot() const {
            return SPHINXMerkleBlock::constructMerkleTree(transactions_);
        }

        // Function to sign the Merkle root with SPHINCS+ private key
        std::string signMerkleRoot(const SPHINXPrivKey& privateKey, const std::string& merkleRoot) {
            // Assuming the SPHINCS+ signing function is available in the library
            std::string signature = SPHINCSPlus::sign(merkleRoot, privateKey);
            return signature;
        }

        // Function to store the Merkle root and signature in the header of the block
        void storeMerkleRootAndSignature(const std::string& merkleRoot, const std::string& signature) {
            // Store the Merkle root and signature in the block header
            // Replace the following lines with your actual storage implementation
            std::cout << "Merkle Root: " << merkleRoot << std::endl;
            std::cout << "Signature: " << signature << std::endl;
            // Add other block-related information as needed
        }

        // Get the hash of the block by calling the calculateBlockHash() function
        std::string getBlockHash() const {
            return calculateBlockHash();
        }

        // Definition of the member function verifyBlock inside the SPHINXBlock namespace
        bool SPHINXBlock::Block::verifyBlock(const SPHINXMerkleBlock::SPHINXPubKey& publicKey) const {
            // Call the verifySignature and verifyMerkleRoot functions from the MerkleBlock class
            return verifySignature(publicKey) && merkleBlock.verifyMerkleRoot(merkleRoot_, transactions_);
        }

        bool SPHINXBlock::Block::verifySignature(const std::string& blockHash, const std::string& signature, const SPHINXMerkleBlock::SPHINXPubKey& publicKey) const {
            // Implementation of the verification logic
            // Add your code here
        }

        bool verifySignature(const SPHINXVerify::SPHINXPubKey& publicKey) const {
            // Calculate the block hash
            std::string blockHash = calculateBlockHash();

            // Verify the signature of the block using the block hash, signature, and public key
            return verifySignature(blockHash, signature_, publicKey);
        }

        bool mineBlock(uint32_t difficulty) {
            std::string target(difficulty, '0');  // Create a target string with the specified difficulty level

            while (true) {
                nonce_++;  // Increment the nonce

                // Calculate the block hash
                std::string blockHash = calculateBlockHash();

                // Check if the block hash meets the target difficulty
                if (blockHash.substr(0, difficulty) == target) {
                    // Block successfully mined
                    return true;
                }
            }

            // Block mining failed
            return false;
        }

        // Setters and getters for the remaining member variables
        void setMerkleRoot(const std::string& merkleRoot) {
            merkleRoot_ = merkleRoot;
        }

        // Sets the signature of the block
        void setSignature(const std::string& signature) {
            signature_ = signature;
        }

        // Sets the block height (the position of the block within the blockchain)
        void setBlockHeight(uint32_t blockHeight) {
            blockHeight_ = blockHeight;
        }

        // Sets the nonce (a random value used in the mining process to find a valid block hash)
        void setNonce(uint32_t nonce) {
            nonce_ = nonce;
        }

        // Sets the difficulty level of mining (a measure of how hard it is to find a valid block hash)
        void setDifficulty(uint32_t difficulty) {
            difficulty_ = difficulty;
        }

        // Sets the transactions included in the block
        void setTransactions(const std::vector<std::string>& transactions) {
            transactions_ = transactions;
        }

        // Returns the previous hash (the hash of the previous block in the blockchain)
        std::string getPreviousHash() const {
            return previousHash_;
        }

        // Returns the Merkle root (the root hash of the Merkle tree constructed from the transactions)
        std::string getMerkleRoot() const {
            return merkleRoot_;
        }

        // Returns the signature of the block
        std::string getSignature() const {
            return signature_;
        }

        // Returns the block height (the position of the block within the blockchain)
        uint32_t getBlockHeight() const {
            return blockHeight_;
        }

        // Returns the timestamp (the time when the block was created)
        std::time_t getTimestamp() const {
            return timestamp_;
        }

        // Returns the nonce (a random value used in the mining process to find a valid block hash)
        uint32_t getNonce() const {
            return nonce_;
        }

        // Returns the difficulty level of mining (a measure of how hard it is to find a valid block hash)
        uint32_t getDifficulty() const {
            return difficulty_;
        }

        // Returns the transactions included in the block
        std::vector<std::string> getTransactions() const {
            return transactions_;
        }

        Block::Block(const std::string& prevBlockHash, const std::string& timestamp, const std::string& nonce, const std::vector<Transaction>& transactions) {
            this->prevBlockHash = prevBlockHash;
            this->timestamp = timestamp;
            this->nonce = nonce;
            this->transactions = transactions;

            // Assuming the Merkle tree construction function is already implemented in SPHINXMerkleBlock namespace
            std::string merkleRoot = SPHINXMerkleBlock::constructMerkleTree(transactions);
            this->setMerkleRoot(merkleRoot); // Set the Merkle root for this block
        }

        // Block headers
        nlohmann::json toJson() const {
            // Convert the block object to JSON format
            nlohmann::json blockJson;

            blockJson["previousHash"] = previousHash_;     // Store the previous hash in the JSON object
            blockJson["merkleRoot"] = merkleRoot_;         // Store the Merkle root in the JSON object
            blockJson["signature"] = signature_;           // Store the signature in the JSON object
            blockJson["blockHeight"] = blockHeight_;       // Store the block height in the JSON object
            blockJson["timestamp"] = timestamp_;           // Store the timestamp in the JSON object
            blockJson["nonce"] = nonce_;                   // Store the nonce in the JSON object
            blockJson["difficulty"] = difficulty_;         // Store the difficulty in the JSON object

            nlohmann::json transactionsJson = nlohmann::json::array();
            for (const std::string& transaction : transactions_) {
                transactionsJson.push_back(transaction);   // Store each transaction in the JSON array
            }
            blockJson["transactions"] = transactionsJson;  // Store the transactions array in the JSON object

            return blockJson;                              // Return the JSON object
        }

        void fromJson(const nlohmann::json& blockJson) {
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

        bool save(const std::string& filename) const {
            // Convert the block object to JSON format
            nlohmann::json blockJson = toJson();

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

        static Block load(const std::string& filename) {
            // Open the input file stream
            std::ifstream inputFile(filename);
            if (inputFile.is_open()) {
                // Parse the JSON data from the file
                nlohmann::json blockJson;
                inputFile >> blockJson;
                inputFile.close();

                // Create a new block object and initialize it from the parsed JSON
                Block loadedBlock("");
                loadedBlock.fromJson(blockJson);
                return loadedBlock; // Return the loaded block
            }
            throw std::runtime_error("Failed to load block from file: " + filename); // Throw an exception if the file could not be opened
        }

        bool saveToDatabase(SPHINXDb::DistributedDb& distributedDb) const {
            // Convert the block object to JSON format
            nlohmann::json blockJson = toJson();

            // Get the block hash as the database key
            std::string blockId = getBlockHash();

            // Convert the JSON data to a string
            std::string blockData = blockJson.dump();

            // Save the block data to the distributed database
            distributedDb.saveData(blockData, blockId);

            return true;
        }

        static Block loadFromDatabase(const std::string& blockId, SPHINXDb::DistributedDb& distributedDb) {
            std::string blockData = distributedDb.loadData(blockId); // Load the block data from the distributed database
            nlohmann::json blockJson = nlohmann::json::parse(blockData); // Parse the JSON string

            Block loadedBlock("");
            loadedBlock.fromJson(blockJson); // Initialize the block from the JSON
            return loadedBlock;
        }
    };
} // namespace SPHINXBlock

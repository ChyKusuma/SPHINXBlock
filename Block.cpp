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
// This code appears to be a C++ program that implements various classes and namespaces related to blockchain functionality

// SPHINXVerify Namespace:
    // The SPHINXVerify namespace contains a class called SPHINX_PublicKey, which represents a public key used for signature verification.
    // It has a single member variable publicKey, which holds the public key value.
    // The constructor SPHINX_PublicKey(const std::string& key) initializes the publicKey variable with the provided key.
    // The verifySignature function in the SPHINXVerify namespace verifies the signature of a block by calling a Crypto::verify function, which is assumed to be implemented elsewhere. It takes three parameters: blockHash, signature, and publicKey. It returns true if the signature is verified successfully, and false otherwise.

// SPHINXHash Namespace:
    // The SPHINXHash namespace contains a function called SPHINX_256, which calculates the SPHINX-256 hash of the given data. The implementation of this function is not provided in the code snippet.

// SPHINXDb Namespace:
    // The SPHINXDb namespace contains a class called DistributedDb, which represents a distributed database used to store data with block hashes as keys.
    // The saveData function stores data in the distributed database using the provided data and blockHash parameters.
    // The loadData function retrieves data from the distributed database based on the given blockId. It returns an empty string if no data is found for the given blockId.

// SPHINXMerkleBlock Namespace:
    // The SPHINXMerkleBlock namespace contains a class called MerkleBlock, which is responsible for constructing and verifying Merkle trees.
    // The constructMerkleTree function constructs the Merkle tree for a vector of signed transactions.
    // The verifyMerkleRoot function verifies the provided Merkle root against the signed transactions by calling the buildMerkleRoot function and comparing the calculated root with the provided root.
    // The buildMerkleRoot function is a helper function that recursively builds the Merkle root using a vector of transactions.

// SPHINXBlock Namespace:
    // The SPHINXBlock namespace contains a class called Block, representing a block in the blockchain.
    // It has several member variables representing block attributes such as previousHash_, merkleRoot_, signature_, blockHeight_, timestamp_, nonce_, difficulty_, transactions_, and a pointer to a SPHINXChain::Chain object called blockchain_.
    // The MAX_BLOCK_SIZE and MAX_TIMESTAMP_OFFSET are constants defining the maximum size of a block and the maximum allowed time difference between the current time and the block's timestamp, respectively.

// The Block class provides various functions to perform operations on the block:
    // calculateBlockHash calculates the hash of the block by concatenating relevant block data and solving the nonce using the Proof-of-Work algorithm.
    // addTransaction adds a transaction to the list of transactions in the block.
    // calculateMerkleRoot calculates and returns the Merkle root of the transactions using the SPHINXMerkleBlock::constructMerkleTree function.
    // getBlockHash returns the hash of the block by calling the calculateBlockHash function.
    // verifyBlock verifies the block by checking its signature and Merkle root using the provided public key.
    // verifySignature verifies the signature of the block using the provided block hash, signature, and public key.
    // mineBlock attempts to mine the block by finding a valid block hash that meets the specified difficulty level.
    // Various setter and getter functions are provided to access and modify the block's member variables.
    // toJson converts the block object to JSON format.
    // fromJson parses a JSON object and assigns values to the corresponding member variables.
    // save saves the block to a file in JSON format.
    // load loads a block from a file in JSON format.
    // saveToDatabase saves the block data to a distributed database using the provided SPHINXDb::DistributedDb object.
    // loadFromDatabase loads a block from the distributed database based on the given block ID using the provided SPHINXDb::DistributedDb object.
    
// The code represents a simplified implementation of a blockchain system with functionality related to block creation, verification, mining, Merkle tree construction, and database interaction.
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
#include "PoW.hpp"


using json = nlohmann::json;


// Forward declarations

// SPHINXVerify namespace
namespace SPHINXVerify {
    class SPHINX_PublicKey; // Forward declaration of the SPHINX_PublicKey class
    bool verifySignature(const std::string& blockHash, const std::string& signature, const SPHINX_PublicKey& publicKey); // Forward declaration of the verifySignature function
}

// SPHINXHash namespace
namespace SPHINXHash {
    std::string SPHINX_256(const std::string& data); // Function declaration for SPHINX_256
}

// SPHINXVerify namespace (continued)
namespace SPHINXVerify {
    bool verifySignature(const std::string& blockHash, const std::string& signature, const SPHINXVerify::SPHINX_PublicKey& publicKey) {
        // Verify the signature of the block using the provided signature and public key
        bool verified = Crypto::verify(blockHash, signature, publicKey.publicKey);
        return verified;
    }
}

// SPHINXMerkleBlock namespace
namespace SPHINXMerkleBlock {
    class MerkleBlock; // Forward declaration of the MerkleBlock class
    std::string constructMerkleTree(const std::vector<SPHINXTrx::Transaction>& signedTransactions); // Function declaration for constructMerkleTree
    bool verifyMerkleRoot(const std::string& merkleRoot, const std::vector<SPHINXTrx::Transaction>& signedTransactions); // Function declaration for verifyMerkleRoot
}

namespace SPHINXBlock {
    class Block {
    private:
        std::string previousHash_;     // Hash of the previous block in the blockchain
        std::string merkleRoot_;       // Root hash of the Merkle tree constructed from the transactions
        std::string signature_;        // Signature of the block
        uint32_t blockHeight_;         // Height (position) of the block in the blockchain
        std::time_t timestamp_;        // Timestamp indicating when the block was created
        uint32_t nonce_;               // Nonce used in the mining process to find a valid block hash
        uint32_t difficulty_;          // Difficulty level of mining
        std::vector<std::string> transactions_;   // Transactions included in the block
        SPHINXChain::Chain* blockchain_;  // Pointer to the blockchain object

    public:
        static const uint32_t MAX_BLOCK_SIZE = 1000;         // Maximum size of a block
        static const uint32_t MAX_TIMESTAMP_OFFSET = 600;    // Maximum allowed time difference between the current time and the block's timestamp

        // Constructor
        Block(const std::string& previousHash)
            : previousHash_(previousHash), blockHeight_(0), nonce_(0), difficulty_(0) {
            timestamp_ = std::time(nullptr);    // Set the timestamp to the current time
        }

        // Function to calculate the hash of the block
        std::string calculateBlockHash() const {
            std::string dataToHash = previousHash_ + merkleRoot_ + std::to_string(timestamp_) + std::to_string(nonce_);
            for (const std::string& transaction : transactions_) {
                dataToHash += transaction;
            }

            // Solve the nonce using the Proof-of-Work algorithm from PoW.hpp
            int difficulty = 4; // Specify the desired difficulty level
            std::string blockHash = SPHINXPoW::solveNonce(dataToHash, difficulty);

            return blockHash;
        }

        // Add a transaction to the list of transactions in the block
        void addTransaction(const std::string& transaction) {
            transactions_.push_back(transaction);
        }

        // Calculate and return the Merkle root of the transactions
        std::string calculateMerkleRoot() const {
            return SPHINXMerkleBlock::constructMerkleTree(transactions_);
        }

        // Get the hash of the block by calling the calculateBlockHash() function
        std::string getBlockHash() const {
            return calculateBlockHash();
        }

        bool verifyBlock(const SPHINXVerify::SPHINX_PublicKey& publicKey) const {
            // Verify the block's signature and Merkle root
            return verifySignature(publicKey) && SPHINXMerkleBlock::verifyMerkleRoot(merkleRoot_, transactions_);
        }

        bool verifySignature(const std::string& blockHash, const std::string& signature, const SPHINXVerify::SPHINX_PublicKey& publicKey);

        bool verifySignature(const SPHINXVerify::SPHINX_PublicKey& publicKey) const {
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

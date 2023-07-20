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



#ifndef SPHINXBLOCK_HPP
#define SPHINXBLOCK_HPP

#pragma once

#include <stdexcept>
#include <fstream> 
#include <iostream>
#include <ctime>
#include <string>
#include <vector>
#include <array>

#include "Hash.hpp"
#include "Sign.hpp"
#include "json.hpp"
#include "MerkleBlock.hpp"
#include "Chain.hpp"
#include "PoW.hpp"
#include "db.hpp"
#include "Verify.hpp"
#include "PoW.hpp"
#include "Transaction.hpp"
#include "Params.hpp"


namespace SPHINXVerify {
    class SPHINXPubKey {
    public:
        std::string publicKey;  // Placeholder for the public key

        // Constructor
        SPHINXPubKey(const std::string& key) : publicKey(key) {}
    };

    // Function to verify the signature of a block
    bool verifySignature(const std::string& blockHash, const std::string& signature, const SPHINXPubKey& publicKey);
}

namespace SPHINXHash {
    // Function to calculate the SPHINX-256 hash of data
    std::string SPHINX_256(const std::string& data);
}

namespace SPHINXDb {
    class DistributedDb {
    public:
        // Function to save data with a specific block hash
        void saveData(const std::string& data, const std::string& blockHash);

        // Function to load data for a given block ID
        std::string loadData(const std::string& blockId);

    private:
        std::unordered_map<std::string, std::string> database_;
    };
}

namespace SPHINXMerkleBlock {
    class MerkleBlock {
    public:
        // Function to construct the Merkle tree
        std::string constructMerkleTree(const std::vector<SPHINXTrx::Transaction>& signedTransactions) const;

        // Function to verify the Merkle root
        bool verifyMerkleRoot(const std::string& merkleRoot, const std::vector<SPHINXTrx::Transaction>& signedTransactions) const;

    private:
        // Function to build the Merkle root recursively
        std::string buildMerkleRoot(const std::vector<std::string>& transactions) const;
    };
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

        // Checkpoint blocks for each network
        const std::vector<std::string>& checkpointBlocks_;

    public:
        // Constructor with the addition of checkpointBlocks parameter
        Block(const std::string& previousHash, const std::vector<std::string>& checkpointBlocks);

        static const uint32_t MAX_BLOCK_SIZE = 1000;         // Maximum size of a block
        static const uint32_t MAX_TIMESTAMP_OFFSET = 600;    // Maximum allowed time difference between the current time and the block's timestamp

        // Constructor
        Block(const std::string& previousHash);

        // Function to calculate the hash of the block
        std::string calculateBlockHash() const;

        // Add a transaction to the list of transactions in the block
        void addTransaction(const std::string& transaction);

        // Calculate and return the Merkle root of the transactions
        std::string calculateMerkleRoot() const;

        // Get the hash of the block by calling the calculateBlockHash() function
        std::string getBlockHash() const;

        // Function to verify the block using checkpoint verification
        bool verifyBlockWithCheckpoint(const SPHINXVerify::SPHINXPubKey& publicKey) const;

        bool verifyBlock(const SPHINXVerify::SPHINXPubKey& publicKey) const;

        bool verifySignature(const std::string& blockHash, const std::string& signature, const SPHINXVerify::SPHINXPubKey& publicKey);

        bool verifySignature(const SPHINXVerify::SPHINXPubKey& publicKey) const;

        bool mineBlock(uint32_t difficulty);

        // Setters and getters for the remaining member variables
        void setMerkleRoot(const std::string& merkleRoot);

        void setSignature(const std::string& signature);

        void setBlockHeight(uint32_t blockHeight);

        void setNonce(uint32_t nonce);

        void setDifficulty(uint32_t difficulty);

        void setTransactions(const std::vector<std::string>& transactions);

        std::string getPreviousHash() const;

        std::string getMerkleRoot() const;

        std::string getSignature() const;

        uint32_t getBlockHeight() const;

        std::time_t getTimestamp() const;

        uint32_t getNonce() const;

        uint32_t getDifficulty() const;

        std::vector<std::string> getTransactions() const;

        nlohmann::json toJson() const;

        void fromJson(const nlohmann::json& blockJson);

        bool save(const std::string& filename) const;

        static Block load(const std::string& filename);

        bool saveToDatabase(SPHINXDb::DistributedDb& distributedDb) const;

        static Block loadFromDatabase(const std::string& blockId, SPHINXDb::DistributedDb& distributedDb);
    };
}

#endif // SPHINXBLOCK_HPP

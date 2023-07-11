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



#ifndef BLOCK_HPP
#define BLOCK_HPP

#pragma once

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

namespace SPHINXVerify {
    class SPHINX_PublicKey {
    public:
        std::string publicKey;  // Placeholder for the public key

        // Constructor
        SPHINX_PublicKey(const std::string& key) : publicKey(key) {}
    };

    // Function to verify the signature of a block
    bool verifySignature(const std::string& blockHash, const std::string& signature, const SPHINX_PublicKey& publicKey);
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
        std::string constructMerkleTree(const std::vector<SPHINXTrx::Transaction>& signedTransactions) const;

        // Function to verify the Merkle root
        bool verifyMerkleRoot(const std::string& merkleRoot, const std::vector<SPHINXTrx::Transaction>& signedTransactions) const;
    };
}

namespace SPHINXBlock {
    class Block {
    private:
        std::string previousHash_;
        std::string merkleRoot_;
        std::string signature_;
        uint32_t blockHeight_;
        std::time_t timestamp_;
        uint32_t nonce_;
        uint32_t difficulty_;
        std::vector<std::string> transactions_;
        SPHINX_Chain::Chain* blockchain_;

    public:
        Block(const std::string& previousHash);

        std::string calculateBlockHash() const;

        void addTransaction(const std::string& transaction);

        std::string calculateMerkleRoot() const;

        std::string getBlockHash() const;

        bool verifyBlock(const SPHINXVerify::SPHINX_PublicKey& publicKey) const;

        bool verifySignature(const SPHINXVerify::SPHINX_PublicKey& publicKey) const;

        void setBlockHeight(uint32_t height);

        uint32_t getBlockHeight() const;

        uint32_t getTransactionCount() const;

        bool isValid() const;

        void setBlockchain(SPHINX_Chain::Chain* blockchain);

        void addToBlockchain();

        // Block headers
        json toJson() const;

        void fromJson(const json& blockJson);

        bool save(const std::string& filename) const;

        static Block load(const std::string& filename);

        bool saveToDatabase(SPHINXDb::DistributedDb& distributedDb) const;

        static Block loadFromDatabase(const std::string& blockId, SPHINXDb::DistributedDb& distributedDb);
    };
}

#endif  // BLOCK_HPP

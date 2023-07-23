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

#include <cstdint>
#include <iostream>
#include <unordered_map>

#include <stdexcept>
#include <fstream> 
#include <iostream>
#include <ctime>
#include <string>
#include <vector>
#include <array>

#include "json.hpp"
#include "Params.hpp"
#include "MerkleBlock.hpp"


namespace SPHINXHash {
    std::string SPHINX_256(const std::string& data);
}

namespace SPHINXMerkleBlock {
    class MerkleBlock;

    std::string constructMerkleTree(const std::vector<SPHINXTrx::Transaction>& signedTransactions);
}

namespace SPHINXDb {
    class DistributedDb;
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
        SPHINXChain::Chain* blockchain_;
        const std::vector<std::string>& checkpointBlocks_;

        std::string storedMerkleRoot_;
        std::string storedSignature_;

    public:
        static const uint32_t MAX_BLOCK_SIZE;
        static const uint32_t MAX_TIMESTAMP_OFFSET;

        Block(const std::string& previousHash);
        Block(const std::string& previousHash, const std::vector<std::string>& checkpointBlocks);

        void addTransaction(const std::string& transaction);
        std::string calculateBlockHash() const;
        std::string calculateMerkleRoot() const;
        void signMerkleRoot(const SPHINXPrivKey& privateKey, const std::string& merkleRoot);
        bool verifySignature(const SPHINXPubKey& publicKey) const;
        bool verifyMerkleRoot(const SPHINXPubKey& publicKey) const;
        bool verifyBlock(const SPHINXPubKey& publicKey) const;
        bool mineBlock(uint32_t difficulty);

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

        Block(const std::string& prevBlockHash, const std::string& timestamp, const std::string& nonce, const std::vector<Transaction>& transactions);

        nlohmann::json toJson() const;
        void fromJson(const nlohmann::json& blockJson);

        bool save(const std::string& filename) const;
        static Block load(const std::string& filename);

        bool saveToDatabase(SPHINXDb::DistributedDb& distributedDb) const;
        static Block loadFromDatabase(const std::string& blockId, SPHINXDb::DistributedDb& distributedDb);

        std::string getStoredMerkleRoot() const;
        std::string getStoredSignature() const;
    };
} // namespace SPHINXBlock

#endif // SPHINX_BLOCK_HPP

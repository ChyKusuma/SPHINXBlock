# SPHINXBlock

## Introduction

This project is dedicated to the world community as an Open-source Post-quantum SPHINXPoW blockchain, means anyone can join and contribute based on his/ her passion and skills. SPHINX is a blockchain protocol designed to provide secure and scalable solutions in the post-quantum era. The project aims to develop a robust and decentralized network using a Proof of Work (PoW) consensus algorithm initiated by the community. We thrilled to introduce a new design for Proof-of-Work operation.

This repository contains code for the SPHINXBlock blockahain project, which is a mining module for the SPHINX blockchain. The SPHINXBlock aims to provide a "Block" in the blockchain systems


## Function Explanation

#### `SPHINXVerify::verifySignature`

This function verifies the signature of a block. It takes the block's hash, signature, and a SPHINX public key as input parameters. The function checks the validity of the block's signature using the provided public key and returns `true` if the signature is valid.

#### `SPHINXHash::SPHINX_256`

This function calculates the SPHINX-256 hash of the given data. It takes the data as input and returns the corresponding hash.

#### `SPHINX_Chain::Chain::addBlock`

This function is used to add a block to the chain. It takes a `SPHINXMerkleBlock::MerkleBlock` object as input and adds it to the chain.

#### `SPHINXDb::DistributedDb::saveData`

This function is responsible for saving data with a specific block hash. It takes the data and block hash as input parameters and saves the data in the distributed database.

#### `SPHINXDb::DistributedDb::loadData`

This function is used to load data for a given block ID. It takes the block ID as input and retrieves the corresponding data from the distributed database.

#### `SPHINXMerkleBlock::MerkleBlock::constructMerkleTree`

This function constructs the Merkle tree for a given vector of `SPHINXTrx::Transaction` objects. It recursively divides the transactions into two halves, constructs the Merkle tree for each half, and then combines the roots of the left and right subtrees. The final root is calculated as the SPHINX-256 hash of the concatenated left and right roots.

#### `SPHINXMerkleBlock::MerkleBlock::verifyMerkleRoot`

This function verifies the Merkle root for a given vector of transactions. It compares the provided Merkle root with the root calculated from the transactions using the `constructMerkleTree` function. If they match, the function returns `true`; otherwise, it returns `false`.

#### `SPHINXBlock::Block`

This class represents a block in the SPHINX blockchain. It contains member functions for various block operations.

- `Block::calculateBlockHash`: This function calculates the block's hash by concatenating the previous hash, Merkle root, timestamp, nonce, and transactions. It also uses the Proof-of-Work algorithm from `SPHINXPoW::solveNonce` to solve the nonce and obtain the final block hash.

- `Block::addTransaction`: This function adds a transaction to the block's transaction vector.

- `Block::calculateMerkleRoot`: This function calculates the Merkle root of the block's transactions using the `constructMerkleTree` function from `SPHINXMerkleBlock::MerkleBlock`.

- `Block::getBlockHash`: This function returns the block's hash by calling `calculateBlockHash`.

- `Block::verifyBlock`: This function verifies the block by checking its signature using the provided public key and verifying the Merkle root using `merkleBlock_.verifyMerkleRoot`.

- `Block::verifySignature`: This function calculates the block's hash and verifies its signature using the provided public key.

- `Block::setBlockHeight`: This function sets the block's height.

- `Block::getBlockHeight`: This function returns the block's height.

- `Block::getTransactionCount`: This function returns the number of transactions in the block.

- `Block::isValid`: This function checks if the block is valid based on the transaction count and timestamp.

- `Block::setBlockchain`: This function sets the blockchain pointer to the provided blockchain object.

- `Block::addToBlockchain`: This function adds the block to the blockchain by calling `blockchain_->addBlock(merkleBlock_)` if the blockchain pointer is valid.

- `Block::toJson` and `Block::fromJson`: These functions convert the block object to JSON format and parse a JSON object to initialize the block, respectively.

- `Block::save` and `Block::load`: These functions save a block to a file in JSON format and load a block from a file, respectively.

- `Block::saveToDatabase` and `Block::loadFromDatabase`: These functions save and load a block to/from a distributed database, respectively, using JSON format.


## Getting Started

To get started with the SPHINXMiner project, follow these steps:

1. Clone the repository: `git clone https://github.com/your-username/SPHINXMiner.git`
2. Install the necessary dependencies.
3. Explore the codebase to understand the project structure and components.
4. Use the provided functions to mine blocks in the SPHINX blockchain.

### This repository is part of the  [SPHINXPoW](https://github.com/SPHINX-HUB-ORG/SPHINXPoW) [SPHINXMiner](https://github.com/SPHINX-HUB-ORG/SPHINXMINER)

Please note that the code in this repository is a part of the SPHINXPoW algorithm, which is currently in development and not fully integrated or extensively tested for functionality. The purpose of this repository is to provide a framework and algorithm for the mining scheme in the SPHINX blockchain project.

As the project progresses, further updates and enhancements will be made to ensure the code's stability and reliability. We encourage contributors to participate in improving and refining the SPHINXPoW algorithm by submitting pull requests and providing valuable insights.

We appreciate your understanding and look forward to collaborative efforts in shaping the future of the SPHINX blockchain project.

# Getting Started
To get started with the SPHINX blockchain project, follow the instructions below:

1. Clone the repository: `git clone https://github.com/ChyKusuma/SPHINXPoW.git`
2. Install the necessary dependencies (List the dependencies or provide a link to the installation guide).
3. Explore the codebase to understand the project structure and components.
4. Run the project or make modifications as needed.


## Contributing
We welcome contributions from the developer community to enhance the SPHINX blockchain project. If you are interested in contributing, please follow the guidelines below:

1. Fork the repository on GitHub.
2. Create a new branch for your feature or bug fix: `git checkout -b feature/your-feature-name` or `git checkout -b bugfix/your-bug-fix`.
3. Make your modifications and ensure the code remains clean and readable.
4. Write tests to cover the changes you've made, if applicable.
5. Commit your changes: `git commit -m "Description of your changes"`.
6. Push the branch to your forked repository: `git push origin your-branch-name`.
7. Open a pull request against the main repository, describing your changes and the problem it solves.
8. Insert your information (i.e name, email) in the authors space.

## License
Specify the license under which the project is distributed (MIT License).

## Contact
If you have any questions, suggestions, or feedback regarding the SPHINX blockchain project, feel free to reach out to us at [sphinxfounders@gmail.com](mailto:sphinxfounders@gmail.com).

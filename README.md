# SPHINXBlock

## Introduction

This project is dedicated to the world community as an Open-source Post-quantum blockchain layer 1 project, means anyone can join and contribute based on his/ her passion and skills. SPHINX is a blockchain protocol designed to provide secure and scalable solutions in the post-quantum era.

This repository contains code for the SPHINXSign project, which is a `Block` module for the SPHINX blockchain framework.


## Components

#### SPHINXVerify Namespace:

- `SPHINX_PublicKey` Class: Represents a public key used for verifying the signature of a block. It has a `publicKey` member variable to store the actual public key value. The constructor initializes the `publicKey` using the provided key.

- `verifySignature` Function: Verifies the signature of a block by taking the block's hash, signature, and public key as input parameters. It internally uses the `Crypto::verify` function (implementation not provided) to perform the signature verification.

#### SPHINXHash Namespace:

- `SPHINX_256` Function: Calculates the SPHINX-256 hash of the provided data and returns the hash value as a string.

#### SPHINX_Chain Namespace:

- `Chain` Class: Represents a blockchain chain. It has a member function `addBlock` that is used to add a block (of type `SPHINXMerkleBlock::MerkleBlock`) to the chain.

#### SPHINXDb Namespace:

- `DistributedDb` Class: Represents a distributed database. It provides two member functions:
  - `saveData`: Saves the provided data with a specific block hash in the database.
  - `loadData`: Loads the data associated with a given block ID from the database.

#### SPHINXMerkleBlock Namespace:

- `MerkleBlock` Class: Represents a Merkle block, which is a component of a block in the blockchain. It provides the following member functions:
  - `constructMerkleTree`: Constructs the Merkle tree for a vector of signed transactions. It recursively divides the transactions into halves, constructs the Merkle tree for each half, and combines the resulting roots using the SPHINX-256 hash algorithm.
  - `verifyMerkleRoot`: Verifies the Merkle root by comparing the provided root with the root calculated from the signed transactions using the `constructMerkleTree` function.

#### SPHINXBlock Namespace:

- `Block` Class: Represents a block in the blockchain. It contains various member functions for block-related operations. Some key functions include:
  - `Block`: Constructor that takes the previous hash as a parameter and initializes other member variables such as block height, timestamp, nonce, and difficulty.
  - `calculateBlockHash`: Calculates the hash of the block by concatenating relevant data and solving the nonce using the Proof-of-Work algorithm from the `SPHINXPoW` module (implementation not provided).
  - `addTransaction`: Adds a transaction to the block.
  - `calculateMerkleRoot`: Calculates the Merkle root of the block by calling the `constructMerkleTree` function of the `SPHINXMerkleBlock` namespace.
  - `verifyBlock`: Verifies the block by checking the block's signature using the provided public key and verifying the Merkle root using the `verifyMerkleRoot` function.
  - `setBlockHeight` and `getBlockHeight`: Set and get the block's height, respectively.
  - `getTransactionCount`: Returns the number of transactions in the block.
  - `isValid`: Checks if the block is valid based on the transaction count and timestamp.
  - `setBlockchain` and `addToBlockchain`: Set the blockchain pointer and add the block to the blockchain, respectively.
  - `toJson` and `fromJson`: Functions to convert the block object to JSON format and vice versa.
  - `save` and `load`: Functions to save and load the block from a file.
  - `saveToDatabase` and `loadFromDatabase`: Functions to save and load the block data to/from a distributed database using the `DistributedDb` class.

  
### This repository is part of the  [SPHINXPoW](https://github.com/SPHINX-HUB-ORG/SPHINXPoW) [SPHINXMiner](https://github.com/SPHINX-HUB-ORG/SPHINXMINER)

Please note that the code in this repository is a part of the SPHINX blockchain algorithm, which is currently in development and not fully integrated or extensively tested for functionality. The purpose of this repository is to provide a framework and algorithm for "Block" in the SPHINX blockchain project.

As the project progresses, further updates and enhancements will be made to ensure the code's stability and reliability. We encourage contributors to participate in improving and refining the SPHINXBlock algorithm by submitting pull requests and providing valuable insights.

We appreciate your understanding and look forward to collaborative efforts in shaping the future of the SPHINX blockchain project.

# Getting Started
To get started with the SPHINX blockchain project, follow the instructions below:

1. Clone the repository: `git clone https://github.com/ChyKusuma/SPHINXBlock.git`
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

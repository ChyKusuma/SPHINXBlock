# SPHINXBlock

## Introduction

This project is dedicated to the world community as an Open-source Post-quantum blockchain layer 1 project, means anyone can join and contribute based on his/ her passion and skills. SPHINX is a blockchain protocol designed to provide secure and scalable solutions in the post-quantum era.

This repository contains code for the SPHINXBlock project, which is a `Block` module for the SPHINX blockchain framework.


## Components

### SPHINXHash namespace

- This namespace contains a declaration for a function named SPHINX_256, which is not provided in this code. It presumably calculates the SPHINX_256 hash of the given data.

### SPHINXMerkleBlock namespace

- This namespace contains a forward declaration of a class named MerkleBlock. The definition of this class is not provided in this code.

### SPHINXBlock namespace

- This is the main namespace for the blockchain-related classes and functionalities.

### Block class

- This is the main class representing a block in the blockchain.
- 
- It contains various private member variables to `store block` information, such as `previous block hash, Merkle root, block height, timestamp, nonce, difficulty, list of transactions, blockchain pointer`, and `checkpoint blocks` reference.
  
- It also contains private member variables to store the `stored Merkle root` and `signature`, which are used in the `signing` and `verification` process.
  
- The class provides several `constructors` and setter/getter functions for member variables.
  
- It has functions to `add transactions to the block, calculate the block hash, calculate the Merkle root, sign the Merkle root using SPHINCS+ private key, verify the block's signature with a given public key, verify the block's Merkle root with a given public key, verify the entire block with a given public key`, and `mine the block` with a specified difficulty.

- The class also provides functions to convert the `block object to JSON` format and `vice versa, save the block to a file, load the block from a file, save the block to a distributed database`, and `load the block` from a `distributed database`.

- Lastly, there are two getter functions, `getStoredMerkleRoot()` and `getStoredSignature()`, to retrieve the stored Merkle root and signature.

- Note: The `SPHINXPrivKey, SPHINXPubKey`, and `Transaction classes/types` used in the code are not defined in this code snippet. They are likely part of the broader SPHINX cryptographic library used in this blockchain implementation.

### Block to and from JSON

- `Block::toJson()` and `Block::fromJson()` These functions are used to convert a block object to JSON format and parse a JSON object to initialize a block object, respectively.

- `Block::save()` and `Block::load()` These functions are used to save a block to a file in JSON format and load a block from a file, respectively.

- `Block::saveToDatabase()` and `Block::loadFromDatabase()` These functions are used to save a block to a distributed database and load a block from the distributed database, respectively.

  
### Note

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

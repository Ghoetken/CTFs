# CTF Challenges Repository

This repository contains a collection of Capture The Flag (CTF) challenges across multiple security domains, including **Reverse Engineering (RE)**, **Cryptography (Crypto)**, and **Binary Exploitation (Pwn)**. Each challenge is designed to demonstrate different aspects of system security, vulnerability analysis, and exploit development.

---

## Playing Challenges
Each challenge is organized into its own folder within its respective category (RE, Crypto, or Pwn). Every challenge directory is self-contained and includes all resources needed to understand, run, and solve the challenge.

Each challenge folder typically contains:

- **Source code** used to create and implement the challenge
- **Optional Docker setup** if the challenge requires a controlled or isolated runtime environment
- **A solution description or exploit script** demonstrating how the challenge can be solved or exploited
- **A `.yml` configuration file** that defines the challenge metadata

The `.yml` file serves as the entry point for understanding and interacting with the challenge. It includes:
- A description of the challenge
- The files or resources the player is allowed to access
- Any additional metadata required to run or evaluate the challenge

To play or solve a challenge, refer to the corresponding `.yml` file first. It will guide what the player is allowed to see and provide the context needed to approach the challenge correctly.

### Docker-based challenges (if included)
If the challenge folder contains a dockerfile then that challenge requires a docker to host a binary. Use the provided docker.sh scripts to build and run the dockers.
'bash 
./docker.sh build
./docker.sh run
...
./docker.sh stop

## Challenge Categories

### Reverse Engineering (RE)
Reverse engineering challenges focus on analyzing compiled binaries to understand their behavior, recover hidden logic, or extract secret data such as flags.

Skills used:
- Static analysis (Ghidra, IDA, GDB)
- Control flow understanding
- Binary decompilation

---

### Cryptography (Crypto)
Crypto challenges involve breaking or analyzing custom or weakened cryptographic systems.

Skills used:
- Classical cryptography analysis
- Modular arithmetic
- Exploitation of weak encryption schemes
- Python scripting for attacks

---

### Binary Exploitation (Pwn)
Pwn challenges focus on exploiting memory corruption vulnerabilities in binaries.

Skills used:
- Buffer overflow exploitation
- Heap exploitation (tcache, unsafe unlink, etc.)
- Return-oriented programming (ROP)
- libc and memory layout analysis
- pwntools automation

# Custom Memory Allocator (C++)

This project implements a simple fixed-size memory pool allocator in C++.
The goal is to understand how low-level memory allocation works and why
custom allocators are used in performance-critical systems.

## Features
- Fixed-size memory pooling
- O(1) allocation and deallocation
- Free list–based design
- No system calls during allocation

## Design Overview
- A large memory buffer is allocated upfront.
- The buffer is split into equal-sized blocks.
- A singly linked free list tracks available blocks.
- Allocation pops a block from the free list.
- Deallocation pushes the block back for reuse.

## Memory Layout (Simplified)

The allocator pre-allocates a large buffer and splits it into fixed-size blocks:

[Block][Block][Block][Block][Block]...

Free blocks are linked together using a singly linked list:

Free List → Block → Block → Block → nullptr

## Why Custom Allocators?
- General-purpose allocators (`malloc/free`) are slower for small, frequent allocations.
- Pooling reduces fragmentation and improves cache locality.
- Similar techniques are used in game engines and runtime systems.
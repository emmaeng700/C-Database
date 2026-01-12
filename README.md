# C DB - SQLite-like Database Implementation

A lightweight, SQLite-inspired database implementation written in C. This project implements a simple database engine with B-tree data structure for efficient data storage and retrieval.

## Table of Contents
- [Overview](#overview)
- [Features](#features)
- [Technical Specifications](#technical-specifications)
- [B-tree Implementation Details](#b-tree-implementation-details)
- [Database Operations](#database-operations)
- [Memory Management](#memory-management)
- [Performance Characteristics](#performance-characteristics)
- [Building and Usage](#building-and-usage)
- [Development Status](#development-status)

## Overview

This project is an educational database implementation that demonstrates core concepts of database systems including:
- B-tree based storage engine
- Paging system for memory management
- Binary data serialization
- Table structures with fixed-size records
- Cursor-based record access

## Features

- **Storage Engine**
    - B-tree implementation for efficient data organization
    - Support for both leaf and internal nodes
    - Page-based storage system with 4KB page size
    - Persistent storage to disk

- **Data Structure**
    - Fixed-size row format
    - Support for storing:
        - ID (integer)
        - Username
        - Email

- **Node Types**
    - Leaf nodes for storing actual data
    - Internal nodes for B-tree structure
    - Automatic node splitting and balancing

## Technical Specifications

### Row Format
- Fixed-size records with predefined offsets
- Supports three fields: ID, username, and email

### Page Structure
- Page size: 4096 bytes (4KB)
- Common node header containing:
    - Node type
    - Root node flag
    - Parent pointer

## B-tree Implementation Details

### Structure Overview
- **Node Types:**
    - Leaf Nodes: Store actual row data
    - Internal Nodes: Store keys and pointers to child nodes
    - All nodes are the same size (4096 bytes) to match the OS virtual memory page size

### Node Specifications

#### Common Node Header Layout
- Node Type: 1 byte
- Is Root flag: 1 byte
- Parent Pointer: 4 bytes
- Total Header Size: 6 bytes

#### Internal Node Layout
- Common Node Header (6 bytes)
- Number of Keys: 4 bytes
- Right Child Pointer: 4 bytes
- Cell Array (key/value pairs)
    - Each cell contains:
        - Child Pointer: 4 bytes
        - Key: 4 bytes
- Maximum number of keys: 3 (configurable via `INTERNAL_NODE_MAX_KEYS`)

#### Leaf Node Layout
- Common Node Header (6 bytes)
- Number of Cells: 4 bytes
- Next Leaf Pointer: 4 bytes
- Cell Array (key/value pairs)
    - Each cell contains:
        - Key: 4 bytes
        - Value: Row data (variable size based on schema)
- Maximum cells: Calculated based on page size and cell size

### B-tree Properties
- Self-balancing tree structure
- All leaf nodes are at the same depth
- Nodes are at least half-full (except root)
- Keys in internal nodes guide tree traversal
- Leaf nodes are linked for efficient range queries

### Time Complexity Analysis

#### Operations

| Operation | Average Case | Worst Case | Notes |
|-----------|--------------|------------|-------|
| Search    | O(log n)     | O(log n)   | Requires traversing tree height |
| Insert    | O(log n)     | O(log n)   | May require node splits |
| Delete    | O(log n)     | O(log n)   | May require rebalancing |
| Range Query | O(log n + k) | O(log n + k) | k is number of elements in range |

#### Space Complexity
- Storage: O(n) where n is the number of records
- Each node uses exactly one page (4KB)
- Internal nodes have lower density due to storing only keys and pointers

### B-tree Commands and Operations

#### Insert Operation
1. Find appropriate leaf node through binary search
2. If leaf node has space:
    - Insert new cell in sorted position

3. If leaf node is full:
    - Split leaf node
    - Create new leaf node
    - Distribute cells evenly
    - Insert new key into parent
    - Update sibling pointers


#### Search Operation
1. Start at root node
2. Internal node traversal:
    - Binary search among keys
    - Follow appropriate child pointer

3. Leaf node:
    - Binary search for target key


#### Range Query Operation

1. Find starting key using search operation
2. Traverse linked leaf nodes until:
    - End key is reached
    - Or no more leaves exist


### Node Split Mechanism

#### Leaf Node Split
- Triggered when inserting into a full leaf node
- Process:
    1. Create new leaf node
    2. Divide cells:
        - Left node: `LEAF_NODE_LEFT_SPLIT_COUNT` cells
        - Right node: `LEAF_NODE_RIGHT_SPLIT_COUNT` cells
    3. Update parent node
    4. Update sibling pointers

#### Internal Node Split
- Triggered when inserting into a full internal node
- Process:
    1. Create new internal node
    2. Redistribute keys and children
    3. Push middle key to parent
    4. Update child parent pointers

## Performance Characteristics

### Advantages
- Guaranteed O(log n) operations
- Efficient range queries through leaf node links
- Good cache performance due to node size matching page size
- Minimal rebalancing compared to other tree structures

### Limitations
- Fixed page size may lead to internal fragmentation
- Node splits can be expensive due to data movement
- Memory overhead from partially filled nodes

## Memory Management

### Page Management
- Each node occupies exactly one page (4KB)
- Pages are loaded on demand
- Dirty pages are written back to disk
- Page cache helps reduce I/O operations

### Node Utilization
- Leaf nodes: At least 50% full (except root)
- Internal nodes: At least 50% full (except root)
- Maximum space efficiency: ~76%
- Minimum space efficiency: ~50%

### Memory Layout Constants
```c
#define PAGE_SIZE 4096
#define TABLE_MAX_PAGES 100
#define COLUMN_USERNAME_SIZE 32
#define COLUMN_EMAIL_SIZE 255
```

## Building and Usage

### Prerequisites
- C compiler (GCC recommended)
- Make build system
- Standard C library

### Building the Project
```bash
# Clone the repository
clone repository
cd c-db

# Build and run the project
gcc -o db_program main.c
./db_program file_name.db
```

### Basic Commands
```sql
-- Create a new record
insert 1 user1 user1@example.com

-- Select records
select

-- Exit the program
.exit

-- Display B-tree
.btree

-- Show constants(Details of nodes in B-tree)
.constants
```

## Development Status

This is an educational project demonstrating database internals. It's designed to help understand:
- How databases work internally
- B-tree data structure implementation
- Memory management in database systems
- Binary data serialization


## Acknowledgments

This project is inspired by SQLite's architecture and various database system design principles.
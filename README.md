# CCC Final Projects

Welcome to the collection of final projects developed for the **Coding Skills 1** course. This repository demonstrates advanced proficiency in C programming, system architecture, and data structure implementation through two distinct, fully functional applications.

## Project Index

### 1. [Smart Parking System](./Smart%20Parking%20System%20Simulation)

> _A high-performance, terminal-based parking management simulation._

The **Smart Parking System** is a complex simulation designed to model real-world parking logistics. It moves beyond simple data storage to handle dynamic states, time-sensitive queues, and spatial management.

**Technical Highlights:**

- **Advanced Data Structures**:
  - **Queues (FIFO)**: Ensures fairness in car entry and exit processing.
  - **Circular Linked Lists**: Simulates the physical loop of multi-level floor navigation.
  - **Stacks (LIFO)**: Implements emergency evacuation protocols where the last car in must be the first out.
- **Bitwise Optimization**: Utilizes bit manipulation to track 64 parking slots per floor using a single 64-bit integer, maximizing memory efficiency.
- **Interactive TUI**: Features a responsive Text User Interface that visualizes the building layout, queues, and system logs in real-time.

### 2. [Student Report Management System](./Student%20Report%20Management%20System)

> _A secure, role-based administration tool for educational records._

The **Student Report Management System (SMS)** focuses on data integrity, security, and user management. It simulates a production-grade record-keeping system with persistent storage and access control.

**Technical Highlights:**

- **Role-Based Access Control (RBAC)**: Implements a secure login system with distinct privileges for **Administrators** (full CRUD), **Staff** (operational access), and **Users** (read-only).
- **Persistent Data Storage**: Uses custom file handling to maintain databases for credentials and student records, ensuring data survives program restarts.
- **Robust Error Handling**: Includes auto-recovery mechanisms for missing credential files and input validation to prevent data corruption.

## Common Technologies

Both projects are built using **Standard C**, emphasizing:

- **Memory Management**: Manual allocation and deallocation to prevent leaks.
- **Modular Design**: Clean separation of logic for maintainability.
- **System Interaction**: Direct interaction with the Linux terminal for IO and display.

## Getting Started

To explore a specific project, navigate to its directory and follow the detailed instructions in its `README.md`.

```bash
# Clone the repository (if applicable)
git clone https://github.com/lviffy/CCC-Final-Projects

# Navigate to the Smart Parking System
cd "Smart Parking System Simulation"
# Follow instructions in README.md to compile and run

# OR

# Navigate to the Student Report Management System
cd "Student Report Management System"
# Follow instructions in README.md to compile and run
```

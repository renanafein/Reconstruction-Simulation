# Reconstruction Simulation 🏙️

## 🎯 Project Overview

This project is an **Object-Oriented C++ simulation** designed to evaluate urban reconstruction plans for settlements (Village, City, Metropolis). The system models building processes, resource management, and strategic decision-making in a turn-based environment.

### Key Features:
* **Strategic Policies:** Implements various selection policies (Naive, Balanced, Economy, Sustainability) to determine facility construction order.
* **State Management:** Tracks and updates plan scores (Life Quality, Economy, Environment) based on operational facilities.
* **Action System:** User interaction is managed via an abstract action framework, including simulation steps (`step`), plan creation, status checks, and data backup/restore.
* **Memory Management:** Developed with strict attention to manual memory management to ensure a robust and clean application.

## 🛠️ Technical Requirements

The project must compile and run on a **CS LAB UNIX machine**.

* **Compiler:** GNU G++ (C++11 standard).
* **Build Tool:** `make`.

### Required Compilation Flags:
`-g -Wall -Weffc++ -std=c++11 -Iinclude`

## ⚙️ Build and Run Instructions

### 1. Build the Project
Run the make command from the root directory:
`make`
* **Output:** The executable `simulation` will be placed in the `bin/` directory.

### 2. Run the Simulation
The program requires a configuration file as a command-line argument:
`./bin/simulation <path_to_config_file>`

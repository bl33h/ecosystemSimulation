# ecosystemSimulation
This project simulates a dynamic ecosystem consisting of plants, herbivores, and carnivores, with parallel processing enabled through OpenMP. The simulation takes place on a grid where each cell can represent different states based on the species present. Over time, species interact through reproduction, resource consumption, and movement according to predefined rules. In the visualization, plants are represented in green, herbivores in yellow, and carnivores in red. This color-coding helps to easily understand how the ecosystem interacts with its species.

<p align="center">
  <br>
  <img src="./others/demostration.gif" alt="wb" width="400">
  <br>
</p>
<p align="center" >
  <a href="#features">Features</a> •
  <a href="#Files">Files</a> •
  <a href="#how-to-use">How To Use</a> •
  <a href="#packages">Packages</a>
</p>

## Features
- **Initialization of the Ecosystem:** The simulation begins with a grid populated by a specified number of plants, herbivores, and carnivores.
- **Time-based Simulation:** The ecosystem evolves over time, with species interacting based on their respective rules—plants grow, herbivores eat plants, and carnivores hunt herbivores.
- **Parallel Processing with OpenMP:** The simulation uses OpenMP to parallelize the grid processing, improving performance and scalability.
- **Interaction Rules:** Species follow specific rules for reproduction, movement, and resource consumption. For example, herbivores seek out plants to eat, while carnivores hunt herbivores.
- **Visualization of Results:** The system outputs the state of the ecosystem after each tick, showing the population of each species and their distribution on the grid.

## Files
- **ecosystem.c:** This file contains the implementation of ecosystem simulation program. It includes necessary libraries for parallel programming, input/output operations, memory management, and time functions. It defines constants for the grid size, number of simulation ticks, reproduction probabilities, and thresholds for plant, herbivore, and carnivore behaviors. Additionally, it includes ANSI color codes for terminal output visualization. The max macro is used to find the maximum of two numbers.
- **miniproyectoOpenMP.pdf:** This file contains the project's documentation.

## Packages
The project requires the following packages:
- **omp.h:** Provides functions and macros for parallel programming using OpenMP.
- **stdio.h:** Provides functions for standard input and output operations.
- **stdlib.h:** Provides functions for memory allocation, process control, conversions, and others.
- **time.h:** Provides functions for manipulating and formatting time and date information.

## How To Use
To clone and run this application, you'll need [Git](https://git-scm.com), the [WSL](https://learn.microsoft.com/en-us/windows/wsl/install) or a Linux based VM, [gcc](https://gcc.gnu.org/install/), and [OpenMP](https://www.geeksforgeeks.org/openmp-introduction-with-installation-guide/) installed on your computer. From your command line:

```
# Clone this repository
$ git clone https://github.com/bl33h/ecosystemSimulation

# Open the project
$ cd src

#
$ gcc -fopenmp -o ecosystem ecosystem.c

# Run the app
$ ./ecosystem
```

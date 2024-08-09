# Ecosystem Simulation with OpenMP
This project simulates a dynamic ecosystem consisting of plants, herbivores, and carnivores, with parallel processing enabled through OpenMP. The simulation takes place on a grid where each cell can represent different states based on the species present. Over time, species interact through reproduction, resource consumption, and movement according to predefined rules.

## Features
- **Initialization of the Ecosystem:** The simulation begins with a grid populated by a specified number of plants, herbivores, and carnivores.
- **Time-based Simulation:** The ecosystem evolves over time, with species interacting based on their respective rules—plants grow, herbivores eat plants, and carnivores hunt herbivores.
- **Parallel Processing with OpenMP:** The simulation uses OpenMP to parallelize the grid processing, improving performance and scalability.
- **Interaction Rules:** Species follow specific rules for reproduction, movement, and resource consumption. For example, herbivores seek out plants to eat, while carnivores hunt herbivores.
- **Visualization of Results:** The system outputs the state of the ecosystem after each tick, showing the population of each species and their distribution on the grid.

### Demostration
In the visualization, plants are represented in green, herbivores in yellow, and carnivores in red. This color-coding helps to easily understand how the ecosystem interacts with its species.

<img src="./others/demostration.gif" alt="final result" width="500">
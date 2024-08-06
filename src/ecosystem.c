#include <omp.h>
#include <stdio.h>

#define GRID_SIZE 100
#define NUM_TICKS 10

typedef struct {
    int plants;
    int herbivores;
    int carnivores;
} Cell;

Cell grid[GRID_SIZE][GRID_SIZE];

void initialize_ecosystem() {
    for (int i = 0; i < GRID_SIZE; i++) {
        for (int j = 0; j < GRID_SIZE; j++) {
            grid[i][j].plants = 100;
            grid[i][j].herbivores = 50;
            grid[i][j].carnivores = 10;
        }
    }
}

void update_ecosystem() {
    for (int tick = 0; tick < NUM_TICKS; tick++) {
        #pragma omp parallel for collapse(2)
        for (int i = 0; i < GRID_SIZE; i++) {
            for (int j = 0; j < GRID_SIZE; j++) {
                // update plants -> increase by 5 each tick
                grid[i][j].plants += 5;

                // update herbivores -> increase based on plant availability
                grid[i][j].herbivores += grid[i][j].plants / 20;

                // update carnivores -> prevent negative values
                int decrease = grid[i][j].herbivores / 20;
                grid[i][j].carnivores = grid[i][j].carnivores > decrease ? grid[i][j].carnivores - decrease : 0;
            }
        }

        // synchronize data
        #pragma omp barrier
        
        // display the state of the ecosystem
        if (tick % 1 == 0) {
            printf("Tick %d:\n", tick);
            for (int i = 0; i < GRID_SIZE; i++) {
                for (int j = 0; j < GRID_SIZE; j++) {
                    printf("Cell[%d][%d] - Plants: %d, Herbivores: %d, Carnivores: %d\n",
                           i, j, grid[i][j].plants, grid[i][j].herbivores, grid[i][j].carnivores);
                }
            }
        }
    }
}

int main() {
    initialize_ecosystem();
    update_ecosystem();
    return 0;
}
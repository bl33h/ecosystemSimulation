#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define GRID_SIZE 10
#define NUM_TICKS 10

// macro to find maximum of two numbers
#define max(a,b) ((a) > (b) ? (a) : (b))

// reproduction probabilities and thresholds
#define REPRO_PLANT_PROB 30
#define HERB_REPRO_THRESHOLD 50
#define CARN_REPRO_THRESHOLD 30

// structure to represent a cell in the grid
typedef struct {
    int plants;
    int herbivores;
    int carnivores;
} Cell;

Cell grid[GRID_SIZE][GRID_SIZE];

// function to initialize the ecosystem
void initializeEcosystem() {
    srand(time(NULL));
    for (int i = 0; i < GRID_SIZE; i++) {
        for (int j = 0; j < GRID_SIZE; j++) {
            grid[i][j].plants = rand() % 100 + 50;
            grid[i][j].herbivores = rand() % 30 + 20;
            grid[i][j].carnivores = rand() % 20 + 5;
        }
    }
}

// function to check if a cell is within the bounds of the grid
int isWithinBounds(int x, int y) {
    return x >= 0 && x < GRID_SIZE && y >= 0 && y < GRID_SIZE;
}

// function to reproduce plants, herbivores, and carnivores
void reproduce(int i, int j, int species) {
    int directions[8][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}, {-1, -1}, {-1, 1}, {1, -1}, {1, 1}};
    for (int d = 0; d < 8; d++) {
        int ni = i + directions[d][0];
        int nj = j + directions[d][1];
        if (isWithinBounds(ni, nj)) {
            if (species == 0 && grid[ni][nj].plants == 0 && (rand() % 100 < REPRO_PLANT_PROB)) {
                grid[ni][nj].plants = 10;
            } else if (species == 1 && grid[ni][nj].herbivores < 2 && grid[i][j].herbivores > HERB_REPRO_THRESHOLD) {
                grid[ni][nj].herbivores++;
            } else if (species == 2 && grid[ni][nj].carnivores < 2 && grid[i][j].carnivores > CARN_REPRO_THRESHOLD) {
                grid[ni][nj].carnivores++;
            }
        }
    }
}

// function to update the ecosystem for each tick
void updateEcosystem() {
    for (int tick = 0; tick < NUM_TICKS; tick++) {
        #pragma omp parallel for collapse(2)
        for (int i = 0; i < GRID_SIZE; i++) {
            for (int j = 0; j < GRID_SIZE; j++) {
                grid[i][j].plants = max(0, grid[i][j].plants + 5 - grid[i][j].herbivores / 5);
                grid[i][j].herbivores = max(0, grid[i][j].herbivores + (grid[i][j].plants / 10) - grid[i][j].carnivores / 5);
                grid[i][j].carnivores = max(0, grid[i][j].carnivores + (grid[i][j].herbivores / 10));

                // reproduction logic
                reproduce(i, j, 0); // plants
                reproduce(i, j, 1); // herbivores
                reproduce(i, j, 2); // carnivores
            }
        }

        // wait for all threads to finish updating the ecosystem
        #pragma omp barrier

        // display the state of the ecosystem per tick
        int totalPlants = 0, totalHerbivores = 0, totalCarnivores = 0;
        printf("\nTick %d:\n", tick);
        for (int i = 0; i < GRID_SIZE; i++) {
            for (int j = 0; j < GRID_SIZE; j++) {
                totalPlants += grid[i][j].plants;
                totalHerbivores += grid[i][j].herbivores;
                totalCarnivores += grid[i][j].carnivores;
                char dominant = 'P';
                if (grid[i][j].herbivores > grid[i][j].plants && grid[i][j].herbivores > grid[i][j].carnivores) dominant = 'H';
                else if (grid[i][j].carnivores > grid[i][j].plants && grid[i][j].carnivores > grid[i][j].herbivores) dominant = 'C';
                printf("%c ", dominant);
            }
            printf("\n");
        }
        printf("Plants: %d, Herbivores: %d, Carnivores: %d\n", totalPlants, totalHerbivores, totalCarnivores);
    }
}

int main() {
    initializeEcosystem();
    updateEcosystem();
    return 0;
}

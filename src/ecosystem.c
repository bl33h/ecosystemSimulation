#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define GRID_SIZE 100
#define NUM_TICKS 2500

// macro to find maximum of two numbers
#define max(a,b) ((a) > (b) ? (a) : (b))

// reproduction probabilities and thresholds
#define REPRO_PLANT_PROB 30
#define HERB_REPRO_THRESHOLD 50
#define HERB_CONSUMED_THRESHOLD 3
#define CARN_REPRO_THRESHOLD 30
#define CARN_CONSUMED_THRESHOLD 2
#define HERBIVORE_DEATH_THRESHOLD 3
#define CARNIVORE_DEATH_THRESHOLD 3

// ANSI color codes
#define RED     "\x1b[31m"
#define GREEN   "\x1b[32m"
#define YELLOW  "\x1b[33m"
#define RESET   "\x1b[0m"

// structure to represent a cell in the grid
typedef struct {
    int plants;
    int herbivores;
    int carnivores;
    int herbivores_consumed;
    int carnivores_consumed;
    int herbivore_energy;
    int carnivore_energy;
    int herbivore_ticks_without_food;
    int carnivore_ticks_without_food;
} Cell;

Cell grid[GRID_SIZE][GRID_SIZE];

void initializeEcosystem() {
    srand(time(NULL));
    for (int i = 0; i < GRID_SIZE; i++) {
        for (int j = 0; j < GRID_SIZE; j++) {
            grid[i][j].plants = rand() % 100 + 50;
            grid[i][j].herbivores = rand() % 30 + 20;
            grid[i][j].carnivores = rand() % 20 + 5;
            grid[i][j].herbivores_consumed = 0;
            grid[i][j].carnivores_consumed = 0;
            grid[i][j].herbivore_energy = 0;
            grid[i][j].carnivore_energy = 0;
            grid[i][j].herbivore_ticks_without_food = 0;
            grid[i][j].carnivore_ticks_without_food = 0;
        }
    }
}

int isWithinBounds(int x, int y) {
    return x >= 0 && x < GRID_SIZE && y >= 0 && y < GRID_SIZE;
}

void reproduce(int i, int j, int species) {
    int directions[8][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}, {-1, -1}, {-1, 1}, {1, -1}, {1, 1}};
    for (int d = 0; d < 8; d++) {
        int ni = i + directions[d][0];
        int nj = j + directions[d][1];
        if (isWithinBounds(ni, nj)) {
            #pragma omp critical
            {
                if (species == 0 && grid[ni][nj].plants == 0 && (rand() % 100 < REPRO_PLANT_PROB)) {
                    grid[ni][nj].plants = 10;
                } else if (species == 1 && grid[i][j].herbivores > HERB_REPRO_THRESHOLD && grid[i][j].herbivores_consumed >= HERB_CONSUMED_THRESHOLD) {
                    grid[ni][nj].herbivores++;
                    grid[i][j].herbivores_consumed = 0;
                } else if (species == 2 && grid[i][j].carnivores > CARN_REPRO_THRESHOLD && grid[i][j].carnivores_consumed >= CARN_CONSUMED_THRESHOLD) {
                    grid[ni][nj].carnivores++;
                    grid[i][j].carnivores_consumed = 0;
                }
            }
        }
    }
}

// herbivores gain 5 units of energy per plant consumed
void herbivoresConsumePlants(int i, int j) {
    if (grid[i][j].plants > 0 && grid[i][j].herbivores > 0) {
        grid[i][j].plants--;
        grid[i][j].herbivores_consumed++;
        grid[i][j].herbivore_energy += 5;
        grid[i][j].herbivore_ticks_without_food = 0;
    } else {
        grid[i][j].herbivore_ticks_without_food++;
    }
}

// carnivores gain 10 units of energy per herbivore consumed
void carnivoresConsumeHerbivores(int i, int j) {
    if (grid[i][j].herbivores > 0 && grid[i][j].carnivores > 0) {
        grid[i][j].herbivores--;
        grid[i][j].carnivores_consumed++;
        grid[i][j].carnivore_energy += 10;
        grid[i][j].carnivore_ticks_without_food = 0;
    } else {
        grid[i][j].carnivore_ticks_without_food++;
    }
}

void moveHerbivores(int i, int j) {
    int directions[8][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}, {-1, -1}, {-1, 1}, {1, -1}, {1, 1}};
    int best_move = -1;
    int max_plants = 0;
    for (int d = 0; d < 8; d++) {
        int ni = i + directions[d][0];
        int nj = j + directions[d][1];
        if (isWithinBounds(ni, nj) && grid[ni][nj].carnivores == 0 && grid[ni][nj].plants > max_plants) {
            max_plants = grid[ni][nj].plants;
            best_move = d;
        }
    }

    // move to the cell with the most plants and no carnivores
    if (best_move != -1) {
        #pragma omp critical
        {
            grid[i + directions[best_move][0]][j + directions[best_move][1]].herbivores++;
            grid[i][j].herbivores--;
        }
    }
}

void moveCarnivores(int i, int j) {
    int directions[8][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}, {-1, -1}, {-1, 1}, {1, -1}, {1, 1}};
    int best_move = -1;
    int max_herbivores = 0;
    for (int d = 0; d < 8; d++) {
        int ni = i + directions[d][0];
        int nj = j + directions[d][1];
        if (isWithinBounds(ni, nj) && grid[ni][nj].herbivores > max_herbivores) {
            max_herbivores = grid[ni][nj].herbivores;
            best_move = d;
        }
    }

    // move to the cell with the most herbivores
    if (best_move != -1) {
        #pragma omp critical
        {
            grid[i + directions[best_move][0]][j + directions[best_move][1]].carnivores++;
            grid[i][j].carnivores--;
        }
    }
}

void checkDeaths() {
    #pragma omp parallel for collapse(2)
    for (int i = 0; i < GRID_SIZE; i++) {
        for (int j = 0; j < GRID_SIZE; j++) {
            if (grid[i][j].herbivores > 0 && grid[i][j].herbivore_ticks_without_food >= HERBIVORE_DEATH_THRESHOLD) {
                grid[i][j].herbivores = 0;
                grid[i][j].herbivore_energy = 0;
            }
            if (grid[i][j].carnivores > 0 && grid[i][j].carnivore_ticks_without_food >= CARNIVORE_DEATH_THRESHOLD) {
                grid[i][j].carnivores = 0;
                grid[i][j].carnivore_energy = 0;
            }
        }
    }
}

void updateEcosystem() {
    for (int tick = 0; tick < NUM_TICKS; tick++) {
        #pragma omp parallel for collapse(2)
        for (int i = 0; i < GRID_SIZE; i++) {
            for (int j = 0; j < GRID_SIZE; j++) {
                moveHerbivores(i, j);
                moveCarnivores(i, j);
                grid[i][j].plants = max(0, grid[i][j].plants + 5 - grid[i][j].herbivores / 5);
                grid[i][j].herbivores = max(0, grid[i][j].herbivores + (grid[i][j].plants / 10) - grid[i][j].carnivores / 5);
                grid[i][j].carnivores = max(0, grid[i][j].carnivores + (grid[i][j].herbivores / 10));

                herbivoresConsumePlants(i, j);
                carnivoresConsumeHerbivores(i, j);

                reproduce(i, j, 0);
                reproduce(i, j, 1);
                reproduce(i, j, 2);
            }
        }

        checkDeaths();

        #pragma omp barrier

        int totalPlants = 0, totalHerbivores = 0, totalCarnivores = 0;
        int totalHerbivoreEnergy = 0, totalCarnivoreEnergy = 0;

        printf("\nTick %d:\n", tick);
        for (int i = 0; i < GRID_SIZE; i++) {
            for (int j = 0; j < GRID_SIZE; j++) {
                totalPlants += grid[i][j].plants;
                totalHerbivores += grid[i][j].herbivores;
                totalCarnivores += grid[i][j].carnivores;
                totalHerbivoreEnergy += grid[i][j].herbivore_energy;
                totalCarnivoreEnergy += grid[i][j].carnivore_energy;

                // the dominant species in the cell (just for the beginning)
                char dominant = 'P';
                if (grid[i][j].herbivores > grid[i][j].plants && grid[i][j].herbivores > grid[i][j].carnivores) dominant = 'H';
                if (grid[i][j].carnivores > grid[i][j].plants && grid[i][j].carnivores > grid[i][j].herbivores) dominant = 'C';
                
                // Print with color
                if (dominant == 'P') {
                    printf(GREEN "P " RESET);
                } else if (dominant == 'H') {
                    printf(YELLOW "H " RESET);
                } else if (dominant == 'C') {
                    printf(RED "C " RESET);
                }
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

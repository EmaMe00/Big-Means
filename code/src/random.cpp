#include "../include/random.h"

int uniformNumber(int max_random_number){
     // Inizializza un generatore di numeri casuali con il seed corrente del sistema
    random_device rd;
    mt19937 generator(rd());

    // Definisci l'intervallo desiderato (ad esempio, tra 1 e 100)
    int lower_bound = 1;
    int upper_bound = max_random_number-1;

    // Definisci la distribuzione uniforme di interi nell'intervallo specificato
    uniform_int_distribution<int> distribution(lower_bound, upper_bound);

    int random_integer = distribution(generator);

    return random_integer;
}

vector<vector<double>> sampleData(int sample, vector<vector<double>> points) {
    // Usa la libreria random per ottenere un motore di numeri casuali
    random_device rd;
    mt19937 gen(rd());

    // Mescola l'intera matrice
    shuffle(points.begin(), points.end(), gen);

    // Estrai i primi 'sample' vettori (ora mescolati casualmente)
    vector<vector<double>> sampledPoints(points.begin(), points.begin() + sample);

    return sampledPoints;
}
#include "../include/usefulFunction.h"

vector<vector<double>> deleteElementInVector(vector<std::vector<double>>& myVector, vector<double> element){
    // Ricerca del punto nel vettore
    auto it = find(myVector.begin(), myVector.end(), element);

    // Verifica se il punto è stato trovato
    if (it != myVector.end()) {
        // Eliminazione del punto
        myVector.erase(it);
        //std::cout << "Punto eliminato con successo.\n";
    } else {
        //std::cout << "Punto non trovato.\n";
    }
    return myVector;
}

vector<vector<double>> deleteDuplicateElements(vector<vector<double>> myVector,vector<double> element) {
    // Utilizzo remove per spostare gli elementi duplicati alla fine del vettore
    auto endIt = remove(myVector.begin(), myVector.end(), element);

    // Eliminazione effettiva degli elementi duplicati utilizzando erase
    myVector.erase(endIt, myVector.end());

    return myVector;
}

int searchElementInVector(vector<vector<double>> myVector, vector<double> elementoDaCercare) {
    for (const auto& vettore : myVector) {
        if (vettore == elementoDaCercare) {
            // Elemento presente
            return 1;
        }
    }
    // Elemento non presente
    return 0;
}

void printPoint(vector<double> point){
    for (int i = 0; i < point.size(); i++)
    {
        cout << point[i] << " ";
    }
}

double getMax(double f1, double f2){
    
    if (f1 > f2){
        return f1;
    }
    return f2;
    
}

double getMin(double f1, double f2){
    
    if (f1 < f2){
        return f1;
    }
    return f2;
    
}

// Funzione di partizione per il Quick Sort
int partition(vector<double> &array, int low, int high) {
    double pivot = array[high];
    int i = low - 1;

    for (int j = low; j < high; ++j) {
        if (array[j] <= pivot) {
            i++;
            std::swap(array[i], array[j]);
        }
    }

    std::swap(array[i + 1], array[high]);
    return i + 1;
}

// Implementazione del Quick Sort
void quickSort(vector<double> &array, int low, int high) {
    //Come usarlo: quickSort(numbers, 0, numbers.size() - 1);
    if (low < high) {
        int pivotIndex = partition(array, low, high);

        // Ricorsivamente ordinare le due metà
        quickSort(array, low, pivotIndex - 1);
        quickSort(array, pivotIndex + 1, high);
    }
}
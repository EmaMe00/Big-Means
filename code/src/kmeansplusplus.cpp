#define PRINTING 0
#define PRINTING_RESULT 0
#define BENCHMARK 0

#include "../include/kmeansplusplus.h"

vector<double> chooseFirstCentroid(vector<vector<double>> points) {
    int randomPosition = uniformNumber(points.size());
    return points[randomPosition];
}

double euclideanDistance(vector<double> v1, vector<double> v2) {
    if (v1.size() != v2.size()) {
        // Gestire il caso in cui i vettori abbiano dimensioni diverse
        cout << "Errore: I vettori devono avere la stessa dimensione per poter calcolare la distanza uclidea. \nControlla di aver aperto correttamente il file!" << endl;
        //cout << "Vettore 1 (grandezza "<< v1.size() <<"): ";
        //printPoint(v1);
        //cout << "Vettore 1 (grandezza "<< v2.size() <<"): ";
        //printPoint(v2);
        //cout << endl;      
        exit(1);
        return -1.0; // O un altro valore che indica un errore
    }
    double sum = 0.0;
    for (size_t i = 0; i < v1.size(); ++i) {
        sum += pow(v2[i] - v1[i], 2);
    }

    //Da formula di distanza euclidea dovrei fare la radice quadrata, ma nella formula utilizzata
    //dal kmeans++ la norma 2 è al quadrato
    return sum;
}

double getMinimumDistanceFromCentroids(vector<double> point, vector<vector<double>> centroids){
    double pointDistance = 0;
    //Dato un punto, calcolo la distanza con ogni centroide, e prendo la minima
    for (int i = 0; i < centroids.size(); i++)
    {
        double newPointDistance = euclideanDistance(point,centroids[i]);
        //Non mi interessa la distanza da se stesso
        if (i == 0)
        {
           pointDistance = newPointDistance;
        } else {
            pointDistance = getMin(newPointDistance,pointDistance);
        }
    }
    return pointDistance;
}

int estraiPunto(vector<double>& probabilita) {
    // Verifica se il vettore delle probabilità è vuoto
    if (probabilita.empty()) {
        cout << "Errore: Vettore delle probabilità vuoto." << endl;
        exit(1);
        return -1;  // Ritorna un valore speciale per indicare un errore
    }

    // Calcola la probabilità cumulativa
    vector<double> prob_cumulativa = probabilita;
    partial_sum(prob_cumulativa.begin(), prob_cumulativa.end(), prob_cumulativa.begin());

    // Genera un numero casuale tra 0 e la somma delle probabilità
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<> dis(0, prob_cumulativa.back());
    double random_value = dis(gen);

    // Trova l'indice dell'elemento corrispondente nella probabilità cumulativa
    auto it = lower_bound(prob_cumulativa.begin(), prob_cumulativa.end(), random_value);

    // Restituisci l'indice estratto
    return distance(prob_cumulativa.begin(), it);
}

double getTotalMinimumDistanceFromCentroids(vector<vector<double>> points, vector<vector<double>> centroids, vector<double>& distance){
    double totalDistance = 0;
    for (int i = 0; i < points.size(); i++)
    {
        double pointDistance = getMinimumDistanceFromCentroids(points[i],centroids);
        distance.push_back(pointDistance);
        totalDistance = totalDistance + pointDistance;
    }
    return totalDistance;
}

vector<vector<double>> kmeansplusplus(vector<vector<double>> points, int k, vector<vector<double>> centroids){
    vector<double> point;
    vector<double> tempNewCentroid;

    double totalDistanceFromCentroids;

    //Inizializzo il primo centroide random
    if (centroids.size() == 0)
    {
        centroids.push_back(chooseFirstCentroid(points));
        //Quando acquisco il centroide non lo considero più tra i punti
        points = deleteDuplicateElements(points,centroids.back());
    }else {
        for (int i = 0; i < centroids.size(); i++)
        {
            points = deleteDuplicateElements(points,centroids[i]);
        }
        k = k - centroids.size() + 1;
    }


    cout << endl << "---------------------" << endl;
    cout << "Inizializzo " << k-1 << " centroidi con K-Means++" << endl;

    #if BENCHMARK
    auto inizio = chrono::high_resolution_clock::now();
    #endif

    //Svolgo il ciclo in base a quanti centroidi devo determinare
    for (int l = 0; l < k-1; l++)
    {
        
        cout << endl << "Numero di centroidi attuali: " << centroids.size() << endl;
        cout << "Cerco il centroide " << centroids.size()+1 << endl;
        
        #if PRINTING
        cout << "Centroidi attuali:" << endl;
        for (int i = 0; i < centroids.size(); i++)
        {
            printPoint(centroids[i]);
            cout << endl;
        }
        #endif
        //Per ogni punto mi determino la probabilità di un punto di diventare un centroide, 
        //dove la probabilità più alta è l'elemento più lontano rispetto i centroidi
        vector<double> pointDistance; //distanza per ogni punto
        totalDistanceFromCentroids = getTotalMinimumDistanceFromCentroids(points,centroids,pointDistance);
        vector<double> centroidsProbability;
        for (int i = 0; i < points.size(); i++)
        {
            centroidsProbability.push_back(pointDistance[i] / totalDistanceFromCentroids);
            #if PRINTING
            cout << "Probabilità di scelta per il punto " << i << "(";
            printPoint(points[i]);
            cout << "): " << centroidsProbability[i] << endl;
            #endif
        }
        int centroidIndex = estraiPunto(centroidsProbability);
        centroids.push_back(points[centroidIndex]);

        #if PRINTING
        cout << endl;
        cout << "Centroide scelto: " << endl;
        printPoint(points[centroidIndex]);
        cout << endl << "Probabilità di scelta: " << centroidsProbability[centroidIndex] << endl;
        #endif
        
        //Quando acquisco il centroide non lo considero più tra i punti
        points = deleteDuplicateElements(points,centroids.back());
    }

    #if BENCHMARK
    auto fine = chrono::high_resolution_clock::now();
    auto tempoTrascorso = chrono::duration_cast<chrono::milliseconds>(fine - inizio);
    // Stampa il tempo trascorso
    cout << "Tempo trascorso per il K-Means++: " << tempoTrascorso.count() << " millisecondi." << endl;
    #endif
    #if PRINTING_RESULT
    cout << "Centroidi scelti dal K-Means++:" << endl;
    for (int i = 0; i < centroids.size(); i++)
    {
        printPoint(centroids[i]);
        cout << endl;
    }
    #endif
    cout << "Trovati tutti i " << k-1 << " centroidi con K-Means++" << endl << endl;
    
    return centroids;
}
#define PRINTING 0
#define PRINTING_RESULT 0
#define PRINTING_LOSS 0
#define BENCHMARK 0

#include "../include/kmeans.h"


double SquaredOfDistances(vector<double> punto1, vector<double> punto2) {
    // Assicurati che entrambi i vettori abbiano la stessa dimensione
    if (punto1.size() != punto2.size()) {
        
        cout << "Errore: I vettori devono avere la stessa dimensione per poter calcolare la distanza uclidea. \nControlla di aver aperto correttamente il file!" << endl;
        //cout << "Vettore 1 (grandezza "<< punto1.size() <<"): ";
        //printPoint(punto1);
        //cout << "Vettore 1 (grandezza "<< punto2.size() <<"): ";
        //printPoint(punto2);
        //cout << endl;
        exit(1);
        return -1.0;  // Valore di errore
    }

    double sum = 0;
    for (int i = 0; i < punto1.size(); i++) {
        double diff = punto2[i] - punto1[i];
        sum += diff * diff;
    }

    return sum;
}

int assignPoint(vector<double> point,vector<vector<double>> centroids, double& minimunDistance){

    int nearestCentroid = -1;
    double squaredOfDistance;
    for (int i = 0; i < centroids.size(); i++)
    {
        double tmpSquaredOfDistance = SquaredOfDistances(point, centroids[i]);

        if (i == 0)
        {
            nearestCentroid = i;
            squaredOfDistance = tmpSquaredOfDistance;
        }   
        if (tmpSquaredOfDistance < squaredOfDistance)
        {
            nearestCentroid = i;
            squaredOfDistance = tmpSquaredOfDistance;
        }
        #if PRINTING
        cout << "Distanza per il punto: ";
        printPoint(point);
        cout << " dal centroide: ";
        printPoint(centroids[i]);
        cout << " (" << i << ") = " << tmpSquaredOfDistance << endl;
        #endif
    }
    #if PRINTING
        cout << "Distanza minima (" <<  nearestCentroid << "): " << squaredOfDistance << endl;
    #endif
    minimunDistance = squaredOfDistance;
    return nearestCentroid;
}

vector<double> computeCentroid(vector<vector<double>> punti) {
    // Verifica se il vettore di punti è vuoto
    if (punti.empty()) {
        cerr << "Il vettore di punti è vuoto. Impossibile calcolare la media.\n";
        return {}; // Restituisci un vettore vuoto in caso di errore
    }

    // Inizializzazione del vettore risultante con la stessa dimensione del primo punto
    vector<double> media(punti[0].size(), 0.0);

    // Somma di tutti i punti
    for (const auto& punto : punti) {
        for (std::size_t i = 0; i < punto.size(); ++i) {
            media[i] += punto[i];
        }
    }

    // Calcolo della media dividendo per il numero di punti
    for (double& valore : media) {
        valore /= punti.size();
    }

    return media;
}

void printPointInCluster(vector<int> vettore) {
    unordered_map<int, int> conteggio;

    for (int valore : vettore) {
        // Incrementa il conteggio per il valore corrente
        conteggio[valore]++;
    }

    cout << endl;
    for (const auto& coppia : conteggio) {
        cout << "Il cluster " << coppia.first << " contiene " << coppia.second << " punti.\n";
    }
}

double kmeans(vector<vector<double>> points, vector<vector<double>>& centroids, vector<vector<double>>& degeneratesCentroids, int k, float epslon, int maxIteration){

    #if BENCHMARK
        auto inizio = chrono::high_resolution_clock::now();
    #endif

    double loss = 0;
    double before_loss = 1; //valore simbolico di inizializzazione
    int iteration = 0;
    //creo un vettore tridimensionale, dove avro delle liste di punti separate per ogni cluster, per esempio pointInCluster[0] è la lista di punti del cluster 0
    vector<vector<vector<double>>> pointInCluster;

    //La condizione iteration == 1 serve per far continuare il ciclo dopo la prima iterazione, poichè before_loss non contiene ancora un valore simbolico
    while ((((before_loss - loss) / before_loss) > epslon && iteration < maxIteration) || iteration == 1)
    {
        cout << "Kmeans iteration: " << iteration << endl;
        #if PRINTING
        cout << endl;
        for (int j = 0; j < k; j++)
        {
            cout << "Centroide di partenza: ";
            printPoint(centroids[j]);
            cout << endl;
        }
        #endif

        if (iteration != 0)
        {
            before_loss = loss;
        }
        //La posizione di questo vettore coincide con la posizione del punto nel vettore points, e il contenuto è il cluster a cui appartiene
        vector<int> assignedPoints;
        loss = 0;

        pointInCluster.clear();
        //inizializzo questo vettore tridimensionale in modo da potrelo usare con le parentesi quadre. 
        for (int j = 0; j < k; j++)
        {
             pointInCluster.push_back(vector<vector<double>>());
        }

        //Per ogni punto devo determinare la distanza dal centroide e assegnarlo
        for (int j = 0; j < points.size(); j++)
        {
            //Ogni volta che determino l'appartenenza a un punto significa che trovo il centroide più vicino e la sua distanza
            //quindi prendendo tutte queste distanze minime e sommandole ottengo proprio la loss
            double tmpLoss = 0;
            assignedPoints.push_back(assignPoint(points[j],centroids,tmpLoss));
            //Assegno i punti
            pointInCluster[assignedPoints[j]].push_back(points[j]);
            loss = tmpLoss + loss;
        } 
        #if PRINTING_LOSS
        cout << "K Means Loss (sample): " << fixed << setprecision(0) << loss << endl;
        double percentage = ((before_loss - loss) / before_loss) * 100;
        if (iteration != 0)
        {
            cout << "Miglioramento rispetto alla precedente Loss: " << fixed << setprecision(5) << percentage << "%" << endl;
        }
        #endif


        #if PRINTING_RESULT
        for (int j = 0; j < k; j++)
        {
           cout << "Punti appartenenti al cluster " << j << ": " << pointInCluster[j].size() << endl;
        }
        #endif

        //Ricalcolo i centroidi
        for (int j = 0; j < k; j++)
        {
            if (pointInCluster[j].size() > 0)
            {
                centroids[j] = computeCentroid(pointInCluster[j]);
            } 
            #if PRINTING_RESULT
            cout << "Nuovo centroide: ";
            printPoint(centroids[j]);
            cout << endl;
            #endif
        }
        #if PRINTING
            cout << "Iterazione: " << iteration << endl;
        #endif
        iteration++;
    }

    #if BENCHMARK
        auto fine = chrono::high_resolution_clock::now();
        auto tempoTrascorso = chrono::duration_cast<chrono::milliseconds>(fine - inizio);
        // Stampa il tempo trascorso
        cout << "Tempo trascorso per il clustering kmeans: " << tempoTrascorso.count() << " millisecondi.";
        cout << endl << endl;
    #endif

    //Mi salvo in degeneratesCentroids tutti i centroidi e anche quelli degeneri se ce ne sono
    degeneratesCentroids = centroids;
    centroids.clear();
    for (int i = 0; i < k; i++) {
        if (pointInCluster[i].size() != 0) {
            centroids.push_back(degeneratesCentroids[i]);
        }
    } 
    //cout << "Numero di centroidi non degeneri: " << centroids.size() << endl;

    return loss;
}

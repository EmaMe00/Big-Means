
#define PRINTING_CENTROIDS 0
#define PRINTING_VALUE 0

#include "../include/bigmeans.h"

double bigmeans(string fileName,int sample, int labelColumn,int headerRow,char delimiter,char lectureType,int k, int maxIterationBigMeans, vector<vector<double>>& final_Centroids){
    
    vector<vector<double>> centroids;
    vector<vector<double>> tmp_centroids;
    vector<vector<double>> tmp_degenerateCentroids;
    vector<vector<double>> points;
    vector<vector<double>> TotalPoints;

    double best_loss;
    double loss = 0;
    
    //Variabili risultato
    int totalRow = 0; 
    int totalColumn = 0;
    if (lectureType == 'f')
    {
        TotalPoints = readData(fileName,headerRow,labelColumn-1,delimiter,&totalRow,&totalColumn);
    }
    
    int iteration = 0;
    while (iteration < maxIterationBigMeans)
    {
        cout << "Iterazione big means: " << iteration+1 << " su " << maxIterationBigMeans << endl;
        //-----------------------INIZIO LETTURA FILE----------------------------

        if (lectureType == 'f'){
            points = sampleData(sample, TotalPoints);
        } else if (lectureType == 'l') {
            points = openAndSampleData(fileName,headerRow,labelColumn-1,delimiter,sample,uniformNumber(9999));
        }

        #if PRINTING_VALUE
        try {
            int row = 0;
            for (const auto& riga : points) {
                cout << "Row: ";
                for (const auto& valore : riga) {
                    cout << valore;
                    cout << " ";
                }
                row++;
                cout << endl;
            }
            cout << "Total points: " << to_string(row) << endl;
        } catch (const exception& e) {
            cerr << "Error: " << e.what() << endl;
            return 1;
        }
        #endif

        //-----------------------FINE LETTURA FILE----------------------------

        //Alla prima iterazione inizializzo un centroide random e scelgo gli altri con k-means++
        if (iteration == 0)
        {
            centroids.push_back(chooseFirstCentroid(points));
            tmp_centroids = centroids;
            tmp_centroids = kmeansplusplus(points,k,tmp_centroids);
        }
        
        //se ho centroidi degeneri li sistemo con kmeans++
        if (tmp_centroids.size() < k)
        {
            tmp_centroids = kmeansplusplus(points,k,tmp_centroids);
        }
         
        float epslon = 0.0001;
        loss = kmeans(points,tmp_centroids,tmp_degenerateCentroids,k,epslon,300);

        //Nella prima iterazione prendo la prima loss che trovo
        if (iteration == 0)
        {
            cout << endl << "BIG MEANS LOSS (iteration " << iteration << "): " << fixed << setprecision(0) << loss << endl;
            centroids = tmp_degenerateCentroids;
            best_loss = loss;
        }
        
        if (best_loss > loss)
        {
            cout << endl << "BIG MEANS LOSS (iteration " << iteration << "): " << fixed << setprecision(0) << loss << endl;
            cout << "MIGLIORAMENTO RISPETTO ALLA LOSS PRECEDENTE (iteration " << iteration << "): " << fixed << setprecision(5) << ((best_loss - loss)/best_loss)*100 << "%" << endl << endl;
            centroids = tmp_degenerateCentroids;
            best_loss = loss;
        }

        iteration++;
    }
    
    #if PRINTING_CENTROIDS
    cout << endl << "BIG MEANS LOSS: " << best_loss << endl;
    for (int j = 0; j < k; j++)
    {
        cout << "Centroide big means: ";
        printPoint(centroids[j]);
        cout << endl;
    }
    #endif

    final_Centroids = centroids;
    return 0;
}
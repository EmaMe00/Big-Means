#include <iostream>
#include <chrono>
#include <fstream>
#include <cstdlib> 
#include <getopt.h> 
#include "../include/readCSV.h"
#include "../include/bigmeans.h"
#include "../include/kmeans.h"
#include "../include/usefulFunction.h"
#include "../include/silhouette.h"

using namespace std;

void displayHelp() {
         cout << "Uso del programma:\n"
              << "  --fileName,        -f      nomefile         Nome del file \n"
              << "  --sample,          -s      numero           Grandezza del campione, grandezza minima 100 \n"
              << "  --startRow,        -r      numero           Riga da cui iniziare la lettura \n"
              << "  --removeColumn,    -c      numero           Numero colonna da rimuovere, -1 se non si vuole rimuovere nessuna colonna \n"
              << "  --delimiter,       -d      carattere        Delimitatore tra le dimensioni di un punto \n"
              << "  --lectureType,     -t      l/f              Lettura totale e campionamento 'f', campionamento diretto 'l' \n"
              << "  --k,               -k      numero           Numero di centroidi k \n"
              << "  --iteration,       -i      numero           Numero di iterazioni per BigMeans \n"
              << "  --benckmark,       -B                       Se si desidera effettuare il benchmark\n"
              << "  --calculateLoss,   -L                       Se si desidera effettuare il calcolo della loss finale rispetto a tutto il dataset (se -B è presente questo avverrà in automatico)\n"
              << "  --silhouette,      -S                       Se si desidera calcolare il Silhouette Score per ogni punto dei differenti cluster (Richiede -B)\n"
              << "  --fbest,           -E      <numero>         Valore della best loss conosciuto (Richiede -B)\n"
              << "  --benchmarkFile,   -F      <nomefile>       Nome del file in cui si desidera avere l'output dei benchmark (Richiede -B)\n"
              << "  --help,            -h                       Mostra questo messaggio di aiuto\n";
}

int main(int argc, char* argv[]) {


    string fileName = "";
    int sample = -1; 
    int labelColumn = -2; 
    int headerRow = -1;
    char delimiter = 'x';
    char lectureType = 'x'; //f per full (leggo tutto il dataset e lo campiono), l (less) campiono in maniera più veloce
    int k = -1;
    int bigMeansIteration = -1;

    //Variabili per i benchmark
    int calculateLoss = 0;
    int benchmark = 0;
    int silhouette = 0;
    double fbest = -1; 
    string bfileName;
    string benckmarkFile = "../benchmark/";
    string benckmarkFileSilhuette = "../benchmark/Silhouette";

    //------------ INIZIO GESTIONE DEI PARAMETRI ----------------


    struct option long_options[] = {
        {"fileName", required_argument, nullptr, 'f'},
        {"sample", required_argument, nullptr, 's'},
        {"startRow", required_argument, nullptr, 'r'},
        {"removeColumn", required_argument, nullptr, 'c'},
        {"delimiter", required_argument, nullptr, 'd'},
        {"lectureType", required_argument, nullptr, 't'},
        {"k", required_argument, nullptr, 'k'},
        {"iteration", required_argument, nullptr, 'i'},
        {"benckmark", no_argument, nullptr, 'B'},
        {"silhouette", no_argument, nullptr, 'S'},
        {"fbest", required_argument, nullptr, 'E'},
        {"benchmarkFile", required_argument, nullptr, 'F'},
        {"calculateLoss", no_argument, nullptr, 'L'},
        {"help", no_argument, nullptr, 'h'}, 
        {nullptr, 0, nullptr, 0} // Terminatore
    };

    int option;
    bool hasError = false;

    while ((option = getopt_long(argc, argv, "f:s:r:c:d:t:k:i:hBSLE:F:", long_options, nullptr)) != -1) {
        switch (option) {
            case 'f':
                fileName = optarg;
                break;
            case 's':
                try {
                    sample = std::stoi(optarg);
                    if (sample < 100)
                    {
                        cout << "Errore: Il parametro 'sample' deve essere un numero intero maggiore uguale a 100." << endl;
                        return 1;
                    }
                } catch (const std::invalid_argument &e) {
                    cout << "Errore: Il parametro 'sample' deve essere un numero intero." << endl;
                    hasError = true;
                } catch (const std::out_of_range &e) {
                    cout << "Errore: Il parametro 'sample' è fuori dal range dei numeri interi." << endl;
                    hasError = true;
                }
                break;
            case 'r':
                try {
                    headerRow = stoi(optarg);
                    if (headerRow < 1)
                    {
                        cout << "Errore: Il parametro 'startRow' deve essere un numero intero maggiore di 0." << endl;
                        return 1;
                    }
                    
                } catch (const std::invalid_argument &e) {
                    cout << "Errore: Il parametro 'startRow' deve essere un numero intero." << endl;
                    hasError = true;
                } catch (const std::out_of_range &e) {
                    cout << "Errore: Il parametro 'startRow' è fuori dal range dei numeri interi." << endl;
                    hasError = true;
                }
                break;
            case 'c':
                try {
                    labelColumn = stoi(optarg);
                    if (labelColumn < -1 || labelColumn == 0)
                    {
                        cout << "Errore: Il parametro 'removeColumn' deve essere un numero maggiore di 0 o, se non si vuole eliminare nessuna colonna, uguale -1." << endl;
                        return 1;
                    }
                } catch (const std::invalid_argument &e) {
                    cout << "Errore: Il parametro 'removeColumn' deve essere un numero maggiore di 0 o, se non si vuole eliminare nessuna colonna, uguale -1." << endl;
                    hasError = true;
                } catch (const std::out_of_range &e) {
                    cout << "Errore: Il parametro 'removeColumn' è fuori dal range dei numeri interi." << endl;
                    hasError = true;
                }
                break;
            case 'd':
                if (strlen(optarg) != 1) {
                    cout << "Errore: Il parametro 'delimiter' deve essere un carattere singolo." << endl;
                    hasError = true;
                } else {
                    delimiter = optarg[0];
                }
                break;
            case 't':
                if (!(optarg[0] == 'l' || optarg[0] == 'f')) {
                    cout << "Errore: Il parametro 'lectureType' deve essere 'l' o 'f'." << endl;
                    hasError = true;
                } else {
                    lectureType = optarg[0];
                }
                break;
            case 'k':
                try {
                    k = stoi(optarg);
                    if (k < 2)
                    {
                        cout << "Errore: Il parametro 'k' deve essere un numero intero maggiore di 1." << endl;
                        return 1;
                    }
                } catch (const invalid_argument &e) {
                    cout << "Errore: Il parametro 'k' deve essere un numero intero." << endl;
                    hasError = true;
                } catch (const std::out_of_range &e) {
                    cout << "Errore: Il parametro 'k' è fuori dal range dei numeri interi." << endl;
                    hasError = true;
                }
                break;
            case 'i':
                try {
                    bigMeansIteration = stoi(optarg);
                    if (bigMeansIteration < 1)
                    {
                        cout << "Errore: Il parametro 'bigMeansIteration' deve essere un numero intero maggiore di 0." << endl;
                        return 1;
                    }       
                } catch (const std::invalid_argument &e) {
                    cout << "Errore: Il parametro 'bigMeansIteration' deve essere un numero intero." << endl;
                    hasError = true;
                } catch (const std::out_of_range &e) {
                    cout << "Errore: Il parametro 'bigMeansIteration' è fuori dal range dei numeri interi." << endl;
                    hasError = true;
                }
                break;
            case 'B':
                calculateLoss = 1;
                benchmark = 1;
                break;
            case 'S':
                silhouette = 1;
                break;
            case 'E':
                try {
                    fbest = stod(optarg);
                    if (fbest < 0)
                    {
                        cout << "Errore: Il parametro 'fbest' deve essere un numero intero maggiore di 0." << endl;
                        return 1;
                    }       
                } catch (const std::invalid_argument &e) {
                    cout << "Errore: Il parametro 'fbest' deve essere un double." << endl;
                    hasError = true;
                } catch (const std::out_of_range &e) {
                    cout << "Errore: Il parametro 'fbest' è fuori dal range dei numeri double." << endl;
                    hasError = true;
                }
                break;
            case 'F':
                bfileName = optarg;
                benckmarkFile = benckmarkFile + bfileName;
                benckmarkFileSilhuette = benckmarkFileSilhuette + bfileName;
                break;
            case 'L':
                calculateLoss = 1;
            break;
            case 'h':
                displayHelp();
                return 1;
                break;
            default:
                cout << "Errore: Opzione non riconosciuta." << endl;
                hasError = true;
                break;
        }
    }

    if (hasError || optind < argc) {
        cout << "Errore: Parametri non validi o mancanti." << endl;
        return 1;
    }

    if (fileName == " " || sample == -1 || labelColumn == -2 || headerRow == -1 || delimiter == 'x' || lectureType == 'x' || k == -1 || bigMeansIteration == -1) 
    {
        cout << "Errore: Parametri non validi o mancanti, devi inserire tutti i parametri per il corretto funzionamento del programma." << endl;
        return 1;
    }

    //Controlli per i benchmark
    if (silhouette == 1 && benchmark == 0)
    {
        cout << "Il calcolod del Silhouette score necessita del flag -B attivo." << endl;
        return 1;
    }

    if (benchmark == 1 )
    {
        if (benckmarkFile == "../benchmark/" || fbest == -1)
        {
            cout << "Inserire il nome del file in cui vuoi stampare i risultati dopo il flag -F e la fbest dopo il flag -E." << endl;
            return 1;
        }
    } else {
        if (benckmarkFile != "../benchmark/" || fbest != -1)
        {
            cout << "I flag -E e -F richiedono attivo il flag -B." << endl;
            return 1;
        }
    }
    
    
    //------------ FINE GESTIONE DEI PARAMETRI ----------------


    vector<vector<double>> final_Centroids;


    if(benchmark == 1){
        auto inizio_bigmeans = chrono::high_resolution_clock::now();
    
        bigmeans(fileName,sample,labelColumn,headerRow,delimiter,lectureType,k,bigMeansIteration,final_Centroids);
    
        auto fine_bigmeans = chrono::high_resolution_clock::now();
        auto tempoTrascorso_secondi_preciso = chrono::duration_cast<chrono::duration<double>>(fine_bigmeans - inizio_bigmeans);
        cout << endl;
        cout << endl << "Genero il resoconto..." << endl;
        cout << endl << "Resoconto: " << endl;
        int totalRow = 0; 
        int totalColumn = 0;
        //-------- CALCOLO LOSS FINALE PER IL CONFRONTO ----------
        //La posizione di questo vettore coincide con la posizione del punto nel vettore points, e il contenuto è il cluster a cui appartiene
        vector<int> assignedPoints;
        vector<vector<vector<double>>> pointInCluster;
        //inizializzo il vettore
        for (int j = 0; j < k; j++)
        {
                pointInCluster.push_back(vector<vector<double>>());
        }
        double loss = 0;
        //Calcolo la loss finale con i centroidi trovati
        vector<vector<double>> points = readData(fileName,headerRow,labelColumn-1,delimiter,&totalRow,&totalColumn);
        for (int j = 0; j < points.size(); j++)
        {
                double tmpLoss = 0;
                //Mi rilascia in output tmpLoss la distanza al quadrato del punto dal cluster più vicino,
                //la somma mi da la loss finale
                assignedPoints.push_back(assignPoint(points[j],final_Centroids,tmpLoss));
                //Assegno i punti
                pointInCluster[assignedPoints[j]].push_back(points[j]);
                loss = tmpLoss + loss;
        }
        cout << "LOSS FINALE: " << loss << endl;
        cout << "Grandezza campione: " << sample << endl;
        cout << "Numero di centroidi: " << k << endl;
        cout << "Iterazioni di Big Means: " << bigMeansIteration << endl;

        double dloss = static_cast<double>(loss);
        double Ea = ((dloss - fbest)/fbest)*100;
        cout << "Ea: " << Ea << endl << endl;
        cout << "Tempo trascorso: " << fixed << setprecision(2) << tempoTrascorso_secondi_preciso.count() << " secondi." << endl;
        // Apre il file in modalità append
        ofstream file(benckmarkFile, std::ios::app);
        if (file.is_open()) {
            file << sample << "," << k << "," << bigMeansIteration << "," << fixed << setprecision(2) << tempoTrascorso_secondi_preciso.count() << "," << loss << "," << Ea << endl;
            file.close();
            if(silhouette == 1){
            ofstream file(benckmarkFileSilhuette, std::ios::app);
            if (file.is_open()){
                for (int j = 0; j < k; j++)
                {
                    cout << "Calcolo il silhouette score per il cluster " << j << endl;
                    double meanSilhouette = 0;
                    for (int i = 0; i < pointInCluster[j].size(); i++)
                    {
                        double a = calculateA(pointInCluster[j][i],pointInCluster[j]);
                        double b = calculateB(pointInCluster[j][i],pointInCluster,k,j);
                        double silhouette = calculateS(a,b);
                        meanSilhouette = meanSilhouette + silhouette;

                        cout << "Silhouette per il punto " << i << ": " << silhouette << endl;
                        file << j << "," << i << "," << silhouette << endl;
                    }
                    cout << "Silhouette media per il cluster " << j << ": " << meanSilhouette/pointInCluster[j].size() << endl;
                    //file << "Silhouette media per il cluster " << j << ": " << meanSilhouette/pointInCluster[j].size() << endl;
                }
                cout << endl;
                file.close();
            }
            }
        }else {
            cout << "Errore nell'apertura del file benchmark: " << "benchmark.txt" << endl;
            return 1;
        }
        
    }else {
        bigmeans(fileName,sample,labelColumn,headerRow,delimiter,lectureType,k,bigMeansIteration,final_Centroids);
        cout << endl;
        cout << endl << "Genero il resoconto..." << endl;
        cout << endl << "Resoconto: " << endl;
        if (calculateLoss == 1){
            int totalRow = 0; 
            int totalColumn = 0;
            //-------- CALCOLO LOSS FINALE PER IL CONFRONTO ----------
            //La posizione di questo vettore coincide con la posizione del punto nel vettore points, e il contenuto è il cluster a cui appartiene
            vector<int> assignedPoints;
            vector<vector<vector<double>>> pointInCluster;
            //inizializzo il vettore
            for (int j = 0; j < k; j++)
            {
                    pointInCluster.push_back(vector<vector<double>>());
            }
            double loss = 0;
            //Calcolo la loss finale con i centroidi trovati
            vector<vector<double>> points = readData(fileName,headerRow,labelColumn-1,delimiter,&totalRow,&totalColumn);
            for (int j = 0; j < points.size(); j++)
            {
                    double tmpLoss = 0;
                    //Mi rilascia in output tmpLoss la distanza al quadrato del punto dal cluster più vicino,
                    //la somma mi da la loss finale
                    assignedPoints.push_back(assignPoint(points[j],final_Centroids,tmpLoss));
                    //Assegno i punti
                    pointInCluster[assignedPoints[j]].push_back(points[j]);
                    loss = tmpLoss + loss;
            }
            cout << "LOSS FINALE: " << loss << endl;
        } 
        cout << "Grandezza campione: " << sample << endl;
        cout << "Numero di centroidi: " << k << endl;
        cout << "Iterazioni di Big Means: " << bigMeansIteration << endl;
    }

    
    
    return 0;
}







#include "../include/readCSV.h"

vector<vector<double>> readData(const string& fileName, int headerRow,int labelColumn, char delimiter,int* totalRow,int* columnNumber){

    ifstream fileInput(fileName);
    if (!fileInput.is_open()) {
        cout << "Impossibile aprire il file." << endl;
        exit(1);
    }

    vector<vector<double>> data;
    int rowAcquired = 0; //variabile per il conteggio delle righe totali
    int column = 0; //variabile per il conteggio delle colonne
    string row;

    // Salto righe d'intestazione
    int headerSkip = 0;
    while (headerSkip < headerRow)
    {
        getline(fileInput,row);
        headerSkip++;
    }

    while (getline(fileInput, row)){
        
        column = 0;
        rowAcquired++; //Conto le righe acquisite

        int cellNumber = 0;
        istringstream ss(row);
        string cell;
        vector<double> rowValues;

        while (getline(ss, cell, delimiter)) {
            column++; //Conto le colonne
            if (labelColumn != cellNumber)
            {
                try
                {   
                    rowValues.push_back(stod(cell));
                }
                catch(const std::exception& e)
                {
                    cerr << "Conversione in double non riuscita! Controlla che il dataset non contenga stringhe o caratteri!" << '\n';
                    exit(1);
                }
            }
            cellNumber++;
        }
        //printPoint(rowValues);
        data.push_back(rowValues); 
    }
    *totalRow = rowAcquired;
    *columnNumber = column;
    fileInput.close();
    return data;
}

//Con questa funzione randomizzo la posizione nel file e prendo la riga successiva a quella della posizione in cui mi trovo
vector<vector<double>> openAndSampleData(const string& fileName, int headerRow,int labelColumn, char delimiter,int sample,int seed){
    
    ifstream file(fileName);
    vector<vector<double>> data;
    if (file.is_open()) {
        
        // Ottieni la lunghezza totale del file
        file.seekg(0, ios::end);
        streampos fileSize = file.tellg();

        //Torno all'inizio
        file.seekg(0, ios::beg);
        // Salto righe d'intestazione
        int headerSkip = 0;
        string line;
        while (headerSkip < headerRow+1)
        {
            getline(file,line);
            headerSkip++;
        }
        // Trova la posizione corrente nel file (dopo le righe di intestazione)
        streampos startPosition = file.tellg();

        // Inizializza il generatore di numeri casuali
        srand(seed);
        int countSample = 0;
    
    vector<streampos> selectedPoints = {}; //Vettore per mantenere memoria dei punti già presi
    streampos randomPosition; 
    while (countSample < sample) {
        // Calcola una posizione casuale nel file (salta le righe header)
        randomPosition = startPosition + rand() % (fileSize - startPosition);
        // Posizionati nella posizione scelta
        file.seekg(randomPosition);

        // Assicurati di iniziare una nuova riga (per evitare letture parziali)
        string dummy;
        getline(file, dummy);

        // Leggi il punto
        getline(file, line);
        if (file.fail()) {
            //std::cerr << "Errore durante la lettura della riga." << std::endl;
            file.close();
            file.open(fileName);
            continue;
        }
        // Controllo di non aver già preso il punto
        streampos pos = file.tellg();
        auto it = find(selectedPoints.begin(), selectedPoints.end(), pos);
        // Verifica se l'elemento è stato trovato
        if (it != selectedPoints.end()) {
            //cout << "L'elemento " << pos << " è presente nel vettore." << endl;
            continue;
        } 
        //cout << file.tellg() << endl;
        //cout << line << endl;
        
        int cellNumber = 0;
        istringstream ss(line);
        string cell;
        vector<double> rowValues;

        while (getline(ss, cell, delimiter)) {
            if (labelColumn != cellNumber) {
                try {
                    rowValues.push_back(stod(cell));
                } catch (const std::exception &e) {
                    cout << "Conversione in double non riuscita! Controlla che il dataset non contenga stringhe o caratteri!" << '\n';
                    exit(1);
                }
            }
            cellNumber++;
        }

        // Verifica se ci sono valori nella riga
        if (rowValues.size() != 0) {
            data.push_back(rowValues);
            selectedPoints.push_back(pos);
            countSample++;
        }
    }
        file.close();
    } else {
        cout << "Impossibile aprire il file." << endl;
        exit(1);
    }
    return data;
}


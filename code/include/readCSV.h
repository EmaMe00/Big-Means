#ifndef CSV_READER_H
#define CSV_READER_H

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <ctime>
#include <cstdlib>
#include "../include/random.h"
#include "../include/usefulFunction.h"

using namespace std;

vector<vector<double>> readData(const string& fileName, int headerRow,int labelColumn, char delimiter,int* totalRow,int* columnNumber);

vector<vector<double>> openAndSampleData(const string& fileName, int headerRow,int labelColumn, char delimiter,int sample,int seed);

#endif // CSV_READER_H

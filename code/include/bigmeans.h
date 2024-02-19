#ifndef BIGMEANS
#define BIGMEANS

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <ctime>
#include <cstdlib>
#include "../include/kmeansplusplus.h"
#include "../include/readCSV.h"
#include "../include/random.h"
#include "../include/kmeans.h"

using namespace std;

double bigmeans(string fileName,int sample, int labelColumn,int headerRow,char delimiter,char lectureType,int k, int maxIterationBigMeans, vector<vector<double>>& final_Centroids);

#endif // BIGMEANS
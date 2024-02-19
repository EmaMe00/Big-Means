#ifndef KMEANSPLUSPLUS
#define KMEANSPLUSPLUS

#include "../include/random.h"
#include "../include/usefulFunction.h"
#include <numeric>

vector<double> chooseFirstCentroid(vector<vector<double>> points);
double euclideanDistance(vector<double> v1, vector<double> v2);
double getMinimumDistanceFromCentroids(vector<double> point, vector<vector<double>> centroids);
int estraiPunto(const vector<double>& probabilita);
double getTotalMinimumDistanceFromCentroids(vector<vector<double>> points, vector<vector<double>> centroids, vector<double>& distance);
vector<vector<double>> kmeansplusplus(vector<vector<double>> points, int k, vector<vector<double>> centroids);

#endif // KMEANSPLUSPLUS
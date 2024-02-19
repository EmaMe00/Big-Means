#ifndef KMEANS
#define KMEANS

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <ctime>
#include <cstdlib>
#include "../include/usefulFunction.h"

using namespace std;

vector<double> computeCentroid(vector<vector<double>> punti);
double SquaredOfDistances(const vector<double> punto1, const vector<double> punto2);
int assignPoint(vector<double> point,vector<vector<double>> centroids, double& minimunDistance);
void printPointInCluster(vector<int> vettore);
double kmeans(vector<vector<double>> points, vector<vector<double>>& centroids, vector<vector<double>>& degeneratesCentroids, int k, float epslon, int maxIteration);
#endif // KMEANS
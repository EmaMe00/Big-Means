#ifndef CSV_SILHOUETTE_H
#define CSV_SILHOUETTE_H

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <ctime>
#include <cstdlib>

using namespace std;

double calculateA(vector<double> point, vector<vector<double>> pointInCluster);
double calculateB(vector<double> point,vector<vector<vector<double>>> pointInCluster,int k,int myCluster);
double calculateS(double a, double b);

#endif
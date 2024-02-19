#ifndef USEFULFUNCTION
#define USEFULFUNCTION

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <ctime>
#include <cstdlib>

using namespace std;
double getMax(double f1, double f2);

double getMin(double f1, double f2);

int partition(vector<double> &array, int low, int high);

void quickSort(vector<double> &array, int low, int high);

void printPoint(vector<double> point);

int searchElementInVector(std::vector<std::vector<double>> myVector, std::vector<double> elementoDaCercare);

vector<vector<double>> deleteElementInVector(vector<std::vector<double>>& myVector, vector<double> element);
vector<vector<double>> deleteDuplicateElements(vector<vector<double>> myVector,vector<double> element);

#endif // USEFULFUNCTION
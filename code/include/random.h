#ifndef RANDOM_H
#define RANDOM_H

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <random>
using namespace std;

vector<vector<double>> sampleData (int sample, vector<vector<double>> points);

int uniformNumber(int max_random_number);

#endif // RANDOM_H
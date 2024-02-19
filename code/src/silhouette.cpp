#include "../include/silhouette.h"
#include "../include/kmeansplusplus.h"

double calculateA(vector<double> point, vector<vector<double>> pointInCluster){

    double totalDistance = 0;
    for (int i = 0; i < pointInCluster.size(); i++)
    {

        //Non considero la distanza del punto da se stesso
        if (pointInCluster[i] == point)
        {
           continue;
        }
        totalDistance = totalDistance + euclideanDistance(pointInCluster[i], point);
        
    }
    totalDistance = totalDistance/(pointInCluster.size()-1);
    
    
    return totalDistance;
}

double calculateB(vector<double> point,vector<vector<vector<double>>> pointInCluster,int k,int myCluster){

    double totalDistance = 0;
    double minDistance = 9999999999999999999;
    for (int i = 0; i < k; i++)
    {
        //Non considero il cluster a cui appartengo
        if (i == myCluster)
        {
           continue;
        }
        for (int j = 0; j < pointInCluster[i].size(); j++)
        {
            totalDistance = totalDistance + euclideanDistance(pointInCluster[i][j], point);
        }
        totalDistance = totalDistance/pointInCluster[i].size();
        if (minDistance > totalDistance)
        {
            minDistance = totalDistance;
        }
        
    }
    return minDistance;
}

double calculateS(double a, double b){
    if (a>b)
    {
        return (b-a)/a;
    }else{
        return (b-a)/b;
    }
}
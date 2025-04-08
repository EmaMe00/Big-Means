# Big-Means

An point assignment clustering algorithm for big data.

R. Mussabayev, N. Mladenovic, B. Jarboui e R. Mussabayev, ≪How to Use K-means for Big Data Clustering?≫

This algorithm was invented by R. Mussabayev, N. Mladenovic, B. Jarboui e R. Mussabayev, the article is available at this url: [How to Use K-means for Big Data Clustering?](https://www.sciencedirect.com/science/article/pii/S0031320322007488).

The Big-Means algorithm uses K-Means (and K-Means++ for initialization) with several samples of size s. When K-Means finds a better solution (at time T) it uses these points to initialize new centroids on a new sample (at time T+1). This process continues until a convergence criterion is reached.

## Instruction 

First of all we need to compile our code, we simply run the following command:

```bash
make 
```

Now we need to go into the bin folder and run this:

```bash
./main −f <dataset file> −s <sample size> −r <initial line> −c <column to remove> −d <delimiter> −t <type of reading>
−k <numbers of centroids> −i <Big Means Iteration> −L
```

Example: 
```bash
./main −f ”../../datasets/d15112.tsp” −s 58508 −r 1 −c 1 −d ’ ’ −t f −k 2 −i 10 −L
```

This is a list with all meaning of parameters:
  - f: dataset file path
  - s: size of sample
  - r: number of the line you want to start reading from
  - c: number of column you want to remove, if you don't want to remove anything insert -1
  - d: delimiter between colums, usually a space ora a tab
  - t: type of lecture for dataset, 'f' for read the whole file or 'l' for read point with  a sample technique (usually f)
  - k: number of cluster we want
  - i: Big Means iteration
  - L: this flag allow you to calculate loss funciton value with your centroids on dataset
  
## Benchmark
For execute different benchmarks on this algorith we have implemented in python one script than you can modify with different settings for different running. If you want to do benckmarks, the command become: 

```bash
./main −f <dataset file> −s <sample size> −r <initial line> −c <column to remove> −d <delimiter> −t <type of reading>
−k <numbers of centroids> −i <Big Means Iteration>
−B −S −F <output file> −E <f-best>
```

where:
- B: benckmark mode
- S: calculate the Silhouette Score
- F: output file path
- E: best solution you know for that dataset

This command should be inserted into the benchmark.py file settings.

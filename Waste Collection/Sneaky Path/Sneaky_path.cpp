#include<stdio.h>
#include<stdlib.h>
#include<iostream>
#include<fstream>
#include<sstream>
#include<string>
#include<vector>
#include<list>
#include <chrono>



#define INF 99999

using namespace std;
using namespace std::chrono;

int min_value = 999999;
int max_value = 0;
int aver_value = 0;
vector<int> store_path1;
int arr[2];
int arr1[2];

/* A utility function to find the vertex with minimum distance  value, from the set of vertices 
not yet included in shortest  path tree*/
int minDistance(int* dist, bool* sptSet,int dimension)
{
	// Initialize min value
	int min = INT_MAX, min_index;
	for (int v = 1; v <= dimension; v++)
		if (sptSet[v] == false && dist[v] <= min)
			min = dist[v], min_index = v;
	return min_index;
}




// need

/*Function to output shortest path from each node to each node (optional)*/

void print_shortest_distance(int **distance, int dimension, ofstream& outfile)
{
	int i, j;
	outfile << "Following matrix shows the shortest distances";
	outfile << " between every pair of vertices \n";
	for (i = 1; i <= dimension; i++)
	{
		for (j = 1; j <= dimension; j++)
		{
			if (i == j)
			{
				outfile << 0 <<"   ";
			}
			if (distance[i][j] == INF)
			{
				outfile << "INF" << "   ";
			}
			else
			{
				outfile << distance[i][j]<<"   ";				
			}
		}
		outfile << endl;	
	}
}

/////need

// Function to store shortest path into 2D matrix of vectors and output it
void printpath(int **parent, int u, int v, vector<int>** actual_path, int i, int j, ofstream& outfile)
{
	// If the the node is parent node print it out and store in to actual_path matrix
	if (parent[u][v] == u)
	{		
		actual_path[i][j].push_back(u);
		return;
	}
	//If the node is not INF value recursivly call printpath function which will print out the shortest path
	//Store node into actual_path matrix 
	if (parent[u][v] != INF)
	{		
		actual_path[i][j].push_back(parent[u][v]);
		printpath(parent, u, parent[u][v], actual_path, i, j,outfile);
	}
}

/// need
/*FloydWarshall algorithm is used to calculate the shorest path from each city to each city
in order to calculate actual car flow on each edge*/

void floydwarshall(int **graph, int dimension, vector<int>** actual_path, int **flow, int start, int end, ofstream& outfile, int *waste)
{

	int** distance = new int*[dimension];
	for (int i = 1; i <= dimension; i++)
	{
		distance[i] = new int[dimension];
	}
	int i, j, k;

	int** parent = new int*[dimension];
	for (int i = 1; i <= dimension; i++)
	{
		parent[i] = new int[dimension];
	}
	for (i = 1; i <= dimension; i++)
		for (j = 1; j <= dimension; j++)
		{
			distance[i][j] = graph[i][j];
			if (i == j)
				parent[i][j] = INF;
			if (graph[i][j] != INF)
				parent[i][j] = i;
			else
				parent[i][j] = INF;
		}


	// Floyd Warshall algorithm, pick the minimum of two and update the parent node.
	for (k = 1; k <= dimension; k++)
	{
		for (i = 1; i <= dimension; i++)
		{
			for (j = 1; j <= dimension; j++)
			{
				if ((distance[i][k] + distance[k][j] < distance[i][j]))
				{
					distance[i][j] = distance[i][k] + distance[k][j];
					parent[i][j] = parent[k][j];
				}
			}
		}
	}


	/*Function which will calculate actual shotest path from each city to each city*/
	print_shortest_distance(distance, dimension, outfile);

	outfile << "\n\n";
	for (i = 1; i <= dimension; i++)
		for (j = 1; j <= dimension; j++)
		{
			if (parent[i][j] != INF && i != j)
			{
				actual_path[i][j].push_back(j);
				printpath(parent, i, j, actual_path, i, j, outfile);
			}
			else
			{
				actual_path[i][j].push_back(i);
			}
		}
	// Output actual shortest path from each node to each node stored in 2D matrix of vectors
	outfile << "\n" << "----------Actual shortest path--------- " << endl;
	for (int k = 1; k <= dimension; k++)
	{
		for (int j = 1; j <= dimension; j++)
		{
			outfile << "   " << "(";
			for (vector<int>::const_iterator i = actual_path[k][j].begin(); i != actual_path[k][j].end(); ++i)
			{
				outfile << *i << " ";
			}
			outfile << ")";
		}
		outfile << endl;
	}

	outfile << endl << endl;
	outfile << "----------------Optimal path to collect waste--------------------" << endl;
	outfile << "Start -> ";
	int source = start;
	vector<int> store;
	for (int p = 0; p < 10; p++)
	{
		if (waste[p] == 1)

		{
			store.push_back(p + 1);
		}
	}

	for (int g = 0; g < store.size(); g++)
	{
		if (store[g] != -1)
		{
			for (vector<int>::const_iterator i = actual_path[store[g]][source].begin(); i != actual_path[store[g]][source].end(); ++i)
			{
				outfile << *i << " ";
				for (int j = 0; j < store.size(); j++)
				{
					if (*i == store[j] && *i != store[g])
					{
						store[j] = -1; // position
					}
				}
			}
			source = store[g];
			outfile << " -> ";
		}
		else
		{			
				continue;			
		}
	}
	outfile << "Finsih" << endl;;
			}
		

	//car_flows(flow,actual_path,dimension,start,end,outfile,graph);	

	
	



int main()
{

	int dimensions;
	int start;
	int end;
	char graph_type;
	char comma;
	int loc1, loc2, value = 0;
	string junk;
	int full_waste[10] = {1,1,1,1,1,1,1,1,1,1};

	/*open input file modify as needed, uncomment for testing*/
	
	
	ifstream infile("N10c.txt");
	ofstream outfile("outputN10c.txt");

	
	/*if input file can't be opened output "File not found"*/
	if (!infile.is_open())
	{
		cout << "File not found!" << endl;
		system("pause");
		return -1;		
	}
	if (!outfile.is_open())
	{
		cout << "No output file was found!" << endl;
		system("pause");
		return -1;
	}

	
	/*Reading in dimenstion, starting point and ending point*/

	infile >> dimensions >> comma >> start >> comma >> end;

	/*Decalring adjacency matrix of vector to store actual path*/
	vector<int>** actual_path = new vector<int>* [dimensions];
	for (int i = 1; i <= dimensions; i++)
	{
		actual_path[i] = new vector<int>[dimensions+1]; 
	}	

	/*Decalring adjesency matrix to store distance between cities*/
	int** graph_distance = new int*[dimensions];
	for (int i = 1; i <= dimensions; i++)
	{
		graph_distance[i] = new int[dimensions];
	}

	/*Declare an adjacency matrix to store flow*/
	int** graph_cars = new int*[dimensions];
	for (int j = 1; j <= dimensions; j++)
	{
		graph_cars[j] = new int[dimensions];
	}

	/*Assign INF values to distance matrix as a default value*/
	for (int k = 1; k <= dimensions; k++)
	{
		for (int m = 1; m <= dimensions; m++)
		{
			graph_distance[k][m] = INF;
		}
	}
	/*assign default value to matrix car flow wchich is 0*/
	for (int k = 1; k <= dimensions; k++)
	{
		for (int m = 1; m <= dimensions; m++)
		{
			graph_cars[k][m] = 0;
		}
	}

	/*readin input file and based on input file form distance and car flow matricies*/
	while (!infile.eof())
	{
		getline(infile,junk);
		infile >> graph_type >> comma >> loc1 >> comma >> loc2 >> comma >> value;
		if (graph_type == 'E')
		{
			graph_distance[loc1][loc2] = value;
		}
		if (graph_type == 'F')
		{
			graph_cars[loc1][loc2] = value;
		}
	}
	/*Output graph E which represents the distance between cities*/

	outfile << "---------------Graph E-------------" << endl;
	for (int i = 1; i <= dimensions; i++)
	{
		outfile << "    "<< i<< "  ";
	}
	outfile << endl;
	outfile << "---------------------------------------------" << endl;
	for (int k = 1; k <= dimensions; k++)
	{
		outfile << k  << "|   ";
		for (int m = 1; m <= dimensions; m++)
		{
			if (graph_distance[k][m] == 99999)
			{
				outfile << " " << "INF" << "    ";
			}
			else
			{
				outfile << " " << graph_distance[k][m] << "    ";
			}
		}
		outfile << endl;
	}
	outfile << endl << endl;

	
	/*Call Floydwarshall function to calculate sortest path from each node to each node*/
	floydwarshall(graph_distance, dimensions,actual_path,graph_cars, start,end,outfile,full_waste);
	return 0;
}

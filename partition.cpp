#include <stdio.h>
#include <bits/stdc++.h>
#include <unordered_map>
#include <math.h>
using namespace std;

vector< vector int > bisection(int l, int r, int start, int finish);

vector< vector int > partition(int l, int r){					//For partitioning
	
	vector< vector <int> > partition;							//Partition to be returned
	
	for(int i = 0; i < vertices[l].size(); i++){
		graph[l][i][-1] = 1;								//Say that all vertices are are in partition 1
	}

	partition = bisection(l, r, 1, pow(2, r));

	//partition.insert(partition.end(), bisection(l, r - 1, 1, pow(2, r - 1)).begin(), bisection(l, r - 1, 1, pow(2, r - 1)).end());
	//partition.insert(partition.end(), bisection(l, r - 1, pow(2, r - 1) + 1, pow(2, r)).begin(), bisection(l, r - 1, pow(2, r - 1) + 1, pow(2, r)).begin());
}

vector < vector int > bisection(int l, int r, int start, int finish){

	vector< vector <int> partition;

	vector< int > this_partition;

	for(int i = 0; i < vertices[l].size(); i++){
		if(vertices[l][i][-1] == start){
			long int root = i;
			//this_partition.push_back[i];
			break;
		}
	}

	int number = 1;
	int max = (n/k)*pow(2, r - 1);

	queue<int > vertex_q;
	vertex_q.push(root);
	long int v2;

	vector<int> flags;
	flags.resize(vertices[l].size());
	flags = {};
	flags[v2] = 1;

	while(number < max){
		v2 = vertex_q.pop();
		this_partition.push_back(v2);
		number++;
		//flags[v2] = 1;
		long int v3;
		for(it = graph[l][v2].begin() + 2; it != graph[l][v2].end(); it++){	//Check it condition
			v3 = it->first;
			if(flags[v3] == 0){
				vertex_q.push(v3);
				flags[v3] = 1;
			}
		}
	}

	if(r == 0){
		partition.push_back(this_partition);
	}
	else{
		partition.insert(partition.end(), bisection(l, r - 1, start, start + pow(2, r - 1)).begin(), bisection(l, r - 1, start, pow(2, r - 1)).end());
		partition.insert(partition.end(), bisection(l, r - 1, start + pow(2, r - 1) + 1, start + pow(2, r)).begin(), bisection(l, r - 1, start + pow(2, r - 1) + 1, pow(2, r)).end());
	}

	return partition;
}

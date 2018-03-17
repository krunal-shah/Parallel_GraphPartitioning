#include "stdio.h"
using namespace std;
#include <bits/stdc++.h>
#include <unordered_map>

vector< pair<long int, long int> > HEM(vector< vector< pair<long int, long int> > >);

int main(){
	vector < vector < pair < long int, long int> > > graph;
	pair < long int, long int> e12;
	pair < long int, long int> e14;
	pair < long int, long int> e21;
	pair < long int, long int> e23;
	pair < long int, long int> e32;
	pair < long int, long int> e35;
	pair < long int, long int> e41;
	pair < long int, long int> e45;
	pair < long int, long int> e53;
	pair < long int, long int> e54;
	e12.first = 2;
	e12.second = 1;
	e14.first = 4;
	e14.second = 2;
	e21.first = 1;
	e21.second = 1;
	e23.first = 3;
	e23.second = 1;
	e32.first = 2;
	e32.second = 1;
	e35.first = 5;
	e35.second = 1;
	e41.first = 1;
	e41.second = 2;
	e45.first = 5;
	e45.second = 1;
	e53.first = 3;
	e53.second = 1;
	e54.first = 4;
	e54.second = 1;
	pair<long int, long int> empty;
	empty.first = 0;
	empty.second = 0;
	vector< pair <long int, long int> > v0;
	vector< pair <long int, long int> > v1;
	vector< pair <long int, long int> > v2;
	vector< pair <long int, long int> > v3;
	vector< pair <long int, long int> > v4;
	vector< pair <long int, long int> > v5;
	v0.push_back(empty);
	v1.push_back(empty);
	v1.push_back(e12);
	v1.push_back(e14);
	v2.push_back(empty);
	v2.push_back(e21);
	v2.push_back(e23);
	v3.push_back(empty);
	v3.push_back(e32);
	v3.push_back(e35);
	v4.push_back(empty);
	v4.push_back(e41);
	v4.push_back(e45);
	v5.push_back(empty);
	v5.push_back(e53);
	v5.push_back(e54);
	graph.push_back(v0);
	graph.push_back(v1);
	graph.push_back(v2);
	graph.push_back(v3);
	graph.push_back(v4);
	graph.push_back(v5);
	
	vector< pair<long int, long int> > matching;
	matching = HEM(graph);
	
	for(int i = 0; i < matching.size(); i++){
		//printf("%ld, %ld\n", matching[i].first, matching[i].second);
	}

	return 0;
}

vector< pair<long int, long int> > HEM(int p, int l){		//For calulating HEM
	
	vector< pair<long int, long int> > matching;		//Matching to be returned
	
	for(int i = 0; i < vertices[k].size(); i++){
		if(graph[k][i][0].second == p)
			graph[k][i][0].first = 0;								//Say that all vertices are unmatched
	}
	for(int i = 0; i < vertices[k].size(); i++){
		if(graph[k][i][0].first == 0){							//If the vertex is unmatched
			long int max = 0;
			long int k = 0;
			for(int j = 1; j < graph[k][i].size(); j++){
				long int v2 = graph[k][i][j].first;
				if(graph[k][i][j].second > max && graph[k][v2][0].first == 0 && graph[k][v2][0].second == p){
					max = graph[k][i][j].second;
				}
			}
			if(max > 0){
				pair<long int, long int> edge;
				edge.first = i;
				edge.second = v2;
				matching.push_back(edge);
				graph[k][v2][0].first = 1;
			}
			graph[k][i][0].first = 1;
		}
	}
	
	return matching;
} 

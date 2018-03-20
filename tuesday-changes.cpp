#include <iostream>
#include <string.h>
#include <math.h>
#include <vector>
#include <map>
#include <map>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <queue>
using namespace std;

// Note: Changed all the function arguments to pass by reference

vector< vector< pair <long int,long int > > >  graph;						//The adjacency list representing the main graph
//The merged_new and merged_old are now local
//vector< vector< int > part;				

vector< pair<long int, long int> > HEM(vector< vector< pair<long int, long int> > > &subgraph){		//For calulating HEM: Input an adjacency list
	vector< pair<long int, long int> > matching;			//Matching to be returned

	vector<int> in_matching;								//Indicates if a vertex has been matched
	in_matching.resize(subgraph.size());						//Takes values 0 and 1

	for(long int i = 1; i < subgraph.size(); i++){			//For each vertex
		if(in_matching[i] == 0){							//If the vertex is unmatched
			
			long int max = -1;
			long int k = 0;
			long int v2;

			for(long int j = 1; j < subgraph[i].size(); j++){	//For every neighbour of vertex i
				if(subgraph[i][j].second > max && in_matching[subgraph[i][j].first] == 0 ){
										//j not matched yet
					max = subgraph[i][j].second;
					v2 = subgraph[i][j].first;
				}
			}
			if(max > 0){									
				pair<long int, long int> edge;					//Add a new edge to the matching
				edge.first = i;
				edge.second = v2;
				matching.push_back(edge);

				in_matching[v2] = 1;							//v2 is matched now
			}
			in_matching[i] = 1;									//i is matched now
		}
	}
	return matching;
} 

long int n;
long int k;
long int p;
long int level;
long int read_input(char* argv[])
{
	ifstream input_file;
	input_file.open (argv[1], ios::in);
	if( !input_file.is_open() )
	{
		cerr << "Error opening the input_file\n";
		return -2;
	}
	input_file >> n;

	graph.resize(n+1);
	
	long int count = 1;
	string line;
	getline(input_file, line);
	while (getline(input_file, line)) 
	{
		istringstream row(line);
		long int num;
		long int j = 0;
		graph[count].push_back(make_pair(0, 1));
		if(row >> j)
		{
			graph[count].push_back(make_pair(j, 1));
			while (row >> j)
			{
				graph[count].push_back(make_pair(j, 1));
			}
		}
		count++;
	}
	return 0;
}

vector< vector < vector <long int> > > bisection(vector< vector< pair< long int, long int> > > &in_graph){ //Input - a graph, output - two subgraphs

	long int size = in_graph.size();

	vector< vector <long int > > subgraph_1;							//To be returned
	vector< vector <long int > > subgraph_2;								

	vector<long int> flags;
	flags.resize(size);

	long int max;														
	for(long int i = 0; i < size; i++){									//Calculate the total weight of vertices in the graph
		max = max + in_graph[i][0].second;
	}																	
	max = max/2;														//This is the (maximum) weight of vertices in subgraph_1
	// printf("k = %lld, n = %lld, r = %lld, max = %lld \n", k, n, r, max);						

	long int number = 0;												//Weight of vertices in the first subgraph

	while(number < max){												//While you don't have enough vertices in a half-subgraph_1
		long int root;													
		for(long int i = 0; i < size; i++){								//Iterate over vertices to find a root
			if(flags[i] == 0){											//Only certain vertices are available for selection
				root = i;									
				break;
			}
		}

		//flags[root] = 1;												//The root has been selected
		queue<long int> vertex_q;										//The queue in BFS										
		vertex_q.push(root);											//Push the root vertex into queue
		// cout << "1 " << root << endl;										
		long int v2;												

		while(number < max){
			if(!vertex_q.empty()){										//If the queue is non-empty

				v2 = vertex_q.front();									//Pop from the front of the queue
				vertex_q.pop();

				if(flags[v2] == 1) continue;

				flags[v2] = 1;											//Indicate that the vertex has been selected
				//subgraph_1.push_back(v2);								//Add this vertex to subgraph_1											
				
				number = number + in_graph[v2][0].second;				//Add the weight of v2

				long int v3;

				map< long int, long int> :: iterator it;
				for(it = in_graph[v2].begin(); it != in_graph[v2].end(); it++){	//Iterate on the neighbours of v2
					if(it->first <= 0)
						continue;

					v3 = it->first;
					if(flags[v3] == 0){
						vertex_q.push(v3);
					}
				}
			}
			else break;
		}
	}

	vector<long int> subgraph_2;

	vector<long int> old_index_list;
	old_index_list.resize(size);

	long int count1 = 0;
	long int count2 = 0;
	for(int i = 0; i < size; i++){
		if(flags[i] == 0){
			count2++;
			old_index_list[i] = count2;
		}
		else{
			count1++;
			old_index_list[i] = count1;
		}
	}

	count1 = 0;
	count2 = 0;

	vector< pair<long int, long int> > empty;
	subgraph_1.push_back(empty);
	subgraph_2.push_back(empty);

	for(int i = 0; i < size; i++){							//Form the subgraphs 1 and 2
		vector< pair<long int, long int> > neighbour_list;
		neighbour_list.push_back(make_pair(in_graph[i][0].first, in_graph[i][0].second));
		//In the new graph, one must indicate the parent id
		if(flags[i] == 0){									//If vertex i is in subgraph_2
			for(j = 1; j < in_graph[i].size(); j++){		//Look at every neighbour of 
				if(flags[in_graph[i][j].first] == 0){
					long int new_index_ = old_index_list[in_graph[i][j].first];
					neighbour_list.push_back(make_pair(in_graph[i][j].first, in_graph[i][j].second));
				}
			}
			count2++;
			subgraph_2.push_back(neighbour_list);
		}
		else{
			for(j = 1; j < in_graph[i].size(); j++){		//Look at every neighbour of 
				if(flags[in_graph[i][j].first] == 1){
					long int new_index_ = old_index_list[in_graph[i][j].first];
					neighbour_list.push_back(make_pair(in_graph[i][j].first, in_graph[i][j].second));
				}
			}
			count1++;
			subgraph_2.push_back(neighbour_list);
		}
	}



	// if(r == 1){
	// 	for(long int i = 0; i < vertices[l].size(); i++){				//Iterate over vertices to find a root
	// 		if(graph[l][vertices[l][i]][-1] == label){		//Only certain vertices are available for selection
	// 			subgraph_2.push_back(vertices[l][i]);
	// 		}
	// 	}
	// 	subgraph_1.push_back(subgraph_2);
	// 	subgraph_1.push_back(subgraph_22);

	// }
	// else if(r > 1){
	// 	vector<vector<long int> > b1 = bisection(l, r - 1, label, n);
	// 	vector<vector<long int> > b2 = bisection(l, r - 1, label + pow(2, r - 1), n);
	// 	subgraph_1.insert(subgraph_1.end(), b1.begin(), b1.end());
	// 	subgraph_1.insert(subgraph_1.end(), b2.begin(), b2.end());
	//}

	vector< vector< vector<long int, long int> > > partition;
	partition.push_back(subgraph_1);
	partition.push_back(subgraph_2);
	return partition;
}

vector< vector< long int > multilevel(vector< vector< pair< long int, long int> > > &in_graph, long int k){			//The whole multilevel algorithm
	
	long int curr_n = in_graph.size();			//Size of the current graph

	vector< vector< vector< long int, long int> > >  graph_list;
	graph_list.resize(1);	
	graph_list[0] = in_graph;					//A local graph list for storing graph at each level. We'll delete this later

	vector< map<long int, pair<long int,long int> > > merged_new;
	vector< map<long int,long int > > merged_old;

	long int level = 0;						//Level

	while(curr_n > 100){					//Uncoarsening part
		vector< pair< long int, long int> > matching = HEM(graph_list[level]);			//Calculate a matching

		if(matching.size() == 0){
			break;																		// Note: Changed continue to break
		}

		curr_n -= matching.size();			//Reevaluate curr_n
		
		merged_old.push_back(map<long int,long int >());
		merged_new.push_back(map<long int, pair<long int,long int> >());

		long int next_counter = 1;
		for (long int j = 0; j < matching.size(); ++j)
		{
			long int a = matching[j].first;
			long int b = matching[j].second;
			long int c = next_counter;
			next_counter++;
			merged_new[level][c] = make_pair(a, b);
			merged_old[level][a] = c;
			merged_old[level][b] = c;
		}
		for (long int j = 1; j < graph_list[level].size(); ++j)
		{
			if(merged_old[level].find(j) == merged_old[level].end())
			{
				merged_old[level][j] = next_counter;
				merged_new[level][next_counter] = make_pair(j, j);
				next_counter++;
			}
		}
		graph_list.push_back(vector< vector< pair <long int,long int > > >(0));
		graph_list[level+1].resize(next_counter);
		for (long int j = 1; j < graph_list[level].size(); ++j)
		{
			if(graph_list[level+1][merged_old[level][j]].size() == 0)
			{
				graph_list[level+1][merged_old[level][j]].push_back(make_pair(graph_list[level][j][0].first, graph_list[level][j][0].second));
			}
			else
			{
				graph_list[level+1][merged_old[level][j]][0].second += graph_list[level][j][0].second;
			}
		}

		for (long int j = 1; j < graph_list[level].size(); ++j)
		{
			long int my_index = j;
			if(graph_list[level][j].size() == 0)
			{
				cout << "Alert " << j << endl;
			}
			long int new_index;
			vector<pair <long int,long int > > *new_list;
			
			if(merged_old[level].find(my_index) != merged_old[level].end())
			{
				new_index = merged_old[level][my_index];
				new_list = &graph_list[level+1][new_index];
				// if(new_list->size()==0)
				// {
				// 	// new_list->push_back(make_pair(0, graph[level][my_index][0].second));
				// }
				// else
				// {
				// 	// (*new_list)[0].second += graph[level][my_index][0].second;	
				// }
			}
			
			vector< pair< long int, long int > >::iterator it;
			map< long int, long int > temp_map;
			for(long int y = 1; y < graph_list[level+1][new_index].size(); y++)
			{
				long int tmp_new = graph_list[level+1][new_index][y].first;
				temp_map[tmp_new] = y;
			}

			for (long int y = 1; y < graph_list[level][my_index].size(); y++)
			{
				long int tmp_index = graph_list[level][my_index][y].first;
				long int tmp_new;
				tmp_new = merged_old[level][tmp_index];
				if(tmp_new == new_index)
					continue;
				if(temp_map.find(tmp_new) != temp_map.end())
				{
					graph_list[level+1][new_index][temp_map[tmp_new]].second += graph_list[level][my_index][y].second;
				}
				else
				{
					new_list->push_back(make_pair(graph_list[level][my_index][y].first, graph_list[level][my_index][y].second));
					temp_map[tmp_new] = new_list->size() - 1;
				}
			}
		}
		level++;
	}


	// Coarsening complete
	vector< vector < vector <long int> > > partitions;
	partitions = bisection(graph_list[level-1]);

	for (int i = 0; i < 2; ++i)
	{
		cout
	}

	// Uncoarsening


	// Recusrive call after freeing up memory

	// graph_list.swap(vector< vector< vector< long int, long int> > >());
	// merged_new.swap(vector< map<long int, pair<long int,long int> > >);
	// merged_old.swap(vector< map<long int,long int > >);

	// multilevel(, k/2);
	// multilevel(, k/2);

}

int main(int argc, char* argv[])
{
	k = atoll(argv[3]);
	p = 1;
	// mynext.resize(p);
	if(read_input(argv)<0)
		return -2;	

	// ofstream output_file;
	// output_file.open (argv[2], ios::out);
	
	multilevel(graph, k);

	// long int curr_n = n;


	
	// while(curr_n > 3)
	// {
	// 	cout << "# Vertices " << curr_n << endl;
	// 	vector< pair< long int, long int> > matching = HEM(level);
	// 	cout << "Matching size = " << matching.size() << endl;
	// 	if(matching.size() == 0)
	// 	{
	// 		continue;
	// 	}
	// 	curr_n -= matching.size();
	// 	if(matching.size() < 1)
	// 	{
	// 		break;
	// 	}
	// 	merged_old.push_back(map<long int,long int >());
	// 	merged_new.push_back(map<long int, pair<long int,long int> >());
	// 	long int next_counter = 1;
	// 	for (long int j = 0; j < matching.size(); ++j)
	// 	{
	// 		long int a = matching[j].first;
	// 		long int b = matching[j].second;
	// 		long int c = next_counter;
	// 		next_counter++;
	// 		merged_new[level][c] = make_pair(a, b);
	// 		merged_old[level][a] = c;
	// 		merged_old[level][b] = c;
	// 	}
	// 	for (long int j = 1; j < graph[level].size(); ++j)
	// 	{
	// 		if(merged_old[level].find(j) == merged_old[level].end())
	// 		{
	// 			merged_old[level][j] = next_counter;
	// 			merged_new[level][next_counter] = make_pair(j, j);
	// 			next_counter++;
	// 		}
	// 	}
	// 	graph.push_back(vector< vector< pair <long int,long int > > >(0));
	// 	graph[level+1].resize(next_counter);
	// 	for (long int j = 1; j < graph[level].size(); ++j)
	// 	{
	// 		if(graph[level+1][merged_old[level][j]].size() == 0)
	// 		{
	// 			graph[level+1][merged_old[level][j]].push_back(make_pair(0, graph[level][j][0].second));
	// 		}
	// 		else
	// 		{
	// 			graph[level+1][merged_old[level][j]][0].second += graph[level][j][0].second;
	// 		}
	// 	}

	// 	for (long int j = 1; j < graph[level].size(); ++j)
	// 	{
	// 		long int my_index = j;
	// 		if(graph[level][j].size() == 0)
	// 		{
	// 			cout << "Alert " << j << endl;
	// 		}
	// 		long int new_index;
	// 		vector<pair <long int,long int > > *new_list;
			
	// 		if(merged_old[level].find(my_index) != merged_old[level].end())
	// 		{
	// 			new_index = merged_old[level][my_index];
	// 			new_list = &graph[level+1][new_index];
	// 			// if(new_list->size()==0)
	// 			// {
	// 			// 	// new_list->push_back(make_pair(0, graph[level][my_index][0].second));
	// 			// }
	// 			// else
	// 			// {
	// 			// 	// (*new_list)[0].second += graph[level][my_index][0].second;	
	// 			// }
	// 		}
			
	// 		vector< pair< long int, long int > >::iterator it;
	// 		map< long int, long int > temp_map;
	// 		for(int y = 1; y < graph[level+1][new_index].size(); y++)
	// 		{
	// 			long int tmp_new = graph[level+1][new_index][y].first;
	// 			temp_map[tmp_new] = y;
	// 		}

	// 		for (int y = 1; y < graph[level][my_index].size(); y++)
	// 		{
	// 			long int tmp_index = graph[level][my_index][y].first;
	// 			long int tmp_new;
	// 			tmp_new = merged_old[level][tmp_index];
	// 			if(tmp_new == new_index)
	// 				continue;
	// 			if(temp_map.find(tmp_new) != temp_map.end())
	// 			{
	// 				graph[level+1][new_index][temp_map[tmp_new]].second += graph[level][my_index][y].second;
	// 			}
	// 			else
	// 			{
	// 				new_list->push_back(make_pair(tmp_new, graph[level][my_index][y].second));
	// 				temp_map[tmp_new] = new_list->size() - 1;
	// 			}
	// 		}
	// 	}
	// 	level++;
	// }
}

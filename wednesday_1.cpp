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

// HYPER PARAMETERS
long int coarsening_limit = 1000;
// Note: Changed all the function arguments to pass by reference

vector< vector< pair <long int,long int > > >  graph;						//The adjacency list representing the main graph				
long int n;
long int r;
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
		graph[count].push_back(make_pair(count, 1));
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
void print_graph(vector< vector< pair< long int, long int> > > &some_graph)
{
	for (int i = 1; i < some_graph.size(); ++i)
	{
		cout << some_graph[i][0].first << " -- ";
		for (int j = 1; j < some_graph[i].size(); ++j)
		{
			cout << some_graph[some_graph[i][j].first][0].first << " ";
		}
		cout << endl;
	}
}

vector< pair<long int, long int> > HEM (vector< vector< pair<long int, long int> > > &subgraph){		//For calulating HEM: Input an adjacency list
	vector< pair<long int, long int> > matching;			//Matching to be returned

	vector<int> in_matching;								//Indicates if a vertex has been matched
	in_matching.resize(subgraph.size());						//Takes values 0 and 1
	// srand(time(NULL));
	cout << 1 << endl;
	long int offset_11 = rand();
	long int offset_12 = rand();
	long int offset_1 = (10000*offset_11 + offset_12);
	long int l = offset_1;
	long int i;
	for(l = 1; l < subgraph.size(); l++){			//For each vertex
		i = (l + offset_1) % (subgraph.size() - 1) + 1;
		if(in_matching[i] == 0){							//If the vertex is unmatched
			long int max = -1;
			long int v2;
			long int offset_2 = rand();
			long int k = 0;
			long int j;
			for(k = 1; k < subgraph[i].size(); k++){	//For every neighbour of vertex i
				j = (k + offset_2) % (subgraph[i].size() - 1) + 1;
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

vector< long int>  bisection(vector< vector< pair< long int, long int> > > &in_graph){ //Input - a graph, output - two subgraphs

	vector< pair< long int, long int> > temp;
	cout << "Max weights for " << in_graph.size() -1 << " vertices" << endl;
	for (int i = 1; i < in_graph.size(); ++i)
	{
		temp.push_back(make_pair(in_graph[i][0].second, i));
	}
	sort(temp.begin(), temp.end());
	// for (int i = in_graph.size() - 1; i > in_graph.size() - 10; i--)
	// {
	// 	cout << temp[i].first << " -- " << temp[i].second << " -- " << in_graph[temp[i].second].size() << endl;
	// }
	// cout << "Complete" << endl;

	long int size = in_graph.size();

	vector<long int> flags(size);

	long int max = 0;														
	for(long int i = 1; i < size; i++){									//Calculate the total weight of vertices in the graph
	 	max = max + in_graph[i][0].second;
	}																	
	max = max/2;														//This is the (maximum) weight of vertices in subgraph_1

	long int number = 0;												//Weight of vertices in the first subgraph

	while(number < max){												//While you don't have enough vertices in a half-subgraph_1
		long int root;													
		for(long int i = 1; i < size; i++){								//Iterate over vertices to find a root
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
				// cout << number << endl;

				long int v3;

				vector< pair< long int, long int> >::iterator it;
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

	printf("max = %ld number =%ld\n", max, number);						

	long int num0 = 0, num1 = 0;
	for(long int i = 1; i < in_graph.size(); i++){
		if(flags[i] == 0) num0 = num0 + in_graph[i][0].second;
		else num1 = num1 + in_graph[i][0].second;
	}
	// cout << "# " << num0 << " " << num1 << endl;

	flags[0] = number;
	return flags;
}

void refine(vector< vector< pair< long int, long int > > > &in_graph, vector< long int > &flags_in, int flag){
	// cout << "Starting refinement for " << in_graph.size() << endl;
	//Refinement process
	vector< long int> gains(in_graph.size(), 0);
	for (long int i = 1; i < gains.size(); ++i)
	{
		int my_part = flags_in[i];
		for (long int j = 1; j < in_graph[i].size(); ++j)
		{
			if(flags_in[in_graph[i][j].first] == my_part)
			{
				gains[i] -= in_graph[i][j].second;
			}
			else
			{
				gains[i] += in_graph[i][j].second;
			}
		}
	}

	map< long int, long int> part0;
	map< long int, long int> part1;
	for (long int i = 1; i < gains.size(); ++i)
	{
		if(flags_in[i] == 0)
		{
			part0[i] = gains[i];
		}
		else
		{
			part1[i] = gains[i];
		}
	}

	long int num0 = 0, num1 = 0;
	for(long int i = 1; i < in_graph.size(); i++)
	{
		if(flags_in[i] == 0) num0 = num0 + in_graph[i][0].second;
		else num1 = num1 + in_graph[i][0].second;
	}

	long int max_gain = 0;
	long int greater_index;

	if(num1 > num0)
	{
		max_gain = part1.rbegin()->second;
		greater_index = 1;
	}
	else if(num0 > num1)
	{
		max_gain = part0.rbegin()->second;
		greater_index = 0;
	}
	else
	{
		if((rand() % 2) == 0)
		{
			max_gain = part0.rbegin()->second;
			greater_index = 0;
		}
		else
		{
			max_gain = part1.rbegin()->second;
			greater_index = 1;
		}
	}
	printf("total = %lu num1 = %ld num0 = %ld\n", in_graph.size(), num1, num0);
	long int max_key;
	int counter = 0;
	long int last_key = -1;
	while(counter < 1)
	{
		if(max_gain > 0){
			counter = 0;
		}
		else counter++;
		if(greater_index == 0)
			max_key = part0.rbegin()->first;
		else
			max_key = part1.rbegin()->first;
		if(last_key == max_key)
			break;
		last_key = max_key;
		for (long int i = 1; i < in_graph[max_key].size(); ++i)
		{
			if(flags_in[max_key] == flags_in[in_graph[max_key][i].first])
			{
				gains[in_graph[max_key][i].first] += 2*in_graph[max_key][i].second;
			}
			else
			{
				gains[in_graph[max_key][i].first] -= 2*in_graph[max_key][i].second;
			}
		}
		flags_in[max_key] = (flags_in[max_key] + 1)%2;
		gains[max_key] = (long int)(-1)*gains[max_key];
		if(greater_index == 1)
		{
			num1 -= in_graph[max_key][0].second;
			num0 += in_graph[max_key][0].second;
		}
		else
		{
			num0 -= in_graph[max_key][0].second;
			num1 += in_graph[max_key][0].second;
		}

		if(num1 > num0)
		{
			max_gain = part1.rbegin()->second;
			greater_index = 1;
		}
		else if(num0 > num1)
		{
			max_gain = part0.rbegin()->second;
			greater_index = 0;
		}
		else
		{
			if((rand() % 2) == 0)
			{
				max_gain = part0.rbegin()->second;
				greater_index = 0;
			}
			else
			{
				max_gain = part1.rbegin()->second;
				greater_index = 1;
			}
		}
	}

	float total = (float)num1 + (float)num0;
	if(flag == 1)
	{
		long int diff;
		if(num1 > num0)
		{
			max_gain = part1.rbegin()->second;
			greater_index = 1;
			diff = num1 - num0;
		}
		else if(num0 > num1)
		{
			max_gain = part0.rbegin()->second;
			greater_index = 0;
			diff = num0 - num1;
		}
		else
		{
			diff = 0;
			if((rand() % 2) == 0)
			{
				max_gain = part0.rbegin()->second;
				greater_index = 0;
			}
			else
			{
				max_gain = part1.rbegin()->second;
				greater_index = 1;
			}
		}
		float tolerance = (float)diff/total;
		while(tolerance >= 0.05)
		{
			if(greater_index == 0)
				max_key = part0.rbegin()->first;
			else
				max_key = part1.rbegin()->first;
			for (long int i = 1; i < in_graph[max_key].size(); ++i)
			{
				if(flags_in[max_key] == flags_in[in_graph[max_key][i].first])
				{
					gains[in_graph[max_key][i].first] += 2*in_graph[max_key][i].second;
				}
				else
				{
					gains[in_graph[max_key][i].first] -= 2*in_graph[max_key][i].second;
				}
			}
			flags_in[max_key] = (flags_in[max_key] + 1)%2;
			gains[max_key] = (long int)(-1)*gains[max_key];
			if(greater_index == 1)
			{
				num1 -= in_graph[max_key][0].second;
				num0 += in_graph[max_key][0].second;
			}
			else
			{
				num0 -= in_graph[max_key][0].second;
				num1 += in_graph[max_key][0].second;
			}

			if(num1 > num0)
			{
				max_gain = part1.rbegin()->second;
				greater_index = 1;
				diff = num1 - num0;
			}
			else if(num0 > num1)
			{
				max_gain = part0.rbegin()->second;
				greater_index = 0;
				diff = num0 - num1;
			}
			else
			{
				diff = 0;
				if((rand() % 2) == 0)
				{
					max_gain = part0.rbegin()->second;
					greater_index = 0;
				}
				else
				{
					max_gain = part1.rbegin()->second;
					greater_index = 1;
				}
			}
			tolerance = (float)diff/total;
		}
	}
	// cout << "Refined" << endl;
}

vector< vector< long int> > multilevel(vector< vector< pair< long int, long int> > > &in_graph, long int k){			//The whole multilevel algorithm
	
	long int curr_n = in_graph.size() - 1;			//Size of the current graph
	cout << "#Vertices = " << curr_n << endl;

	vector< vector< vector< pair< long int, long int> > > >  graph_list;
	graph_list.resize(1);	
	graph_list[0] = in_graph;					//A local graph list for storing graph at each level. We'll delete this later

	vector< map<long int, pair<long int,long int> > > merged_new;
	vector< map<long int,long int > > merged_old;

	long int level = 0;						//Level
	cout << "Ingraph\n";
	// print_graph(in_graph);
	long int match_size = 10;
	while(curr_n > coarsening_limit){					//Uncoarsening part
		vector< pair< long int, long int> > matching = HEM(graph_list[level]);			//Calculate a matching
		random_shuffle(matching.begin(), matching.end());
		cout << "Matching size = " << matching.size() << endl; cout << flush;
		match_size = matching.size();
		if(matching.size() == 0){
			//printf("%ld jdflfjb\n", level);
			break;																		// Note: Changed continue to break
		}

		curr_n -= matching.size();			//Reevaluate curr_n
		
		merged_old.push_back(map<long int,long int >());
		merged_new.push_back(map<long int, pair<long int,long int> >());

		long int next_counter = 1;
		vector< long int> distribute_vertices(graph_list[level].size() - match_size - 1);
		for (int i = 0; i < distribute_vertices.size(); ++i)
		{
			distribute_vertices[i] = i + 1;
		}
		random_shuffle(distribute_vertices.begin(), distribute_vertices.end());
		next_counter = 0;
		for (long int j = 0; j < matching.size(); ++j)
		{
			long int a = matching[j].first;
			long int b = matching[j].second;
			long int c = distribute_vertices[next_counter];
			// if(c==1)
			// {
			// 	printf("a = %ld b = %ld wta = %ld wtb = %ld\n", a, b, graph_list[level][a][0].second, graph_list[level][b][0].second);
			// }
			next_counter++;
			merged_new[level][c] = make_pair(a, b);
			merged_old[level][a] = c;
			merged_old[level][b] = c;
		}

		for (long int j = 1; j < graph_list[level].size(); ++j)
		{
			if(merged_old[level].find(j) == merged_old[level].end())
			{
				merged_old[level][j] = distribute_vertices[next_counter];
				merged_new[level][distribute_vertices[next_counter]] = make_pair(j, j);
				next_counter++;
			}
		}
		graph_list.push_back(vector< vector< pair <long int,long int > > >(0));
		graph_list[level+1].resize(next_counter+1);
		for (long int j = 1; j < graph_list[level].size(); ++j)
		{
			if(graph_list[level+1][merged_old[level][j]].size() == 0)
			{
				graph_list[level+1][merged_old[level][j]].push_back(make_pair(0, graph_list[level][j][0].second));
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
			
			if(merged_old[level].find(my_index) != merged_old[level].end())
			{
				new_index = merged_old[level][my_index];
			}
			
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
					graph_list[level+1][new_index].push_back(make_pair(tmp_new, graph_list[level][my_index][y].second));
					temp_map[tmp_new] = graph_list[level+1][new_index].size() - 1;
				}
			}
		}
		cout << "Coarsened graph" << endl;
		// print_graph(graph_list[level+1]);
		level++;
	}

	cout << "Coarsening complete\n";


	// Coarsening complete
	//vector< vector< vector< pair< long int, long int> > > > partitions;
	vector< vector< long int> > flags;
	flags.resize(level+1);
	flags[level] = bisection(graph_list[level]);

	cout << "Bisection complete\n";
	cout << flush;
	// Uncoarsening
	long int final = level;
	
	//printf("%ld\n", level);
	for (long int level = final; level > 0; level--)
	{
		flags[level-1].resize(graph_list[level-1].size());
		for (long int i = 1; i < graph_list[level].size(); ++i)
		{
			long int my_index = i;
			long int child_a = merged_new[level-1][my_index].first;
			long int child_b = merged_new[level-1][my_index].second;
			if(child_b == child_a)
			{
				flags[level - 1][child_a] = flags[level][my_index];
			}
			else{
				flags[level - 1][child_a] = flags[level][my_index];
				flags[level - 1][child_b] = flags[level][my_index];
			}
		}
		//cout << "here!" << endl;
		if(level == 1)
			refine(graph_list[level-1], flags[level-1], 0);
		else
			refine(graph_list[level-1], flags[level-1], 0);
	}
	// print_graph(graph_list[0]);
	// cout << "Flags for uncoarsened graph\n";
	// for (int i = 1; i < graph_list[0].size(); ++i)
	// {
	// 	cout << flags[0][i] << endl;
	// }
	cout << "Uncoarsening complete\n";
	// cout << flush;

	// Recursive call after freeing up memory

	vector< vector< pair< long int, long int> > > subgraph_1(1);
	vector< vector< pair< long int, long int> > > subgraph_2(1);
	vector<long int> old_index_list;
	old_index_list.resize(in_graph.size());

	long int count1 = 0;
	long int count2 = 0;
	for(int i = 1; i < in_graph.size(); i++){
		if(flags[0][i] == 0){
			count2++;
			old_index_list[i] = count2;
		}
		else{
			count1++;
			old_index_list[i] = count1;
		}
	}

	cout << "Old indices complete\n";
	cout << flush;

	for(long int i = 1; i < in_graph.size(); i++){							//Form the subgraphs 1 and 2
		vector< pair<long int, long int> > neighbour_list;
		neighbour_list.push_back(make_pair(in_graph[i][0].first, in_graph[i][0].second));
		//In the new graph, one must indicate the parent id
		if(flags[0][i] == 0){									//If vertex i is in subgraph_2
			for(long int j = 1; j < in_graph[i].size(); j++){		//Look at every neighbour of 
				if(flags[0][in_graph[i][j].first] == 0){
					long int new_index_ = old_index_list[in_graph[i][j].first];
					neighbour_list.push_back(make_pair(new_index_, in_graph[i][j].second));
				}
			}
			subgraph_2.push_back(neighbour_list);
		}
		else{
			for(long int j = 1; j < in_graph[i].size(); j++){		//Look at every neighbour of 
				if(flags[0][in_graph[i][j].first] == 1){
					long int new_index_ = old_index_list[in_graph[i][j].first];
					neighbour_list.push_back(make_pair(new_index_, in_graph[i][j].second));
				}
			}
			subgraph_1.push_back(neighbour_list);
		}
	}
	// cout << subgraph_1.size() << " " << subgraph_2.size() << endl;

	cout << "Subgraph formation complete\n";
	cout << flush;

	vector< vector< vector< pair< long int, long int> > > > temp_graph_list(0);
	vector< map<long int, pair<long int,long int> > > temp_merged_new(0);
	vector< map<long int, long int > > temp_merged_old(0);
	graph_list.swap(temp_graph_list);
	merged_new.swap(temp_merged_new);
	merged_old.swap(temp_merged_old);

	vector< vector< long int> > part2_ret, part1_ret;

	if(k > 2){
		part1_ret = multilevel(subgraph_1, k/2);;
		part2_ret = multilevel(subgraph_2, k/2);;
	}
	else{
		part2_ret.resize(1);
		part1_ret.resize(1);
		for(int i = 1; i < subgraph_1.size(); i++){
			part1_ret[0].push_back(subgraph_1[i][0].first);
		}
		for(int i = 1; i < subgraph_2.size(); i++){
			part2_ret[0].push_back(subgraph_2[i][0].first);
		}
	}
	part1_ret.insert(part1_ret.end(), part2_ret.begin(), part2_ret.end());
	return part1_ret;

}

int main(int argc, char* argv[])
{
	srand (100);
	r = atoll(argv[3]);
	p = 1;
	if(read_input(argv)<0)
		return -2;	

	ofstream output_file;
	output_file.open (argv[2], ios::out);

	cout << "File read \n";
	cout << flush;
	vector< vector< long int> > partitions = multilevel(graph, r);
	vector< long int> labels(graph.size());
	for (int i = 0; i < partitions.size(); ++i)
	{
		for (int k = 0; k < partitions[i].size(); ++k)
		{
			labels[partitions[i][k]] = i;
		}
	}
	for (int i = 1; i < labels.size(); ++i)
	{
		output_file << labels[i] << " ";
	}
	output_file.close();
}

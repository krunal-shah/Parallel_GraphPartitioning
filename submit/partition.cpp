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


vector< pair<long int, long int> > HEM(long int l);		//For calulating HEM
int cyclic_partition(char* argv[]);
int k_way_partition(char* argv[]);

vector< vector< vector< pair <long int,long int > > > > graph(0);
vector< map<long int, pair<long int,long int> > > merged_new;
vector< map<long int,long int > > merged_old;
long int n;
long int k;
long int p;
long int level;

int main(int argc, char* argv[])
{
	k = atol(argv[3]);
	if(cyclic_partition(argv) < 0)
		return -2;	
	return 0;
}

int cyclic_partition(char* argv[])
{
	ifstream input_file;
	input_file.open (argv[1], ios::in);
	if(!input_file.is_open())
	{
		cerr << "Error opening the input_file\n";
		return -2;
	}
	input_file >> n;
	
	ofstream output_file;
	output_file.open(argv[2], ios::out);
	long int counter = 0;
	for (long int i = 0; i < n; ++i)
	{
		output_file << counter << " ";
		counter = (counter + 1)%k;
	}
	output_file.close();
	return 0;
}


int k_way_partition(char* argv[])
{
	ifstream input_file;
	input_file.open (argv[1], ios::in);
	if( !input_file.is_open() )
	{
		cerr << "Error opening the input_file\n";
		return -2;
	}
	input_file >> n;
	level = 0;

	graph.resize(1);
	graph[level].resize(n+1);
	
	long int count = 1;
	string line;
	getline(input_file, line);
	while (getline(input_file, line)) 
	{
		istringstream row(line);
		long int num;
		long int j = 0;
		// vertices[level].push_back(count);
		// mynext[count%p] = n + 1 + count%p;
		graph[level][count].push_back(make_pair(0, 1));
		if(row >> j)
		{
			graph[level][count].push_back(make_pair(j, 1));
			while (row >> j)
			{
				graph[level][count].push_back(make_pair(j, 1));
			}
		}
		count++;
	}

	// ofstream output_file;
	// output_file.open (argv[2], ios::out);
	
	long int curr_n = n;
	
	while(curr_n > 3)
	{
		cout << "# Vertices " << curr_n << endl;
		vector< pair< long int, long int> > matching = HEM(level);
		cout << "Matching size = " << matching.size() << endl;
		if(matching.size() == 0)
		{
			continue;
		}
		curr_n -= matching.size();
		if(matching.size() < 1)
		{
			break;
		}
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
		for (long int j = 1; j < graph[level].size(); ++j)
		{
			if(merged_old[level].find(j) == merged_old[level].end())
			{
				merged_old[level][j] = next_counter;
				merged_new[level][next_counter] = make_pair(j, j);
				next_counter++;
			}
		}
		graph.push_back(vector< vector< pair <long int,long int > > >(0));
		graph[level+1].resize(next_counter);
		for (long int j = 1; j < graph[level].size(); ++j)
		{
			if(graph[level+1][merged_old[level][j]].size() == 0)
			{
				graph[level+1][merged_old[level][j]].push_back(make_pair(0, graph[level][j][0].second));
			}
			else
			{
				graph[level+1][merged_old[level][j]][0].second += graph[level][j][0].second;
			}
		}

		for (long int j = 1; j < graph[level].size(); ++j)
		{
			long int my_index = j;
			if(graph[level][j].size() == 0)
			{
				cout << "Alert " << j << endl;
			}
			long int new_index;
			vector<pair <long int,long int > > *new_list;
			
			if(merged_old[level].find(my_index) != merged_old[level].end())
			{
				new_index = merged_old[level][my_index];
				new_list = &graph[level+1][new_index];
			}
			
			vector< pair< long int, long int > >::iterator it;
			map< long int, long int > temp_map;
			for(int y = 1; y < graph[level+1][new_index].size(); y++)
			{
				long int tmp_new = graph[level+1][new_index][y].first;
				temp_map[tmp_new] = y;
			}

			for (int y = 1; y < graph[level][my_index].size(); y++)
			{
				long int tmp_index = graph[level][my_index][y].first;
				long int tmp_new;
				tmp_new = merged_old[level][tmp_index];
				if(tmp_new == new_index)
					continue;
				if(temp_map.find(tmp_new) != temp_map.end())
				{
					graph[level+1][new_index][temp_map[tmp_new]].second += graph[level][my_index][y].second;
				}
				else
				{
					new_list->push_back(make_pair(tmp_new, graph[level][my_index][y].second));
					temp_map[tmp_new] = new_list->size() - 1;
				}
			}
		}
		level++;
	}


	return 0;
}



vector< pair<long int, long int> > HEM(long int l)			//For calulating HEM
{		
	vector< pair<long int, long int> > matching;			//Matching to be returned
	for(long int i = 1; i < graph[l].size(); i++)
	{
		if(graph[l][i][0].first == 0)
		{							//If the vertex is unmatched
			long int max = -1;
			long int k = 0;
			vector< pair< long int, long int > >::iterator it;
			it = graph[l][i].begin();
			advance(it, 1);
			long int v2 = it->first;
			for(; it != graph[l][i].end(); it++)//Check it condition
			{
				if(it->second > max && graph[l][it->first][0].first == 0 )
				{
					max = it->second;
					v2 = it->first;
				}
			}
			if(max > 0)
			{
				pair<long int, long int> edge;
				edge.first = i;
				edge.second = v2;
				matching.push_back(edge);
				graph[l][v2][0].first = 1;
			}
			graph[l][i][0].first = 1;
		}
	}
	return matching;
} 

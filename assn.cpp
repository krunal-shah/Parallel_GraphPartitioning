#include <iostream>
#include <string.h>
#include <math.h>
#include <vector>
#include <unordered_map>
#include <fstream>
#include <sstream>
#include <string>
using namespace std;

int main(int argv, char* args[])
{
	ifstream input_file;
	input_file.open ("graph.txt", ios::in);
	ofstream output_file;
	
	if( !input_file.is_open() )
	{
		cerr << "Error by opening the input_file\n";
		return -2;
	}
	long int n;
	vector< vector< pair< long int, long int> > > adj;
	input_file >> n;
	adj.resize(n+1);
	cout << n << endl;
	
	long int i = 1;
	string line;
	getline(input_file, line);
	while (getline(input_file, line)) 
	{
		istringstream row(line);
		long int num;
		long int j = 0;
		while (row >> j)
		{
			cout << j << " " << i << endl;
			adj[i].push_back(make_pair(j, 1));
		}
		i++;
	}


}
vector< pair<long int, long int> > HEM(int p, int l){		//For calulating HEM
	
	vector< pair<long int, long int> > matching;		//Matching to be returned
	
	for(int i = 0; i < vertices[l].size(); i++){
		if(graph[l][i][0] == p)
			graph[l][i][-1] = 0;								//Say that all vertices are unmatched
	}
	for(int i = 0; i < vertices[l].size(); i++){
		if(graph[l][i][-1] == 0){							//If the vertex is unmatched
			long int max = 0;
			long int k = 0;
			map< long int, long int> :: iterator = it;
			for(it = graph[l][i].begin() + 2; it != graph[l][i].end(); it++){	//Check it condition
				long int v2 = it->first;
				if(it->second > max && graph[l][v2][-1] == 0 && graph[l][v2][0] == p){
					max = it->second;
				}
			}
			if(max > 0){
				pair<long int, long int> edge;
				edge.first = i;
				edge.second = v2;
				matching.push_back(edge);
				graph[l][v2][-1] = 1;
			}
			graph[l][i][-1] = 1;
		}
	}
	
	return matching;
} 

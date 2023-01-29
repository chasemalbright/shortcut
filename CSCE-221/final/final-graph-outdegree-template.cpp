// Find a vertex with the heighest out-degree
//

#include <iostream>
#include <vector>

using namespace std;


void find_highest_outdegree(vector<vector<int>>& adj, int &hnode, int &count)
{
    // for out degrees, we wanto find which sublist contains the most edges
    // and record that vertex and the number of ones int the sublist
    
    count = 0;
    hnode = 0;
    int currcount = 0;
    for (int i = 0; i < adj.size(); i++){
        
        for (int j = 0; j < adj[i].size(); j++){
            if (adj[i][j]){
                currcount++; // count the number of out degree edges in each vertex
            }
        }
        if (currcount > count){
            hnode = i;
            count = currcount;
        }
        currcount = 0;
    }

}


int main()
{
  vector<vector<int>> adj = {
                {0,0,0,0,1},
			     {1,0,1,1,0},
			     {1,0,0,0,0},
			     {0,1,0,0,1},
			     {1,0,1,1,0}};

  int hnode, count;
  find_highest_outdegree(adj, hnode, count);
  cout << "vertex with highest out-degree: " << hnode << endl;
  cout << "highest out-degree: " << count << endl;

  return 0;
}
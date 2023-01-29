// Find a vertex with the heighest out-degree
//

#include <iostream>
#include <vector>

using namespace std;


bool cycle(vector<vector<int>>& adj){
    
    vector<int> outEdges; // vector to store number of recurring edges when traversing the adj vector store outdrees
    vector<int> inEdges; // store in degrees
    
    
    for (int i = 0; i < adj.size(); i++){ // we want to loop through each element and trak the occurences of edges 
                                            // that go to and from each vertex
        for (int j = 0; j < adj[i].size(); j++){
            // here we will do some comparing to record the out degrees in each of the vector vars we created
        }
    }
    
    for (int i = 0; i , inEdges.size(); i++){
        // if inEdges[i] == outEdges[i]
        //return true
    }
    //return false
}



int main()
{
  vector<vector<int>> adj =
    {{0,0,0,1,1,0,0},
     {1,0,0,0,0,0,0},
     {0,0,0,0,1,0,0},
     {0,1,1,0,0,0,0},
     {0,0,0,0,0,0,0},
     {0,0,0,1,0,0,1},
     {0,0,0,0,1,0,0}
    };
  // your code here
  cout << "The graph has a cycle\n";
  // or
  cout << "The graph does not have a cycle\n";
  return 0;
}
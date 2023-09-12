#include <iostream>
#include <limits.h> 
#include <queue> 
#include <vector> 
#include <string.h> 
using namespace std;

bool bfs(vector<vector<int>> rGraph, int s, int t, vector<int>& parent){
    const int V = rGraph.size();
    bool visited[V];
    memset(visited, 0, sizeof(visited)); 
    queue<int> q;
    q.push(s);
    visited[s] = true; 
    parent[s] = -1; 
    while (!q.empty()) { 
        int u = q.front(); 
        q.pop(); 
        for (int v = 0; v < V; v++) { 
            if (visited[v] == false && rGraph[u][v] > 0) { 
                if (v == t) { 
                    parent[v] = u; 
                    return true; 
                }
                q.push(v);
                parent[v] = u; 
                visited[v] = true; 
            }}}return false; 
}

int fordFulkerson(vector<vector<int>> graph, int s, int t){
    int u, v;
    const int V = graph.size(); 
    vector<vector<int>> rGraph(V , vector<int>(V , 0)); 
    for (u = 0; u < V ; u++)
        for (v = 0; v < V ; v++)
            rGraph[u][v] = graph[u][v]; 
    vector<int> parent(V); 
    int max_flow = 0; 
    while (bfs(rGraph, s, t, parent)) { 
        int path_flow = INT_MAX; 
        for (v = t; v != s; v = parent[v]) { 
            u = parent[v];
            path_flow = min(path_flow, rGraph[u][v]);
        }
        for (v = t; v != s; v = parent[v]) {
            u = parent[v];
            rGraph[u][v] -= path_flow;
            rGraph[v][u] += path_flow;
        }max_flow += path_flow;
    }cout<<endl;
    for(int i = 0;i<rGraph.size();i++){for(int j = 0;j<rGraph.size();j++){cout<<rGraph[i][j]<<" ";}cout<<endl;}cout<<endl;
    return max_flow; 
}
int main(){
    vector<vector<int>> graph = { 
                    {0, 10, 10, 4, 0, 0, 0, 0},
                    {0, 0, 7, 9, 6, 0, 5, 0},
                    {0, 0, 0, 0, 4, 7, 0, 0},
                    {0, 0, 0, 0, 0, 7, 0, 0},
                    {0, 0, 0, 0, 0, 6, 9, 0},
                    {0, 0, 0, 0, 0, 0, 0, 8},
                    {0, 0, 0, 0, 0, 0, 0, 10},
                    {0, 0, 0, 0, 0, 0, 0, 0}};
    cout << "The maximum possible flow is "
         << fordFulkerson(graph, 0, 7); 
    return 0;}
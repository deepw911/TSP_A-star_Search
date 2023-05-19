// Name: Deep Walke
// BT19CSE122

// Assumptions
// consider fully connected graph

#include <iostream>
#include <stdio.h>
#include <vector>
#include <set>
#include <queue>
#include <map>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <fstream>
using namespace std;

struct edge
{
    int a;
    int b;
    int w;
};

struct node
{
    int v;
    int cost; //g_value
    int f_value;
    set<int> visited_list;
    vector<int> path;
};

class myComparator
{
public:
    int operator()(node a, node b)
    {
        return a.f_value > b.f_value;
    }
};

// ---------------------------------------------

bool comp(edge a, edge b)
{
    return a.w < b.w;
}

int find(int a,vector<int> &parent)
{
    if (parent[a] == -1)
        return a;
    return parent[a] = find(parent[a],parent);
}

void merge(int a, int b,vector<int> &parent)
{
    parent[a] = b;
}

int getMST(vector<edge> v)
{
    vector<int> parent(10000, -1);
    int m = v.size();

    sort(v.begin(), v.begin() + m, comp);

    int sum = 0;
    for (int i = 0; i < m; i++)
    {
        int a = find(v[i].a,parent);
        int b = find(v[i].b,parent);

        if (a != b)
        {
            sum += v[i].w;
            merge(a, b,parent);
        }
    }
    return sum;
}

// ---------------------------------------------

// get edges for not visited nodes only
vector<edge> get_unvisited_edges(set<int> visited_list, int current, vector<vector<int>> adj_matrix)
{
    // visited_list.insert(current);
    vector<edge> unvisited;
    int n = adj_matrix.size();
    vector<bool> visited(n, false);
    
    for (auto i : visited_list)
    {
        visited[i] = true;
    }
    

    visited[current] = true;
    for (int i = 0; i < n; i++)
    {
        if (!visited[i])
        {
            for (int j = 0; j < i; j++)
            {
                if (!visited[j])
                    unvisited.push_back({i, j, adj_matrix[i][j]});
            }
        }
    }
    return unvisited;
}

// Implement A* search -> recursive
void A_search(vector<vector<int>> adj_matrix)
{
    priority_queue<node, vector<node>, myComparator> fringeList;
    set<int> visitedList;
    fringeList.push({0, 0, 0, visitedList, {}});
    int node_generated = 0;
    int node_expanded = 0;

    int n = adj_matrix.size();

    while (!fringeList.empty())
    {
        node_expanded++;
        node current_node = fringeList.top();
        fringeList.pop();
        cout << current_node.v << " " << current_node.cost << " f_value:" << current_node.f_value << endl;
        int current = current_node.v;
        current_node.path.push_back(current);
        current_node.visited_list.insert(current);

        if (current_node.visited_list.size() == n && current == 0)
        {
            cout << "\n";
            cout << "cost: " << current_node.cost << endl;

            // print the path
            cout << "Path: ";
            for (int i = 0; i < current_node.path.size(); i++)
            {
                cout << current_node.path[i] << "->";
            }
            cout << "\n";
            cout << "Number of nodes generated: " << node_generated << endl;
            cout << "Number of nodes expanded: " << node_expanded << endl;
            return;
        }
        else if (current_node.visited_list.size() == n)
        {
            node_generated++;
            current_node.v = 0;
            current_node.cost += adj_matrix[current][0];
            current_node.f_value = current_node.cost;
            fringeList.push(current_node);
        }
        else
        {
            vector<int> neighbours;
            for (int i = 0; i < n; i++)
            {
                if (current != i && current_node.visited_list.find(i) == current_node.visited_list.end())
                {
                    neighbours.push_back(i);
                }
            }
            int h_val, g_val;
            for (auto x : neighbours)
            {
                node_generated++;
                vector<edge> v = get_unvisited_edges(current_node.visited_list, x, adj_matrix);

                h_val = getMST(v);
                g_val = current_node.cost + adj_matrix[current][x];

                node new_node;
                new_node.v = x;
                new_node.cost = g_val;
                new_node.f_value = g_val + h_val;
                new_node.visited_list = current_node.visited_list;
                new_node.path = current_node.path;
                fringeList.push(new_node);
            }

        }
    }

    cout << "NO solution for given TSP\n";
}

// To check if given graph is fully connected
bool isFullyConnected(vector<vector<int>> adj_matrix)
{
    int n = adj_matrix.size();
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            if (adj_matrix[i][j] == 0 && i != j)
            {
                return false;
            }
        }
    }
    return true;
}

int main(int argc, char const *argv[])
{
    string input_file(argv[1]);
    fstream file(input_file);

    int n, a, b, w;
    file >> n;


    vector<vector<int>> adj_matrix(n, vector<int>(n, 0));
    while (file >> a >> b >> w)
    {
        adj_matrix[a - 1][b - 1] = w;
        adj_matrix[b - 1][a - 1] = w;
    }

    if (!isFullyConnected(adj_matrix))
    {
        cout << "Given graph is not fully connected\n";
        return 0;
    }
    
    A_search(adj_matrix);

    return 0;

    // Few examples

    // cost=80
    // path=0 1 3 2 0
    // adj_matrix={{0, 10, 15, 20},
    //             {10, 0, 35, 25},
    //             {15, 35, 0, 30},
    //             {20, 25, 30, 0}};

    // adj_matrix={{0, 2, 0, 6, 0},
    //             {2, 0, 3, 8, 5},
    //             {0, 3, 0, 0, 7},
    //             {6, 8, 0, 0, 9},
    //             {0, 5, 7, 9, 0}};

    // cost=35
    // path: 0 1 3 2 0
    // adj_matrix={{0, 10,15,	20},
    //             {5,	0,9,10},
    //             {6,13,0,12},
    //             {8,8,9,0}};

    // cout<<"ADJ MATRIX\n\n";
    // cout<<"{";
    // for(int i=0;i<n;i++){
    //     cout<<"{";
    //     for(int j=0;j<n;j++){
    //         cout<<adj_matrix[i][j];
    //         if(j!=n-1)
    //             cout<<",";
    //     }
    //     if(i!=n-1)
    //         cout<<"},";
    //     else
    //         cout<<"}";
    // }

    // for(int i=0;i<n;i++){
    //     for(int j=0;j<n;j++){
    //         cout<<adj_matrix[i][j]<<" ";
    //     }
    //     cout<<"\n";
    // }

    // cout<<"\n}\n\n";

    
}


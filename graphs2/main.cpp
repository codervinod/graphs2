//
//  main.cpp
//  graphs2
//
//  Created by Vinod Gupta on 11/4/15.
//  Copyright (c) 2015 vinodg. All rights reserved.
//

#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <stack>
using namespace std;

class Vertex
{
public:
    Vertex(int i):key(i) {}
    unordered_set<int> _neighbours;
    int key;
};

class Graph
{
public:
    Graph(int n)
    {
        for(int i=0;i<n;++i)
        {
            _graph[i] = new Vertex(i);
        }
    }
    
    void addEdge(int a,int b)
    {
        _graph[a]->_neighbours.insert(b);
        //_graph[b]->_neighbours.insert(a);
    }
    void visit(Vertex *v)
    {
        cout<<v->key<<endl;
    }
    void DFSUtil(Vertex *curr,unordered_set<int> &visited)
    {
        
        if(!curr)
        {
            return;
        }
        visited.insert(curr->key);
        visit(curr);
        for(auto nv:curr->_neighbours)
        {
            if(visited.find(nv) == visited.end())
            {
                DFSUtil(_graph[nv],visited);
            }
        }
        
    }
    void DFS()
    {
        unordered_set<int> visited;
        
        if(!_graph.empty())
            DFSUtil(_graph.begin()->second,visited);
    }
    
    void BFS()
    {
        unordered_set<int> visited;
        
        if(_graph.empty())
        {
            return;
        }
        queue<Vertex*> q;
        
        Vertex *curr = _graph.begin()->second;
        
        q.push(curr);
        
        while(!q.empty())
        {
            size_t qs = q.size();
            for(int i=0;i<qs;++i)
            {
                curr = q.front();
                q.pop();
                visit(curr);
                
                for(auto cn: curr->_neighbours)
                {
                    if(visited.find(cn) == visited.end())
                    {
                        visited.insert(cn);
                        q.push(_graph[cn]);
                    }
                }
            }
        }
        
    }
    
    bool isBipartiteUtil(Vertex *v,unordered_map<int,int> &color)
    {
        
        if(!v)
        {
            return true;
        }
        
        int expected_color = (color[v->key]==1)?0:1;
        
        for(auto vn:v->_neighbours)
        {
            Vertex *neighbour = _graph[vn];
            
                
            if(color.find(neighbour->key) != color.end())
            {
                if(color[neighbour->key] != expected_color)
                {
                    //cout<<neighbour->key<<" node already colored with"<<color[neighbour->key]<<" wanted "<<expected_color<<endl;
                    return false;
                }
            }
            else
            {
                color[neighbour->key] = expected_color;
                //cout<<"coloring node "<<neighbour->key<<" with "<<color[neighbour->key]<<endl;
                if(!isBipartiteUtil(neighbour,color))
                {
                    return false;
                }
            }
        }
        return true;
        
    }
    
    bool isBipartite()
    {
        unordered_map<int,int> color;
        if(_graph.empty())
        {
            return false;
        }
        Vertex *curr = _graph.begin()->second;
        color[curr->key] = 0;
        //cout<<"coloring node "<<curr->key<<" with "<<0<<endl;
        return isBipartiteUtil(curr,color);
    }
    
    bool hasCycleUtil(int parent,Vertex *curr,unordered_set<int> &visited,
                    unordered_set<int> &recurStack)
    {
        visited.insert(curr->key);
        recurStack.insert(curr->key);
        
        cout<<"visiting "<<curr->key<<endl;
        
        for(auto cn:curr->_neighbours)
        {
            if(cn==parent)
                continue;
            
            if(recurStack.find(cn) != recurStack.end())
            {
                cout<<"already visited "<<cn<<endl;
                return true;
            }
            
            if(visited.find(cn) == visited.end())
            {
                Vertex *neighbour = _graph[cn];
                
                if(hasCycleUtil(curr->key,neighbour,visited,recurStack))
                {
                    return true;
                }
            }
        }
        
        recurStack.erase(curr->key);
        cout<<"removing from stack"<<curr->key<<endl;
        return false;
    }
    
    bool hasCycle()
    {
        if(_graph.empty())
        {
            return false;
        }
        
        unordered_set<int> visited,recurStack;
        
        Vertex *curr = _graph.begin()->second;
        return hasCycleUtil(-1,curr,visited,recurStack);
    }
    
    void topologySortUtil(Vertex *curr,unordered_set<int>& visited,stack<int> &topoStack)
    {
        cout<<"visiting"<<curr->key<<endl;
        visited.insert(curr->key);
        for(auto cn:curr->_neighbours)
        {
            if(visited.find(cn) == visited.end())
            {
                Vertex *neighbour = _graph[cn];
                topologySortUtil(neighbour,visited,topoStack);
            }
        }
        
        topoStack.push(curr->key);
    }
    
    vector<int> topologySort()
    {
        vector<int> result;
        
        
        if(_graph.empty())
        {
            return result;
        }
        
        stack<int> topoStack;
        unordered_set<int> visited;
        for(auto &gn: _graph)
        {
            Vertex *curr = gn.second;
            
            if(visited.find(gn.first) == visited.end())
            {
                topologySortUtil(curr,visited,topoStack);
            }
        }
        
        while(!topoStack.empty())
        {
            result.push_back(topoStack.top());
            topoStack.pop();
        }
        return result;
    }
private:
    unordered_map<int, Vertex*> _graph;
};

int main(int argc, const char * argv[]) {
    Graph g(6);
    
    g.addEdge(5,2);
    g.addEdge(5,0);
    g.addEdge(4,0);
    g.addEdge(4,1);
    g.addEdge(2,3);
    g.addEdge(3,1);
    
    /*
    g.DFS();
    bool isb = g.isBipartite();
    cout<<"Is graph bipartite"<<(isb?" yes": " no")<<endl;
    
    bool hc=g.hasCycle();
    cout<<"Graph has cycle? "<<(hc?"yes":"no")<<endl;
    */
    
    vector<int> topo = g.topologySort();
    for(auto i:topo)
    {
        cout<<i<<endl;
    }
    
    return 0;
}

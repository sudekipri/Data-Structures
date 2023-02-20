#pragma once
#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <map>
#include <string>

using namespace std;

struct Node {
    string word;
    vector<Node*> adj;  //list of adjacent nodes

    Node(string _word) : word(_word) {};
};

class Graph {

private:
    unordered_map<string, Node*> nodes;

    bool oneCharAway(string s1, string s2) {
        if (s1 == s2) return false;
        
        int l1 = s1.length();
        int l2 = s2.length();

        if (abs(l1 - l2) > 1) return false;

        int i = 0, j = 0, diff = 0;

        while (i < l1 && j < l2) {
            char f = s1[i];
            char s = s2[j];

            if (f != s) {
                diff++;
                if (l1 > l2) i++;
                if (l2 > l1) j++;
                if (l1 == l2) { i++; j++;}
            }
            else {
                i++; j++;
            }
            if (diff > 1) return false;
        }

        return !((diff == 1 && l1 != l2 && (i != l1 || j != l2)));
    }
   
    //BFS
    bool path_exists(string w1, string w2, map<string, string> &pred, map<string, int> &dist) {

        // Base case
        if (w1 == w2)
            return true;

        int n = (int)nodes.size();

        unordered_map<string, bool> visited(false);
        queue<string> q;

        visited[w1] = true;
        dist[w1] = 0;
        q.push(w1);

        while (!q.empty())
        {
            w1 = q.front();
            q.pop();

            // Get all adjacent vertices of the dequeued vertex s
            // If a adjacent has not been visited, then mark it
            // visited  and enqueue it       
            for (auto x : nodes[w1]->adj)
            {      
                if (!visited[x->word])
                {
                    visited[x->word] = true;
                    dist[x->word] = dist[w1] + 1;
                    pred[x->word] = w1;
                    q.push(x->word);

                    // If this adjacent node is the destination node,
                    // then return true
                    if (x->word == w2)
                        return true;
                }
            }
        }

        return false;
    }

    string difference(string s1, string s2) {
        string action = "";

        if (s1 == s2) return action;

        int l1 = s1.length();
        int l2 = s2.length();

        if (abs(l1 - l2) > 1) return action;

        int i = 0, j = 0, diff = 0;

        while (i < l1 && j < l2) {
            char f = s1[i];
            char s = s2[j];

            if (f != s) {
                diff++;
                if (l1 > l2) {
                    action += " (remove ";
                    action += f;
                    action += " at position ";
                    action += to_string(i);
                    action += ")";
                    i++;
                }
                if (l2 > l1) {

                    action += " (insert ";
                    action += s;
                    action += " after position ";
                    action += to_string(i);
                    action += ")";

                    j++;
                }
                if (l1 == l2) { 

                    i++; j++; 

                    action += " (change ";
                    action += f;
                    action += " at position ";
                    action += to_string(i);
                    action += " to ";
                    action += s;
                    action += ")";
                }
            }
            else {
                i++; j++;
            }
            if (diff > 1) return "";
        }

        if (diff == 0) {
            if (l1 > l2) {
                action += " (remove ";
                action += s1[l1-1];
                action += " at position ";
                action += to_string(l2);
                action += ")";
            }
            else if(l1 < l2) {
                action += " (insert ";
                action += s2[l2-1];
                action += " after position ";
                action += to_string(l1);
                action += ")";
            }
        }

        return action;
    }

public:

    Graph() {};
    ~Graph() {};

    bool exists(string word) {
        return nodes.find(word) != nodes.end();
    }

    void insert(string word) {
        if (!exists(word)) {
            nodes[word] = new Node(word);
        }

        for (auto node : nodes) {
            if (oneCharAway(node.first, word)) {
                node.second->adj.push_back(nodes[word]);
                nodes[word]->adj.push_back(node.second);
            }
        }
    }

    void print(string w) {
        if (!exists(w)) return;

        for (auto n : nodes[w]->adj) {
            cout << n->word << " ";
        }

        cout << endl;
    }

    void transform(string w1, string w2) {
        
        map<string, string> pred;
        map<string, int> dist;

        if (!path_exists(w1, w2, pred, dist)) {
            cerr << "Could not find a suitable path from " << w1 << " to " << w2 << endl;
        }

        vector<string> path;
        string crawl = w2;
        path.push_back(crawl);

        while (exists(pred[crawl])) {
            path.push_back(pred[crawl]);
            crawl = pred[crawl];
        }

        cout << endl;

        for (int i = path.size() - 1; i >= 0; i--)
            cout << path[i] << (i == path.size() - 1 ? "" : difference(path[i + 1], path[i])) << "\n";
    
        cout << endl;
    }

};


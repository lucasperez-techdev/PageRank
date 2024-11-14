#pragma once

#include <iostream>
#include <unordered_map>
#include <vector>
#include <iomanip>
#include <string>

using namespace std;

class AdjacencyList
{
private:
    unordered_map<int, vector<int>> graph; // To store the adjacency list
    unordered_map<int, string> id_to_url;  // Maps IDs to URLs
    unordered_map<string, int> url_to_id;  // Maps IDs to URLs
    unordered_map<int, double> ranks;      // To store the rank of each webpage
    unordered_map<int, int> out_degrees;   // To store the out-degree of each node
    int next_id;                           // Counter for unique IDs

    void initializeRanks();                  // Initializes the ranks of each node
    void computeRanks(int power_iterations); // Performs power iterations to compute ranks
    int getOrCreateID(string url);           // Retrieves or assigns a unique ID for a URL

public:
    AdjacencyList();                       // Constructor
    void addEdge(string from, string to);  // Adds an edge to the graph
    string PageRank(int power_iterations); // Calculates PageRank after power_iterations
    string displayRanks();                 // Displays the ranks of all pages
    string parseInput(string input);       // Specifically for catch testing
    void printGraph();                     // Debugging purposes
};

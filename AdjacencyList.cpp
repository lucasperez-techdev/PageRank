#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <sstream>
#include <unordered_map>
#include "AdjacencyList.h"

using namespace std;

// prints the PageRank of all pages after p powerIterations in ascending
// alphabetical order of webpages and rounding rank to two decimal places
string AdjacencyList::PageRank(int power_iterations)
{
    initializeRanks();

    // Computes ranks through the passed in number of power iterations
    computeRanks(power_iterations);

    // printGraph();

    // Generates and returns a string of ranks in alphabetical order of URLs
    return displayRanks();
}

// Constructor that ensures that all maps are empty
AdjacencyList::AdjacencyList()
{
    next_id = 1;
    graph.clear();
    id_to_url.clear();
    url_to_id.clear();
    ranks.clear();
    out_degrees.clear();
}

// Initializes ranks to 1 / num_pages for future calculations
void AdjacencyList::initializeRanks()
{
    int num_pages = url_to_id.size();
    if (num_pages == 0)
        return;

    double initial_rank = 1.0 / num_pages;
    for (auto entry : id_to_url)
    {
        int id = entry.first;
        ranks[id] = initial_rank;
    }
}

void AdjacencyList::computeRanks(int power_iterations)
{
    // Makes sure that number of pages isnt 0
    int num_pages = url_to_id.size();
    if (num_pages == 0)
        return;

    unordered_map<int, double> temp_ranks;

    for (int iter = 0; iter < power_iterations - 1; iter++)
    {
        // Resets temp_ranks for the current iteration
        for (auto entry : ranks)
        {
            int id = entry.first;
            temp_ranks[id] = 0.0;
        }

        // Distributes ranks to neighbors
        for (auto entry : ranks)
        {
            int from_id = entry.first;
            double current_rank = entry.second;

            if (out_degrees[from_id] > 0)
            {
                double rank_share = current_rank / out_degrees[from_id];
                for (int to_id : graph[from_id])
                {
                    temp_ranks[to_id] += rank_share;
                }
            }
        }

        // Updates ranks with temp_ranks for the next power iteration
        for (auto entry : temp_ranks)
        {
            int id = entry.first;
            ranks[id] = entry.second;
        }

        // Debug: Calculates total rank after each iteration
        /*
        double total_rank = 0.0;
        for (auto entry : ranks)
        {
            total_rank += entry.second;
        }
        cout << "Iteration " << iter + 1 << " Total Rank: " << total_rank << endl;

        // Debug: Prints ranks after each iteration
        cout << "After iteration " << iter + 1 << ":\n";
        for (auto entry : ranks)
        {
            cout << id_to_url[entry.first] << ": " << entry.second << "\n";
        }
        cout << "\n";
        */
    }
}

int AdjacencyList::getOrCreateID(string url)
{
    // Checks if URL already has an ID
    if (url_to_id.find(url) == url_to_id.end())
    {
        // Assigns new ID
        url_to_id[url] = next_id;
        id_to_url[next_id] = url;
        next_id++;
    }
    return url_to_id[url];
}

void AdjacencyList::addEdge(string from, string to)
{
    // Gets or create unique IDs for the from and to URLs
    int from_id = getOrCreateID(from);
    int to_id = getOrCreateID(to);

    // Adds the to page to the adjacency list of the from page
    graph[from_id].push_back(to_id);

    // Updates the out-degree of the from page
    out_degrees[from_id]++;
}
string AdjacencyList::displayRanks()
{
    // Creates a vector of pairs to hold URL and its corresponding rank
    vector<pair<string, double>> url_ranks;

    // Populates the vector with URLs and their ranks
    for (auto entry : id_to_url)
    {
        int id = entry.first;
        string url = entry.second;
        double rank = ranks.at(id);
        url_ranks.emplace_back(url, rank);
    }

    // Sorts the vector by URL in ascending alphabetical order
    sort(url_ranks.begin(), url_ranks.end());

    // Prints each URL and its rank, rounded to two decimal places and also stores into a stringstream for returning purposes
    ostringstream oss;
    for (auto url_rank : url_ranks)
    {
        cout << url_rank.first << " " << fixed << setprecision(2) << url_rank.second << "\n";
        oss << url_rank.first << " " << fixed << setprecision(2) << url_rank.second << "\n";
    }
    return oss.str();
}

// For Catch testing purposes
string AdjacencyList::parseInput(string input)
{
    istringstream iss(input);
    int num_edges, power_iterations;

    // Read the number of edges and power iterations
    iss >> num_edges >> power_iterations;

    // Read each edge and add it to the graph
    string from, to;
    for (int i = 0; i < num_edges; i++)
    {
        iss >> from >> to;
        addEdge(from, to);
    }

    return PageRank(power_iterations);
}

// For debugging purposes making sure everything was associated correctly
void AdjacencyList::printGraph()
{
    for (auto entry : graph)
    {
        int from_id = entry.first;
        cout << "From " << id_to_url[from_id] << " to: ";
        for (int to_id : entry.second)
        {
            cout << id_to_url[to_id] << " ";
        }
        cout << endl;
    }
}
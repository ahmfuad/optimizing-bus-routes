#include<bits/stdc++.h>
using namespace std;


struct Edge {
    int to;
    double distance;
    double fare;
};

struct Node {
    int id;
    double distance;  // Total distance traveled so far
    double population_sum;  // Sum of populations of nodes visited so far
    int nodes_visited;  // Number of nodes visited so far

    // Custom comparison to ensure max population, min distance, and min fare
    bool operator<(const Node& other) const {
      // We want to minimize distance, so use > for distance comparison
      if (distance != other.distance) {
          return distance > other.distance;  // Minimize distance
      }
      // We want to maximize the average population
      double avg_population = population_sum / nodes_visited;
      double other_avg_population = other.population_sum / other.nodes_visited;
      if (avg_population != other_avg_population) {
          return avg_population < other_avg_population;  // Maximize population
      }
      // Fare will be calculated dynamically using the edges in the Dijkstra process.
      return false;  // If all else is equal, consider them the same for now
    }
};


// Graph adjacency list and population data
unordered_map<int, vector<Edge>> graph;
unordered_map<int, int> population;


map<string, int> number_map; // mapped stopage names with number;
map<int, string> area_map;

void preprocess() {

  ifstream file;
  ofstream out;
  
  file.open("all_stops_duplicate.txt");

  string s;
  int i = 1;
  while(getline(file, s)) {
    number_map[s] = i++;
  }

  file.close();
  file.clear();
  
  file.open("all_edges_with_distance.csv");
  out.open("all_edges_number_with_distance.txt");

  while(getline(file, s)) {
    
    string temp = "";
    bool found = 0;
    
    for(auto i : s) {
      if(i == ',') {
        found = 1;
        out << number_map[temp];
        if(number_map[temp] ==0) {
          out << temp;
        }
        out << " ";
        temp = "";
      }
      else {
        temp += i;
      }
    }

    out << temp << endl;
  }

  for(auto i: number_map) {
    area_map[i.second] = i.first;
  }
  out.close();
}

int s_to_i(string s) {

  int i = 0;
 
    // Traversing string
  for (char c : s) {
      // Checking if the element is number
      if (c >= '0' && c <= '9') {
          i = i * 10 + (c - '0');
      }
  }

  return i;
}

void get_population() {

  ifstream file("unique_nodes_with_population.csv");
  string s;
  int v;
  //cout << "HERE" << endl;
  while(getline(file, s)) {
    
    bool found = 0;
    string temp1 = "", temp2 = "";
    for(auto i: s) {
      if(i == ',') {
        found = 1;
      }
      else {
        if(found) {
          temp2 += i;
        }
        else {
          temp1 += i;
        }
      }
    }
    //sscanf(temp2, "%d", v);
    population[number_map[temp1]] = s_to_i(temp2);
  }
}

vector<int> Graph[1001];
double distances[1001][1001];

void addEdge(int from, int to, double distance) {

  Graph[from].push_back(to);
  Graph[to].push_back(from);  // assuming undirected graph
  distances[from][to] = distance;
  distances[to][from] = distance;
}


void generate_graph() {
  //cout << "THIS" << endl;
  ifstream file;
  file.open("all_edges_number_with_distance.txt");
  
  string line;
  int u;
  double dist;

  //stringstream ss;


  vector<pair<int, double>> temp;

  if(!file.is_open()) {
    //cout << "NOt" << endl;
    return;
  }

  while(getline(file, line)) {
    stringstream ss;
    ss<<line;
    ss>>u>>dist;
    
    //cout << u << " " << dist << endl;
    //cout << u << " " << dist << endl;
    if(dist==0) {
      temp.clear();
      temp.push_back({u, dist});
    }
    else {
      for(int i=0; i < temp.size(); i++) {
        addEdge(temp[i].first, u, dist - temp[i].second);
      }
      temp.push_back({u, dist});
    } 
  }

  file.close();

}


void show_graph() {

  ofstream out;
  out.open("graph_output.txt");
  
  for (int i=0; i<1001; i++) {
    if(Graph[i].size()==0) continue;
    int from_node = i;
    out << "Node " << from_node << " connects to:\n";
    for(auto j: Graph[i]) {
      out << "-> Node: " << j << " | Distance: " << distances[from_node][j] << endl;
    }

    // const vector<Edge>& edges = node.second;
    
    // out << "Node " << from_node << " connects to:\n";
    // for (const Edge& edge : edges) {
    //   out << "  -> Node " << edge.to << " | Distance: " << edge.distance 
    //        << " | Fare: " << edge.fare << endl;
    // }
    out << endl;
  }

}

// Function to calculate the composite score (taking average population into account)
double calculate_composite_score(double distance, double avg_population, double fare) {
    double w1 = 1.0;
    double w2 = 2.0; 
    double w3 = 3.0; 

    return (w1 * avg_population) - (w2 * distance) - (w3 * fare) / (w1 + w2 + w3);
}

// // Modified Dijkstra's algorithm to consider population, distance, and fare
// double dijkstra(int start, int target) {
//     priority_queue<Node> pq;
//     vector<double> dist(graph.size(), numeric_limits<double>::infinity());
    
//     // Start from the source node
//     pq.push({start, 0.0, population[start], 1});
//     dist[start] = 0.0;

//     while (!pq.empty()) {
//         Node current = pq.top();
//         pq.pop();

        
//         if (current.id == target) {
//             double avg_population = current.population_sum / current.nodes_visited;
//             return calculate_composite_score(current.distance, avg_population, current.distance * 2.45);  // fare = 2.45 * distance
//         }


//         for (const Edge& edge : graph[current.id]) {
//             double new_dist = current.distance + edge.distance;
//             double new_fare = new_dist * 2.45;  // Fare calculated as 2.45 * distance
//             double new_population_sum = current.population_sum + population[edge.to];
//             int new_nodes_visited = current.nodes_visited + 1;

//             // If a shorter path is found
//             if (new_dist < dist[edge.to]) {
//                 dist[edge.to] = new_dist;
//                 pq.push({edge.to, new_dist, new_population_sum, new_nodes_visited});
//             }
//         }
//     }

    
//     return numeric_limits<double>::infinity();
// }


// double dijkstra_with_path(int start, int target, vector<int>& path) {

//     cout << start << " " << target << endl;
//     priority_queue<Node> pq;
//     vector<double> dist(graph.size(), numeric_limits<double>::infinity());
//     vector<int> prev(graph.size(), -1);  // To store the path

//     // Start from the source node
//     pq.push({start, 0.0, population[start], 1});
//     dist[start] = 0.0;

//     while (!pq.empty()) {

//         Node current = pq.top();
        
//         cout << current.
//         pq.pop();

//         // If we reached the target node, reconstruct the path
//         if (current.id == target) {
//             path.clear();
//             for (int at = target; at != -1; at = prev[at]) {
//                 path.push_back(at);
//             }
//             reverse(path.begin(), path.end());  // Reverse to get the path from start to target

//             double avg_population = current.population_sum / current.nodes_visited;
//             double fare = current.distance * 2.45;
//             return calculate_composite_score(current.distance, avg_population, fare);
//         }

//         // Explore neighbors
//         for (const Edge& edge : graph[current.id]) {
//             double new_dist = current.distance + edge.distance;
//             double new_fare = new_dist * 2.45;  // Fare calculated as 2.45 * distance
//             double new_population_sum = current.population_sum + population[edge.to];
//             int new_nodes_visited = current.nodes_visited + 1;

//             // If a shorter path is found
//             if (new_dist < dist[edge.to]) {
//                 dist[edge.to] = new_dist;
//                 prev[edge.to] = current.id;  // Store predecessor
//                 pq.push({edge.to, new_dist, new_population_sum, new_nodes_visited});
//             }
//         }
//     }

//     // If no path is found, return a large number to indicate failure
//     return numeric_limits<double>::infinity();
// }



// Function to calculate the composite score
// double calculate_composite_score(double distance, double avg_population, double fare) {
//     return (1 * avg_population - 2 * distance - 3 * fare) / (1 + 2 + 3);
// }

// DFS function to explore all paths and calculate the composite score
void dfs(int node, int target, 
         vector<int>& current_path, set<int>& visited, double current_distance, 
         double current_population_sum, int nodes_visited, double& max_score, vector<int>& best_path) {

    // If we reached the target node, compute the composite score
    if (node == target) {
        double avg_population = current_population_sum / nodes_visited;
        double fare = current_distance * 2.45;  // Fare = 2.45 * total distance
        double score = calculate_composite_score(current_distance, avg_population, fare);

        // Update the best path if we found a higher score
        if (score > max_score) {
            max_score = score;
            best_path = current_path;
        }
        return;
    }

    // Mark the node as visited
    visited.insert(node);

    // Explore neighbors
    for (const Edge& edge : graph[node]) {
        if (visited.find(edge.to) == visited.end()) {  // If not visited
            current_path.push_back(edge.to);
            dfs(edge.to, target, current_path, visited, 
                current_distance + edge.distance, current_population_sum + population[edge.to], 
                nodes_visited + 1, max_score, best_path);
            current_path.pop_back();
        }
    }

    // Backtrack, remove the node from the visited set
    visited.erase(node);
}

// Brute force function to find the best path based on the composite score
double brute_force_max_composite_score(int start, int target, vector<int>& best_path) {
    vector<int> current_path = {start};
    set<int> visited;
    double max_score = -numeric_limits<double>::infinity();  // Initialize with a very low score

    // Start DFS from the start node
    dfs(start, target, current_path, visited, 0.0, population[start], 1, max_score, best_path);

    return max_score;
}


int vis[1001];
vector<pair<int, vector<int>>> allPath;
void dfs(int u, int target, vector<int> path, double population_sum, double current_distance) {
  //cout << area_map[u] << " ";
  if(vis[u]) {
    return;
  }

  if(u==target) {
    path.push_back(target);
    double avg_population = (population_sum) / path.size();
    double fare = (current_distance) * 2.45;  // Fare = 2.45 * total distance
    //current_distance += distances[last][target];
    double score = calculate_composite_score(current_distance, avg_population, fare);
    allPath.push_back({score, path});
    cout << endl;
    return;
    
  }

  vis[u] = 1;
  path.push_back(u);
  double avg_population = population_sum / path.size();
  double fare = (current_distance) * 2.45;
  double score = calculate_composite_score(current_distance, avg_population, fare);
  
  for(auto i: Graph[u]) {
    //cout << area_map[i] << " " << area_map[u] << endl;
    if(!vis[i]) dfs(i, target, path, population_sum+population[i], current_distance+distances[u][i]);
  }

  //vis[u] = 0;
  return;
}




int main() {

  preprocess();
  get_population();
  generate_graph();
  show_graph();
  //freopen("graph_output.txt", "w", stdout);
  

   // Store the path
  vector<int> path;
  freopen("needed_output.txt", "w", stdout);
  
  //sort(Graph[number_map["Chondra"]].begin(), Graph[number_map["Chondra"]].begin());
  cout << number_map["Sciencelab"] << endl;
  dfs(number_map["Ghatarchor"], number_map["Kanchpur"], path, population[number_map["Ghatarchor"]], 0);
  //cout << endl;
  //cout << allPath.size() << endl;


  // for(auto i: Graph[number_map["Chondra"]]) {
  //   cout << area_map[i] << " ";
  // }
  // cout << endl;
  // for(auto i: Graph[number_map["Nondon Park"]]) {
  //   cout << area_map[i] << " ";
  // }
  // cout << endl;

  for(auto i: allPath) {
    cout << "Score: " << i.first << endl;
    cout << "Route -> ";
    for(auto j: i.second) {
      for(auto k: number_map) {
        if(k.second == j) {
          cout << k.first << " -> ";
        }
      }
    }
    cout << endl;
  }

  return 0;

}
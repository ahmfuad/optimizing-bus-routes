#include<bits/stdc++.h>
using namespace std;


struct Edge {
    int a, b;
    double cost;
};


// int n, m, v;
vector<Edge> edges;
const int INF = 1000000000;

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

  //Graph[from].push_back(to);
  //Graph[to].push_back(from);  // assuming undirected graph
  distances[from][to] = distance;
  distances[to][from] = distance;
  edges.push_back({from, to, distance});
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
  out.close();
}


//const int INF = 1000000000;
vector<pair<int, double>> adj[201];


void addEdge_2(int from, int to, double distance) {

  //Graph[from].push_back(to);
  //Graph[to].push_back(from);  // assuming undirected graph
  distances[from][to] = distance;
  distances[to][from] = distance;
  adj[from].push_back({to, distance});
  //edges.push_back({from, to, distance});
}


void generate_graph_2() {
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

void dijkstra(int s, vector<int> & d, vector<int> & p) {
    int n = 500;
    d.assign(n, INF);
    p.assign(n, -1);
    vector<bool> u(n, false);

    d[s] = 0;
    for (int i = 0; i < n; i++) {
        int v = -1;
        for (int j = 0; j < n; j++) {
            if (!u[j] && (v == -1 || d[j] < d[v]))
                v = j;
        }

        if (d[v] == INF)
            break;

        u[v] = true;
        //cout << v << endl;
        for (auto edge : adj[v]) {
            int to = edge.first;
            int len = edge.second;

            if (d[v] + len < d[to]) {
                d[to] = d[v] + len;
                p[to] = v;
            }
        }
    }
}



int main() {

    preprocess();
    get_population();
    generate_graph_2();
    show_graph();
    cout << "EIKHANE" << endl;
    int s = number_map["Ghatarchor"], t = number_map["Kanchpur"];

    vector<int> d;
    //d[v] = 0;
    vector<int> p;

    dijkstra(s, d, p);
    cout << "EIKHANE" << endl;
    // //cout << "EIKHANE" << endl;
    // for (;;) {
    //     bool any = false;
    //     for (Edge e : edges)
    //         if (d[e.a] < INF)
    //             if (d[e.b] > d[e.a] + e.cost) {
    //                 d[e.b] = d[e.a] + e.cost;
    //                 p[e.b] = e.a;
    //                 any = true;
    //             }
    //     if (!any)
    //         break;
    // }
    // //cout << "EIKHANE" << endl;
    // if (d[t] == INF)
    //     cout << "No path from " << area_map[v] << " to " << area_map[t] << ".";
    // else {
    //     vector<int> path;
    //     for (int cur = t; cur != -1; cur = p[cur])
    //         path.push_back(cur);
    //     reverse(path.begin(), path.end());

    //     cout << "Path from " << area_map[v] << " to " << area_map[t] << ": ";
    //     for (int u : path)
    //         cout << area_map[u] << ' ';
    // }

    vector<int> path;
    if(p[t]==-1) {
      cout << "NO PATH" << endl;
      return 0;
    }
    for (int v = t; v != s; v = p[v]){
      cout << p[v] << endl;
      path.push_back(v);
    }
        
    path.push_back(s);
    cout << "EIKHANE" << endl;
    reverse(path.begin(), path.end());


    cout << "Path from " << area_map[s] << " to " << area_map[t] << ": " << endl; 
    for(auto i: path) {
      cout << area_map[i] << " ";
    }
    cout << endl;
}


#include <bits/stdc++.h>
using namespace std;

int main() {
  
  freopen("all-bus-stops.txt", "r", stdin);
  freopen("unique-bus-stops.txt", "w", stdout);
  
  string s;
  
  map<string, int> bus_stop_occurence;

  while(!feof(stdin)) {
     getline(cin, s);
     //cin.ignore();
     bus_stop_occurence[s]++; 
  }

  for(auto i: bus_stop_occurence ){
    cout << i.first << endl;
  }


}
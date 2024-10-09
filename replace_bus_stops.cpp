#include<bits/stdc++.h>
using namespace std;


int main() {
  //FILE *fp=freopen("data.txt","r",stdin);
  //FILE * fp = freopen("all-bus-stops.txt", "r", stdin);
  freopen("to_replace.txt", "r", stdin);
  freopen("to_replace_output.txt", "w", stdout);


  std::vector<string> v;
  string s;
  int j=0, n; cin >> n;

  while(j++ <= n) {
     getline(cin, s);
        
     v.push_back(s);
     //bus_stop_occurence[s]++; 
  }
  cin >> n;
  j=0;
  
  while(j++ <= n) {
     getline(cin, s);
     //cout << s << endl;
     string temp="", temp2 = "";
     bool found = 0;

     for(auto i: s) {
      if(i==',') {
        found = 1;
      }
      else {
        if(found) {
          temp2 += i;
        }
        else {
          temp += i;
        }
      }
     }
     
     for(int i=0; i<v.size(); i++) {
      if(v[i]==temp) {
        v[i] = temp2;
      }
     }
     //bus_stop_occurence[s]++; 
  }


  

  for(auto i: v) {
    cout << i << endl;
  }



}
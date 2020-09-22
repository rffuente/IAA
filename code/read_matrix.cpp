#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
 
using namespace std;

vector<vector<int> > read_matrix(string route)
{
    ifstream in(route);
    vector<vector<int> > v;
    
    if (in) {
        string line;
        
        while (getline(in, line)) {
            v.push_back(vector<int>());
            
            // Break down the row into column values
            stringstream split(line);
            int value;
            
            while (split >> value)
                v.back().push_back(value);
        }
    }

    return v;
};
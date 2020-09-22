#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include "read_matrix.h"
#include "functions.h"

using namespace std;

int main()
{
    //all teams 
    string all_teams_names = "ATL NYM PHI MON FLA PIT CIN CHI STL MIL HOU COL SF SD LA ARI";
    //name of data
    string route = "data/data4.txt";

    vector<vector<int> > D;
    vector<string> teams;
    vector<int> teams_index;
    //Read the data
    D = read_matrix(route);

    //select the first N teams, where N is the number of data file (dataN.txt)
    vector<string> aux = split(route,"data");
    vector<string> aux2 = split(aux[2],".");
    vector<string> aux3 = split(all_teams_names," ");
    teams = slice(aux3, 0, stoi(aux2[0]));
    //team index list
    for (int i = 0; i < D.size(); i++) {
        teams_index.push_back(i);
    }

    //Inputs
    //int L = 1;
    //int U = 3;
    int n = stoi(aux2[0]);
    int k = 0;
    //int n = 12;

    int RR = n;
    int CC = 2*n-2+k;
    vector<vector<int> > S_tr(RR);
    for ( int i = 0 ; i < RR ; i++ )
    {
        S_tr[i].resize(CC);
    }

    //Values for ACO constants
	//int iterations = 5;

	//int ants = n;

	// if (ALPHA == 0) { stochastic search & sub-optimal route }
	double alpha = 0.5;
	// if (BETA  == 0) { sub-optimal route }
	double beta = 0.8;
	// Estimation of the suspected best route.
	double Q = 100;
	// Pheromones evaporation. 
	//double ro = 0.2;
	// Maximum pheromone random number.
	//int taumax = 2;

    //cout << "C1: " << verifyC1(S_tr, n, k) << "\n";
    //cout << "C2: " << verifyC2(S_tr, n, k) << "\n";
   	//cout << "C3: " << verifyC3(S_tr, n, k) << "\n";
    //cout << "C4: " << verifyC4(S_tr, n, 3, k) << "\n";
    //cout << "C5: " << verifyC5(S_tr, n, 1, k) << "\n";
    //cout << "C6: " << verifyC6(S_tr, n, k) << "\n";

    //cout << "Obj: " << objetive_function(D, S_tr, n, k) << "\n";
    vector<vector<double> > visibility;
    vector<vector<vector<double> > >pheromones;
    visibility = create_visibility(n, Q, D);
    pheromones = create_pheromones(n, k);
    //print_visibility(visibility, n);
    cout << "------------------------------------------------------------------------------------" << endl;
    cout << "------------------------------------------------------------------------------------" << endl;
    cout << "------------------------------------------------------------------------------------" << endl;
	AFC(k, n, visibility, pheromones, alpha, beta, D);
}
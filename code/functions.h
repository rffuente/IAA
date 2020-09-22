#ifndef functions_h
#define functions_h

using namespace std;

//This function split a string with a token given.
vector<int> remove_element(vector<int> vec, int element);
vector<string> split(string str, string token);
vector<string> slice(vector<string> v, int n, int m);

int psi(int g, int t);
int omega(int g);
bool C1(vector<vector<int> > S_tr, int t, int r);
bool C2(vector<vector<int> > S_tr, int t, int n, int t_p, int k);
bool C3(vector<vector<int> > S_tr, int t, int n, int k);
bool C4(vector<vector<int> > S_tr, int t, int r, int u);
bool C5(vector<vector<int> > S_tr, int t, int r, int l);
bool C6(vector<vector<int> > S_tr, int t, int r);

int verifyC1(vector<vector<int> > S_tr, int n, int k);
int verifyC2(vector<vector<int> > S_tr, int n, int k);
int verifyC3(vector<vector<int> > S_tr, int n, int k);
int verifyC4(vector<vector<int> > S_tr, int n, int u, int k);
int verifyC5(vector<vector<int> > S_tr, int n, int l, int k);
int verifyC6(vector<vector<int> > S_tr, int n, int k);

//Objective function
int fi(vector<vector<int> > D, vector<vector<int> > S_tr, int t, int r, int n, int k);
int gamma(vector<vector<int> > D, vector<vector<int> > S_tr, int t, int n, int k);
int objetive_function(vector<vector<int> > D, vector<vector<int> > S_tr, int n, int k);

//Movement
double ITA(vector<vector<int> > D, double Q, int i, int j, int n);
vector<vector<double> > create_visibility(int n, double Q, vector<vector<int> > D);
vector<vector<vector<double> > >create_pheromones(int n, int k);
void print_visibility(vector<vector<double> > visibility, int n);
void print_pheromone(vector<vector<vector<double> > >pheromones, int n, int k);
int Pij(int t, int r, vector<vector<double> > visibility, vector<vector<vector<double> > > pheromones, int n,
    int s, double alfa, double beta, vector <int> list_candidates);

//FC - CBJ
void AFC(int k, int n, vector<vector<double> > visibility, 
    vector<vector<vector<double> > >pheromones,
    double alfa, double beta, vector<vector<int> > D);

#endif 
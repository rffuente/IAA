#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <iomanip>
#include <random>
#include <ctime>
#include <math.h> 
 
using namespace std;

vector<int> remove_element(vector<int> vec, int element)
{
    vec.erase(std::remove(vec.begin(), vec.end(), element), vec.end());
    return vec;
};

vector<string> split(string str, string token)
{
    vector<string>result;
    while(str.size()){
        int index = str.find(token);
        if(index!=string::npos){
            result.push_back(str.substr(0,index));
            str = str.substr(index+token.size());
            if(str.size()==0)result.push_back(str);
        }
        else{
            result.push_back(str);
            str = "";
        }
    }
    return result;
};

vector<string> slice(vector<string> v, int n, int m)
{
    auto first = v.cbegin() + n;
    auto last = v.cbegin() + m + 1;

    vector<string> vec(first, last);
    return vec;
};

//////////////////////////////////////////////
// TTP FUNCTION
//////////////////////////////////////////////

//n: Numbers of participating teams
//l: Lower bound for the AtMost constraint
//u: Upper bound for the AtMost constraint
//T: mapping function which assigns the integer numbers [1..n] to the teams
//D: nxn distance matrix.

//Some functions for constraints
int psi(int g, int t)
{
    if(g == t)
    {
        return 1;
    }
    return 0;
};

int omega(int g)
{
    if(g > 0)
    {
        return 1;
    }
    else if(g < 0)
    {
        return -1;
    }
    return 0;
};

// r: round
// t: team
//Constraint
bool C1(vector<vector<int> > S_tr, int t, int r, int n){
    if(-S_tr[abs(S_tr[t][r])-1][r] > 0)
    {
        return -S_tr[abs(S_tr[t][r])-1][r] == (t+1);
    }
    return -S_tr[abs(S_tr[t][r])-1][r] == -(t+1);
};

bool C2(vector<vector<int> > S_tr, int t, int n, int t_p, int k)
{
    int i,sum = 0;
    for(i=0; i < 2*n-2+k; i++)
    {
        sum = sum + psi(abs(S_tr[t][i]), (t_p+1));
    }
    return sum == 2;
};

bool C3(vector<vector<int> > S_tr, int t, int n, int k)
{
    int i,sum = 0;
    for(i=0; i < 2*n-2+k; i++)
    {
        sum = sum + S_tr[t][i];
    }
    return sum == 0;
};

bool C4(vector<vector<int> > S_tr, int t, int r, int u)
{
    int i,sum = 0;
    for(i=0; i <= u-1; i++)
    {
        sum = sum + omega(S_tr[t][r+i]);
    }
    return abs(sum) <= u;
};

bool C5(vector<vector<int> > S_tr, int t, int r, int l)
{
    int i,sum = 0;
    for(i=0; i <= l-1; i++)
    {
        sum = sum + omega(S_tr[t][r+i]);
    }
    return abs(sum) >= l;
};

bool C6(vector<vector<int> > S_tr, int t, int r)
{
    if(S_tr[t][r] != 0 and S_tr[t][r+1] != 0)
    {
        return abs(S_tr[t][r]) != abs(S_tr[t][r+1]);
    }
    else
    {
        return true;
    }
};

/////////////////////////////////////////////////////////////////
//Verify constraints
int verifyC1(vector<vector<int> > S_tr, int n, int k)
{
    int i,j;
    for(i=0; i < n; i++)
    {
        for(j = 0; j < 2*n-2+k; j++)
        {
            if(not C1(S_tr, i, j, n))
            {
                return 0;
            }
        }
    }
    return 1;
}

int verifyC2(vector<vector<int> > S_tr, int n, int k)
{
    int i,j;
    for(i=0; i < n; i++)
    {
        for(j=0; j < n;j++)
        {
            if(i != j)
            { 
                if(not C2(S_tr, i, n, j, k))
                {
                    return 0;
                }
            }
        }
    }
    return 1;
}

int verifyC3(vector<vector<int> > S_tr, int n, int k)
{
    int i;
    for(i=0; i < n; i++)
    {
        if(not C3(S_tr, i, n, k))
        {
            return 0;
        }
    }
    return 1;
}

int verifyC4(vector<vector<int> > S_tr, int n, int u, int k)
{
    int i,j;
    for(i=0; i < n; i++)
    {
        for(j=0; j < 2*n - 1 - u + k; j++)
        {
            if(not C4(S_tr, i, j, u))
            {
                return 0;
            }
        }
    }
    return 1;
}

int verifyC5(vector<vector<int> > S_tr, int n, int l, int k)
{
    int i,j;
    for(i=0; i < n; i++)
    {
        for(j=0; j < 2*n - 1 - l + k; j++)
        {
            if(not C5(S_tr, i, j, l))
            {
                return 0;
            }
        }
    }
    return 1;
}

int verifyC6(vector<vector<int> > S_tr, int n, int k)
{
    int i,j;
    for(i=0; i < n; i++)
    {
        for(j=0; j < 2*n - 3 + k; j++)
        {
            if(not C6(S_tr, i, j))
            {
                return 0;
            }
        }
    }
    return 1;
}
//Objective function

int fi(vector<vector<int> > D, vector<vector<int> > S_tr, int t, int r, int n, int k)
{
    //boundary cases
    //round 0
    //cout << "t: " << t << ", r: " << r << endl;
    if(r == 0 and S_tr[t][r] > 0)
    {
        return 0;
    }

    else if(r == 0 and S_tr[t][r] < 0)
    {
        return D[t][abs(S_tr[t][r])-1];
    }

    else if(r == 0 and S_tr[t][r] == 0)
    {
        return 0;
    }

    //round max (2*n-2+k)
    else if(r == (2*n-2+k) and S_tr[t][r-1] < 0)
    {
        return D[abs(S_tr[t][r-1])-1][t];
    }

    else if(r == (2*n-2+k) and S_tr[t][r-1] > 0)
    {
        return 0;
    }

    else if(r == (2*n-2+k) and S_tr[t][r-1] == 0)
    {
        bool flag = true;
        for(int i = r; i > 0; i--)
        {
            if(flag)
            {
                if(S_tr[t][i-1] < 0)
                {
                    return D[abs(S_tr[t][i-1])-1][t];
                    flag = false;
                }
                else if(S_tr[t][i-1] > 0)
                {
                    flag = false;
                    return 0;
                }
            }
        }
        return 0;
    }
    //middle clases (between round 1 and round 2*n-3+k)
    //if we dont have a by in a slot
    else if(S_tr[t][r-1] > 0 and S_tr[t][r] > 0)
    {
        return 0;
    }
    else if(S_tr[t][r-1] < 0 and S_tr[t][r] < 0)
    {
        return D[abs(S_tr[t][r-1])-1][abs(S_tr[t][r])-1];
    }
    else if(S_tr[t][r-1] < 0 and S_tr[t][r] > 0)
    {
        return D[abs(S_tr[t][r-1])-1][t];
    }
    else if(S_tr[t][r-1] > 0 and S_tr[t][r] < 0)
    {
        return D[t][abs(S_tr[t][r])-1];
    }

    //if we have a bye
    else if(S_tr[t][r-1] > 0 and S_tr[t][r] == 0)
    {
        return 0;
    }

    else if(S_tr[t][r-1] < 0 and S_tr[t][r] == 0)
    {
        return 0;
    }

    //REVISAR ESTE CASO TAMBIEN
    else if(S_tr[t][r-1] == 0 and S_tr[t][r] > 0)
    {
        for(int i = r; i >= 0; i--)
        {
            if(S_tr[t][i-1] < 0)
            {
                return D[abs(S_tr[t][i-1])-1][t];
            }   
            else if(S_tr[t][i-1] > 0)
            {
                return 0;
            }    
        }
        return 0;
    }
    //REVISAR ESTE CASO
    else if(S_tr[t][r-1] == 0 and S_tr[t][r] < 0)
    {
        for(int i = r; i >= 0; i--)
        {
            if(S_tr[t][i-1] > 0)
            {
                return D[t][abs(S_tr[t][r])-1];
            }   
            else if(S_tr[t][i-1] < 0)
            {
                return D[abs(S_tr[t][i-1])-1][abs(S_tr[t][r])-1];
            }   
        }
        return D[t][abs(S_tr[t][r])-1];
    }
    else
    {
        return 0;
    }

}
int gamma(vector<vector<int> > D, vector<vector<int> > S_tr, int t, int n, int k)
{
    int i, sum = 0;
    for(i = 0; i <= 2*n-2+k; i++)
    {
        //cout << "t: " << t << ", r: " << i << endl;
        //cout << fi(D, S_tr, t, i, n, k) << endl;
        //cout << endl;
        sum = sum + fi(D, S_tr, t, i, n, k);
    }
    return sum;
}

int objetive_function(vector<vector<int> > D, vector<vector<int> > S_tr, int n, int k)
{
    int i, sum = 0;
    for(i = 0; i < n; i++)
    {
        sum = sum + gamma(D, S_tr, i, n, k);
    }
    return sum;
}

///////////////////////////////////////////////////////////////
//Movement

double ITA(vector<vector<int> > D, double Q, int i, int j, int n)
{
    if(1 <= i and i <= n and 1 <= j and j <= n)
    {
        return 1.0;
    }
    else
    {
        return Q/D[abs(i)-1][abs(j)-1];
    }
};

vector<vector<double> > create_visibility(int n, double Q, vector<vector<int> > D)
{
    //create visibility 
    vector<vector<double>> visibility(2*n+1, vector<double>(2*n+1, 0));
    int aux, aux2;

    //fill visibility matrix with 1.0 or Q / d[i][j].
    for(int i = 1 ; i <= 2*n+1 ; i++ )
    {
        for (int j = 1; j <= 2*n+1; j++) 
        {
            aux = i;
            aux2 = j;
            if( (i%(n+1)) != i)
            {
                aux = -(i%(n+1)+1);
            }
            if( (j%(n+1)) != j)
            {
                aux2 = -(j%(n+1)+1);
            }

            if((i == 2*n+1 or j == 2*n+1) and i != j)
            {
                visibility[i-1][j-1] = 1;
            }
            else if(abs(aux) != abs(aux2))
            {
                visibility[i-1][j-1] = ITA(D, Q, aux, aux2, n);
            }
        }
    }
    return visibility;
};

vector<vector<vector<double> > >create_pheromones(int n, int k)
{
    //create pheromone 
    vector<vector<vector<double>>> pheromones(2*n-2+k, 
        vector<vector<double>>(2*n+1, vector<double>(2*n+1)));

    for(int i = 0 ; i < 2*n-2+k ; i++ )
    {
        for(int j = 0 ; j <= 2*n ; j++ )
        {
            for(int l = 0 ; l <= 2*n ; l++ )
            {
                pheromones[i][j][l] = 0.1;
            }
        }
    }

    return pheromones;
};

void print_visibility(vector<vector<double> > visibility, int n)
{
    std::cout << std::fixed;
    std::cout << std::setprecision(3);
    cout << "VISIBILITY MATRIX: " << endl;
    for(int i = 0 ; i <= 2*n ; i++ )
    {
        for (int j = 0; j <= 2*n; j++) 
        {
            cout << visibility[i][j] << "  ";
        }
        cout << "\n";
    }
};

void print_pheromone(vector<vector<vector<double> > >pheromones, int n, int k)
{
    std::cout << std::fixed;
    std::cout << std::setprecision(3);
    cout << "PHEROMONE MATRIX: " << endl;
    for(int i = 0 ; i < 2*n-2+k ; i++ )
    {
        for (int j = 0; j < 2*n; j++) 
        {
            for(int l = 0; l < 2*n; l++)
            {
                cout << pheromones[i][j][l] << "  ";
            }
            cout << "\n";
        }
        cout << "\n";
    }
}

int Pij(int t, int r, vector<vector<double> > visibility, vector<vector<vector<double> > >pheromones, int n,
    int s, double alfa, double beta, vector <int> list_candidates)
{
    double sum = 0;
    double aux = 0;
    vector <double> P;
    vector <int> P_index;
    for(int i = 0; i < list_candidates.size(); i++)
    {
        sum = sum + pow(visibility[t][list_candidates[i]],alfa) * pow(pheromones[r][t][list_candidates[i]],beta);
    }
    for(int i = 0; i < list_candidates.size(); i++)
    {
        aux = aux + (pow(visibility[t][list_candidates[i]],alfa) * pow(pheromones[r][t][list_candidates[i]],beta))/sum;
        P.push_back(aux);
        P_index.push_back(list_candidates[i]);
    }
    /*
    cout << "arreglo tau * ita: ";
    for (int  i = 0; i < P.size(); i ++)
    {  
        cout << P[i] << " ";
    }
    cout << endl;
    cout << "indice: ";
    for (int  i = 0; i < P_index.size(); i ++)
    {
        cout << P_index[i] << " ";
    }
    cout << endl;
    */
    std::mt19937 gen(s);
    std::uniform_real_distribution<> dis(0, 1);//uniform distribution between 0 and 1
    double random = dis(gen);
    //cout << "random: " << random << endl;

    for(int i = 1; i < P.size() - 1; i++)
    {
        if(random > P[i-1] and random < P[i])
        {
            return P_index[i];
        }

    }
    return P_index[0];
};

void AFC(int k, int n, vector<vector<double> > visibility, 
    vector<vector<vector<double> > >pheromones,
    double alfa, double beta, vector<vector<int> > D)
{
    int empty_slot = -(n+1);
    //Solution
    vector<vector<vector<int>>> S_tr(2, 
        vector<vector<int>>(n, vector<int>(2*n-2+k,empty_slot)));

    //J
    vector<vector<int>> J(n, vector<int>(2*n-2+k, 0));

    vector<vector<vector<int>>>list_vetoed;
    for(int i = 0; i < n; i++)
    {
        list_vetoed.push_back(vector<vector<int>>());
    }
    for(int i = 0; i < n; i++)
    {
        for(int j = 0; j < 2*n-2+k; j++)
        {
            list_vetoed[i].push_back(vector<int>());   
        }
    }

    //vector<int> list_vetoed;
    vector<int> list_candidates;

    //backjump
    int backjumping = 0;

    int max_backjumping = 60*n;
    if(n > 12)
    {
        max_backjumping = 100*n;
    }
    if(n > 14)
    {
        max_backjumping = 140*n;
    }
    int reset = 0;

    for(int round = 0; round < 2*n-2+k; round++)
    {
        //list_vetoed.clear();
        for(int team = 0; team < n; team ++)
        {
            if(S_tr[0][team][round] == empty_slot)
            {
                //restart ant
                if(backjumping > max_backjumping)
                {
                    reset = reset + 1;
                    cout << "reset ant number: " << reset << endl;
                    for(int j = 0; j < 2*n-2+k; j++)
                    {
                        for(int i = 0; i < n; i++)
                        {
                            list_vetoed[i][j].clear();
                            S_tr[0][i][j] = empty_slot;
                        }
                    }
                    team = 0;
                    round = 0;
                    backjumping = 0;
                }

                //list of candidates with specific team and round
                list_candidates.clear();
                //fill list of candidates
                //cout << "team: " << team << ", round: " << round << endl;
                for(int i = 0; i < 2*n; i++)
                {
                    if(i != team and i%n != team )
                    {
                        list_candidates.push_back(i);
                    }
                }
                //byes
                if(k != 0)
                {
                    list_candidates.push_back(2*n);   
                }

                //remove elements from list according to constraints
                //if there is a k byes in a team then we can't choose a bye.
                int byes_count = 0;
                for(int i = 0; i < n; i++)
                {
                    if(S_tr[0][team][i] == 0)
                    {
                        byes_count = byes_count + 1;
                    }
                }

                if(byes_count == k)
                {
                    list_candidates = remove_element(list_candidates, 2*n);
                }

                if(team == n-1)
                {
                    list_candidates = remove_element(list_candidates, 2*n);
                }

                //eliminate all oponent teams from all rounds with a specific team
                for(int i = 0; i < round; i++)
                {
                    if(S_tr[0][team][i] != empty_slot)
                    {
                        if(S_tr[0][team][i] > 0)
                        {
                            list_candidates = remove_element(list_candidates, S_tr[0][team][i]-1);
                        }
                        else if(S_tr[0][team][i] < 0)
                        {
                            list_candidates = remove_element(list_candidates, abs(S_tr[0][team][i])+n-1);
                        }
                    }
                }

                //eliminate all oponents teams from specific round
                for(int i = 0; i < n; i++)
                {
                    if(S_tr[0][i][round] != empty_slot and S_tr[0][i][round] != 0)
                    {
                        list_candidates = remove_element(list_candidates, abs(S_tr[0][i][round])-1);
                        list_candidates = remove_element(list_candidates, abs(S_tr[0][i][round])+n-1);
                    }
                    if(S_tr[0][i][round] == 0)
                    {
                        list_candidates = remove_element(list_candidates, abs(S_tr[0][i][round])-1);
                        list_candidates = remove_element(list_candidates, abs(S_tr[0][i][round])+n-1);
                    }
                }

                //eliminate oponent team from previous round
                if(round > 0)
                {
                    if(S_tr[0][team][round-1] < 0)
                    {
                        list_candidates = remove_element(list_candidates, abs(S_tr[0][team][round-1])-1);
                    }
                    else if(S_tr[0][team][round-1] > 0)
                    {
                        list_candidates = remove_element(list_candidates, abs(S_tr[0][team][round-1])+n-1);
                    }
                }
                //eliminate three consecutive oponents team
                if(round > 2)
                {
                    //cout << "lista candidatos antes de round round > 2: " << endl;
                    //for (int i = 0; i < list_candidates.size(); i++)
                    // {
                    //    cout << list_candidates[i] << " ";
                    //}
                    //cout << endl;

                    if(S_tr[0][team][round-1] < 0 and
                       S_tr[0][team][round-2] < 0 and
                       S_tr[0][team][round-3] < 0)
                    {
                        for(int i = 0; i < list_candidates.size(); i++)
                        {
                            if(list_candidates[i]%n != list_candidates[i])
                            {
                                list_candidates = remove_element(list_candidates, list_candidates[i]);
                                i = i - 1;
                            }
                        }
                    }

                    else if(S_tr[0][team][round-1] > 0 and
                       S_tr[0][team][round-2] > 0 and
                       S_tr[0][team][round-3] > 0)
                    {
                        for(int i = 0; i < list_candidates.size(); i++)
                        {
                            //cout << "list_candidates[i]: " << list_candidates[i] << endl;
                            //cout << "list_candidates[i] mod n: " << list_candidates[i]%n << endl;
                            if(list_candidates[i]%n == list_candidates[i])
                            {
                                list_candidates = remove_element(list_candidates, list_candidates[i]);
                                i = i - 1;
                            }
                        }

                    }

                }

                for(int l = 0; l < list_vetoed[team][round].size(); l ++)
                { 
                    list_candidates = remove_element(list_candidates, list_vetoed[team][round][l]); 
                }
                /*
                cout << "lista de candidatos: " << endl;
                for (int i = 0; i < list_candidates.size(); i++)
                {
                    cout << list_candidates[i] << " ";
                }
                cout << endl;
                */

                if(list_candidates.size()!=0)
                {
                    //RECORDAAAAAAAAR
                    //recordar escoger valor, y ver si oponente tiene viabilidad.
                    int oponent_team = Pij(team, round, visibility, pheromones, n, time(0), alfa, beta, list_candidates);
                    //cout << "oponente escogido: " << endl;
                    //cout << oponent_team << endl;

                    //if oponent team is bye, then we choose random oponent.

                    if(oponent_team == 2*n)
                    {
                        vector<int> valid_oponent_team;
                        bool valid_team;
                        int byes_count_2 = 0;
                        for(int i = team+1; i < n; i++)
                        {
                            valid_team = true;
                            for(int j = 0; j < round; j++)
                            {
                                //cout << "i: " << i << ", j: " << j << endl;
                                //cout << "valor S_tr[0][i][j]: " << S_tr[0][i][j] << endl;

                                if(S_tr[0][i][j] == 0)
                                {
                                    byes_count_2 = byes_count_2 + 1;
                                }
                                //cout << byes_count_2 << endl;
                                if(byes_count_2 == k)
                                {
                                    valid_team = false;
                                }
                            }
                            if(valid_team)
                            {
                                valid_oponent_team.push_back(i);
                            }
                            byes_count_2 = 0;
                        }
                        if(valid_oponent_team.size() != 0)
                        {
                            int random_oponent_team_index = rand() % valid_oponent_team.size();
                            int random_oponent = valid_oponent_team[random_oponent_team_index];
                            //cout << "oponent: " << random_oponent << endl;
                            S_tr[0][team][round] = 0;
                            S_tr[0][random_oponent][round] = 0;
                            J[team][round] = 1;
                        }
                        else
                        {
                            bool flag = false;
                            list_vetoed[team][round].push_back(2*n);
                            backjumping = backjumping + 1;
                            for(int j = 0; j < 2*n-2+k; j++)
                            {
                                for(int i = 0; i < n; i++)
                                {
                                    if(i == team + 1 and j == round)
                                    {
                                        flag = true;
                                    }
                                    if(flag)
                                    {
                                        list_vetoed[i][j].clear();
                                    }

                                }
                            }
                            //list_vetoed.push_back(oponent_team);
                            team = team-1;
                        }
                    }
                    else if(oponent_team%n == oponent_team)
                    {
                        if(round > 2 and
                           S_tr[0][oponent_team][round-1] < 0 and 
                           S_tr[0][oponent_team][round-2] < 0 and
                           S_tr[0][oponent_team][round-3] < 0)
                        {
                            //no se puede poner oponente en esa posicion
                            //se agrega a lista de votados y se hace backjump
                            bool flag = false;
                            list_vetoed[team][round].push_back(oponent_team);
                            backjumping = backjumping + 1;
                            for(int j = 0; j < 2*n-2+k; j++)
                            {
                                for(int i = 0; i < n; i++)
                                {
                                    if(i == team + 1 and j == round)
                                    {
                                        flag = true;
                                    }
                                    if(flag)
                                    {
                                        list_vetoed[i][j].clear();
                                    }

                                }
                            }
                            //list_vetoed.push_back(oponent_team);
                            team = team-1;

                        }
                        else
                        {
                            S_tr[0][team][round] = oponent_team+1;
                            //pos donde estoy + 1 para asignar equipo
                            S_tr[0][oponent_team][round] = -(team+1);
                            J[team][round] = 1;
                        }
                    }
                    else
                    {
                        if(round > 2 and
                           S_tr[0][oponent_team%n][round-1] > 0 and 
                           S_tr[0][oponent_team%n][round-2] > 0 and
                           S_tr[0][oponent_team%n][round-3] > 0)
                        {
                            //list_vetoed.push_back(oponent_team);
                            bool flag = false;
                            list_vetoed[team][round].push_back(oponent_team);
                            backjumping = backjumping + 1;
                            for(int j = 0; j < 2*n-2+k; j++)
                            {
                                for(int i = 0; i < n; i++)
                                {
                                    if(i == team + 1 and j == round)
                                    {
                                        flag = true;
                                    }
                                    if(flag)
                                    {
                                        list_vetoed[i][j].clear();
                                    }

                                }
                            }
                            team = team-1;
                        }
                        else
                        {
                            //equipo, ronda
                            S_tr[0][team][round] = -((oponent_team%n)+1);
                            //pos donde estoy + 1 para asignar equipo
                            S_tr[0][oponent_team%n][round] = (team+1);
                            J[team][round] = 1;
                        }
                    }
                }

                else
                {
                    //normal backjump
                    if(round > 0 )
                    {
                        int aux = S_tr[0][0][round-1];
                        if(aux > 0)
                        {
                            aux = abs(aux) - 1;
                        }
                        else if(aux < 0)
                        {
                            aux = abs(aux) + n - 1;
                        }
                        else if(aux == 0)
                        {
                            aux = 2*n;
                        }

                        list_vetoed[0][round-1].push_back(aux);
                        backjumping = backjumping + 1;

                        bool flag = false;
                        for(int j = 0; j < 2*n-2+k; j++)
                        {
                            for(int i = 0; i < n; i++)
                            {
                                if(i == 1 and j == round-1)
                                {
                                    flag = true;
                                }
                                if(flag)
                                {
                                    list_vetoed[i][j].clear();
                                    S_tr[0][i][j] = empty_slot;
                                }

                            }
                        }
                        S_tr[0][0][round-1] = empty_slot;
                        team = -1;
                        round = round-1;
                    }

                }

            }
            /*
            cout << "backjumping count: " << backjumping << endl;
            cout << "Matriz intermedia S_tr: " << endl; 
            for(int i = 0; i < n; i++)
            {
                printf("%-3d ",i+1);
                for(int j = 0; j < 2*n-2+k; j++)
                {
                    printf("%d   ",S_tr[0][i][j]);
                    //cout << S_tr[0][i][j] << " ";
                }
                cout << endl;
            }
            */
            
        }
    }

    cout << "Matriz final S_tr: " << endl; 
    for(int i = 0; i < n; i++)
    {
        printf("%-3d ",i+1);
        for(int j = 0; j < 2*n-2+k; j++)
        {
            printf("%-3d   ",S_tr[0][i][j]);
            //cout << S_tr[0][i][j] << " ";
        }
        cout << endl;
    }

    cout << "backjumping: " << backjumping << endl;
    //cout << "C1: " << verifyC1(S_tr[0], n) << "\n";
    //cout << "C2: " << verifyC2(S_tr[0], n) << "\n";
    //cout << "C3: " << verifyC3(S_tr[0], n) << "\n";
    //cout << "C4: " << verifyC4(S_tr[0], n, 3) << "\n";
    //cout << "C5: " << verifyC5(S_tr[0], n, 1) << "\n";
    //cout << "C6: " << verifyC6(S_tr[0], n) << "\n";
    //cout << "Obj: " << objetive_function(D, S_tr[0], n, k) << "\n";


}
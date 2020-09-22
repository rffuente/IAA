#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

int find_element_by_index(vector<int> row, int index)
{
    vector<int>::iterator itr = find(row.begin(), row.end(), index);
    if (itr != row.cend()) 
    {
        return distance(row.begin(), itr);
    }
    else 
    {
        return -1;
    }

}

void print_matrix(vector<vector<int> > matrix)
{
    int i,j;
    for( i = 0; i < matrix.size(); i++)
    {
        for(j = 0; j < matrix[0].size(); j++)
        {
            cout << matrix[i][j] << ' ';
        }
        cout << "\n";
    }
}

vector<vector<int> > swap_rounds(vector<vector<int> > matrix, int R1, int R2)
{
    int i;
    for(i = 0; i < matrix.size() ; i++)
    {
        swap(matrix[i][R1], matrix[i][R2]);
    }
    return matrix;
}

vector<vector<int> > swap_homes(vector<vector<int> > matrix, int T1, int T2)
{
    int index1, index2;
    index1 = find_element_by_index(matrix[T1-1], T2);
    index2 = find_element_by_index(matrix[T1-1], -T2);
    swap(matrix[T1-1][index1], matrix[T1-1][index2]);
    cout << index1 << endl;
    cout << index2 << endl;
    index1 = find_element_by_index(matrix[T2-1], T1);
    index2 = find_element_by_index(matrix[T2-1], -T1);
    swap(matrix[T2-1][index1], matrix[T2-1][index2]);

    return matrix;
}

vector<vector<int> > swap_teams(vector<vector<int> > matrix, int T1, int T2)
{
    int i, index1, index2;

    for (i = 0; i < matrix[0].size(); i++)
    {
        if(matrix[T1-1][i] != T2 and matrix[T1-1][i] != -T2)
        {
            cout << i << endl;
            index1 = abs(matrix[T1-1][i]);
            index2 = abs(matrix[T2-1][i]);
            //cout << 
            swap(matrix[index1-1][i], matrix[index2-1][i]);
            swap(matrix[T1-1][i], matrix[T2-1][i]);
        }
    }

    return matrix;
}

vector<vector<int> > partial_swap_rounds(vector<vector<int> > matrix, int R1, int R2, int T)
{
    int index1,index2,index3;
    for (int i = 0; i < matrix.size(); i++)
    {
        if (abs(matrix[i][R1-1]) == abs(matrix[abs(matrix[T-1][R2-1])][R2-1]) )
        {
            index1 = i;
        }
    }

    index2 = abs(matrix[T-1][R1-1])-1;
    index3 = abs(matrix[T-1][R2-1])-1;
    swap(matrix[T-1][R1-1], matrix[T-1][R2-1]);
    swap(matrix[index1][R1-1],matrix[index1][R2-1]);
    swap(matrix[index2][R1-1],matrix[index2][R2-1]);
    swap(matrix[index3][R1-1],matrix[index3][R2-1]);

    return matrix;
}

vector<vector<int> > partial_swap_teams(vector<vector<int> > matrix, int T1, int T2, int R)
{
    
    swap(matrix[T1-1][R-1], matrix[T2-1][R-1]);

    return matrix;
}

int main()
{
    int i;
    int RR = 4;
    int CC = 6;
    vector<vector<int> > matrix(RR);
    for ( i = 0 ; i < RR ; i++ )
       matrix[i].resize(CC);

    matrix[0][0] = 3;
    matrix[1][0] = 4;
    matrix[2][0] = -1;
    matrix[3][0] = -2;

    matrix[0][1] = 4;
    matrix[1][1] = 3;
    matrix[2][1] = -2;
    matrix[3][1] = -1;

    matrix[0][2] = 2;
    matrix[1][2] = -1;
    matrix[2][2] = 4;
    matrix[3][2] = -3;

    matrix[0][3] = -3;
    matrix[1][3] = -4;
    matrix[2][3] = 1;
    matrix[3][3] = 2;

    matrix[0][4] = -2;
    matrix[1][4] = 1;
    matrix[2][4] = -4;
    matrix[3][4] = 3;

    matrix[0][5] = -4;
    matrix[1][5] = -3;
    matrix[2][5] = 2;
    matrix[3][5] = 1;

    cout << "matriz antes" << endl;
    print_matrix(matrix);
    //matrix = swap_rounds(matrix, 2,3);
    //matrix = swap_homes(matrix, 2,3);
    //matrix = swap_teams(matrix, 2,3);
    matrix = partial_swap_rounds(matrix, 1, 5, 2);
    //cout << find_element_by_index(matrix[0],3) << endl;
    cout << "matriz despues" << endl;
    print_matrix(matrix);
}
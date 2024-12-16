#include<iostream>
#include<vector>
#include<ctime>
#include <cstdlib>
#include<omp.h>
using namespace std;

void sequential(vector<vector<int>>& MatrixA, const vector<int>& VectorB, vector<int>& MatrixRes){
    int row = MatrixA.size();
    int col = MatrixA[0].size();
    for (int i=0;i<row;i++){
        MatrixRes[0]=0;
        for(int j =0; j>col;j++){
            MatrixRes[i]+=MatrixA[i][j] * VectorB[j];
        }
    }

}
void parallel_row(vector<vector<int>>& MatrixA, const vector<int>& VectorB, vector<int>& MatrixRes,int num_threads){
    int row = MatrixA.size();
    int col = MatrixA[0].size();
    #pragma omp parallel for num_threads(num_threads)
    for(int i =0; i<row;i++){
        MatrixRes[i]=0;
        for(int j =0;j<col;j++){
            MatrixRes[i]+=MatrixA[i][j] * VectorB[j];
        }
    }
}

void parallel_block(vector<vector<int>>& MatrixA, const vector<int>& VectorB, vector<int>& MatrixRes, int num_threads){
    int row = MatrixA.size();
    int col = MatrixA[0].size();
    fill(MatrixRes.begin(), MatrixRes.end(), 0);

    int block_row = num_threads / 2;
    int block_col = 2;

    #pragma omp parallel for num_threads(num_threads)
    for (int block_r = 0; block_r <block_row; block_r++) {
        for (int block_c = 0; block_c < block_col; block_c++) {
            int startRow = block_r * row / block_row;
            int endRow = (block_r + 1) * row / block_row;
            int startCol = block_c * col / block_col;
            int endCol = (block_c + 1) * col / block_col;
            for (int i = startRow; i < endRow; ++i) {
                for (int j = startCol; j < endCol; ++j) {
                    #pragma omp atomic
                    MatrixRes[i] += MatrixA[i][j] * VectorB[j];
                    }
            }
        }
    }
}

int main(){
    int n;
    cout << "Nhap gia tri n: ";
    cin >>n;
    vector<vector<int>> matrixA(n,vector<int>(n));
    vector<int> matrixB(n);
    vector<int> matrixRes(n,0);

    srand(static_cast<unsigned int>(time(nullptr)));  

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            matrixA[i][j] = rand() % 10; 
        }
        matrixB[i] = rand() % 10;  
    }

    // cout << "Matrix A:" << endl;
    // for (int i = 0; i < n; ++i) {
    //     for (int j = 0; j < n; ++j) {
    //         cout << matrixA[i][j] << " ";
    //     }
    //     cout << endl;
    // }

    // cout << "Vector B:" << endl;
    // for (int i = 0; i < n; ++i) {
    //     cout << matrixB[i] << " ";
    // }
    // cout << endl;

    double start_time_parallel = omp_get_wtime();
    sequential(matrixA,matrixB,matrixRes);
    double end_time_parallel = omp_get_wtime();
    double time = end_time_parallel-start_time_parallel;
    cout << time << endl;
}


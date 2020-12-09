//Authors: Luan Khong
//		   Phong Vu
//		   Vu Mai
//
//Project: 2
//Purpose: Calculate the largest submatrix

#include <iostream>
#include <mpi.h>
#include <fstream>
#include <string>
#include <iomanip>
using namespace std;

void displayMatrix(int matrix[2][2]);
void displayMatrix(int** matrix, int numOfRows, int numOfCols);
void readMatrixFromFile(int** matrix, string fileName, int numOfRows, int numOfCols);
void generateSubmatrix(int** matrix, int x, int y, int subMatrix[2][2]);
int calculateMatrix(int matrix[2][2]);

int main(int argc, char **argvs)
{
	MPI_Init(&argc, &argvs);
	int rank = 0;
	int numOfProcess = 0;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &numOfProcess);

	//Check one of the argument
	if (numOfProcess <= 1)
	{
		cout << "Invalid argument: Assign at least 2 processes" << endl;
		MPI_Finalize();
		return -1;
	}

	if (rank == 0)	//the master
	{
		int subMatrix[2][2];
		int largestSubMatrix[2][2];
		int one_result;
		int largest;
		int largest_x;
		int largest_y;
		int current_submatrix;
		int rank_found_largest;

		string fileName = argvs[1];
		int numOfRows = atoi(argvs[2]);
		int numOfCols = atoi(argvs[3]);

		//Declare a 2-d matrix
		int** matrix = new int*[numOfRows];
		for (int x = 0; x < numOfRows; x++)
		{
			matrix[x] = new int[numOfCols];
		}

		readMatrixFromFile(matrix, fileName, numOfRows, numOfCols);

		//This variable is used to keep track of which worker to send to. Along with a modolu later on.
		current_submatrix = 0;

		largest = 0;
		for (int x = 0; x < numOfRows - 1; x++)
		{
			for (int y = 0; y < numOfCols - 1; y++)
			{
				generateSubmatrix(matrix, x, y, subMatrix);

				//Store which rank of the worker to deal with
				int temp_rank = (current_submatrix % (numOfProcess - 1)) + 1;

				//send to a worker a submatrix
				MPI_Send(subMatrix, 4, MPI_INT, temp_rank, temp_rank, MPI_COMM_WORLD);

				//receive a result from a worker
				MPI_Recv(&one_result, 1, MPI_INT, temp_rank, temp_rank, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

				//Keep track of the largest submatrix
				if (one_result > largest)
				{
					rank_found_largest = temp_rank;
					largest = one_result;
					largest_x = x;
					largest_y = y;
					largestSubMatrix[0][0] = subMatrix[0][0];
					largestSubMatrix[0][1] = subMatrix[0][1];
					largestSubMatrix[1][0] = subMatrix[1][0];
					largestSubMatrix[1][1] = subMatrix[1][1];
				}

				//Increasing this will allow us to deal with a different worker next loop
				current_submatrix++;
			}
		}

		cout << "\nTHE FINAL RESULT: " << largest << " at coordinate(0-based) x = " << largest_x << "  y = " << largest_y << " found by process rank " << rank_found_largest << endl << endl;

		cout << "The largest submatrix: " << endl;
		displayMatrix(largestSubMatrix);

		cout << "Given Matrix:" << endl;
		displayMatrix(matrix, numOfRows, numOfCols);

		//cleanup
		delete(matrix);
	}
	else if (rank < numOfProcess && rank >= 1)	//the workers
	{
		int workerSubMatrix[2][2];
		int result = 0;
		int numOfWorkers = numOfProcess - 1;

		//number of submatrices
		int n = (atoi(argvs[2]) - 1) * (atoi(argvs[3]) - 1);

		//1). Loops as many times as the number of tasks the master sends you.
		//2). If there are more workers than there are total number of tasks,
		//		then those extra workers will not do anything, hence they will loop 0 times.
		for (int x = rank; x <= n; x += numOfWorkers)
		{
			//receive a submatrix from the master
			MPI_Recv(workerSubMatrix, 4, MPI_INT, 0, rank, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);

			//calculation
			result = calculateMatrix(workerSubMatrix);

			//The printing was different for linux than windows. Comment out the cout's below if you do not wish to see the process results
			cout << "Process " << rank << ((rank >= 10) ? ": " : ":  ");
			cout << setw(5) << left << result << endl;
			if(rank == numOfProcess - 1) cout << endl;

			//send the result to the master
			MPI_Send(&result, 1, MPI_INT, 0, rank, MPI_COMM_WORLD);
		}
	}

	MPI_Finalize();

	return 0;
}

void displayMatrix(int** matrix, int numOfRows, int numOfCols)
{
	for (int x = 0; x < numOfRows; x++)
	{
		for (int y = 0; y <numOfCols; y++)
		{
			cout << setw(5) << left << matrix[x][y];
		}
		cout << endl;
	}
	cout << endl;
}

void displayMatrix(int matrix[2][2])
{
	for (int x = 0; x < 2; x++)
	{
		for (int y = 0; y <2; y++)
		{
			cout << setw(5) << left << matrix[x][y];
		}
		cout << endl;
	}
	cout << endl;
}

void readMatrixFromFile(int** matrix, string fileName, int numOfRows, int numOfCols)
{
	fstream read;

	read.open(fileName);

	for (int x = 0; x < numOfRows; x++)
	{
		for (int y = 0; y < numOfCols; y++)
		{
			read >> matrix[x][y];
		}
	}

	read.close();
}

void generateSubmatrix(int** matrix, int x, int y, int subMatrix[2][2])
{
	subMatrix[0][0] = matrix[x][y];
	subMatrix[0][1] = matrix[x][y + 1];
	subMatrix[1][0] = matrix[x + 1][y];
	subMatrix[1][1] = matrix[x + 1][y + 1];
}

int calculateMatrix(int matrix[2][2])
{
	int total = 0;

	for (int x = 0; x < 2; x++)
	{
		for (int y = 0; y < 2; y++)
		{
			total += matrix[x][y];
		}
	}

	return total;
}

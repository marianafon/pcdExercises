#include <stdio.h>
#include <stdlib.h>

void Print_two_d(int two_d[][], int rows, int cols){
	int i, j = 0;	
	for(i = 0; i < rows; i++)
	{
		printf(" |");
		for(j = 0; j < cols; j++)
		{
			printf(" %d |", two_d[i][j]);
		}
		printf("\n");
	}	
}

int main(){
	int two_d[3][4];
	int i, j = 0;

	for(i = 0; i < 3; i++)
	{
		for(j = 0; j<4; j++)
		{
			two_d[i][j] = (i+j);
		}
	}

	Print_two_d(two_d, 3, 4);

	return 0;
}
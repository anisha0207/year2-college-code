// https://u.osu.edu/cstutorials/2018/09/28/how-to-debug-c-program-using-gdb-in-6-simple-steps/
# include <stdio.h>

int main()
{
	int i, num, j;
	printf("Enter the number: ");
	scanf("%d", &num );

	for(i=1; i<num; i++)
		j=j*i;

	printf("The factorial of %d is %d\n",num,j);
}
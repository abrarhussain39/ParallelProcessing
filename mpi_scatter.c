#include <string.h>
#include <stdio.h>
#include <mpi.h>
#include <math.h>
#include <time.h>
int main(int argc, char** argv) {
  int rank, size,N;


  MPI_Init(&argc, &argv);


  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);


  int original[N];
  int i,j,k;
  if(rank==0){
  printf("Enter the value of N : (array size):");
  scanf("%d",&N);
}
 MPI_Bcast(&N, 1, MPI_INT, 0, MPI_COMM_WORLD);
if (rank == 0) {
    printf("Enter %d elements of an array: ", N);
    fflush(stdout);
    for (i = 0 ; i < N; i++) {
      scanf("%d", original + i);
    }
    printf("processor 0:\n");
    for (i = 0 ; i < N; i++) {
      printf("%d\n", original[i]);
    }
  }


  int interval[N / size];
  for (i = 0; i < (N / size); i++) {
 double start_time,end_time,tmp,time_overhead,total_time,local_time,commu_time;
  MPI_Barrier(MPI_COMM_WORLD);
  tmp=MPI_Wtime();
  start_time=MPI_Wtime();
  for(j=0;j<(N / size); j++)
  MPI_Scatter(original, N / size, MPI_INT,
              &interval, N / size, MPI_INT, 0, MPI_COMM_WORLD);
  end_time=MPI_Wtime();
  time_overhead=start_time-tmp;
  total_time=end_time-start_time-time_overhead;
  local_time=total_time/(N/size);
  
    printf("\nProcess %d has %d|\n", rank, interval[i]);
    printf("\ntime is %f\n",local_time);
    
  }


  MPI_Finalize();
  return 0;

}

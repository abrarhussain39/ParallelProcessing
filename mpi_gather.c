#include <string.h>
#include <stdio.h>
#include <mpi.h>

int main(int argc, char** argv) {
  int rank, size,N;

  
  MPI_Init(&argc, &argv);


  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  int original[N];
  int i;
if(rank==0){
  printf("Enter the value of N (array size): ");
  scanf("%d",&N);
}
 MPI_Bcast(&N, 1, MPI_INT, 0, MPI_COMM_WORLD);

  if (rank == 0) {
    printf("Enter %d values: ", N);
    fflush(stdout);
    for (i = 0 ; i < N; i++) {
      scanf("%d", original+i);
    }
    printf("\nprocessor %d:\n",rank);
    for (i = 0 ; i < N; i++) {
      printf("\n %d", original[i]);
    }
  }

  int interval[N / size];
double start_time,tmp,end_time,time_overhead,total_time,local_time,commu_time;

  MPI_Scatter(original, N / size, MPI_INT,&interval, N / size, MPI_INT, 0, MPI_COMM_WORLD);
  tmp=MPI_Wtime();
  start_time=MPI_Wtime();
  MPI_Gather(&interval, N / size, MPI_INT,original, N / size, MPI_INT, 0, MPI_COMM_WORLD);
  end_time=MPI_Wtime();
  time_overhead=start_time-tmp;
  total_time=end_time-start_time-time_overhead;
  local_time=total_time/(N/size);
  if (rank == 0) {
    printf("\nFinal Array ");
    for (i = 0; i < N; i++) {
      printf("\nProcessor %d has %d", rank,original[i]);
    }
    printf("\n");
  }

 
  MPI_Finalize( );
  return 0;
}
#include <mpi.h>
#include <stdio.h>
#include <math.h>

int main (argc, argv)
int argc;
char **argv;
{
   int done=0,size, rank,i,n;
   
   double pieval= 3.141592653589793238462643;
   double pi=0.0, calpi=0.0,var,mypi=0.0;

   MPI_Init( &argc, &argv );
   MPI_Comm_size(MPI_COMM_WORLD, &size);
   MPI_Comm_rank(MPI_COMM_WORLD, &rank);
   MPI_Status status;

      while(!done){
        if (rank == 0)
        {

	    printf("Enter even value for intervals: (0 for quitting) ");
	    scanf("%d",&n);
      }
        
   
   if( rank == 0 ) 
   printf("Calculating pi using %d divisions\n", n);
 if( rank == 0 ) 
   printf("Calculating value using %d processors\n", size);
        
   MPI_Bcast( &n, 1, MPI_INT, 0, MPI_COMM_WORLD); 
   double interval=1/(double)n; 
    
    for(i=rank;i<=n;i+=size){
        double x=(i)*interval;

        pi=4/(1+(x*x));
      
   if ((i!=0)&&(i!=(n)))
        {
            if(i%2==0)
                pi=2*pi;
            else if (i%2!=0)
                pi=4*pi;
    calpi=calpi+pi;  
        }       
  else if(i==n){
    calpi=calpi+pi;
  } 
  else if(i==0){
    calpi=calpi+pi;

  }
  
    }
  if(rank!=0){
  MPI_Send(&calpi, 1, MPI_DOUBLE, 0, rank, MPI_COMM_WORLD);
        
  }
  double sum=calpi;
  
    if(rank==0){
  
        for(i=1;i<size;i++)
        {   
            MPI_Recv(&var, 1, MPI_DOUBLE, i, i, MPI_COMM_WORLD,&status) ;      
                sum=sum+var;
                mypi=sum/(3*n);
           
        }
  
        printf("Calculating pi value using simpsons rule %.16f,Error is %.16f\n",mypi,fabs(mypi-pieval));
    }
    done=1;
    }

   MPI_Finalize();
   return 0;
}
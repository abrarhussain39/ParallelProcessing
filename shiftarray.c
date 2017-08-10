#include"mpi.h"
#include<stdio.h>
#include<math.h>
void SHIFT(int original[], int result[], int sfactor) {
    int myid, noofproc,sf,var;
    int N=2;
    MPI_Request request;
    MPI_Status status;
    MPI_Comm_size(MPI_COMM_WORLD,&noofproc);
    MPI_Comm_rank(MPI_COMM_WORLD,&myid);


    for(var=0;var<N;var++) {
        result[var] = original[var];
    }
    if(myid == 0) {

        if(sfactor > 0){
            int i;
            for(i=0;i<sfactor;i++){
                int temp=result[0],j;
                            MPI_Isend(&result[N-1], 1, MPI_INT, (myid + 1)%noofproc, (myid + 1)%noofproc, MPI_COMM_WORLD, &request) ;
                            MPI_Recv(&result[0], 1 , MPI_INT, noofproc -1, myid, MPI_COMM_WORLD, &status);
                for(j=N-1;j>1;j--){
                    result[j] = result[j-1];
                }
result[j] = temp;
            }
        } else {
            int i;
            for(i=0;i>sfactor;i--){
                int temp=result[N-1],j;
                MPI_Isend(&result[0], 1, MPI_INT, noofproc-1, noofproc-1, MPI_COMM_WORLD, &request);
                MPI_Recv(&result[N-1], 1, MPI_INT, (myid + 1)%noofproc, myid, MPI_COMM_WORLD, &status);
                for(j=0; j < N-2 ; j++){
                    result[j] = result[j+1];
                }
                result[j] = temp;
            }
        }
    }
    else {
        if(sfactor > 0){
            int i;
            for(i=0; i < sfactor ; i++){
                int temp=result[0],j;
                MPI_Recv(&result[0], 1, MPI_INT, myid - 1, myid, MPI_COMM_WORLD, &status);
                MPI_Send(&result[N-1], 1, MPI_INT, (myid + 1)%noofproc, (myid + 1)%noofproc, MPI_COMM_WORLD);
                for(j=N-1;j>1;j--){
                                        result[j] = result[j-1];
                                        }
                result[j] = temp;
            }
        } else {
            int fabs_n = -sfactor;
            int i;
            for(i=0; i > sfactor; i--) {
                int temp=result[N-1],j;
                MPI_Recv(&result[N-1], 1, MPI_INT, (myid + 1)%noofproc, myid, MPI_COMM_WORLD, &status);
                MPI_Send(&result[0], 1, MPI_INT, myid - 1, myid - 1, MPI_COMM_WORLD);
                for(j=0; j < N-2 ; j++){
                                        result[j] = result[j+1];
                                }
                result[j] = temp;
            }
        }
    }

}


int main(int argc,char *argv[])
{

    int sf, myid, noofproc, sfactor;
     MPI_Init(&argc,&argv);
    MPI_Comm_size(MPI_COMM_WORLD,&noofproc);
    MPI_Comm_rank(MPI_COMM_WORLD,&myid);

    int N=2;

    MPI_Bcast(&N, 1, MPI_INT, 0, MPI_COMM_WORLD);
    int input_array[N], result[N];


    if(myid == 0) {
        int i;
        printf("\nEnter %d elements: Processor %d ",N,myid);
        for(i=0; i<N; i++) {
            scanf("%d",&input_array[i]);
        }
        int j;
        for(j=0;j<noofproc;j++){
            MPI_Barrier(MPI_COMM_WORLD);
        }
    } else {
        MPI_Status status;
        int i,j;
        for(j=0;j<myid;j++){
            MPI_Barrier(MPI_COMM_WORLD);
        }
        printf("\nEnter %d elements: Processor %d ",N,myid);
        for(i=0; i<N; i++) {
                        scanf("%d",&input_array[i]);
                }
        for(;j<noofproc;j++){
                        MPI_Barrier(MPI_COMM_WORLD);
                }
    }


    if(myid == 0) {
        printf("\nEnter the value of shift factor : ");
        scanf("%d",&sfactor);
    }
    MPI_Bcast(&sfactor, 1, MPI_INT, 0, MPI_COMM_WORLD);
     sf=sfactor%(noofproc*N);
    SHIFT(input_array, result, sf);


        if(myid == 0) {
        int i;
        printf("\nOriginal array values\n");
        printf("\nProcessor %d ",myid);
        for(i=0; i<N; i++) {
            printf("\n %d",input_array[i]);
        }
        int j;
        for(j=0;j<noofproc;j++){
            MPI_Barrier(MPI_COMM_WORLD);
            }
    } else {
        MPI_Status status;
        int i,j;
        for(j=0;j<myid;j++){
            MPI_Barrier(MPI_COMM_WORLD);
        }
        printf("\n Processor %d ",myid);
                for(i=0; i<N; i++) {
                        printf("\n %d",input_array[i]);
                }
        for(;j<noofproc;j++){
                        MPI_Barrier(MPI_COMM_WORLD);
                }
    }


        if(myid == 0) {
        printf("\nResult Array:\n");
        int i;
        printf("\n Processor %d ",myid);
        for(i=0; i<N; i++) {
            printf("\n %d",result[i]);
        }
        int j;
        for(j=0;j<noofproc;j++){
            MPI_Barrier(MPI_COMM_WORLD);
        }
} else {
        MPI_Status status;
        int i,j;
        for(j=0;j<myid;j++){
            MPI_Barrier(MPI_COMM_WORLD);
        }
        printf("\nProcessor %d ",myid);
                for(i=0; i<N; i++) {
                        printf("\n %d",result[i]);
                }
        for(;j<noofproc;j++){
                        MPI_Barrier(MPI_COMM_WORLD);
                }
    }



    MPI_Finalize();
    return 0;
}


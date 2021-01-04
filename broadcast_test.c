#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <sys/time.h>
#include <time.h>


#ifndef PACKET_SIZE 
    #define PACKET_SIZE 5
#endif

#ifndef BROADCAST 
    #define BROADCAST 0
#endif

double t, t1, t2;
static int packets[PACKET_SIZE];


int main( int argc, char* argv[] ) {   
    int rank, nproc, num_steps, buf;

    MPI_Init( &argc, &argv );
    MPI_Comm_size( MPI_COMM_WORLD, &nproc ); // number of process
    MPI_Comm_rank( MPI_COMM_WORLD, &rank );  // PID

    if(BROADCAST){

        if(rank == 0) {
           buf = 777;
        }

        t1 = MPI_Wtime();
        MPI_Bcast(packets, PACKET_SIZE, MPI_INT, 0, MPI_COMM_WORLD);
        t1 = MPI_Wtime();
        t  = t2 - t1;
        printf("BROADCAST DONE! Rank: %d\n", rank);       

    } else {
        if(rank == 0) {
            MPI_Status status;        
            t1 = MPI_Wtime();
            for (int i=0; i<nproc; i++) {
                MPI_Send(packets, PACKET_SIZE, MPI_INT, i, 0, MPI_COMM_WORLD);
                printf("\tAll packets were received from %d. Ack: %d\n", 0, i);
            }
            t2 = MPI_Wtime();
            t  = t2 - t1;
        } 
            MPI_Status status;
            MPI_Recv(packets, PACKET_SIZE, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
            printf("\t%d received from %d (size: %d)\n", rank, 0, PACKET_SIZE); 
        
    }
    MPI_Finalize();
    return 0;
}
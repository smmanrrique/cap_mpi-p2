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
double tstart, tend, ttotal;
struct timeval tv;

double get_clock() {
   struct timeval tv; int ok;
   ok = gettimeofday(&tv, (void *) 0);
   if (ok<0) { printf("gettimeofday error");  }
   return (tv.tv_sec * 1.0 + tv.tv_usec * 1.0E-6);
}

int main( int argc, char* argv[] ) {   
    int rank, nproc, resultlen;
    char name[MPI_MAX_PROCESSOR_NAME];

    int packet_size = PACKET_SIZE;
    int broadcast   = BROADCAST;

    if (argc >= 2) {
        packet_size = atoi(argv[1]);
        broadcast   = atoi(argv[2]);
    }

    int packets[packet_size];

    MPI_Init( &argc, &argv );
    MPI_Comm_size( MPI_COMM_WORLD, &nproc ); // number of process
    MPI_Comm_rank( MPI_COMM_WORLD, &rank );  // PID
    MPI_Get_processor_name( name, &resultlen );

    tstart = get_clock();
    if(broadcast){
        t1 = MPI_Wtime();
        MPI_Bcast(packets, packet_size, MPI_INT, 0, MPI_COMM_WORLD);
        t2 = MPI_Wtime();
        t  = (t2 - t1);
        printf("%d, %d, %d,,,, %s, %u,,,, %f", broadcast, nproc, sizeof(packets), name, rank, t);
        // printf("BROADCAST DONE! Rank: %d\n", rank);   
    } else {
        MPI_Status status;

        if(rank == 0) {     
            t1 = MPI_Wtime();
            for (int i=0; i<nproc; i++) {
                MPI_Send(packets, packet_size, MPI_INT, i, 0, MPI_COMM_WORLD);
                // printf("\tAll packets were received from %d. Ack: %d\n", 0, i);
            }
            t2 = MPI_Wtime();
            t  = (t2 - t1);
            printf("%d, %d, %d,,,, %s, %u,,,, %f", broadcast, nproc, sizeof(packets), name, rank, t); 
        } else {
            t1 = MPI_Wtime();
            MPI_Recv(packets, packet_size, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
            t2 = MPI_Wtime();
            t  = (t2 - t1);  
            // printf("\t%d received from %d (size: %d)\n", rank, 0, packet_size); 
            printf("%d, %d, %d,,,, %s, %u,,,, %f", broadcast, nproc, sizeof(packets), name, rank, t);           
        }        
    }
    tend   = get_clock();
	ttotal = (tend - tstart);
    printf(",%12.10lf\n",ttotal);

    MPI_Finalize();
    return 0;
}
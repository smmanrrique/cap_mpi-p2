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

    int experiment  = -1;
    int packet_size = PACKET_SIZE;
    int broadcast   = BROADCAST;

    if (argc >= 2) {
        experiment  = atoi(argv[1]);
        packet_size = atoi(argv[2]);
        broadcast   = atoi(argv[3]);
    }

    MPI_Init( &argc, &argv );
    MPI_Comm_size( MPI_COMM_WORLD, &nproc ); // number of process
    MPI_Comm_rank( MPI_COMM_WORLD, &rank );  // PID
    MPI_Get_processor_name( name, &resultlen );

    void *packets = malloc(packet_size);

    tstart = get_clock();
    if(broadcast){
        t1 = MPI_Wtime();
        MPI_Bcast(packets, packet_size, MPI_INT, 0, MPI_COMM_WORLD);
        t2 = MPI_Wtime();
        t  = (t2 - t1);
        printf("%d, %d, %d, %d, %d,,,, %s, %u,,,, %f", experiment, broadcast, nproc, packet_size, name, rank, t);
    } else {
        MPI_Status status;

        if(rank == 0) {     
            t1 = MPI_Wtime();
            for (int i=0; i<nproc; i++) {
                MPI_Send(packets, packet_size, MPI_INT, i, 0, MPI_COMM_WORLD);
            }
            t2 = MPI_Wtime();
            t  = (t2 - t1);
            printf("%d, %d, %d, %d, %d,,,, %s, %u,,,, %f", experiment, broadcast, nproc, packet_size, name, rank, t); 
        } else {
            t1 = MPI_Wtime();
            MPI_Recv(packets, packet_size, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
            t2 = MPI_Wtime();
            t  = (t2 - t1);  
            printf("%d %d, %d, %d,,,, %s, %u,,,, %f", experiment, broadcast, nproc, packet_size, name, rank, t);           
        }        
    }
    tend   = get_clock();
	ttotal = (tend - tstart);
    printf(",%12.10lf\n",ttotal);

    MPI_Finalize();
    return 0;
}
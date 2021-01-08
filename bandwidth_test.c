#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <sys/time.h>
#include <time.h>


#ifndef PACKET_SIZE 
    #define PACKET_SIZE 5
#endif

#ifndef N_PACKETS 
    #define N_PACKETS 15
#endif

struct Pair { 
    int pid_src, pid_dst, tag_id; 
}; 

double t, t1, t2;
struct timeval tv;

double get_clock() {
   struct timeval tv; int ok;
   ok = gettimeofday(&tv, (void *) 0);
   if (ok<0) { printf("gettimeofday error");  }
   return (tv.tv_sec * 1.0 + tv.tv_usec * 1.0E-6);
}

struct Pair get_process(int rank, int nproc) {
    struct Pair pids;
    pids.pid_src = rank%nproc;
    pids.pid_dst = (rank+nproc/2)%nproc;
    pids.tag_id  = pids.pid_src + pids.pid_dst;
    return pids;
}

int main( int argc, char* argv[] ) {    
    struct Pair result;    
    int rank, nproc, send_ack, recv_ack, resultlen;
    char name[MPI_MAX_PROCESSOR_NAME];

    int experiment  = -1;
    int packet_size = PACKET_SIZE;
    int n_packets   = N_PACKETS;

    if (argc >= 2) {
        experiment  = atoi(argv[1]);
        packet_size = atoi(argv[2]);
        n_packets   = atoi(argv[3]);
    }
    
    MPI_Init( &argc, &argv );
    MPI_Comm_size( MPI_COMM_WORLD, &nproc ); // number of process
    MPI_Comm_rank( MPI_COMM_WORLD, &rank );  // PID
    MPI_Get_processor_name( name, &resultlen );

    void *send_packets = malloc(packet_size);
    void *recv_packets = malloc(packet_size);

    result = get_process(rank, nproc);

    if (rank < result.pid_dst) {
        MPI_Status status;       

        t1 = get_clock();
        for (int i=0; i<n_packets; i++) {
            MPI_Send(send_packets, packet_size, MPI_BYTE, result.pid_dst, result.tag_id, MPI_COMM_WORLD);
        }
        MPI_Recv(&recv_ack, 1, MPI_INT, result.pid_dst, result.tag_id, MPI_COMM_WORLD, &status);
        t2 = get_clock();
        t  = (t2 - t1);
        printf("%d, ,%d, %d,%d,, SR,%s, %u, %u, %u, %u, %12.10lf\n", experiment, nproc, packet_size, n_packets, name, rank, result.pid_src, result.pid_dst, result.tag_id, t);
    } else  {
        MPI_Status status;
        send_ack = 9;

        t1 = get_clock();
        int i = 1;
        while (i<n_packets+1) {
            MPI_Recv(recv_packets, packet_size, MPI_BYTE, result.pid_dst, result.tag_id, MPI_COMM_WORLD, &status);
            i++;
        }
        MPI_Send(&send_ack, 1, MPI_INT, result.pid_dst, result.tag_id, MPI_COMM_WORLD);
        t2 = get_clock();
        t  = (t2 - t1);
        printf("%d, ,%d, %d, %d,, RS, %s, %u, %u, %u, %u, %12.10lf\n", experiment, nproc, packet_size, n_packets, name, rank, result.pid_src, result.pid_dst, result.tag_id, t); 
    }
    
    MPI_Finalize();
    return 0;
}
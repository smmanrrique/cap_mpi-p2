#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <sys/time.h>
#include <time.h>


#ifndef PACKET_SIZE 
    #define PACKET_SIZE 5
#endif

#ifndef N_BOUNCES 
    #define N_BOUNCES 3
#endif

struct Pair { 
    int pid_src, pid_dst, tag_id; 
}; 

double t, t1, t2;

struct Pair get_process(int rank, int nproc) {
    struct Pair pids;
    pids.pid_src = rank%nproc;
    pids.pid_dst = (rank+nproc/2)%nproc;
    pids.tag_id  = pids.pid_src + pids.pid_dst;
    // printf("SRC: %u DST: %u TAG %u\n", pids.pid_src, pids.pid_dst, pids.tag_id);
    return pids;
}

int main( int argc, char* argv[] ) {
    struct Pair result;    
    int rank, nproc, send_ack, recv_ack;

    int packet_size = PACKET_SIZE;
    int n_bounces   = N_BOUNCES;

    if (argc >= 2) {
        packet_size = atoi(argv[1]);
        n_bounces   = atoi(argv[2]);
    }

    MPI_Init( &argc, &argv );
    MPI_Comm_size( MPI_COMM_WORLD, &nproc ); // number of process
    MPI_Comm_rank( MPI_COMM_WORLD, &rank );  // PID

    result = get_process(rank, nproc);

    if (rank < result.pid_dst) {
        MPI_Status status;

        int send_packets[packet_size];
        int recv_packets[packet_size];
        int n_packets_ping = 0;

        while (n_packets_ping < n_bounces) {
            // printf("n_packets_ping: %d\n", n_packets_ping);
            t1 = MPI_Wtime();
            MPI_Send(send_packets, packet_size, MPI_INT, result.pid_dst, result.tag_id, MPI_COMM_WORLD);
            // printf("%d sent to %d\n", rank, result.pid_dst);
            MPI_Recv(recv_packets, packet_size, MPI_INT, result.pid_dst, result.tag_id, MPI_COMM_WORLD, &status);
            // printf("\twaiting for %d...\n", result.pid_dst);
            t2 = MPI_Wtime();
            t  = t2 - t1;
            // printf("Bounce time: %f [secs]\n", t);
            printf("%d, %d, SR, %u, %u, %u, %f\n", packet_size, n_bounces, result.pid_src, result.pid_dst, result.tag_id, t);
            n_packets_ping++;
        }
        
    } else  {        
        MPI_Status status;

        int send_packets[packet_size];
        int recv_packets[packet_size];
        int n_packets_pong = 0;        

        while(n_packets_pong < n_bounces) {
            // printf("n_packets_pong: %d\n", n_packets_pong);
            t1 = MPI_Wtime();
            MPI_Recv(recv_packets, packet_size, MPI_INT, result.pid_dst, result.tag_id, MPI_COMM_WORLD, &status);
            // printf("%d received from %d\n", rank, result.pid_dst);
            MPI_Send(send_packets, packet_size, MPI_INT, result.pid_dst, result.tag_id, MPI_COMM_WORLD);
            // printf("\twaiting for %d...\n", result.pid_dst);
            t2 = MPI_Wtime();
            t  = t2 - t1;
            // printf("Bounce time: %f [secs]\n", t);
            printf("%d, %d, RS, %u, %u, %u, %f\n", packet_size, n_bounces, result.pid_src, result.pid_dst, result.tag_id, t);
            n_packets_pong++;
        }
    }

    MPI_Finalize();
    return 0;
}
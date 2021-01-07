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

long double t, t1, t2;
double tstart, tend, ttotal;
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
    int n_bounces   = N_BOUNCES;

    if (argc >= 2) {
        experiment  = atoi(argv[1]);
        packet_size = atoi(argv[2]);
        n_bounces   = atoi(argv[3]);
    }

    MPI_Init( &argc, &argv );
    MPI_Comm_size( MPI_COMM_WORLD, &nproc ); // number of process
    MPI_Comm_rank( MPI_COMM_WORLD, &rank );  // PID
    MPI_Get_processor_name( name, &resultlen );

    result = get_process(rank, nproc);
    
    if (rank < result.pid_dst) {
        MPI_Status status;

        void *send_packets = malloc(packet_size);
        void *recv_packets = malloc(packet_size);
        int n_packets_ping = 0;

        tstart = get_clock();
        ttotal = 0;
        while (n_packets_ping < n_bounces) {
            t1 = MPI_Wtime();
            MPI_Send(send_packets, packet_size, MPI_BYTE, result.pid_dst, result.tag_id, MPI_COMM_WORLD);
            MPI_Recv(recv_packets, packet_size, MPI_BYTE, result.pid_dst, result.tag_id, MPI_COMM_WORLD, &status);
            t2 = MPI_Wtime();
            t  = (t2 - t1);
            n_packets_ping++;
            printf("%d,,%d, %d,,%d, SR, %s, %u, %u, %u, %u, %lf, \n", experiment, nproc, packet_size, n_bounces, name, rank, result.pid_src, result.pid_dst, result.tag_id, t);
        }
        tend   = get_clock();
        ttotal = (tend - tstart);
        printf("%d,,%d, %d,,%d, RS, %s, %u, %u, %u, %u, %lf, %12.10lf\n", experiment, nproc, packet_size, n_bounces, name, rank, result.pid_src, result.pid_dst, result.tag_id, t, ttotal);
    
    } else  {       
        MPI_Status status;

        void *send_packets = malloc(packet_size);
        void *recv_packets = malloc(packet_size);
        int n_packets_pong = 0;        

        tstart = get_clock();
        ttotal = 0;
        while(n_packets_pong < n_bounces) {
            t1 = MPI_Wtime();
            MPI_Recv(recv_packets, packet_size, MPI_BYTE, result.pid_dst, result.tag_id, MPI_COMM_WORLD, &status);
            MPI_Send(send_packets, packet_size, MPI_BYTE, result.pid_dst, result.tag_id, MPI_COMM_WORLD);
            t2 = MPI_Wtime();
            t  = (t2 - t1);
            n_packets_pong++;
            printf("%d,,%d, %d,,%d, RS, %s, %u, %u, %u, %u, %f, \n", experiment, nproc, packet_size, n_bounces, name, rank, result.pid_src, result.pid_dst, result.tag_id, t);
        }
        tend   = get_clock();
        ttotal = (tend - tstart);
        printf("%d,,%d, %d,,%d, RS, %s, %u, %u, %u, %u, %f, %12.10lf\n", experiment, nproc, packet_size, n_bounces, name, rank, result.pid_src, result.pid_dst, result.tag_id, t, ttotal);
    }
    MPI_Finalize();
    return 0;
}
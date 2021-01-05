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
    // printf("SRC: %u DST: %u TAG %u\n", pids.pid_src, pids.pid_dst, pids.tag_id);
    return pids;
}

int main( int argc, char* argv[] ) {
    struct Pair result;    
    int rank, nproc, send_ack, recv_ack, resultlen;
    char name[MPI_MAX_PROCESSOR_NAME];

    int packet_size = PACKET_SIZE;
    int n_packets   = N_PACKETS;

    if (argc >= 2) {
        packet_size = atoi(argv[1]);
        n_packets   = atoi(argv[2]);
    }

    int packet_size_bits = packet_size*16;
    int send_packets[packet_size];
    int recv_packets[packet_size];

    MPI_Init( &argc, &argv );
    MPI_Comm_size( MPI_COMM_WORLD, &nproc ); // number of process
    MPI_Comm_rank( MPI_COMM_WORLD, &rank );  // PID
    MPI_Get_processor_name( name, &resultlen );

    result = get_process(rank, nproc);

    tstart = get_clock();
    if (rank < result.pid_dst) {
        MPI_Status status;       

        t1 = MPI_Wtime();
        for (int i=0; i<n_packets; i++) {
            MPI_Send(send_packets, packet_size, MPI_INT, result.pid_dst, result.tag_id, MPI_COMM_WORLD);
            // printf("\t%d sent to %d (size: %d)\n", rank, result.pid_dst, packet_size);
        }
        MPI_Recv(&recv_ack, 1, MPI_INT, result.pid_dst, result.tag_id, MPI_COMM_WORLD, &status);
        t2 = MPI_Wtime();
        t  = (t2 - t1);
        // printf("\tAll packets were received from %d. Ack: %d. Time: %f [sec]\n", result.pid_src, recv_ack, t);
        printf(",%d, %d,,,%s, %u, %u, %u, %u, %f", packet_size_bits, n_packets, name, rank, result.pid_src, result.pid_dst, result.tag_id, t);
    } else  {
        MPI_Status status;
        send_ack = 9;

        t1 = MPI_Wtime();
        int i = 1;
        while (i<n_packets+1) {
            MPI_Recv(recv_packets, packet_size, MPI_INT, result.pid_dst, result.tag_id, MPI_COMM_WORLD, &status);
            // printf("\tNum:%d. %d received to %d (size: %d)\n", i, rank, result.pid_src, packet_size); 
            i++;
        }
        MPI_Send(&send_ack, 1, MPI_INT, result.pid_dst, result.tag_id, MPI_COMM_WORLD);
        t2 = MPI_Wtime();
        t  = (t2 - t1);
        // printf("\tAll packets were received from %d. Ack: %d. Time: %f [sec]\n", result.pid_src, recv_ack, t);  
        printf(",%d, %d,,, %s, %u, %u, %u, %u, %f", packet_size_bits, n_packets, name, rank, result.pid_src, result.pid_dst, result.tag_id, t); 
    }
    tend   = get_clock();
	ttotal = (tend - tstart);
    printf(",%12.10lf\n",ttotal);

    MPI_Finalize();
    return 0;
}
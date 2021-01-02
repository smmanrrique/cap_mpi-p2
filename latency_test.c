#include <stdio.h>
#include <mpi.h>
#include <sys/time.h>
#include <time.h>


#ifndef PACKET_SIZE 
    #define PACKET_SIZE 5
#endif

#ifndef N_BOUNDS 
    #define N_BOUNDS 3
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
    return pids;
}

int main( int argc, char* argv[] ) {
    struct Pair result;    
    int rank, nproc;
    int send_ack, recv_ack;

    MPI_Init( &argc, &argv );
    MPI_Comm_size( MPI_COMM_WORLD, &nproc ); // number of process
    MPI_Comm_rank( MPI_COMM_WORLD, &rank );  // PID

    result = get_process(rank, nproc);

    if(rank < result.pid_dst) {
        MPI_Status status;

        int send_packets[PACKET_SIZE];
        int recv_packets[PACKET_SIZE];
        int n_packets_ping = 0;

        while (n_packets_ping < N_BOUNDS){
            t1 = MPI_Wtime();
            MPI_Send(send_packets, PACKET_SIZE, MPI_INT, result.pid_dst, result.tag_id, MPI_COMM_WORLD);
            printf("%d sent to %d\n", rank, result.pid_dst);
            MPI_Recv(recv_packets, PACKET_SIZE, MPI_INT, result.pid_dst, result.tag_id, MPI_COMM_WORLD, &status);
            printf("\twaiting for %d...\n", result.pid_dst);
            t2 = MPI_Wtime();
            t  = t2 - t1;
            printf("Bounce time: %f [secs]\n", t);
            n_packets_ping++;
        }
        
    } else  {        
        MPI_Status status;

        int send_packets[PACKET_SIZE];
        int recv_packets[PACKET_SIZE];
        int n_packets_pong = 0;        

        while(n_packets_pong < N_BOUNDS){
            t1 = MPI_Wtime();
            MPI_Recv(recv_packets, PACKET_SIZE, MPI_INT, result.pid_dst, result.tag_id, MPI_COMM_WORLD, &status);
            printf("%d received from %d\n", rank, result.pid_dst);
            MPI_Send(send_packets, PACKET_SIZE, MPI_INT, result.pid_dst, result.tag_id, MPI_COMM_WORLD);
            printf("\twaiting for %d...\n", result.pid_dst);
            t2 = MPI_Wtime();
            t  = t2 - t1;
            printf("Bounce time: %f [secs]\n", t);
            n_packets_pong++;
        }
    }

    MPI_Finalize();
    return 0;
}
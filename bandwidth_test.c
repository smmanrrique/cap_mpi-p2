#include <stdio.h>
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

static int send_packets[PACKET_SIZE];
static int recv_packets[PACKET_SIZE];

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
    int rank, nproc, send_ack, recv_ack;

    MPI_Init( &argc, &argv );
    MPI_Comm_size( MPI_COMM_WORLD, &nproc ); // number of process
    MPI_Comm_rank( MPI_COMM_WORLD, &rank );  // PID

    result = get_process(rank, nproc);

    if(rank < result.pid_dst) {
        MPI_Status status;        
        t1 = MPI_Wtime();
        for (int i=0; i<N_PACKETS; i++) {
            MPI_Send(send_packets, PACKET_SIZE, MPI_INT, result.pid_dst, result.tag_id, MPI_COMM_WORLD);
            printf("\t%d sent to %d (size: %d)\n", rank, result.pid_dst, PACKET_SIZE);
        }
        MPI_Recv(&recv_ack, 1, MPI_INT, result.pid_dst, result.tag_id, MPI_COMM_WORLD, &status);
        t2 = MPI_Wtime();
        t  = t2 - t1;
        printf("\tAll packets were received from %d. Ack: %d. Time: %f [sec]\n", result.pid_src, recv_ack, t);

    } else  {
        MPI_Status status;
        send_ack = 9;

        t1 = MPI_Wtime();
        int i = 1;
        while (i<N_PACKETS+1) {
            MPI_Recv(recv_packets, PACKET_SIZE, MPI_INT, result.pid_dst, result.tag_id, MPI_COMM_WORLD, &status);
            printf("\tNum:%d. %d received to %d (size: %d)\n", i, rank, result.pid_src, PACKET_SIZE); 
            i++;
        }
        MPI_Send(&send_ack, 1, MPI_INT, result.pid_dst, result.tag_id, MPI_COMM_WORLD);
        t2 = MPI_Wtime();
        t  = t2 - t1;
        // printf("\tAll packets were received from %d. Ack: %d. Time: %f [sec]\n", result.pid_src, recv_ack, t);   
    }

    MPI_Finalize();
    return 0;
}
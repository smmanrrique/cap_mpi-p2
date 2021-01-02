/* filename: hello_word_mpi.c */
#include<stdio.h>
#include<mpi.h>

int main( int argc, char* argv[] ) {

    int i, psrc, pdst, rank, nproc;
    float buffs[400];
    float buffr[400];
    MPI_Status status;

    /* Aqui comienza el paralelismo */
    MPI_Init( &argc, &argv );
    MPI_Comm_size( MPI_COMM_WORLD, &nproc ); // number of process
    MPI_Comm_rank( MPI_COMM_WORLD, &rank ); // PID

    psrc = (rank+1)%nproc;
    pdst = (rank+nproc/2 + 1)%nproc;

    // ring
    for(i = 0; i<nproc/2;i++){
        buffs[0] = 3.544;
        // , 5.544, 6.544, 6.544}
        MPI_Send(buffs, 400, MPI_FLOAT, pdst, 1, MPI_COMM_WORLD);
        MPI_Recv(buffr, 400, MPI_FLOAT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
    }

    if(rank % 2 == 0 ){
        printf( "SEND  %d --> %d  \n ", rank, pdst );
    }else{
        printf( "RECIVE => %d [%d]\n ", rank, &buffr[0] );
    }
    // int i, psrc, pdst, rank, nproc;

    // int isbuf, irbuf;
    // MPI_Status status;

    // MPI_Init( &argc, &argv );
    // MPI_Comm_size( MPI_COMM_WORLD, &nproc );
    // MPI_Comm_rank( MPI_COMM_WORLD, &rank );

    // psrc = (rank+1)%nproc;
    // pdst = (rank+nproc/2 + 1)%nproc;

    // ring
    // for(i = 0; i<nproc;i++){
    //     if(rank % 2 == 0 ){
    //         isbuf = pdst;
    //         MPI_Send( &isbuf, 1, MPI_INTEGER, pdst, 1, MPI_COMM_WORLD);
    //         printf( "SEND --> %d VALUE %d \n ", pdst, irbuf );
    //     }else{
    //         MPI_Recv( &irbuf, 1, MPI_INTEGER, psrc, 1, MPI_COMM_WORLD, &status);
    //         printf( "recibi  %d proceso %d \n ", irbuf, rank );
    //     }

    // }

    // if(rank == 0) {
    // isbuf = 9;
    // MPI_Send( &isbuf, 1, MPI_INTEGER, 1, 1, MPI_COMM_WORLD);
    // } else if(rank == 1) {
    // MPI_Recv( &irbuf, 1, MPI_INTEGER, 0, 1, MPI_COMM_WORLD,
    // &status);
    // printf( " %d\n", irbuf );
    // }

    printf( "Hello word! I'm process nro. %d over a total of %d process.\n", rank, nproc );

    MPI_Finalize();
    return 0;
}
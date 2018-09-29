#include<mpi.h>
#include<stdio.h>

/*
comandos:

cd .ssh
ssh-keygen -t rsa
arquivo: id_rsa
enter duas vezes
cat id_rsa.pub > authorized_keys

ssh sem senha:
cd .ssh
scp id_rsa.pub ip:~/.ssh/
scp authorized_keys ip:~/.ssh/

*/

int main(int argc, char **argv){

    int rank, size;
    
    MPI_Init(&argc, &argv);
    
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    
    printf("Hello world! I'm %d of %d\n", rank, size);
    
    MPI_Finalize();
    return 0;

}

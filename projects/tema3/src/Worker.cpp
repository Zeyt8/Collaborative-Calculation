#include "Worker.h"

Worker::Worker(int rank) : Process(rank)
{
}

Worker::~Worker()
{
}

void Worker::Start()
{
    // Receive coordinator
    MPI_Recv(&coordinator, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    // Receive rank
    MPI_Recv(&rank, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    // Receive topology. Coordinator sends j's workers
    for (int j = 0; j < NR_COORDINATORS; j++)
    {
        ReceiveTopology(j);
    }
    PrintTopology();
    // Receive V
    MPI_Status status;
    MPI_Probe(coordinator, 0, MPI_COMM_WORLD, &status);
    int count;
    MPI_Get_count(&status, MPI_INT, &count);
    V = new int[count];
    MPI_Recv(V, count, MPI_INT, coordinator, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    // Compute new V
    for (int i = 0; i < count; i++)
    {
        V[i] = V[i] * 5;
    }
    // Send V to coordinator
    SendMessage(V, count, MPI_INT, coordinator, 0);
}

void Worker::ReceiveTopology(int fromTag)
{
    // Probe for size
    MPI_Status status;
    MPI_Probe(coordinator, fromTag, MPI_COMM_WORLD, &status);
    int count;
    MPI_Get_count(&status, MPI_INT, &count);
    // Add to topology
    topology[fromTag].resize(count);
    MPI_Recv(&topology[fromTag][0], count, MPI_INT, coordinator, fromTag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
}

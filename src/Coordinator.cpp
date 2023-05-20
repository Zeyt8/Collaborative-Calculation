#include "Coordinator.h"
#include <algorithm>
#include <fstream>

int mod(int a, int b)
{
    return (a % b + b) % b; 
}

Coordinator::Coordinator(int rank, int error) : Process(rank)
{
    if (error == 1 && rank == 1)
    {
        leftCoordinator = -1;
    }
    else
    {
        leftCoordinator = mod(rank - 1, NR_COORDINATORS);
    }
    if (error == 1 && rank == 0)
    {
        rightCoordinator = -1;
    }
    else
    {
        rightCoordinator = mod(rank + 1, NR_COORDINATORS);
    }
    totalWorkers = 0;
    this->error = error;
}

Coordinator::~Coordinator()
{
}

void Coordinator::Start()
{
    ReadWorkers();
    // Add own workers to topology
    topology[rank].insert(topology[rank].end(), workers.begin(), workers.end());
    // Send own workers to right coordinator
    if (rightCoordinator != -1)
    {
        SendMessage(&topology[rank][0], topology[rank].size(), MPI_INT, rightCoordinator, rank);
    }
    // Send own workers to left coordinator
    if (error == 1 && leftCoordinator != -1)
    {
        SendMessage(&topology[rank][0], topology[rank].size(), MPI_INT, leftCoordinator, rank);
    }
    // Receive workers from coordinator
    for (int i = 0; i < NR_COORDINATORS; i++)
    {
        if (i == rank)
            continue;
        // Receive workers from left coordinator
        if (error == 0 || (leftCoordinator != -1 && isToLeftWithError(rank, i)))
        {
            ReceiveWorkers(i, leftCoordinator);
            // Send workers to right coordinator
            if (rightCoordinator != -1)
            {
                SendMessage(&topology[i][0], topology[i].size(), MPI_INT, rightCoordinator, i);
            }
        }
        // If error and is to the right in topology and has right coordinator
        if (error == 1 && rightCoordinator != -1 && isToRightWithError(rank, i))
        {
            ReceiveWorkers(i, rightCoordinator);
            // Send workers to left coordinator
            if (leftCoordinator != -1)
            {
                SendMessage(&topology[i][0], topology[i].size(), MPI_INT, leftCoordinator, i);
            }
        }
    }
    PrintTopology();
    for (int i = 0; i < NR_COORDINATORS; i++)
    {
        totalWorkers += topology[i].size();
    }
    // Send topology to workers
    for (uint32_t i = 0; i < workers.size(); i++)
    {
        // Send own rank
        SendMessage(&rank, 1, MPI_INT, workers[i], 0);
        // Send worker rank
        SendMessage(&workers[i], 1, MPI_INT, workers[i], 0);
        // Send topology
        for (int j = 0; j < NR_COORDINATORS; j++)
        {
            SendMessage(&topology[j][0], topology[j].size(), MPI_INT, workers[i], j);
        }
    }
    if (rank != 0)
    {
        Compute();
    }
}

void Coordinator::ScatterV(int size)
{
    // Send V to workers
    int workerChunk = size / workers.size();
    if (size % workers.size() != 0)
    {
        workerChunk++;
    }
    for (uint32_t i = 0; i < workers.size(); i++)
    {
        int sizeLeft = size - i * workerChunk;
        SendMessage(V + i * workerChunk, std::min(workerChunk, sizeLeft), MPI_INT, workers[i], 0);
    }
}

void Coordinator::ReadWorkers()
{
    std::ifstream file("cluster" + std::to_string(rank) + ".txt");
    int n;
    file >> n;
    for (int i = 0; i < n; i++)
    {
        int w;
        file >> w;
        workers.push_back(w);
    }
    file.close();
}

void Coordinator::ReceiveWorkers(int fromTag, int from)
{
    // Probe for size
    MPI_Status status;
    MPI_Probe(from, fromTag, MPI_COMM_WORLD, &status);
    int count;
    MPI_Get_count(&status, MPI_INT, &count);
    // Add to topology
    topology[fromTag].resize(count);
    MPI_Recv(&topology[fromTag][0], count, MPI_INT, from, fromTag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
}

void Coordinator::Compute()
{
    // Receive V from right coordinator
    MPI_Status status;
    MPI_Probe(rightCoordinator, 0, MPI_COMM_WORLD, &status);
    int count;
    MPI_Get_count(&status, MPI_INT, &count);
    V = new int[count];
    MPI_Recv(V, count, MPI_INT, rightCoordinator, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    // Calculate chunk
    int chunk = 0;
    if (rank == 3)
    {
        chunk = count * workers.size() / (totalWorkers - topology[0].size());
        // Send rest of V to left coordinator
        SendMessage(V + chunk, count - chunk, MPI_INT, leftCoordinator, 0);
    }
    else if (rank == 2)
    {
        chunk = count * workers.size() / (totalWorkers - topology[0].size() - topology[3].size());
        // Send rest of V to left coordinator
        SendMessage(V + chunk, count - chunk, MPI_INT, leftCoordinator, 0);
    }
    else if (rank == 1)
    {
        chunk = count;
    }
    ScatterV(chunk);
    // Receive V from workers
    int index = 0;
    for (uint32_t i = 0; i < workers.size(); i++)
    {
        MPI_Probe(workers[i], 0, MPI_COMM_WORLD, &status);
        MPI_Get_count(&status, MPI_INT, &count);
        MPI_Recv(V + index, count, MPI_INT, workers[i], 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        index += count;
    }
    // Send V to right coordinator
    SendMessage(V, index, MPI_INT, rightCoordinator, rank);
    // Coordinator 3 and 2 receive V from coordinators to the left and send it to the right
    for (int i = rank - 1; i >= 1; i--)
    {
        MPI_Probe(leftCoordinator, i, MPI_COMM_WORLD, &status);
        MPI_Get_count(&status, MPI_INT, &count);
        int V2[count];
        MPI_Recv(V2, count, MPI_INT, leftCoordinator, i, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        SendMessage(V2, count, MPI_INT, rightCoordinator, i);
    }
}

bool Coordinator::isToRightWithError(int self, int other)
{
    if (self == 0)
    {
        return false;
    }
    if (other > self || other == 0)
    {
        return true;
    }
    return false;
}

bool Coordinator::isToLeftWithError(int self, int other)
{
    if (other == 0)
    {
        return false;
    }
    if (self > other || self == 0)
    {
        return true;
    }
    return false;
}

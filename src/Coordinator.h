#pragma once
#include "Process.h"

#define NR_COORDINATORS 4

class Coordinator : public Process
{
protected:
    int leftCoordinator;
    int rightCoordinator;
    std::vector<int> workers;
    int totalWorkers = 0;
    int* V;
    int error;
public:
    Coordinator(int rank, int error);
    ~Coordinator();
    void Start() override;
protected:
    void ScatterV(int size);
private:
    void ReadWorkers();
    
    /// @brief Receive fromTag's workers from left coordinator
    /// @param fromTag The tag whose workers to receive
    /// @param from Coordinator to receive from (left or right)
    void ReceiveWorkers(int fromTag, int from);

    /// @brief Function that only gets called for non Leader Coordinators during Start
    void Compute();

    /// @brief Checks if coordinator is to the right in the topology with error
    /// @param self self rank
    /// @param other other coordinator rank
    /// @return true if it is to the right
    bool isToRightWithError(int self, int other);

    /// @brief Checks if coordinator is to the left in the topology with error
    /// @param self self rank
    /// @param other other coordinator rank
    /// @return true if it is to the left
    bool isToLeftWithError(int self, int other);
};
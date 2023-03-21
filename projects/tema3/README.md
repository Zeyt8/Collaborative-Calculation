# Collaborative calculation in distributed systems

We have created three types of processes: Worker, Coordinator, and Leader. In the main function, an instance of the appropriate class is created based on the rank, and the start function is called on that instance.

The ring traversed to the right has the following order: 0 -> 1 -> 2 -> 3 -> 0

## Establishing the Topology

Each process has an array of vectors in which it stores the topology.

Coordinators establish their neighbors and read from their workers' files to add them to a list of workers.

The idea we started with is for each coordinator to send its list of workers to the right to make a complete round of the ring.

The coordinator sends its list of workers to the neighbor to its right. Then it waits to receive a list of workers from each coordinator other than itself with a tag equal to the rank of the coordinator whose workers they are.

After receiving the list, it updates the topology and sends the list of workers further to the neighbor to the right.

This process ends when a coordinator receives its own list of workers.

After a coordinator has the complete topology, it displays it on the console and starts communicating with its workers. It sends three messages to each of them:

- The rank of the coordinator
- The rank of the worker it's communicating with
- The topology

Workers display the topology on the console.

## Performing Calculations

The leader creates vector V and calculates chunk, which represents the number of numbers that each worker needs to calculate.

The leader keeps chunk * the number of workers in its cluster of numbers. It sends the rest of the vector to the coordinator on its left.

The coordinator receives the vector and does the same thing. chunk is recalculated, but only considering the remaining workers.

The last coordinator keeps everything it receives.

Each coordinator sends its workers a piece of the vector it has kept.

A worker calculates the vector multiplied element by element by 5 and sends it back to the coordinator.

The coordinator receives the vector from each worker in the same order in which it was sent.

After receiving the vector from all workers, the coordinator sends it to the coordinator on its right, who receives it and sends it further until it reaches the leader.

The leader receives the vector in the same order in which it was sent and displays it on the console.

## Handling Communication Channel Defects

### Topology

If an error occurs between 0 and 1, the idea of each coordinator sending to the right no longer works.

The idea we started with is for each coordinator to send its list of workers to the left and right to reach the ends.

The coordinator sends the list of workers to the left and right if there is a corresponding neighbor.

A coordinator now listens to both its left and right neighbors to receive the list of workers from all coordinators on its left and right in the new topology.

When it receives a message, it updates the topology and sends it further, preserving the direction of the message.

The messages will stop when they reach one of the ends because they can no longer send further while preserving the direction.

The transmission of the topology to workers works in the same way.

### Calculations

Due to the way the calculations are divided, no modification is required. The data is sent 0 -> 3 -> 2 -> 1 and then back along the reverse path, so an error on the 0-1 link does not affect the calculation.

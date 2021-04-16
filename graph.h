/** Graph.h
 * ===========================================================
 * Name: CS220
 * Modified by: <YOUR NAME HERE>
 * Section:
 * Project: PEX5
 * Purpose: The definition of a graph.
 * ===========================================================
 */

#ifndef GRAPH_H
#define GRAPH_H

#include <stdlib.h>

typedef struct graph {
    int numberVertices;
    void* vertices;  // Array of verticies
    int** edges;     // Adjacency matrix (2D Array)
} Graph;

/** -------------------------------------------------------------------
 * Allocate and initialize memory needed to hold a graph data structure
 * @param numberVertices the number of vertices in the graph
 * @param bytesPerNode the number of bytes used to represent a
 *                     single vertex of the graph - use sizeof()
 * @return a pointer to a graph struct
 */
Graph* graphCreate(int numberVertices, int bytesPerNode);

/** -------------------------------------------------------------------
 * Delete a graph data structure
 * @param graph the graph to delete
 */
void graphDelete(Graph* graph);

/** -------------------------------------------------------------------
 * Set the state of an edge in a graph
 * @param graph the graph to modify
 * @param fromVertex the beginning vertex of the edge
 * @param toVertex the ending vertex of the edge
 * @param state the state of the edge
 */
void graphSetEdge(Graph* graph, int fromVertex, int toVertex, int state);

/** -------------------------------------------------------------------
 * Get the state of an edge in a graph
 * @param graph the graph
 * @param fromVertex the starting vertex of the edge
 * @param toVertex the ending vertex of the edge
 * @return the state of the edge
 */
int graphGetEdge(Graph* graph, int fromVertex, int toVertex);

#endif  // GRAPH_H
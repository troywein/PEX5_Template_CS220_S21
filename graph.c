/** Graph.c
 * ===========================================================
 * Name: CS220
 * Modified by: <YOUR NAME HERE>
 * Section:
 * Project: PEX5
 * Purpose: The implementation of a graph.
 * ===========================================================
 */

#include "graph.h"

#include <math.h>
#include <memory.h>
#include <stdio.h>
#include <stdlib.h>

#include "gameLogic.h"

/** -------------------------------------------------------------------
 * Allocate and initialize memory needed to hold a graph data structure.
 * @param numberVertices the number of vertices in the graph
 * @param bytesPerNode the number of bytes used to represent a
 *                     single vertex of the graph
 * @return a pointer to a graph struct
 */
Graph* graphCreate(int numberVertices, int bytesPerNode) {
    // TODO
    return NULL;
}

/** -------------------------------------------------------------------
 * Delete a graph data structure
 * @param graph the graph to delete
 */
void graphDelete(Graph* graph) {
    // TODO
}

/** -------------------------------------------------------------------
 * Set the state of an edge in a graph
 * @param graph the graph to modify
 * @param fromVertex the beginning vertex of the edge
 * @param toVertex the ending vertex of the edge
 * @param state the state of the edge
 */
void graphSetEdge(Graph* graph, int fromVertex, int toVertex, int state) {
    // TODO
}

/** -------------------------------------------------------------------
 * Get the state of an edge in a graph
 * @param graph the graph
 * @param fromVertex the starting vertex of the edge
 * @param toVertex the ending vertex of the edge
 * @return the state of the edge
 */
int graphGetEdge(Graph* graph, int fromVertex, int toVertex) {
    // TODO
    return -1;
}
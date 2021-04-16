/** gameLogic.h
 * ===========================================================
 * Name: CS220
 * Modified by: <PUT YOUR NAME HERE>
 * Section:
 * Project: PEX5
 * Purpose: The definition of data and functions needed to
 *          implement a Shannon Switching Game.
 * ===========================================================
 */

#ifndef GAME_LOGIC_H
#define GAME_LOGIC_H

#include "Graph.h"
#include "graphics.h"

#define FALSE 0
#define TRUE 1

// The values stored in the edge adjacency matrix
#define NO_EDGE 0      // No edge exists
#define NORMAL_EDGE 1  // Normal edge
#define LOCKED_EDGE 2  // An edge selected by the "SHORT player"

// The types of graph vertices.
#define NORMAL_VERTEX 0
#define STARTING_VERTEX 1
#define ENDING_VERTEX 2

// A single vertex in a graph
typedef struct vertex {
    int x;  // (x,y) location of the vertex in the graphics window
    int y;
    int type;  // NORMAL_VERTEX, STARTING_VERTEX, or ENDING_VERTEX
} Vertex;

// A structure for transferring edge data between functions.
typedef struct edge {
    int fromVertex;
    int toVertex;
} Edge;

// The two players are called "cut" and "short". The "cut" player
// removes edges from the graph. The "short" player tries to select
// edges that create a path from the starting and ending nodes.
#define CUT_PLAYER 0
#define SHORT_PLAYER 1

// Each player's turn can be made from user input (HUMAN_PLAYER)
// or by computer logic (AI_PLAYER).
#define HUMAN_PLAYER 0
#define AI_PLAYER 1

// Game status
#define GAME_UNDERWAY 0
#define SHORT_WINS 1
#define CUT_WINS 2

/** -------------------------------------------------------------------
 * Set the type of player.
 * @param player Either CUT_PLAYER or SHORT_PLAYER
 * @param type Either HUMAN_PLAYER or AI_PLAYER
 */
void setPlayerType(int player, int type);

/** -------------------------------------------------------------------
 * Set the active player. This should be called only once at
 * the start of the game to determine who goes first.
 * @param player Either CUT_PLAYER or SHORT_PLAYER
 */
void setActivePlayer(int player);

/** -------------------------------------------------------------------
 * Create a set of random vertices for the game graph. The location
 * of each vertex must meet certain constraints:
 *    1) It must be inside the game window.
 *    2) It must be "margin" pixels aways from the window borders.
 *    3) It must be "minDistance" away from all other vertices.
 * @param graph the graph to modify
 * @param max_X the width of the window along the x axis
 * @param max_Y the height of the window along the y axis
 * @param margin the number of pixels to keep clear along all borders
 * @param minDistance the minimum distance between any two vertices
 * @param randomize if TRUE, the location of the vertices is random.
 *                  if FALSE, the same graph every time.
 */
void createVertices(Graph *graph, int max_X, int max_Y, int margin, int minDistance, int randomize);

/** -------------------------------------------------------------------
 * Create a set of random edges for a graph that meet the following
 * constraints:
 *   1) Each vertex can have [minNumberEdges, maxNumberEdges] edges
 *   2) Each edge must have a length that is less than
 *      edgeLengthPercent*max(windowWidth, windowHeight)
 * @param graph the graph to modify
 * @param minNumberEdges the minimum number of edges that each vertex must have
 * @param maxNumberEdges the maximum number of edges that each vertex can have
 * @param windowWidth the width of the game window
 * @param windowHeight the height of the game window
 * @param edgeLengthPercent limits the length of edges in the graph
 */
void createEdges(Graph *graph, int minNumberEdges, int maxNumberEdges, int windowWidth,
                 int windowHeight, double edgeLengthPercent);

/** -------------------------------------------------------------------
 * Print the value that define a graph. Great for debuggin!
 * @param graph the graph
 */
void printGraph(Graph *graph);

/** -------------------------------------------------------------------
 * Draw the graph on the window.
 * @param graph the graph
 * @param hwnd handle to the window where the graph is to be drawn
 * @param labelVertices if TRUE, the array index of each vertex is displayed
 */
void drawGraph(Graph *graph, HWND hwnd, int labelVertices);

/** -------------------------------------------------------------------
 * Take one turn in the Shannon Switching game. If the turn was successful,
 * the active player changes to the other player and the window is redrawn.
 * If the turn was not taken, the active player is unchanged and the window
 * is not redrawn.
 * @param graph the graph
 * @param mouseX the location of the mouse (mouseX, mouseY)
 * @param mouseY
 */
void doTurn(Graph *graph, int mouseX, int mouseY);

#endif  // GAME_LOGIC_H
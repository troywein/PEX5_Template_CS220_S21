/** gameLogic.c
 * ===========================================================
 * Name: CS220
 * Modified by: <PUT YOUR NAME HERE>
 * Section:
 * Project: PEX5
 * Purpose: The implementation of functions needed to
 *          create a Shannon Switching Game.
 * ===========================================================
 */

#include "gameLogic.h"

#include <math.h>
#include <stdio.h>
#include <time.h>

#define MAXIMUM_TRIES_TO_SET_LOCATION 1000
#define MAXIMUM_TRIES_TO_CREATE_EDGE 1000
#define MAXIMUM_TRIES_TO_FIND_RANDOM_EDGE 5000
#define VERTEX_DRAW_RADIUS 5
#define CLICK_TOLERANCE 5.0

// The type of the two game players.
static int playerType[2] = {HUMAN_PLAYER, HUMAN_PLAYER};
static char *playerTypeNames[2] = {" (Human)", " (AI)"};

static int activePlayer = CUT_PLAYER;
static char *activePlayerName[] = {"Cut's turn", "Short's turn"};

// Local functions - intentionally not included in the header file
static void setLocation(Vertex *vertex, int width, int height, int margin);
static int distance(Vertex *v1, Vertex *v2);
static int someVerticesTooClose(Vertex *vertices, int last, int minimumDistance);
static int compareVertices(const void *e1, const void *e2);
static int degree(Graph *graph, int index);
static void findEdge(Graph *graph, int mouseX, int mouseY, Edge *edge);
static int humanPlayer(Graph *graph, int mouseX, int mouseY);
static int aiPlayer(Graph *graph);

// Vertices are sorted according to their distance from the origin (0,0)
static Vertex origin = {0, 0, 0};

void setPlayerType(int player, int type) {
    if ((player == CUT_PLAYER || player == SHORT_PLAYER) &&
        (type == HUMAN_PLAYER || type == AI_PLAYER)) {
        playerType[player] = type;
    }
}

void setActivePlayer(int player) {
    if (player == CUT_PLAYER || player == SHORT_PLAYER) {
        activePlayer = player;
    }
}

void createVertices(Graph *graph, int max_X, int max_Y, int margin, int minDistance,
                    int randomize) {
    // Create a local variable to point to the vertex array to simplify the code.
    Vertex *vertices = (Vertex *)graph->vertices;

    if (randomize) {
        // Use the current time to randomize the random number generator.
        time_t t;
        srand((unsigned)time(&t));
    }

    // Set a random location for the first vertex.
    setLocation(vertices, max_X, max_Y, margin);

    // Set a random location for all the other vertices, but make sure they are
    // not too close to any of the other vertices that have already been created.
    int numberTries;
    int tooClose;
    for (int j = 1; j < graph->numberVertices; j++) {
        numberTries = 0;
        tooClose = TRUE;
        while (tooClose && numberTries < MAXIMUM_TRIES_TO_SET_LOCATION) {
            numberTries++;
            setLocation(vertices + j, max_X, max_Y, margin);

            tooClose = someVerticesTooClose(vertices, j, minDistance);
        }
        if (numberTries >= MAXIMUM_TRIES_TO_SET_LOCATION) {
            printf("ERROR in createVertices. Too many tries to create a new vertex.");
            exit(1);
        }
    }

    // Sort the vertices based on their distance from the origin (0,0)
    qsort(graph->vertices, (size_t)graph->numberVertices, sizeof(Vertex), compareVertices);

    // Make the first and last vertex the starting and ending vertices
    // of the Shannon switching game.
    vertices[0].type = STARTING_VERTEX;
    vertices[graph->numberVertices - 1].type = ENDING_VERTEX;
}

/** -------------------------------------------------------------------
 * Set the location of a vertex.
 * @param vertex the vertex to change
 * @param width the width of the window
 * @param height the height of the window
 * @param margin the margins around the borders
 */
void setLocation(Vertex *vertex, int width, int height, int margin) {
    vertex->x = (rand() % (width - (2 * margin))) + margin;
    vertex->y = (rand() % (height - (2 * margin))) + margin;
    vertex->type = NORMAL_VERTEX;
}

/** -------------------------------------------------------------------
 * Check to verify that a vertex is not too close to any of the other
 * vertices in the graph.
 * @param vertices the array of vertices
 * @param last the vertex to check
 * @param minimumDistance the minimum distance between two vertices
 * @return TRUE if the distance between vertices[last] and every
 *         other vertex is greater than minimumDistance.
 *         return FALSE otherwise.
 */
int someVerticesTooClose(Vertex *vertices, int last, int minimumDistance) {
    // Check to make sure vertices[last] is not too close to other vertices
    Vertex *lastVertex = vertices + last;
    int k = 0;
    while (k < last && distance(lastVertex, vertices + k) > minimumDistance) {
        k++;
    }
    return k < last;  // TRUE if one vertex is less than minimumDistance away.
}

/** -------------------------------------------------------------------
 * Calculate the distance between two vertices
 * @param v1 vertex 1
 * @param v2 vertex 2
 * @return the distance between the two vertices
 */
int distance(Vertex *v1, Vertex *v2) {
    double dx = (double)v1->x - (double)v2->x;
    double dy = (double)v1->y - (double)v2->y;
    return (int)(sqrt(dx * dx + dy * dy));
}

/** -------------------------------------------------------------------
 * Compare two vertices. This is used by qsort to sort the vertices array.
 * @param e1 a pointer to a vertex
 * @param e2 a pointer to a vertex
 * @return -1 if e1 < e1; 0 if e1 == e2, +1 if e1 > e2
 */
int compareVertices(const void *e1, const void *e2) {
    Vertex *v1 = (Vertex *)e1;
    Vertex *v2 = (Vertex *)e2;
    int dist1 = distance(v1, &origin);
    int dist2 = distance(v2, &origin);
    if (dist1 < dist2)
        return -1;
    else if (dist1 == dist2)
        return 0;
    else
        return 1;
}

void createEdges(Graph *graph, int minNumberEdges, int maxNumberEdges, int windowWidth,
                 int windowHeight, double edgeLengthPercent) {
    Vertex *vertices = (Vertex *)graph->vertices;

    // The number of edges for each vertex is random within a range
    int edgeRange = (maxNumberEdges - minNumberEdges) + 1;
    int existingEdges;
    int desiredEdges;
    int edgesNeeded;
    int toVertex;

    // Restrict the edges to the close vertices by restricting the length
    // of an edge.
    int maxEdgeLength;
    if (windowWidth > windowHeight) {
        maxEdgeLength = (int)(windowWidth * edgeLengthPercent);
    } else {
        maxEdgeLength = (int)(windowHeight * edgeLengthPercent);
    }

    for (int nodeIndex = 0; nodeIndex < graph->numberVertices; nodeIndex++) {
        existingEdges = degree(graph, nodeIndex);
        desiredEdges = (rand() % edgeRange) + minNumberEdges;
        edgesNeeded = desiredEdges - existingEdges;
        for (int j = 0; j < edgesNeeded; j++) {
            toVertex = nodeIndex;
            int tries = 0;
            while (tries < MAXIMUM_TRIES_TO_CREATE_EDGE && toVertex == nodeIndex) {
                toVertex = rand() % graph->numberVertices;
                if (toVertex != nodeIndex) {
                    int existingEdge = graphGetEdge(graph, nodeIndex, toVertex);
                    if (existingEdge == NO_EDGE &&
                        distance(vertices + nodeIndex, vertices + toVertex) <= maxEdgeLength) {
                        break;
                    }
                }
                tries++;
            }
            if (tries > MAXIMUM_TRIES_TO_CREATE_EDGE) {
                printf("Error in createEdges. The maximum number of tries was exceeded.\n");
                exit(1);
            }
            graphSetEdge(graph, nodeIndex, toVertex, NORMAL_EDGE);
        }
    }
}

/** -------------------------------------------------------------------
 * Calculate the number of edges that are connected to a specific vertex.
 * @param graph the graph
 * @param index which vertex, an array index [0, graph->numberNodes-1]
 * @return the number of edges leaving vertex[index]
 */
int degree(Graph *graph, int index) {
    int count = 0;
    for (int j = 0; j < graph->numberVertices; j++) {
        if (graph->edges[index][j] > NO_EDGE) {
            count++;
        }
    }
    return count;
}

void printGraph(Graph *graph) {
    Vertex *vertices = (Vertex *)graph->vertices;
    for (int j = 0; j < graph->numberVertices; j++) {
        printf("VERTEX %3d:   X=%4d    Y=%4d  TYPE=%1d\n", j, vertices[j].x, vertices[j].y,
               vertices[j].type);
    }

    for (int j = 0; j < graph->numberVertices; j++) {
        printf("FROM %d   TO: ", j);
        for (int k = 0; k < graph->numberVertices; k++) {
            printf(" %3d", graph->edges[j][k]);
        }
        printf("\n");
    }
}

void drawGraph(Graph *graph, HWND hwnd, int labelVertices) {
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(hwnd, &ps);
    int brush;
    int pen;
    char textString[40];
    Vertex *vertices = (Vertex *)graph->vertices;

    clearBackground(hwnd, hdc, SOLID_THIN_WHITE, MY_WHITE_BRUSH);

    // Display the player's name and the player's type in the upper-left corner.
    strcpy(textString, activePlayerName[activePlayer]);
    strcat(textString, playerTypeNames[playerType[activePlayer]]);
    drawText(hdc, 3, 3, textString);

    // Draw the edges
    for (int from = 0; from < graph->numberVertices; from++) {
        for (int to = from + 1; to < graph->numberVertices; to++) {
            if (graph->edges[from][to] > NO_EDGE) {
                if (graph->edges[from][to] == LOCKED_EDGE) {
                    // Draw a locked edge
                    pen = SOLID_THICK_RED;
                } else {
                    // Draw a normal edge
                    pen = SOLID_THIN_BLACK;
                }
                drawLine(hdc, vertices[from].x, vertices[from].y, vertices[to].x, vertices[to].y,
                         pen);
            }
        }
    }

    // Draw the vertices
    for (int j = 0; j < graph->numberVertices; j++) {
        if (vertices[j].type == NORMAL_VERTEX) {
            pen = SOLID_THIN_BLACK;
            brush = MY_BLACK_BRUSH;
        } else {  // vertices[j].type == STARTING_VERTEX
            pen = SOLID_THIN_GREEN;
            brush = MY_GREEN_BRUSH;
        }

        drawCircle(hdc, vertices[j].x, vertices[j].y, VERTEX_DRAW_RADIUS, pen, brush);

        if (labelVertices) {
            sprintf(textString, "%d", j);
            drawText(hdc, vertices[j].x - 20, vertices[j].y - 20, textString);
        }
    }
    EndPaint(hwnd, &ps);
}

/** -------------------------------------------------------------------
 * Determine if a mouse click has selected a specific graph edge
 * @param x1 the starting coordinate (x1,y1) of the edge
 * @param y1
 * @param x2 the ending coordinate (x2,y2) of the edge
 * @param y2
 * @param mouseX the location of the mouse (mouseX, mouseY)
 * @param mouseY
 * @return TRUE if the mouse location is within CLICK_TOLERANCE pixels
 *         of any part of the edge. Return FALSE otherwise.
 */
int edgeClicked(int x1, int y1, int x2, int y2, int mouseX, int mouseY) {
    double d = 10000.0;
    double onX, onY;  // The projection of the mouse onto the line segment
    double t;         // parametric parameter for a line segment from (x1,y1) to (x2,y2)

    double dx = x2 - x1;
    double dy = y2 - y1;
    t = ((mouseX - x1) * (x2 - x1) + (mouseY - y1) * (y2 - y1)) / (dx * dx + dy * dy);
    if (t < 0 || t > 1.0) {
        // The projected point is not on the line segment
        return FALSE;
    } else {
        // Calculate the projected point.
        onX = x1 + dx * t;
        onY = y1 + dy * t;

        // Find the distance from the mouse to the projected point.
        double dx2 = mouseX - onX;
        double dy2 = mouseY - onY;
        double dist = sqrt(dx2 * dx2 + dy2 * dy2);

        return dist <= CLICK_TOLERANCE;
    }
}

/** -------------------------------------------------------------------
 * Find an edge that is within CLICK_TOLERANCE of the mouse location.
 * @param graph the graph
 * @param mouseX the location of the mouse (mouseX, mouseY)
 * @param mouseY
 * @param edge output of the function. If the edge is (-1,-1) then
 *             no edge was found. If the edge is (A,B) on return
 *             then the edge that goes from node A to node B was
 *             within CLICK_TOLERANCE of the mouse location.
 */
void findEdge(Graph *graph, int mouseX, int mouseY, Edge *edge) {
    // Assume that no edge can be found
    edge->fromVertex = -1;
    edge->toVertex = -1;

    Vertex *vertices = (Vertex *)graph->vertices;

    // Search all the edges and find the closest one
    for (int from = 0; from < graph->numberVertices; from++) {
        for (int to = 0; to < graph->numberVertices; to++) {
            if (graph->edges[from][to] == NORMAL_EDGE) {
                if (edgeClicked(vertices[from].x, vertices[from].y, vertices[to].x, vertices[to].y,
                                mouseX, mouseY)) {
                    // printf("Found %3d %3d\n", from, to);
                    edge->fromVertex = from;
                    edge->toVertex = to;
                    return;
                }
            }
        }
    }
}

/** -------------------------------------------------------------------
 * Take a turn of the game by a human player. The location of the
 * user's mouse click is used to detemine the edge that is changed.
 * @param graph the graph
 * @param mouseX the location of the mouse (mouseX, mouseY)
 * @param mouseY
 * @return TRUE if the turn was taken. Return FALSE if the turn
 *         was not taken (e.g., if the user did not select an edge.)
 */
int humanPlayer(Graph *graph, int mouseX, int mouseY) {
    Edge edge;
    findEdge(graph, mouseX, mouseY, &edge);

    if (edge.fromVertex == -1) {
        // No edge was found; therefore this turn still needs
        // to be made.
        return FALSE;
    } else {
        if (activePlayer == SHORT_PLAYER) {
            graphSetEdge(graph, edge.fromVertex, edge.toVertex, LOCKED_EDGE);
        } else {  // activePlayer == CUT_PLAYER
            graphSetEdge(graph, edge.fromVertex, edge.toVertex, NO_EDGE);
        }
        return TRUE;
    }
}

/** -------------------------------------------------------------------
 * Make a move for an AI player.
 * @param graph the graph
 * @return TRUE if the turn was taken. Return FALSE if the turn
 *         was not taken.
 */
// TODO: Replace this function with a separate "AI" function for
// each player and add intelligence to how it selects an edge.  It
// can also be done with a single function that checks which player
// is moving via the "activePlayer" variable
int aiPlayer(Graph *graph) {
    // Count the number of edges
    int tries = 0;
    int from = 0;
    int to = 0;
    while (tries < MAXIMUM_TRIES_TO_FIND_RANDOM_EDGE) {
        // Select a random edge
        from = rand() % graph->numberVertices;
        to = rand() % graph->numberVertices;
        if (graph->edges[from][to] == NORMAL_EDGE) {
            break;
        }
        tries++;
    }
    if (tries >= MAXIMUM_TRIES_TO_FIND_RANDOM_EDGE) {
        printf("AI failed to find a random edge.\n");
        return FALSE;
    }
    // Either cut or lock the edge
    if (activePlayer == SHORT_PLAYER) {
        graphSetEdge(graph, from, to, LOCKED_EDGE);
    } else {  // activePlayer == CUT_PLAYER
        graphSetEdge(graph, from, to, NO_EDGE);
    }

    return TRUE;
}

void doTurn(Graph *graph, int mouseX, int mouseY) {
    int turnIsDone = FALSE;

    if (playerType[activePlayer] == HUMAN_PLAYER) {
        turnIsDone = humanPlayer(graph, mouseX, mouseY);
    } else {
        turnIsDone = aiPlayer(graph);
    }

    // TODO: Check for a winner, a loser, or game over!

    if (turnIsDone) {
        activePlayer = 1 - activePlayer;  // swap active player
        redraw();
    }
}
//
// Created by Ilya Atmazhitov on 21.08.2024.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "Graph.h"
#include "Stack.h"


void initGraph(Graph* graph) {
    graph -> vertices = NULL;
    graph -> verticesCount = 0;
    graph -> edges = NULL;
}


void addVertex(Graph* graph, const char* computerName, unsigned int portNumber) {
    graph->verticesCount++;
    graph->vertices = (Vertex*)realloc(graph->vertices, graph->verticesCount * sizeof(Vertex));
    graph->vertices[graph->verticesCount - 1].computerName = strdup(computerName);
    graph->vertices[graph->verticesCount - 1].portNumber = portNumber;

    graph->edges = (Edge**)realloc(graph->edges, graph->verticesCount * sizeof(Edge*));
    for (size_t i = 0; i < graph->verticesCount - 1; i++) {
        graph->edges[i] = (Edge*)realloc(graph->edges[i], graph->verticesCount * sizeof(Edge));
        graph->edges[i][graph->verticesCount - 1].allowedPorts = NULL;
        graph->edges[i][graph->verticesCount - 1].portsCount = 0;
    }
    graph->edges[graph->verticesCount - 1] = (Edge*)calloc(graph->verticesCount, sizeof(Edge));
}

bool checkVertex(Graph* graph, const char* vertex) {
    for (int i = 0; i < graph->verticesCount; i++) {
        if (strcmp(graph->vertices[i].computerName, vertex) == 0) {
            return true;
        }
    }
    return false;
}

int findVertex(Graph* graph, const char* vertexName) {
    for (int i = 0; i < graph->verticesCount; i++) {
        if (strcmp(graph->vertices[i].computerName, vertexName) == 0) {
            return i;
        }
    }
    return -1;
}

Vertex* getVertexByName(Graph* graph, const char* vertexName) {
    for (int i = 0; i < graph->verticesCount; i++) {
        if (strcmp(graph->vertices[i].computerName, vertexName) == 0) {
            return &(graph->vertices[i]);
        }
    }
    return NULL;
}


void addEdge(Graph* graph, const char* from, const char* to, unsigned int* ports, unsigned int portsCount) {
    int fromIndex = findVertex(graph, from);
    int toIndex = findVertex(graph, to);

    if (fromIndex == -1 || toIndex == -1) {
        printf("Vertex('s) do/does not exist");
        exit(1);
    }

    Edge* newEdge = (Edge*)malloc(sizeof(Edge));
    newEdge->allowedPorts = (unsigned int*)malloc(portsCount * sizeof(unsigned int));
    memcpy(newEdge->allowedPorts, ports, portsCount * sizeof(unsigned int));
    newEdge->portsCount = portsCount;

    graph->edges[fromIndex][toIndex] = *newEdge;

    free(newEdge);
}

void deleteVertex(Graph* graph, const char* vertexName) {
    int vertexIndex = findVertex(graph, vertexName);
    if (vertexIndex == -1) {
        printf("Vertex '%s' not found in the graph.\n", vertexName);
        return;
    }

    free(graph->vertices[vertexIndex].computerName);
    for (int i = vertexIndex; i < graph->verticesCount - 1; i++) {
        graph->vertices[i] = graph->vertices[i + 1];
    }
    graph->verticesCount--;
    graph->vertices = (Vertex*)realloc(graph->vertices, graph->verticesCount * sizeof(Vertex));

    for (int i = 0; i < graph->verticesCount; i++) {
        free(graph->edges[i][vertexIndex].allowedPorts);
        for (int j = vertexIndex; j < graph->verticesCount; j++) {
            graph->edges[i][j] = graph->edges[i][j + 1];
        }
        graph->edges[i] = (Edge*)realloc(graph->edges[i], graph->verticesCount * sizeof(Edge));
    }

    free(graph->edges[vertexIndex]);
    for (int i = vertexIndex; i < graph->verticesCount; i++) {
        graph->edges[i] = graph->edges[i + 1];
    }
    graph->edges = (Edge**)realloc(graph->edges, graph->verticesCount * sizeof(Edge*));
}

void deleteEdge(Graph* graph, const char* from, const char* to) {
    int fromIndex = findVertex(graph, from);
    int toIndex = findVertex(graph, to);

    if (fromIndex == -1 || toIndex == -1) {
        printf("Vertex('s) do/does not exist.\n");
        return;
    }

    free(graph->edges[fromIndex][toIndex].allowedPorts);
    graph->edges[fromIndex][toIndex].allowedPorts = NULL;
    graph->edges[fromIndex][toIndex].portsCount = 0;
}

void updateVertex(Graph* graph, const char* vertexName, const char* newComputerName, unsigned int newPortNumber) {
    int vertexIndex = findVertex(graph, vertexName);
    if (vertexIndex == -1) {
        printf("Vertex '%s' not found in the graph.\n", vertexName);
        return;
    }

    free(graph->vertices[vertexIndex].computerName);
    graph->vertices[vertexIndex].computerName = strdup(newComputerName);
    graph->vertices[vertexIndex].portNumber = newPortNumber;
}


void updateEdge(Graph* graph, const char* from, const char* to, unsigned int* newPorts, unsigned int newPortsCount) {
    int fromIndex = findVertex(graph, from);
    int toIndex = findVertex(graph, to);

    if (fromIndex == -1 || toIndex == -1) {
        printf("Vertex('s) do/does not exist.\n");
        return;
    }

    free(graph->edges[fromIndex][toIndex].allowedPorts);
    graph->edges[fromIndex][toIndex].allowedPorts = (unsigned int*)malloc(newPortsCount * sizeof(unsigned int));
    memcpy(graph->edges[fromIndex][toIndex].allowedPorts, newPorts, newPortsCount * sizeof(unsigned int));
    graph->edges[fromIndex][toIndex].portsCount = newPortsCount;
}


void printAdjacencyList(Graph* graph) {
    for (int i = 0; i < graph->verticesCount; i++) {
        printf("%s (Port: %u) -> ", graph->vertices[i].computerName, graph->vertices[i].portNumber);
        for (int j = 0; j < graph->verticesCount; j++) {
            if (graph->edges[i][j].allowedPorts != NULL) {
                printf("%s (Ports: ", graph->vertices[j].computerName);
                for (int k = 0; k < graph->edges[i][j].portsCount; k++) {
                    printf("%u", graph->edges[i][j].allowedPorts[k]);
                    if (k < graph->edges[i][j].portsCount - 1) {
                        printf(", ");
                    }
                }
                printf(") ");
            }
        }
        printf("\n");
    }
}


void buildGraphViz(Graph* graph, const char* filename) {
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        printf("Error opening file.\n");
        return;
    }

    fprintf(file, "digraph G {\n");
    fprintf(file, "    node [shape=record];\n");

    // Вершины
    for (int i = 0; i < graph->verticesCount; i++) {
        fprintf(file, "    v%d [label=\"{%s|Port: %u}\"];\n", i, graph->vertices[i].computerName, graph->vertices[i].portNumber);
    }

    // Ребра
    for (int i = 0; i < graph->verticesCount; i++) {
        for (int j = 0; j < graph->verticesCount; j++) {
            if (graph->edges[i][j].allowedPorts != NULL) {
                fprintf(file, "    v%d -> v%d [label=\"Ports: ", i, j);
                for (int k = 0; k < graph->edges[i][j].portsCount; k++) {
                    fprintf(file, "%u", graph->edges[i][j].allowedPorts[k]);
                    if (k < graph->edges[i][j].portsCount - 1) {
                        fprintf(file, ", ");
                    }
                }
                fprintf(file, "\"];\n");
            }
        }
    }

    fprintf(file, "}\n");
    fclose(file);
}



void dfsServiceSearchUtil(Graph* graph, int vertex, unsigned int servicePort, bool* visited) {
    Stack* stack = createStack();
    push(stack, vertex);
    visited[vertex] = true;

    while (!isEmpty(stack)) {
        int current = top(stack);
        pop(stack);

        if (graph->vertices[current].portNumber == servicePort) {
            printf("%s\n", graph->vertices[current].computerName);
        }

        for (int i = 0; i < graph->verticesCount; i++) {
            if (!visited[i] && graph->edges[current][i].portsCount > 0) {
                bool canAccess = false;
                for (int j = 0; j < graph->edges[current][i].portsCount; j++) {
                    if (graph->edges[current][i].allowedPorts[j] == servicePort) {
                        canAccess = true;
                        break;
                    }
                }
                if (canAccess) {
                    visited[i] = true;
                    push(stack, i);
                }
            }
        }
    }

    deleteStack(stack);
}


void dfsServiceSearch(Graph* graph, const char* sourceName, unsigned int servicePort) {
    int sourceIndex = findVertex(graph, sourceName);
    if (sourceIndex == -1) {
        printf("Source vertex '%s' not found.\n", sourceName);
        return;
    }

    bool* visited = (bool*)calloc(graph->verticesCount, sizeof(bool));
    dfsServiceSearchUtil(graph, sourceIndex, servicePort, visited);
    free(visited);
}


int minKey(int* key, bool* mstSet, int verticesCount) {
    int min = INT_MAX, min_index;

    for (int v = 0; v < verticesCount; v++) {
        if (!mstSet[v] && key[v] < min) {
            min = key[v];
            min_index = v;
        }
    }

    return min_index;
}

int shortestPath(Graph* graph, const char* source, const char* destination, unsigned int servicePort) {
    int sourceIndex = findVertex(graph, source);
    int destinationIndex = findVertex(graph, destination);

    if (sourceIndex == -1 || destinationIndex == -1) {
        printf("Source or destination vertex not found.\n");
        return INT_MAX;
    }

    int verticesCount = graph->verticesCount;
    int* distances = (int*)malloc(verticesCount * sizeof(int));
    for (int i = 0; i < verticesCount; i++) {
        distances[i] = INT_MAX;
    }
    distances[sourceIndex] = 0;
    for (int i = 0; i < verticesCount - 1; i++) {
        for (int u = 0; u < verticesCount; u++) {
            for (int v = 0; v < verticesCount; v++) {
                if (distances[u] != INT_MAX && graph->edges[u][v].portsCount > 0) {
                    bool validPort = false;
                    for (int j = 0; j < graph->edges[u][v].portsCount; j++) {
                        if (graph->edges[u][v].allowedPorts[j] == servicePort) {
                            validPort = true;
                            break;
                        }
                    }
                    if (validPort && distances[v] > distances[u] + 1) {
                        distances[v] = distances[u] + 1;
                    }
                }
            }
        }
    }

    return distances[destinationIndex];
}


void minimumSpanningTree(Graph* graph, unsigned int servicePort) {
    int verticesCount = graph->verticesCount;
    int* parent = (int*)malloc(verticesCount * sizeof(int));
    int* key = (int*)malloc(verticesCount * sizeof(int));
    bool* mstSet = (bool*)malloc(verticesCount * sizeof(bool));

    for (int i = 0; i < verticesCount; i++) {
        key[i] = INT_MAX;
        mstSet[i] = false;
    }

    key[0] = 0;
    parent[0] = -1;

    for (int count = 0; count < verticesCount - 1; count++) {
        int u = minKey(key, mstSet, verticesCount);
        mstSet[u] = true;

        for (int v = 0; v < verticesCount; v++) {
            if (graph->edges[u][v].portsCount > 0) {
                bool validPort = false;
                for (int j = 0; j < graph->edges[u][v].portsCount; j++) {
                    if (graph->edges[u][v].allowedPorts[j] == servicePort) {
                        validPort = true;
                        break;
                    }
                }

                if (validPort && !mstSet[v] && key[v] > 1) {
                    parent[v] = u;
                    key[v] = 1;
                }
            }
        }
    }

    printf("Edges of the Minimum Spanning Tree:\n");
    for (int i = 1; i < verticesCount; i++) {
        printf("%s - %s\n", graph->vertices[parent[i]].computerName, graph->vertices[i].computerName);
    }

    free(parent);
    free(key);
    free(mstSet);
}


void readGraphFromFile(Graph* graph, const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error opening file.\n");
        return;
    }

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        char* token = strtok(line, " ");
        if (token == NULL) {
            continue;
        }

        if (strcmp(token, "vertex") == 0) {
            token = strtok(NULL, " ");
            char* computerName = token;
            token = strtok(NULL, " ");
            unsigned int portNumber = atoi(token);
            addVertex(graph, computerName, portNumber);
        } else if (strcmp(token, "edge") == 0) {
            token = strtok(NULL, " ");
            char* from = token;
            token = strtok(NULL, " ");
            char* to = token;
            token = strtok(NULL, " ");
            int portsCount = atoi(token);
            unsigned int* ports = (unsigned int*)malloc(portsCount * sizeof(unsigned int));
            for (int i = 0; i < portsCount; i++) {
                token = strtok(NULL, " ");
                ports[i] = atoi(token);
            }
            addEdge(graph, from, to, ports, portsCount);
            free(ports);
        }
    }

    fclose(file);
}

void deleteGraph(Graph* graph) {
    if (graph->vertices != NULL) {
        for (int i = 0; i < graph->verticesCount; i++) {
            free(graph->vertices[i].computerName);
        }
        free(graph->vertices);
    }

    if (graph->edges != NULL) {
        for (int i = 0; i < graph->verticesCount; i++) {
            for (int j = 0; j < graph->verticesCount; j++) {
                free(graph->edges[i][j].allowedPorts);
            }
            free(graph->edges[i]);
        }
        free(graph->edges);
    }

    graph->vertices = NULL;
    graph->verticesCount = 0;
    graph->edges = NULL;
}


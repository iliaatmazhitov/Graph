//
// Created by Ilya Atmazhitov on 20.08.2024.
//

#ifndef INC_2_LABORATORY5_GRAPH_H
#define INC_2_LABORATORY5_GRAPH_H

#include <stdbool.h>

// Структура для хранения информации об ребре графа
typedef struct {
    unsigned int* allowedPorts; // Перечень портов, по которым разрешена передача трафика
    unsigned int portsCount; // Количество портов в массиве
} Edge;

// Структура для хранения информации о вершине графа
typedef struct {
    char* computerName; // Уникальное имя компьютера
    unsigned int portNumber; // Номер порта, на котором запущен сетевой сервис
} Vertex;

// Структура для хранения графа
typedef struct {
    Vertex* vertices; // Массив вершин графа
    int verticesCount; // Количество вершин в графе
    Edge** edges; // Матрица смежности графа
} Graph;

void initGraph(Graph* graph);
void addVertex(Graph* graph, const char* computerName, unsigned int portNumber);
bool checkVertex(Graph* graph, const char* vertex);
int findVertex(Graph* graph, const char* vertexName);
Vertex* getVertexByName(Graph* graph, const char* vertexName);
void addEdge(Graph* graph, const char* from, const char* to, unsigned int* ports, unsigned int portsCount);
void deleteVertex(Graph* graph, const char* vertexName);
void deleteEdge(Graph* graph, const char* from, const char* to);
void updateVertex(Graph* graph, const char* vertexName, const char* newComputerName, unsigned int newPortNumber);
void updateEdge(Graph* graph, const char* from, const char* to, unsigned int* newPorts, unsigned int newPortsCount);
void printAdjacencyList(Graph* graph);
void buildGraphViz(Graph* graph, const char* filename);
void dfsServiceSearchUtil(Graph* graph, int vertex, unsigned int servicePort, bool* visited);
void dfsServiceSearch(Graph* graph, const char* sourceName, unsigned int servicePort);
int minKey(int* key, bool* mstSet, int verticesCount);
int shortestPath(Graph* graph, const char* source, const char* destination, unsigned int servicePort);
void minimumSpanningTree(Graph* graph, unsigned int servicePort);
void readGraphFromFile(Graph* graph, const char* filename);
void deleteGraph(Graph* graph);

#endif //INC_2_LABORATORY5_GRAPH_H

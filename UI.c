//
// Created by Ilya Atmazhitov on 22.08.2024.
//

#include <limits.h>
#include "UI.h"

void print_menu() {
    printf("Select an action:\n");
    printf("1. Make graph\n");
    printf("2. Graph initialization\n");
    printf("3. Add vertex\n");
    printf("4. Add edge\n");
    printf("5. Erase vertex\n");
    printf("6. Erase edge\n");
    printf("7. Change vertex name\n");
    printf("8. Change edge information\n");
    printf("9. Output graph like adjacency list\n");
    printf("10. Output graph with GraphViz\n");
    printf("11. Graph traversal\n");
    printf("12. The shortest path between vertexes\n");
    printf("13. Path with maximum rating\n");
    printf("14. Input graph from file\n");
    printf("15. Erase graph\n");
    printf("16. Exit\n");
    printf("\n");
}

void implementation() {
    Graph graph;
    initGraph(&graph);

    int choice;
    char source[256], destination[256], filename[256];
    unsigned int servicePort;

    char computerName[256];
    char from[256], to[256];
    int portsCount;
    unsigned int* ports;
    char vertexName[256];
    char fromVertex[256], toVertex[256];
    char oldVertexName[256], newVertexName[256];
    unsigned int newPortNumber;
    int newPortsCount;
    unsigned int* newPorts;

    while (1) {
        print_menu();
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                // Создание нового графа
                initGraph(&graph);
                printf("New graph created.\n");
                break;
            case 2:
                // Инициализация графа
                initGraph(&graph);
                printf("Graph initialized.\n");
                break;
            case 3:
                // Добавление вершины
                printf("Enter computer name: ");
                scanf("%s", computerName);
                printf("Enter port number: ");
                scanf("%u", &newPortNumber);
                addVertex(&graph, computerName, newPortNumber);
                printf("Vertex added.\n");
                break;
            case 4:
                // Добавление ребра
                printf("Enter source vertex: ");
                scanf("%s", from);
                printf("Enter destination vertex: ");
                scanf("%s", to);
                printf("Enter number of allowed ports: ");
                scanf("%d", &portsCount);
                ports = (unsigned int*)malloc(portsCount * sizeof(unsigned int));
                printf("Enter allowed ports: ");
                for (int i = 0; i < portsCount; i++) {
                    scanf("%u", &ports[i]);
                }
                addEdge(&graph, from, to, ports, portsCount);
                free(ports);
                printf("Edge added.\n");
                break;
            case 5:
                // Удаление вершины
                printf("Enter vertex name: ");
                scanf("%s", vertexName);
                deleteVertex(&graph, vertexName);
                printf("Vertex deleted.\n");
                break;
            case 6:
                // Удаление ребра
                printf("Enter source vertex: ");
                scanf("%s", fromVertex);
                printf("Enter destination vertex: ");
                scanf("%s", toVertex);
                deleteEdge(&graph, fromVertex, toVertex);
                printf("Edge deleted.\n");
                break;
            case 7:
                // Изменение имени вершины
                printf("Enter old vertex name: ");
                scanf("%s", oldVertexName);
                printf("Enter new vertex name: ");
                scanf("%s", newVertexName);
                printf("Enter new port number: ");
                scanf("%u", &newPortNumber);
                updateVertex(&graph, oldVertexName, newVertexName, newPortNumber);
                printf("Vertex updated.\n");
                break;
            case 8:
                // Изменение информации о ребре
                printf("Enter source vertex: ");
                scanf("%s", fromVertex);
                printf("Enter destination vertex: ");
                scanf("%s", toVertex);
                printf("Enter new number of allowed ports: ");
                scanf("%d", &newPortsCount);
                newPorts = (unsigned int*)malloc(newPortsCount * sizeof(unsigned int));
                printf("Enter new allowed ports: ");
                for (int i = 0; i < newPortsCount; i++) {
                    scanf("%u", &newPorts[i]);
                }
                updateEdge(&graph, fromVertex, toVertex, newPorts, newPortsCount);
                free(newPorts);
                printf("Edge updated.\n");
                break;
            case 9:
                // Вывод графа в виде списка смежности
                printAdjacencyList(&graph);
                break;
            case 10:
                // Вывод графа с помощью GraphViz
                printf("Enter filename for GraphViz output: ");
                scanf("%s", filename);
                buildGraphViz(&graph, filename);
                printf("GraphViz file generated.\n");
                break;
            case 11:
                // Обход графа
                printf("Enter source vertex: ");
                scanf("%s", source);
                printf("Enter service port: ");
                scanf("%u", &servicePort);
                dfsServiceSearch(&graph, source, servicePort);
                break;
            case 12:
                // Поиск кратчайшего пути между вершинами
                printf("Enter source vertex: ");
                scanf("%s", source);
                printf("Enter destination vertex: ");
                scanf("%s", destination);
                printf("Enter service port: ");
                scanf("%u", &servicePort);
                int shortestPathDistance = shortestPath(&graph, source, destination, servicePort);
                if (shortestPathDistance != INT_MAX) {
                    printf("The shortest path distance between %s and %s is %d\n", source, destination, shortestPathDistance);
                } else {
                    printf("No path found between %s and %s\n", source, destination);
                }
                break;
            case 13:
                // Поиск пути с максимальным рейтингом
                // Замените этот случай, если вы не реализовали функцию graph_longest_path_external
                printf("This feature is not implemented yet.\n");
                break;
            case 14:
                // Чтение графа из файла
                printf("Enter filename to read graph from: ");
                scanf("%s", filename);
                readGraphFromFile(&graph, filename);
                printf("Graph read from file.\n");
                break;
            case 15:
                // Удаление графа
                deleteGraph(&graph);
                printf("Graph deleted.\n");
                break;
            case 16:
                // Выход из программы
                printf("Exiting...\n");
                deleteGraph(&graph);
                return;
            default:
                printf("Invalid choice. Please try again.\n");
                break;
        }
        printf("\n");
    }
}
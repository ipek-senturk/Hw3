#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define N 60

typedef struct node {
    int vertex;
    struct node *next;
}NODE;

typedef struct graph {
    int numOfVertices;
    int *visited;
    int *inDegreeArray;
    int *trackVisitedArray;
    struct node **adjList;
}GRAPH;

void takeInput(int *, int, int);
int *createArray(int);
int **createMatrix(int, int);
void printMatrix(int **, int);
void matrixToList(int **, GRAPH *);
void deleteMatrix(int **, int);
NODE *createNode(int);
GRAPH *createGraph(int);
void addEdge(GRAPH *, int, int);
int calculateInDegree(GRAPH *, int);
void printAdjacencyList(GRAPH *, int *);
int detectCycleDFS(GRAPH *, int);
int isGraphCyclic(GRAPH *);
void takeCourses(GRAPH *);
int checkCourses(int *, int);
void deleteGraph(GRAPH *);

int main() {
    int i, j, n, isCyclic, areAllCoursesTaken = 0, **adjacencyMatrix;
	GRAPH *courseGraph;

    printf("\nDers sayisini giriniz.\nEn fazla 60 en az 1 ders olabilir.\n");
    takeInput(&n, N, 1);

    adjacencyMatrix = createMatrix(n, n);
    courseGraph = createGraph(n);
    printMatrix(adjacencyMatrix, n);
    matrixToList(adjacencyMatrix, courseGraph);
    deleteMatrix(adjacencyMatrix, n);

    isCyclic = isGraphCyclic(courseGraph);
    while(isCyclic) {
        printf("Grafta dongu var. Komsuluk matrisini tekrar giriniz.\n");
        deleteGraph(courseGraph);
        courseGraph = createGraph(n);
        adjacencyMatrix = createMatrix(n, n);
        printMatrix(adjacencyMatrix, n);
        matrixToList(adjacencyMatrix, courseGraph);
        isCyclic = isGraphCyclic(courseGraph);
    }

    for(i = 0; i < n; i++) {
        courseGraph->inDegreeArray[i] = calculateInDegree(courseGraph, i);
    }

    printAdjacencyList(courseGraph, courseGraph->inDegreeArray);
    
    for(i = 0; i < courseGraph->numOfVertices; i++) {
        courseGraph->visited[i] = 0;
    }

    i = 0;
    printf("\n");
    while(!areAllCoursesTaken) {
        i += 1;
        printf("Donem %d dersleri: ", i);
        for(j = 0; j < courseGraph->numOfVertices; j++) {
            if(courseGraph->inDegreeArray[j] == 0 && courseGraph->visited[j] == 0) {
                printf("Ders-%d ", j);
            }
        }
        printf("\n");
        takeCourses(courseGraph);
        areAllCoursesTaken = checkCourses(courseGraph->visited, n);
    }
    printf("\nBuna gore ogrenci bolumu %d donemde bitirir.\n\n", i);
    deleteGraph(courseGraph);

    return 0;
}

void takeInput(int *n, int upperBound, int lowerBound) {
    scanf("%d", n);
    while(*n > upperBound || *n < lowerBound) {
        printf("Tekrar deneyiniz: ");
        scanf("%d", n);
    }
}

int *createArray(int size) {
    int *array = calloc(size, sizeof(int));
    if(array == NULL) {
        printf("Unable to allocate memory. Exiting the program.");
        exit(1);
    }
    return array;
}

int **createMatrix(int rows, int columns) {
    int i, j;
    int **matrix = calloc(rows, sizeof(int *));

    for(i = 0; i < rows; i++) {
        matrix[i] = createArray(columns);
    }

    for(i = 0; i < rows; i++) {
        printf("-------------------\n");
        for(j = 0; j < columns; j++) {
            printf("Adj[%d][%d] = ", i, j);
            takeInput(&matrix[i][j], 1, 0);
        }
    }
    return matrix;
}

void printMatrix(int **matrix, int size) {
    int i, j;
    printf("-----Komsuluk matrisi-----\n");
    for(i = 0; i < size; i++) {
        for(j = 0; j < size; j++) {
            printf("%d  ", matrix[i][j]);
        }
        printf("\n");
    }
    printf("---------------------------\n");
}

void matrixToList(int **matrix, GRAPH *graph) {
    int i, j, size = graph->numOfVertices;
    for(i = 0; i < size; i++) {
        for(j = 0; j < size; j++) {
            if(matrix[i][j] == 1) {
                addEdge(graph, i, j);
            }
        }
    }
}

void deleteMatrix(int **matrix, int size) {
    int i;
    for(i = 0; i < size; i++) {
        free(matrix[i]);
    }
    free(matrix);
}

NODE *createNode(int vertex) {
    NODE *newNode = (NODE *)malloc(sizeof(NODE));
    if(newNode == NULL) {
        printf("Unable allocate memory. Exiting the program.\n");
        exit(1);
    }
    newNode->vertex = vertex;
    newNode->next = NULL;
    return newNode;
}

GRAPH *createGraph(int numOfVertices) {
    int i;
    GRAPH *newGraph = (GRAPH *)malloc(sizeof(GRAPH));
    if(newGraph == NULL) {
        printf("Unable allocate memory. Exiting the program.\n");
        exit(1);
    }
    newGraph->numOfVertices = numOfVertices;
    newGraph->visited = createArray(numOfVertices);
    newGraph->trackVisitedArray = createArray(numOfVertices);
    newGraph->inDegreeArray = createArray(numOfVertices);
    newGraph->adjList = (NODE **)malloc(sizeof(NODE *) * numOfVertices);
    if(newGraph->adjList == NULL) {
        printf("Unable allocate memory. Exiting the program.\n");
        exit(1);
    }
    for(i = 0; i < numOfVertices; i++) {
        newGraph->adjList[i] = NULL;
    }
    return newGraph;
}

void addEdge(GRAPH *graph, int source, int destination) {
	NODE *newNode = createNode(destination);
	newNode->next = graph->adjList[source];
	graph->adjList[source] = newNode;
}

int calculateInDegree(GRAPH *graph, int nodeIndex) {
    NODE *temp;
    int i, inDegree = 0;
    for(i = 0; i < graph->numOfVertices; i++) {
        temp = graph->adjList[i];
        while(temp != NULL) {
            if(temp->vertex == nodeIndex) {
                inDegree += 1;
            }
            temp = temp->next;
        }
    }
    return inDegree;
}

void printAdjacencyList(GRAPH *graph, int *inDegrees) {
    int i;
    NODE *temp;
    printf("\n-----Komsuluk Listesi-----\n");
    for(i = 0; i < graph->numOfVertices; i++) {
        temp = graph->adjList[i];
        printf("%d (%d) -> ", i, inDegrees[i]);
        while(temp != NULL) {
            if(temp->next != NULL) {
                printf("%d, ", temp->vertex);
            } else {
                printf("%d ", temp->vertex);
            } 
            temp = temp->next;
        }
        printf("\n");
    }
    printf("--------------------------\n");
}

int detectCycleDFS(GRAPH *graph, int vertex) {
    int cycleFound = 0;
    NODE *adjacentVertex = graph->adjList[vertex];
    graph->visited[vertex] = 1;
    graph->trackVisitedArray[vertex] = 1;
    while(adjacentVertex != NULL && !cycleFound) {
        if(!graph->visited[adjacentVertex->vertex] && detectCycleDFS(graph, adjacentVertex->vertex)) {
            cycleFound = 1;
        } else if(graph->trackVisitedArray[adjacentVertex->vertex]) {
            cycleFound = 1;
        } else {
            adjacentVertex = adjacentVertex->next;
        }
    }
    graph->trackVisitedArray[vertex] = 0;
    if (cycleFound) {
        return 1;
    } else {
        return 0;
    }
}

int isGraphCyclic(GRAPH *graph) {
    int i = 0, cycleFound = 0;
    while(i < graph->numOfVertices && !cycleFound) {
        if (!graph->visited[i]) {
            cycleFound = detectCycleDFS(graph, i);
        }
        i += 1;
    }
    return cycleFound;
}

void takeCourses(GRAPH *graph) {
    NODE *temp; 
    int i, *tempDegrees = createArray(graph->numOfVertices);
    memcpy(tempDegrees, graph->inDegreeArray, graph->numOfVertices * sizeof(int));

    for(i = 0; i < graph->numOfVertices; i++) {
        if((graph->inDegreeArray[i] == 0 && tempDegrees[i] == 0) && (graph->visited[i] == 0)) {
            graph->visited[i] = 1;
            temp = graph->adjList[i];
            while(temp != NULL) {
                tempDegrees[temp->vertex] -= 1;
                temp = temp->next;
            }
        }
    }
    memcpy(graph->inDegreeArray, tempDegrees, graph->numOfVertices * sizeof(int));
    free(tempDegrees);
}

int checkCourses(int *taken, int size) {
    int i = 0;
    while(i < size && taken[i] == 1) {
        i += 1;
    }
    if(i == size) {
        return 1;
    } else {
        return 0;
    }
}

void deleteGraph(GRAPH *graph) {
    int i;
    NODE *temp, *nextNode;
    for(i = 0; i < graph->numOfVertices; i++) {
        temp = graph->adjList[i];
        while(temp != NULL) {
            nextNode = temp->next;
            free(temp);
            temp = nextNode;
        }
    }
    free(graph->adjList);
    free(graph->inDegreeArray);
    free(graph->trackVisitedArray);
    free(graph->visited);
    free(graph);
}

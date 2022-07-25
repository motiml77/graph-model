
#define _CRT_SECURE_NO_WARNINGS
#define SIZE 1000
#define infi INT_MAX

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <fstream>

using namespace std;

/*structs*/
typedef struct Node {
	int numV;
	struct Node* next;
};
struct queue {
	int items[SIZE];
	int front;
	int rear;
};
struct Graph {
	int numVertices;
	struct Node** adjLists;
	int* visited;
};

/*function for queue*/
struct queue* createQueue() {
	struct queue* q = (struct queue*)malloc(sizeof(struct queue));
	q->front = -1;
	q->rear = -1;
	return q;
}
int isEmpty(struct queue* q) {
	if (q->rear == -1)
		return 1;
	else
		return 0;
}
void enqueue(struct queue* q, int value) {
	if (q->rear == SIZE - 1)
		printf("\n");
	else {
		if (q->front == -1)
			q->front = 0;
		q->rear++;
		q->items[q->rear] = value;
	}
}
int dequeue(struct queue* q) {
	int item;
	if (isEmpty(q)) {
		//printf("Queue is empty");
		item = -1;
	}
	else {
		item = q->items[q->front];
		q->front++;
		if (q->front > q->rear) {
			//printf("Resetting queue ");
			q->front = q->rear = -1;
		}
	}
	return item;
}
void printQueue(struct queue* q) {
	int i = q->front;

	if (isEmpty(q)) {
		printf("\n");
	}
	else {
		printf("\n");
		/*	for (i = q->front; i < q->rear + 1; i++) {
				printf("%d ", q->items[i]);*/
	}
}

/*functions*/
void addToList(Node** head, int num)
{
	while (*head)
	{
		head = &(*head)->next;
	}

	*head = (Node*)malloc(sizeof(**head));
	(*head)->numV = num;
	(*head)->next = NULL;
}
int* bfs(struct Graph* graph, int startVertex) {
	int* dist = (int*)malloc(sizeof(int) * graph->numVertices);
	graph->visited = (int*)malloc(sizeof(int) * graph->numVertices);
	for (int i = 0; i < graph->numVertices; i++) {
		dist[i] = infi;
		graph->visited[i] = 0;
	}
	struct queue* q = createQueue();

	graph->visited[startVertex] = 1;
	enqueue(q, startVertex);
	dist[startVertex] = 0;
	struct Node* temp;
	int adjVertex, currentVertex;
	while (!isEmpty(q)) {
		printQueue(q);
		currentVertex = dequeue(q);

		temp = graph->adjLists[currentVertex];

		while (temp) {
			adjVertex = temp->numV;

			if (graph->visited[adjVertex] == 0) {
				graph->visited[adjVertex] = 1;
				enqueue(q, adjVertex);
				dist[adjVertex] = dist[currentVertex] + 1;
			}
			temp = temp->next;
		}
	}
	return dist;
}
Graph build_random_graph(int numV, double p) {
	Graph tmp;
	tmp.numVertices = numV;
	Node** list = (Node**)calloc(numV, sizeof(Node*));
	int i = 0, j = 0;
	double r;
	srand(time(NULL));

	for (i = 0; i < numV; i++) {
		for (j = 0; j < numV, j != i; j++) {
			r = ((double)rand() / (double)(RAND_MAX));
			if (r <= p) {
				addToList(&list[i], j);
				addToList(&list[j], i);
			}
		}
	}
	tmp.adjLists = list;
	tmp.visited = NULL;
	return tmp;
}
int diameter(Graph& graph)
{
	int diam = 0, max = 0, i, j;
	int size = graph.numVertices;
	int* dist;


	for (i = 0; i < size; i++)
	{
		dist = bfs(&graph, i);
		for (j = 0; j < size; j++) {
			if (max < dist[j])
				max = dist[j];
		}
		if (diam < max)
			diam = max;
		max = 0;
	}
	return diam;
}
int is_isolated(Graph& graph) {
	int i = 0;
	int size = graph.numVertices;
	int* dist;
	for (i = 0; i < size; i++)
		if (graph.adjLists[i] == NULL)
			return 1;
	return 0;
}
int connectivity(Graph& graph) {
	if (is_isolated(graph))
		return 0;
	int size = graph.numVertices;
	int randV = rand() % size;
	int i = 0;

	bfs(&graph, randV);
	for (i = 0; i < size; i++) {
		if (graph.visited[i] != 1)
			return 0;
	}
	return 1;
}






void main() {
	int num_of_v = 950;

	double threshold1[] = { 0.001,0.002,0.004,0.005,0.006,0.01,0.102,0.11,0.17,0.21 };  // for connectivity and isolated
	double threshold2[] = { 0.004,0.005,0.006,0.01,0.102,0.11,0.17,0.21,0.25,0.29 }; //for diam


	ofstream myfile;
	myfile.open("test.csv");
	myfile << "Test DIAM  \n";
	int diam;
	Graph graph;

	for (int i = 5; i < 6; ++i) {
		myfile << "\n" << threshold2[i];
		for (int j = 0; j < 1; ++j) {
			myfile << " , ,";
			graph = build_random_graph(num_of_v, threshold2[i]);

			if (!connectivity(graph))
			{
				myfile << "infi \n";
			}
			else {
				diam = diameter(graph);
				myfile <<diam <<"\n";
			}

		}
	}

	myfile.close();
}

//for (int i = 0; i < 2; ++i) {
//	myfile << "\n" << threshold1[i];
//	for (int j = 0; j < 5; ++j) {
//		myfile << " , ,";
//		graph = build_random_graph(num_of_v, threshold1[i]);
//
//		if (!is_isolated(graph))
//		{
//			myfile << "0 \n";
//		}
//		else {
//			myfile << "1 \n";
//		}

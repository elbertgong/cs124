// TODO!!!!!!!!!!!!!!!
// Finish up the chart
// Writeup: procedure, analysis, and discussion
// Get the makefiles and program to work on remote Linux
// Test code under valgrind
// Keep track of potential optimizations (maybe instead of crawling
// in deletemin0 just iterate thru points for min where searched == 1,
// change the initialization value for (points + i)->dist)

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <float.h>
#include <math.h>
#include <string.h>


typedef struct {
	int num; // number from 1 to n
	float dist;
	unsigned char searched;
	// searched: 0 means not inserted into heap, 1 means inserted not deleted, 2 means deleted
} vertex;

// heapnodes will live in the linked list heap
typedef struct heapnode {
	vertex* p;
	struct heapnode* prev;
	struct heapnode* next;
} heapnode;

// global heap for 0D graph
static heapnode* heap0;

// void insert0(vertex* point, float dist)
//		insert into the heap (or updates an existing heap node)
void insert0(vertex* point, float dist) {
	// update distance
	point->dist = dist;
	assert(point->searched != 2);
	if (point->searched == 1) return;
	point->searched = 1;
	
	// actually insert a new node into the heap
	heapnode* n = malloc(sizeof(heapnode));
	n->p = point;
	n->prev = NULL;
	if (heap0) {
		n->next = heap0;
		heap0->prev = n;
	}
	else {
		n->next = NULL;
	}
	heap0 = n;
}

// vertex* deletemin0(node0* heap)
//		Deletes min from heap and returns it
vertex* deletemin0() {
	heapnode* crawler = heap0;
	heapnode* minptr = heap0;
	while (crawler != NULL) {
		if (crawler->p->dist < minptr->p->dist)
			minptr = crawler;
		crawler = crawler->next;
	}

	// we've found minimum, now extract it from linked list
	if (minptr->prev) {
		minptr->prev->next = minptr->next;
	}
	else {
	    heap0 = minptr->next;
	}
	if (minptr->next) {
		minptr->next->prev = minptr->prev;
	}
	vertex* x = minptr->p;
	assert (x->searched == 1);
	x->searched = 2;
	free(minptr);
	return x;
}



// float mst0(int numpoints)
//		Returns the length of a randomly-generated 0D MST
float mst0(int numpoints) {	
	vertex* points = (vertex *)malloc(sizeof(vertex) * numpoints);
	for (int i = 0; i < numpoints; i++) {
		(points + i)->num = i;
		(points + i)->dist = 10;
		(points + i)->searched = 0;
	}
	// since we're going to add the first point into the heap
	points[0].dist = 0;
    points[0].searched = 1;

    heap0 = malloc(sizeof(heapnode));
    // first thing in the heap will be the first point
	heap0->p = points;
	heap0->prev = NULL;
	heap0->next = NULL;

	// prim's algorithm
	while(heap0 != NULL) {
		vertex* p = deletemin0();
		// printf("newmin %f %f %f %d\n", p->x, p->y, p->dist, p->searched);
		
		// look for new edges and potentially insert them into heap
		for(int i = 0; i < numpoints; i++) {
			if(i != p->num && (points + i)->searched != 2) {
				float new_edge = (float) rand() / RAND_MAX;
				if(new_edge < (points + i)->dist && new_edge < 1) // change to k(numpoints)
					insert0((points + i), new_edge);
			}
		}
	}

	// find the total length of the mst
	float total = 0;
	for (int i = 0; i < numpoints; i++) {
		assert((points + i)->searched == 2);
		total += (points + i)->dist;
	}
	free(points);
	return total;
}



// each vertex gets represented by a point
typedef struct {
	float x;
	float y;
	float z; 
	float w;
	float dist;
	unsigned char searched;
	// searched: 0 means not inserted into heap, 1 means inserted not deleted, 2 means deleted
} point;

// nodes will live in the linked list heap
typedef struct node {
	point* p;
	struct node* prev;
	struct node* next;
} node;

// global heap for non-0D graph
static node* heap;

// void insert(point* point, float dist)
//		inserts into the heap (or updates an existing heap node)
void insert(point* point, float dist) {
	// update distance
	point->dist = dist;
	assert(point->searched != 2);
	if (point->searched == 1) return;
	point->searched = 1;
	
	// actually insert a new node into the heap
	node* n = malloc(sizeof(node));
	n->p = point;
	n->prev = NULL;
	if (heap) {
		n->next = heap;
		heap->prev = n;
	}
	else {
		n->next = NULL;
	}
	heap = n;
}


// point* deletemin(node* heap)
//		Deletes min from heap and returns it
point* deletemin() {
	node* crawler = heap;
	node* minptr = heap;
	while (crawler != NULL) {
		if (crawler->p->dist < minptr->p->dist)
			minptr = crawler;
		crawler = crawler->next;
	}

	// we've found minimum, now extract it from linked list
	if (minptr->prev) {
		minptr->prev->next = minptr->next;
	}
	else {
	    heap = minptr->next;
	}
	if (minptr->next) {
		minptr->next->prev = minptr->prev;
	}
	point* x = minptr->p;
	assert (x->searched == 1);
	x->searched = 2;
	free(minptr);
	return x;
}

// float closer(point* p, point* q, int numpoints, int dim)
//		Returns a new distance if the distance between p and q is less than
//		q's original "distance" and less than k(numpoints), -1 otherwise
float closer(point* p, point* q, int numpoints, int dim) {
	float newdist = 1;
	if(dim == 2)
		newdist = sqrt(pow((p->x - q->x), 2) + pow((p->y - q->y), 2));
	else if (dim == 3)
		newdist = sqrt(pow((p->x - q->x), 2) + pow((p->y - q->y), 2) 
			+ pow((p->z - q->z), 2));
	else if (dim == 4)
		newdist = sqrt(pow((p->x - q->x), 2) + pow((p->y - q->y), 2) 
			+ pow((p->z - q->z), 2) + pow((p->w - q->w), 2));
	if (newdist >= q->dist || newdist >= 1) return -1; // replace the 1 with k(numpoints)
	return newdist;
}


// float mst(int numpoints, int dim)
//		Returns the length of a randomly-generated 2D MST
float mst(int numpoints, int dim) {
	point* points = malloc(sizeof(point) * numpoints);
	for (int i = 0; i < numpoints; i++) {
		(points + i)->x = (float) rand() / RAND_MAX;
		(points + i)->y = (float) rand() / RAND_MAX;
		(points + i)->z = (dim == 3 || dim == 4) ? ((float) rand() / RAND_MAX) : 0.0;
		(points + i)->w = (dim == 4) ? ((float) rand() / RAND_MAX) : 0.0;
		(points + i)->dist = 10;
		(points + i)->searched = 0;
	}
	// since we're going to add the first point into the heap
    points[0].dist = 0;
    points[0].searched = 1;
    
    // To see all points
    // for (int i = 0; i < numpoints; i++) {
    //     printf("(%f, %f, %f, %f) dist:%f searched:%d\n", (points+i)->x, (points+i)->y, 
    //     	(points+i)->z, (points+i)->w, (points+i)->dist, (points+i)->searched);
    // } 
    
    heap = malloc(sizeof(node));
    // first thing in the heap will be the first point
	heap->p = points;
	heap->prev = NULL;
	heap->next = NULL;

	// prim's algorithm
	while(heap != NULL) {
		point* p = deletemin();
		// printf("newmin (%f, %f, %f, %f) %f %d\n", 
		// 	p->x, p->y, p->z, p->w, p->dist, p->searched);

		// look for new edges and potentially insert them into linked list
		for (int i = 0; i < numpoints; i++) {
			if ((points + i)->searched == 0 || (points + i)->searched == 1) {
				float length = closer(p, points + i, numpoints, dim);
				if (length >= 0) {
					(points + i)->dist = length;
					insert((points + i), length);
				}
			}
		}
	}

	// find the total length of the mst
	float total = 0;
	for (int i = 0; i < numpoints; i++) {
		assert((points + i)->searched == 2);
		total += (points + i)->dist;
	}
	free(points);
	return total;
}

int main(int argc, char** argv) {
	if (argc != 5) {
		printf("Usage: randmst flag numpoints numtrials dimensions\n");
		return 1;
	}
	// int flag = *argv[1];
	int numpoints = atoi(argv[2]);
	int numtrials = atoi(argv[3]);
	int dimensions = atoi(argv[4]);

	time_t t;
  	srand((unsigned) time(&t));

	float cumul = 0;
	for (int i = 0; i < numtrials; i++) {
		switch(dimensions) {
   			case 0:
    			cumul += mst0(numpoints);
    		    break;
    		case 2:
    		case 3:
    		case 4:
    			cumul += mst(numpoints, dimensions);
    			break;
    		default :
   				printf("Usage: invalid dimension\n");
   			return 1;
		}
	}
	float ans = cumul / ((float) numtrials);

	printf("%2f %d %d %d\n", ans, numpoints, numtrials, dimensions);
	return 0;
}
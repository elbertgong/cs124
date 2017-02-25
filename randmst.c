// can you think of a way to combine functions so that we don't have unique ones for each dimension?
// For instance, insert2 and insert0 are almost the same.. any idea how to integrate them?
// maybe separate the insert and delete functions into a header file


// Big Important issues:
// need to implement/determine k(numpoints). 
// need to implement the other dimensions.
// need to make it faster somehow- the 0D one can't get up to 20000
// need to comment this up better- my fault.


#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <float.h>
#include <math.h>
#include <string.h>


typedef struct node0 {
	int vertex;
	float dist;
	struct node0* next;
} node0;

typedef struct {
	node0* neighbors;
	float dist;
	unsigned char searched;
} point0;

typedef struct heapnode0 {
	point0* p;
	struct heapnode0* prev;
	struct heapnode0* next;
} heapnode0;

// Global heap for 2D graph
static heapnode0* heap0;

// void insert0(point0* point, float dist)
//		Insert0 into the heap (or updates an existing heap node)
void insert0(point0* point, float dist) {
	point->dist = dist;
	assert(point->searched != 2);
	if (point->searched == 1) return;
	point->searched = 1;
	
	// actually put a new node into the heap
	heapnode0* n = malloc(sizeof(heapnode0));
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

// point0* deletemin0(node0* heap)
//		Deletes min from heap and returns it
point0* deletemin0() {
	heapnode0* crawler = heap0;
	heapnode0* minptr = heap0;
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
	point0* x = minptr->p;
	assert (x->searched == 1);
	x->searched = 2;
	free(minptr);
	return x;
}



// float mst0(int numpoints)
//		Returns the length of a randomly-generated 0D MST
// for this one, generate numpoints random numbers and return max - min
float mst0(int numpoints) {
    point0* points = malloc(sizeof(point0) * numpoints);
	for (int i = 0; i < numpoints; i++) {
		(points + i)->neighbors = NULL;
		(points + i)->dist = 10; // i know it's dumb, it just has to be bigger than sqrt(26)
		(points + i)->searched = 0;
		for (int j = 0; j < i; j++) {
			float dist_ij = (float) rand() / RAND_MAX;
			if (dist_ij < 1) { // later change to k(numpoints)

				// maybe abstract out the process of inserting into a neighbors?
				node0* new = malloc(sizeof(node0));
				new->vertex = i;
				new->dist = dist_ij;
				if ((points + j)->neighbors) {
					new->next = (points + j)->neighbors;
				}
				else {
					new->next = NULL;
				}
				(points + j)->neighbors = new;
				/*
				node0* new2 = malloc(sizeof(node0));
				new2->vertex = j;
				new->dist = dist_ij;
				if ((points + i)->neighbors) {
					new2->next = (points + i)->neighbors;
				}
				else {
					new2->next = NULL;
				}
				(points + i)->neighbors = new2;
				*/
			}
		}
	}
    points[0].dist = 0;
    points[0].searched = 1;

    heap0 = malloc(sizeof(heapnode0));
	heap0->p = points;
	heap0->prev = NULL;
	heap0->next = NULL;

	while(heap0 != NULL) {
		point0* p = deletemin0();
		// printf("newmin %f %f %f %d\n", p->x, p->y, p->dist, p->searched);
		 
		// look for new edges and potentially insert them into linked list
		while (p->neighbors) {
			point0* neighbor = points + p->neighbors->vertex;
			if (neighbor->searched == 0 || neighbor->searched == 1) {
				if (p->neighbors->dist < neighbor->dist)
					insert0(neighbor, p->neighbors->dist);
			}
			// remove the neighbor from adjacency list
			node0* temp = p->neighbors;
			p->neighbors = temp->next;
			free(temp);
		}
	}

	float total = 0;
	for (int i = 0; i < numpoints; i++) {
		assert((points + i)->searched == 2);
		total += (points + i)->dist;
	}
	free(points);
	return total;
}





typedef struct {
	float x;
	float y;
	float dist;
	unsigned char searched;
	// searched: 0 means not inserted into heap, 1 means inserted not deleted, 2 means deleted
} point2;

typedef struct node2 {
	point2* p;
	struct node2* prev;
	struct node2* next;
} node2;

// Global heap for 2D graph
static node2* heap;

// void insert2(point2* point, float dist)
//		Inserts into the heap (or updates an existing heap node)
void insert2(point2* point, float dist) {
	point->dist = dist;
	assert(point->searched != 2);
	if (point->searched == 1) return;
	point->searched = 1;
	
	// actually put a new node into the linked list
	node2* n = malloc(sizeof(node2));
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


// point2* deletemin2(node2* heap)
//		Deletes min from heap and returns it
point2* deletemin2() {
	node2* crawler = heap;
	node2* minptr = heap;
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
	point2* x = minptr->p;
	assert (x->searched == 1);
	x->searched = 2;
	free(minptr);
	return x;
}

// float closer2(point2* p, point2* q, int numpoints)
//		Returns a new distance if the distance between p and q is less than
//		q's original "distance" and less than k(numpoints), -1 otherwise
float closer2(point2* p, point2* q, int numpoints) {
	float newdist = sqrt(pow((p->x - q->x), 2) + pow((p->y - q->y), 2));
	if (newdist >= q->dist || newdist >= 1) return -1; // replace the 1 with k(numpoints)
	return newdist;
}


// float mst2(int numpoints)
//		Returns the length of a randomly-generated 2D MST
float mst2(int numpoints) {
	point2* points = malloc(sizeof(point2) * numpoints);
	for (int i = 0; i < numpoints; i++) {
		(points + i)->x = (float) rand() / RAND_MAX;
		(points + i)->y = (float) rand() / RAND_MAX;
		(points + i)->dist = 10; // i know it's dumb, it just has to be bigger than sqrt(26)
		(points + i)->searched = 0;
	}
    points[0].dist = 0;
    points[0].searched = 1;
    
    /* To see all points
    for (int i = 0; i < numpoints; i++) {
        printf("origpo %f %f %f %d\n", (points+i)->x, (points+i)->y, (points+i)->dist, (points+i)->searched);
    } */
    
    heap = malloc(sizeof(node2));
	heap->p = points;
	heap->prev = NULL;
	heap->next = NULL;

	while(heap != NULL) {
		point2* p = deletemin2();
		// printf("newmin %f %f %f %d\n", p->x, p->y, p->dist, p->searched);
		
		// look for new edges and potentially insert them into linked list
		for (int i = 0; i < numpoints; i++) {
			if ((points + i)->searched == 0 || (points + i)->searched == 1) {
				float length = closer2(p, points + i, numpoints);
				if (length >= 0) {
					(points + i)->dist = length;
					insert2((points + i), length);
				}
			}
		}
	}

	float total = 0;
	for (int i = 0; i < numpoints; i++) {
		assert((points + i)->searched == 2);
		total += (points + i)->dist;
	}
	free(points);
	return total;
}


// float mst3(int numpoints)
//		Returns the length of a randomly-generated 3D MST
float mst3(int numpoints) {
    printf("mst3 not implemented\n");
    return 0;
}


// float mst4(int numpoints)
//		Returns the length of a randomly-generated 4D MST
float mst4(int numpoints) {
    printf("mst4 not implemented\n");
    return 0;
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
    			cumul += mst2(numpoints);
    			break;
    		case 3:
    			cumul += mst3(numpoints);
    			break;
    		case 4:
    			cumul += mst4(numpoints);
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
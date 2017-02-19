// can you think of a way to combine functions so that we don't have unique ones for each dimesion?
// do we want to seed random number generator?
// i didn't log into harvard's server yet- haven't tested the code
// need to check out piazza

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h> // don't think we need this one, but not sure
#include <time.h>
#include <float.h>
#include <math.h>

// timestamp()
//		Return the current time as a double.
static inline double timestamp(void) {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec + tv.tv_usec / 1000000.0;
}

// float mst0(int numoints)
//		Returns the length of a randomly-generated 0D MST



typedef struct {
	float x;
	float y;
	float dist;
	unsigned char searched;
	// searched is 0 for not inserted into heap, 1 for inserted not deleted, 2 for deleted
} point2;

typedef struct node2 {
	point2* p;
	struct node2* prev;
	struct node2* next;
} node2;


// void insert2(point2* point, float dist, node2* heap)
//		Inserts into the heap (or updates an existing heap node)
void insert2(point2* point, float dist, node2* heap) {
	point->dist = dist;
	assert(point->searched != 2);
	if (point->searched = 1) return;
	point->searched = 1;
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
point2* deletemin2(node2* heap) {
	node2* crawler = heap;
	node2* minptr = heap;
	while (crawler != NULL) {
		if (crawler->p->dist < minptr->p->dist)
			minptr = crawler;
		crawler = crawler->next;
	}
	// linked list manipulations
	if (minptr->prev) {
		minptr->prev->next = minptr->next;
	}
	if (minptr->next) {
		minptr->next->prev = minptr->prev;
	}
	point2* x = minptr->p;
	free(minptr);
	assert (x->searched = 1);
	x->searched = 2;
	return x;
}

// float closer2(point2* p, point2* q, int numpoints)
//		Returns a new distance if the distance between p and q is less than
//		q's original "distance" and less than k(n), -1 otherwise
float closer2(point2* p, point2* q, int numpoints) {
	newdist = sqrt((p->x - q->x)^2 + (p->y - q->y)^2);
	if (newdist >= q->dist || newdist >= 1) return -1; // replace the 1 with k(numpoints)
	return newdist;
}


// float mst2(int numpoints)
//		Returns the length of a randomly-generated 2D MST
float mst2(int numpoints) {
	point2* points = malloc(sizeof(point2) * numpoints); // need to free everything later???
	for (int i = 0; i < numpoints; i++) {
		*(points + i).x = (float) rand() / RAND_MAX;
		*(points + i).y = (float) rand() / RAND_MAX;
		*(points + i).dist = FLT_MAX; // are floats 4 bytes???
		*(points + i).searched = 0; // change up the serched
	}
	points[0].dist = 0;

	node2* heap = malloc(sizeof(node2));
	heap->p = points;
	heap->prev = NULL;
	heap->next = NULL;

	while(heap != NULL) {
		point2* p = deletemin2(heap);
		for (int i = 0; i < numpoints; i++) {
			if ((*points + i).searched == 0 || (*points + i).searched == 1) {
				float length = closer2(p, points + i, numpoints);
				if (length >= 0) {
					(points + i)->dist = length;
					insert2((points + i), length, heap);
				}
			}
		}
	}

	float total = 0;
	for (int i = 0; i < numpoints; i++) {
		assert((points + i)->searched == 2);
		total += (points + i)->dist;
	}
	return total;
}


// float mst3(int numpoints)
//		Returns the length of a randomly-generated 3D MST


// float mst4(int numpoints)
//		Returns the length of a randomly-generated 4D MST






int main(int argc, char** argv) {
	if (argc != 4) {
		printf("Usage: randmst flag numpoints numtrials dimensions\n");
		return 1;
	}
	int flag = argv[1];
	int numpoints = argv[2];
	int numtrials = argv[3];
	int dimensions = argv[4];

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
    		case 3:
    			cumul += mst3(numpoints);
    		case 4:
    			cumul += mst4(numpoints);
    		default :
   			printf("Usage: invalid dimension\n");
   			return 1;
		}
	}
	float ans = cumul / float(numtrials);

	printf("%2f %d %d %d\n", ans, numpoints, numtrials, dimensions);
	return 0;
}
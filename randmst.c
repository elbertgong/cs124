#include <stdio.h>
#include <sys/time.h>

// float random()
//		Returns a random float between 0 and 1
float random() {
	return 0.5;
}

// timestamp()
//		Return the current time as a double.
static inline double timestamp(void) {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec + tv.tv_usec / 1000000.0;
}

// float mst0(int numtrials)
//		Returns the length of a randomly-generated 0D MST




typedef struct {
	float x;
	float y;
	float dist;
	unsigned char searched;
} point2;

typedef struct node2 {
	point2* p;
	node2* prev;
	node2* next;
}

// void insert2(point2* point, float dist, node2* heap)
//		Inserts into the heap

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
}

// float closer2(point2* p, point2* q, float origdist)
//		Returns a new distance if the distance between p and q is less than
//		p's original "distance" and less than k(n), -1 otherwise


// float mst2(int numtrials)
//		Returns the length of a randomly-generated 2D MST
float mst2(int numpoints) {
	point2* points = malloc(sizeof(point2) * numpoints); // need to free everything later???
	for (int i = 0; i < numpoints; i++) {
		*(points + i).x = random();
		*(points + i).y = random();
		*(points + i).dist = 0xffffffff; // are floats 4 bytes???
		*(points + i).searched = 0;
	}
	points[0].dist = 0;

	node2* heap = malloc(sizeof(node2));
	heap->p = points;
	heap->prev = NULL;
	heap->next = NULL;

	while(heap != NULL) {
		point2* p = deletemin2(heap);
		for (int i = 0; i < numpoints; i++) {
			if (!(*points + i).searched) {
				float length = closer2(p, points + i, p->dist);
				if (length >= 0) {
					(points + i)->dist = length;
					insert2((points + i), length, heap);
				}
			}
		}
	}

	float total = 0;
	for (int i = 0; i < numpoints; i++)
		total += (points + i)->dist;
	return total;
}


// float mst3(int numtrials)
//		Returns the length of a randomly-generated 3D MST


// float mst4(int numtrials)
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
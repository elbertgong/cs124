#include <stdio.h>
#include <sys/time.h>

// mst0(int numtrials)
//		Returns the length of a randomly-generated MST


// timestamp()
//		Return the current time as a double.
static inline double timestamp(void) {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec + tv.tv_usec / 1000000.0;
}


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
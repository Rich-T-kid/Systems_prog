void kmeans_clustering(float* pixels, int num_pixels, int
num_centroids, int max_iters, int seed, float* centroids, int* labels);

/*
float* pixels: The input array of pixels, each pixel is a 3 byte
RGB value

int num_pixels: the number of pixels in the above array

int num_centroids: the number of centroids, which is the same as the
number of pixels in the colormap.

int max_iters: maximum number of iterations for the K-means
algorithm

int seed: see to use for the srand() function. Use srand()/rand()

float* centroids: array of 3 byte centroids.

int* labels: The color in the map for each pixel - one byte for each
pixel

*/
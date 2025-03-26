#include <stdlib.h>
#include <math.h>
#include <float.h>

void kmeans_clustering(float* pixels, int num_pixels, int num_centroids, int max_iters, int seed, float* centroids, int* labels) {
    srand(seed);

    // 1. Random initialization of centroids
    for (int i = 0; i < num_centroids; i++) {
        int rand_idx = rand() % num_pixels;
        centroids[i * 3 + 0] = pixels[rand_idx * 3 + 0];
        centroids[i * 3 + 1] = pixels[rand_idx * 3 + 1];
        centroids[i * 3 + 2] = pixels[rand_idx * 3 + 2];
    }

    for (int iter = 0; iter < max_iters; iter++) {
        // Create accumulators for means
        float* new_centroids = calloc(num_centroids * 3, sizeof(float));
        int* counts = calloc(num_centroids, sizeof(int));

        // 2. Assign pixels to nearest centroid
        for (int i = 0; i < num_pixels; i++) {
            float r = pixels[i * 3 + 0];
            float g = pixels[i * 3 + 1];
            float b = pixels[i * 3 + 2];

            float min_dist = FLT_MAX;
            int best_idx = 0;

            for (int j = 0; j < num_centroids; j++) {
                float cr = centroids[j * 3 + 0];
                float cg = centroids[j * 3 + 1];
                float cb = centroids[j * 3 + 2];

                float dist = (r - cr)*(r - cr) + (g - cg)*(g - cg) + (b - cb)*(b - cb);
                if (dist < min_dist) {
                    min_dist = dist;
                    best_idx = j;
                }
            }

            labels[i] = best_idx;
            new_centroids[best_idx * 3 + 0] += r;
            new_centroids[best_idx * 3 + 1] += g;
            new_centroids[best_idx * 3 + 2] += b;
            counts[best_idx]++;
        }

        // 3. Recompute centroids
        for (int j = 0; j < num_centroids; j++) {
            if (counts[j] > 0) {
                centroids[j * 3 + 0] = new_centroids[j * 3 + 0] / counts[j];
                centroids[j * 3 + 1] = new_centroids[j * 3 + 1] / counts[j];
                centroids[j * 3 + 2] = new_centroids[j * 3 + 2] / counts[j];
            }
        }

        free(new_centroids);
        free(counts);
    }
}

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
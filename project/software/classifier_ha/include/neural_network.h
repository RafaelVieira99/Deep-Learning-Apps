#ifndef NEURAL_NETWORK_H_
#define NEURAL_NETWORK_H_

#include <stdint.h>

#define MNIST_IMAGE_WIDTH 28
#define MNIST_IMAGE_HEIGHT 28
#define MNIST_IMAGE_SIZE MNIST_IMAGE_WIDTH * MNIST_IMAGE_HEIGHT
#define MNIST_LABELS 10

typedef struct neural_network_t_ {
    float b[MNIST_LABELS];
    float W[MNIST_LABELS][MNIST_IMAGE_SIZE];
} neural_network_t;


void neural_network_hypothesis(float *pixels, const neural_network_t * network, float activations[MNIST_LABELS]);

















#endif

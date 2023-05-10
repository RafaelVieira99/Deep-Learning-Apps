#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "include/neural_network.h"


/**
 * Use the weights and bias vector to forward propogate through the neural
 * network and calculate the activations.
 */
void neural_network_hypothesis(float *pixels, const neural_network_t * network, float activations[MNIST_LABELS])
{
    int ki, kj;
    float ksum, kmax;

    for (ki = 0; ki < MNIST_LABELS; ki++) {
        for (kj = 0; kj < MNIST_IMAGE_SIZE; kj++) {
            activations[ki] += network->W[ki][kj] * pixels[kj];
        }
    }

    /**
     * Calculate the softmax vector from the activations. This uses a more
     * numerically stable algorithm that normalises the activations to prevent
     * large exponents.
     */

    for (ki = 1, kmax = activations[0]; ki < MNIST_LABELS; ki++) {
        if (activations[ki] > kmax) {
            kmax = activations[ki];
        }
    }

    for (ki = 0, ksum = 0; ki < MNIST_LABELS; ki++) {
        activations[ki] = exp(activations[ki] - kmax);
        ksum += activations[ki];
    }

    for (ki = 0; ki < MNIST_LABELS; ki++) {
        activations[ki] /= ksum;
    }
}


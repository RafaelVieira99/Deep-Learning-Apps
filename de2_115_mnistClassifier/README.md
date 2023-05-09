This was my first ever attempt to create an hardware accelerator for a processor (NIOS II) but the project actually turned out quite nice.
The report and the video are in Portuguese, as this was part of a college project

In this project we use an SD card with images from the MNIST dataset, read it and classify those handwritten numbers inside the FPGA. Which is running C code inside NIOS II with some IP next to it trying to accelerate the classification. The weights are stored in the LUTs and used during these operations.

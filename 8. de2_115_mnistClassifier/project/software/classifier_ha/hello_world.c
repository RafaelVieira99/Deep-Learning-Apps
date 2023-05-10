
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "terasic_lib/terasic_includes.h"
#include "terasic_fat/FatFileSystem.h"
#include "include/neural_network.h"
#include "../classifier_ha_bsp/system.h"
#include "../classifier_ha_bsp/drivers/inc/altera_up_avalon_video_character_buffer_with_dma.h"
#include "../classifier_ha_bsp/drivers/inc/altera_avalon_pio_regs.h"
#include "../classifier_ha_bsp/drivers/src/altera_avalon_timer_ts.c"
#include "../classifier_ha_bsp/drivers/src/altera_avalon_timer_sc.c"
#include "../classifier_ha_bsp/drivers/src/altera_avalon_timer_vars.c"
#include "../classifier_ha_bsp/drivers/inc/altera_avalon_timer.h"
#include "../classifier_ha_bsp/drivers/inc/altera_avalon_timer_regs.h"


#include "system.h"
#include "sys/alt_stdio.h"
#include "sys/alt_irq.h"
#include "altera_avalon_pio_regs.h"
#include "altera_avalon_timer_regs.h"


// FUNCTION DECLARATIONS
//-----------------------------
bool listfiles(FAT_HANDLE hFat);
int readimage(FAT_HANDLE hFat,int *pixels,char *filename);
void file_exists(FAT_HANDLE hFat, const char *filename);
void displayimage(int *pixels,int predict,int label, float time);
void draw_menu2(short unsigned int nimages);
void draw_menu3();
void draw_menu();

float ConvertNumberToFloat(unsigned long number)
{
    int mantissaShift =  23;
    unsigned long exponentMask =  0x7f800000;
    int bias =  127;
    int signShift = 31;

    int sign = (number >> signShift) & 0x01;
    int exponent = ((number & exponentMask) >> mantissaShift) - bias;

    int power = -1;
    float total = 0.0;
    for ( int i = 0; i < mantissaShift; i++ )
    {
        int calc = (number >> (mantissaShift-i-1)) & 0x01;
        total += calc * pow(2.0, power);
        power--;
    }
    float value = (sign ? -1 : 1) * pow(2.0, exponent) * (total + 1.0);

    return value;
}

//-----------------------------
int step_version(const neural_network_t network,FAT_HANDLE hFat ){
	alt_up_char_buffer_dev * char_buffer = NULL ;
	char_buffer = alt_up_char_buffer_open_dev ("/dev/video_character_buffer_with_dma_0");
	alt_up_char_buffer_init(char_buffer);
	alt_up_char_buffer_clear(char_buffer);

	float activations[MNIST_LABELS], max_activation,time,avgtime,freq;
	int i,j, predict,label,delay;
	char nome[14];

	time=0;
	avgtime=0;
	i=0;
	delay = 0;

	float FN[10];
	float TP[10];
	float FP[10];

	float precision[10];
	float recall[10];
	float f1scores[10];
	for (i = 0; i<10;i++){
		precision[i]=0;
		recall[i]=0;
		f1scores[i]=0;
		FN[i]=0;
		TP[i]=0;
		FP[i]=0;
	}

	int *pixels = NULL;
	pixels = (int *) malloc(sizeof(int)*784);

	alt_up_char_buffer_string(char_buffer ," Press key 0 to start first sample ",24,24);
	i=21;
	while(1){
		if(IORD_ALTERA_AVALON_PIO_DATA(KEY_BASE) == 14){

			while(delay<1000000)
				delay++;
			delay=0;

			snprintf(nome, 14, "%d.txt", i);
			label = readimage(hFat,pixels,nome);

			// Calculate the activations for each image using the neural network
		    int ki, kj;
		    float ksum, kmax;

		    unsigned int ui;

		    alt_timestamp_start();
		    //pixel preprocessing and initializing
		    IOWR_ALTERA_AVALON_PIO_DATA(SYNC_DATA_BASE,1<<7);
		    for (kj = 0; kj < MNIST_IMAGE_SIZE; kj++){
		    	float f = pixels[kj] / 255.0;
				memcpy(&ui, &f, sizeof (ui));
				IOWR_ALTERA_AVALON_PIO_DATA(PIXEL_INDEX_BASE,kj);
				IOWR_ALTERA_AVALON_PIO_DATA(FLOATDATA_OUTPUT_BASE,ui);
		    }

		    //activations initializing
		    IOWR_ALTERA_AVALON_PIO_DATA(SYNC_DATA_BASE, (1<<4)+(1<<5));
		    for (ki = 0; ki < MNIST_LABELS; ki++) {
		    	float f = network.b[ki];
		    	memcpy(&ui, &f, sizeof (ui));
		    	IOWR_ALTERA_AVALON_PIO_DATA(ACTIVATIONS_INDEX_BASE, ki);
		    	IOWR_ALTERA_AVALON_PIO_DATA(FLOATDATA_OUTPUT_BASE, ui);
		    }


			for (ki = 0; ki < 10; ki++) {

				IOWR_ALTERA_AVALON_PIO_DATA(ACTIVATIONS_INDEX_BASE, ki);
				IOWR_ALTERA_AVALON_PIO_DATA(SYNC_DATA_BASE, (1<<5));
				IOWR_ALTERA_AVALON_PIO_DATA(SYNC_DATA_BASE,0);
		    	IOWR_ALTERA_AVALON_PIO_DATA(SYNC_DATA_BASE,1);

				for (kj = 0; kj < 784; kj++) {

			    	IOWR_ALTERA_AVALON_PIO_DATA(WEIGHT_INDEX_BASE,ki*784+kj); //double index
			    	IOWR_ALTERA_AVALON_PIO_DATA(SYNC_DATA_BASE, (1<<8));

			    	IOWR_ALTERA_AVALON_PIO_DATA(PIXEL_INDEX_BASE,kj); //pixel idx
			    	IOWR_ALTERA_AVALON_PIO_DATA(SYNC_DATA_BASE, (1<<8)+(1<<9));

			    	IOWR_ALTERA_AVALON_PIO_DATA(SYNC_DATA_BASE,0);
					IOWR_ALTERA_AVALON_PIO_DATA(SYNC_DATA_BASE,2);
			    	IOWR_ALTERA_AVALON_PIO_DATA(SYNC_DATA_BASE, (1<<4)+(1<<8)+(1<<9));
					IOWR_ALTERA_AVALON_PIO_DATA(SYNC_DATA_BASE,0);
			    	IOWR_ALTERA_AVALON_PIO_DATA(SYNC_DATA_BASE,1);

				}

			}


			for (ki = 0; ki < MNIST_LABELS; ki++) {
				IOWR_ALTERA_AVALON_PIO_DATA(ACTIVATIONS_INDEX_BASE, ki);
		    	IOWR_ALTERA_AVALON_PIO_DATA(SYNC_DATA_BASE,0);
		    	IOWR_ALTERA_AVALON_PIO_DATA(SYNC_DATA_BASE,1);
		    	unsigned int a = IORD_ALTERA_AVALON_PIO_DATA(RESULTS_INPUT_BASE);
		    	activations[ki] = ConvertNumberToFloat(a);
			}


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

			// Set predict to the index of the greatest activation
			for (j = 0, predict = 0, max_activation = activations[0]; j < MNIST_LABELS; j++) {
				if (max_activation < activations[j]) {
					max_activation = activations[j];
					predict = j;
				}
			}

			time = alt_timestamp();
			freq = alt_timestamp_freq();
			time = (time *1000 ) / freq; //get execution time (ms)
			avgtime += time;

			displayimage(pixels,predict,label,time);
			i++;

			if(label == predict){
				TP[label]+=1;
			}
			else{
				FN[label]+=1;
				FP[predict]+=1;
			}

		}

		if(IORD_ALTERA_AVALON_PIO_DATA(KEY_BASE) == 13)
			break;
	}



	alt_up_char_buffer_clear(char_buffer);

	avgtime = avgtime/(i+1);
	char str[20];

	alt_up_char_buffer_string(char_buffer ,"Statistics: ",28,5);
	alt_up_char_buffer_string(char_buffer ,"Avg time per sample:",28,15);
	sprintf(str, "%.3f ms", avgtime);
	alt_up_char_buffer_string(char_buffer ,str,28+30,15);

	alt_up_char_buffer_string(char_buffer ,"label",10,22);
	alt_up_char_buffer_string(char_buffer ,"Precision",25,22);
	alt_up_char_buffer_string(char_buffer ,"Recall",40,22);
	alt_up_char_buffer_string(char_buffer ,"F1 Score",55,22);

	for (i = 0; i<10; i++){
		sprintf(str, "|%d ->", i);
		alt_up_char_buffer_string(char_buffer ,str,10,24+i);

		precision[i]=TP[i]/(TP[i]+FP[i]);
		sprintf(str, "%.2f", precision[i]);
		alt_up_char_buffer_string(char_buffer ,str,25,24+i);

		recall[i]=TP[i]/(TP[i]+FN[i]);
		sprintf(str, "%.2f", recall[i]);
		alt_up_char_buffer_string(char_buffer ,str,40,24+i);

		f1scores[i]=2*((precision[i]*recall[i])/(precision[i]+recall[i]));
		sprintf(str, "%.2f", f1scores[i]);
		alt_up_char_buffer_string(char_buffer ,str,55,24+i);
	}

	alt_up_char_buffer_string(char_buffer ,"press key[0] to restart ",28,48);

	free(pixels);
	return 1;
}
//-----------------------------
int fast_version1(const neural_network_t network,FAT_HANDLE hFat, short unsigned int nimages ){
	int *pixels = NULL;
	pixels = (int *) malloc(sizeof(int)*784);

	float activations[MNIST_LABELS], max_activation,activations2[MNIST_LABELS];
	float time,avgtime;
	float freq;
	int i,j, predict,label;

	float FN[10];
	float TP[10];
	float FP[10];

	float precision[10];
	float recall[10];
	float f1scores[10];
	for (i = 0; i<10;i++){
		precision[i]=0;
		recall[i]=0;
		f1scores[i]=0;
		FN[i]=0;
		TP[i]=0;
		FP[i]=0;
	}

	char nome[14];
	time=0;
	avgtime=0;
	freq = alt_timestamp_freq();

	for (i=21; i<(nimages); i++){

		snprintf(nome, 14, "%d.txt", i);//
		label = readimage(hFat,pixels,nome);



		// Calculate the activations for each image using the neural network
	    int ki, kj;
	    float ksum, kmax;

	    unsigned int ui;

	    alt_timestamp_start();
	    float f[MNIST_IMAGE_SIZE];
	    //pixel preprocessing and initializing
	    IOWR_ALTERA_AVALON_PIO_DATA(SYNC_DATA_BASE,1<<7);
	    for (kj = 0; kj < MNIST_IMAGE_SIZE; kj++){
	    	f[kj] = pixels[kj] / 255.0;
	    	float f3 = f[kj];
			memcpy(&ui, &f3, sizeof (ui));
			IOWR_ALTERA_AVALON_PIO_DATA(PIXEL_INDEX_BASE,kj);
			IOWR_ALTERA_AVALON_PIO_DATA(FLOATDATA_OUTPUT_BASE,ui);
	    }

	    //activations initializing
	    IOWR_ALTERA_AVALON_PIO_DATA(SYNC_DATA_BASE, (1<<4)+(1<<5));
	    for (ki = 0; ki < MNIST_LABELS; ki++) {
	    	float f2 = network.b[ki];
	    	memcpy(&ui, &f2, sizeof (ui));
	    	IOWR_ALTERA_AVALON_PIO_DATA(ACTIVATIONS_INDEX_BASE, ki);
	    	IOWR_ALTERA_AVALON_PIO_DATA(FLOATDATA_OUTPUT_BASE, ui);
	    }


		for (ki = 0; ki < 10; ki++) {
			//load activation index
			IOWR_ALTERA_AVALON_PIO_DATA(ACTIVATIONS_INDEX_BASE, ki);
			//clear flip-flops
			IOWR_ALTERA_AVALON_PIO_DATA(SYNC_DATA_BASE, (1<<5));
			//load new value for the flip-flops
			IOWR_ALTERA_AVALON_PIO_DATA(SYNC_DATA_BASE,0);
	    	IOWR_ALTERA_AVALON_PIO_DATA(SYNC_DATA_BASE,1);
	    	//cpu-wise initialization
	    	activations2[ki] = network.b[ki];
	    	//pixel cicle, only half of iterations are needed
			for (kj = 0; kj < 392; kj++) {
				//load weight index
		    	IOWR_ALTERA_AVALON_PIO_DATA(WEIGHT_INDEX_BASE,ki*784+kj);
		    	//enable rom read
		    	IOWR_ALTERA_AVALON_PIO_DATA(SYNC_DATA_BASE, (1<<8));
		    	//load pixel index
		    	IOWR_ALTERA_AVALON_PIO_DATA(PIXEL_INDEX_BASE,kj);
		    	//enable ALU clock and keep rom read enabled
		    	IOWR_ALTERA_AVALON_PIO_DATA(SYNC_DATA_BASE, (1<<8)+(1<<9));
		    	//nios-wise calculation with 392 pixels of offset (calculating the other half of the image)
		    	activations2[ki] += network.W[ki][kj+392] * f[kj+392];
		    	//load value from ALU to flip flop
		    	IOWR_ALTERA_AVALON_PIO_DATA(SYNC_DATA_BASE,0);
				IOWR_ALTERA_AVALON_PIO_DATA(SYNC_DATA_BASE,2);
				//enable writes on activation RAM
		    	IOWR_ALTERA_AVALON_PIO_DATA(SYNC_DATA_BASE, (1<<4)+(1<<8)+(1<<9));
		    	//load new value for the activation DFF
				IOWR_ALTERA_AVALON_PIO_DATA(SYNC_DATA_BASE,0);
		    	IOWR_ALTERA_AVALON_PIO_DATA(SYNC_DATA_BASE,1);
		    	//repeat for next pixel
			}
		}


		//copy back the RTL calculations and sum them to CPU activations
		for (ki = 0; ki < MNIST_LABELS; ki++) {
			IOWR_ALTERA_AVALON_PIO_DATA(ACTIVATIONS_INDEX_BASE, ki);
	    	IOWR_ALTERA_AVALON_PIO_DATA(SYNC_DATA_BASE,0);
	    	IOWR_ALTERA_AVALON_PIO_DATA(SYNC_DATA_BASE,1);
	    	unsigned int a = IORD_ALTERA_AVALON_PIO_DATA(RESULTS_INPUT_BASE);
	    	activations[ki] = ConvertNumberToFloat(a);
	    	activations[ki] += activations2[ki];
		}

		time = alt_timestamp();
		//calculate softmax
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


		// Set predict to the index of the greatest activation
		for (j = 0, predict = 0, max_activation = activations[0]; j < MNIST_LABELS; j++) {
			if (max_activation < activations[j]) {
				max_activation = activations[j];
				predict = j;
			}
		}

		if(label == predict){
			TP[label]+=1;
		}
		else{
			FN[label]+=1;
			FP[predict]+=1;
		}


		time = (time *1000 ) / freq; //get execution time (ms)
		avgtime += time;

	}

	//SHOW STATS
	alt_up_char_buffer_dev * char_buffer = NULL ;
	char_buffer = alt_up_char_buffer_open_dev ("/dev/video_character_buffer_with_dma_0");
	alt_up_char_buffer_init(char_buffer);
	alt_up_char_buffer_clear(char_buffer);

	time    = avgtime;
	avgtime = avgtime/nimages;
	char str[20];

	alt_up_char_buffer_string(char_buffer ,"Statistics: ",28,5);
	alt_up_char_buffer_string(char_buffer ,"Execution time:",28,15);
	sprintf(str, "%.3f s", time/1000);
	alt_up_char_buffer_string(char_buffer ,str,28+30,15);

	alt_up_char_buffer_string(char_buffer ,"Avg time per sample:",28,17);
	sprintf(str, "%.3f ms", avgtime);
	alt_up_char_buffer_string(char_buffer ,str,28+30,17);

	alt_up_char_buffer_string(char_buffer ,"label",10,22);
	alt_up_char_buffer_string(char_buffer ,"Precision",25,22);
	alt_up_char_buffer_string(char_buffer ,"Recall",40,22);
	alt_up_char_buffer_string(char_buffer ,"F1 Score",55,22);

	for (i = 0; i<10; i++){
		sprintf(str, "|%d ->", i);
		alt_up_char_buffer_string(char_buffer ,str,10,24+i);

		precision[i]=TP[i]/(TP[i]+FP[i]);
		sprintf(str, "%.2f", precision[i]);
		alt_up_char_buffer_string(char_buffer ,str,25,24+i);

		recall[i]=TP[i]/(TP[i]+FN[i]);
		sprintf(str, "%.2f", recall[i]);
		alt_up_char_buffer_string(char_buffer ,str,40,24+i);

		f1scores[i]=2*((precision[i]*recall[i])/(precision[i]+recall[i]));
		sprintf(str, "%.2f", f1scores[i]);
		alt_up_char_buffer_string(char_buffer ,str,55,24+i);
	}

	alt_up_char_buffer_string(char_buffer ,"press key[0] to restart ",28,48);
	free(pixels);
	return 1;
}
int fast_version(const neural_network_t network,FAT_HANDLE hFat, short unsigned int nimages ){
	int *pixels = NULL;
	pixels = (int *) malloc(sizeof(int)*784);

	float activations[MNIST_LABELS], max_activation;
	float time,avgtime;
	float freq;
	int i,j, predict,label;


	float FN[10];
	float TP[10];
	float FP[10];

	float precision[10];
	float recall[10];
	float f1scores[10];
	for (i = 0; i<10;i++){
		precision[i]=0;
		recall[i]=0;
		f1scores[i]=0;
		FN[i]=0;
		TP[i]=0;
		FP[i]=0;
	}

	char nome[14];
	time=0;
	avgtime=0;
	freq = alt_timestamp_freq();
	for (i=21; i<(nimages); i++){

		snprintf(nome, 14, "%d.txt", i);//
		label = readimage(hFat,pixels,nome);
	    int ki, kj;
	    float ksum, kmax;

	    alt_timestamp_start();
		//normalize pixel values between 0 and 1
		float f[784];
		for (j = 0; j<784;j++){
			f[j]=pixels[j]/255.0;
		}

		// Calculate the activations for each image using the neural network
		//start timer

	    for (ki = 0; ki < MNIST_LABELS; ki++) {
	    	activations[ki] = network.b[ki];
	        for (kj = 0; kj < MNIST_IMAGE_SIZE; kj++) {
	            activations[ki] += network.W[ki][kj] * f[kj];
	        }
	    }
	    time = alt_timestamp();//end timer

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


		// Set predict to the index of the greatest activation
		for (j = 0, predict = 0, max_activation = activations[0]; j < MNIST_LABELS; j++) {
			if (max_activation < activations[j]) {
				max_activation = activations[j];
				predict = j;
			}
		}

		if(label == predict){
			TP[label]+=1;
		}
		else{
			FN[label]+=1;
			FP[predict]+=1;
		}

		time = (time *1000 ) / freq; //get execution time (ms)
		avgtime += time;
	}

	//SHOW STATS
	alt_up_char_buffer_dev * char_buffer = NULL ;
	char_buffer = alt_up_char_buffer_open_dev ("/dev/video_character_buffer_with_dma_0");
	alt_up_char_buffer_init(char_buffer);
	alt_up_char_buffer_clear(char_buffer);

	time    = avgtime;
	avgtime = avgtime/nimages;
	char str[20];

	alt_up_char_buffer_string(char_buffer ,"Statistics: ",28,5);
	alt_up_char_buffer_string(char_buffer ,"Execution time:",28,15);
	sprintf(str, "%.3f s", time/1000);
	alt_up_char_buffer_string(char_buffer ,str,28+30,15);

	alt_up_char_buffer_string(char_buffer ,"Avg time per sample:",28,17);
	sprintf(str, "%.3f ms", avgtime);
	alt_up_char_buffer_string(char_buffer ,str,28+30,17);

	alt_up_char_buffer_string(char_buffer ,"label",10,22);
	alt_up_char_buffer_string(char_buffer ,"Precision",25,22);
	alt_up_char_buffer_string(char_buffer ,"Recall",40,22);
	alt_up_char_buffer_string(char_buffer ,"F1 Score",55,22);

	for (i = 0; i<10; i++){
		sprintf(str, "|%d ->", i);
		alt_up_char_buffer_string(char_buffer ,str,10,24+i);

		precision[i]=TP[i]/(TP[i]+FP[i]);
		sprintf(str, "%.2f", precision[i]);
		alt_up_char_buffer_string(char_buffer ,str,25,24+i);

		recall[i]=TP[i]/(TP[i]+FN[i]);
		sprintf(str, "%.2f", recall[i]);
		alt_up_char_buffer_string(char_buffer ,str,40,24+i);

		f1scores[i]=2*((precision[i]*recall[i])/(precision[i]+recall[i]));
		sprintf(str, "%.2f", f1scores[i]);
		alt_up_char_buffer_string(char_buffer ,str,55,24+i);
	}

	alt_up_char_buffer_string(char_buffer ,"press key[0] to restart ",28,48);

	free(pixels);
	return 1;
}
int fast_version3(const neural_network_t network,FAT_HANDLE hFat, short unsigned int nimages ){
	int *pixels = NULL;
	pixels = (int *) malloc(sizeof(int)*784);

	float activations[MNIST_LABELS], max_activation;
	float time,avgtime;
	float freq;
	int i,j, predict,label;

	float FN[10];
	float TP[10];
	float FP[10];

	float precision[10];
	float recall[10];
	float f1scores[10];
	for (i = 0; i<10;i++){
		precision[i]=0;
		recall[i]=0;
		f1scores[i]=0;
		FN[i]=0;
		TP[i]=0;
		FP[i]=0;
	}

	char nome[14];
	time=0;
	avgtime=0;
	freq = alt_timestamp_freq();

	for (i=21; i<(nimages); i++){

		snprintf(nome, 14, "%d.txt", i);//
		label = readimage(hFat,pixels,nome);
		// Calculate the activations for each image using the neural network
	    int ki, kj;
	    float ksum, kmax;

	    unsigned int ui;

	    alt_timestamp_start();
	    //pixel preprocessing and initializing
	    IOWR_ALTERA_AVALON_PIO_DATA(SYNC_DATA_BASE,1<<7);
	    for (kj = 0; kj < MNIST_IMAGE_SIZE; kj++){
	    	float f = pixels[kj] / 255.0;
			memcpy(&ui, &f, sizeof (ui));
			IOWR_ALTERA_AVALON_PIO_DATA(PIXEL_INDEX_BASE,kj);
			IOWR_ALTERA_AVALON_PIO_DATA(FLOATDATA_OUTPUT_BASE,ui);
	    }

	    //activations initializing
	    IOWR_ALTERA_AVALON_PIO_DATA(SYNC_DATA_BASE, (1<<4)+(1<<5));
	    for (ki = 0; ki < MNIST_LABELS; ki++) {
	    	float f = network.b[ki];
	    	memcpy(&ui, &f, sizeof (ui));
	    	IOWR_ALTERA_AVALON_PIO_DATA(ACTIVATIONS_INDEX_BASE, ki);
	    	IOWR_ALTERA_AVALON_PIO_DATA(FLOATDATA_OUTPUT_BASE, ui);
	    }

		for (ki = 0; ki < 10; ki++) {

			IOWR_ALTERA_AVALON_PIO_DATA(ACTIVATIONS_INDEX_BASE, ki);
			IOWR_ALTERA_AVALON_PIO_DATA(SYNC_DATA_BASE, (1<<5));
			IOWR_ALTERA_AVALON_PIO_DATA(SYNC_DATA_BASE,0);
	    	IOWR_ALTERA_AVALON_PIO_DATA(SYNC_DATA_BASE,1);

			for (kj = 0; kj < 784; kj++) {

		    	IOWR_ALTERA_AVALON_PIO_DATA(WEIGHT_INDEX_BASE,ki*784+kj); //double index
		    	IOWR_ALTERA_AVALON_PIO_DATA(SYNC_DATA_BASE, (1<<8));

		    	IOWR_ALTERA_AVALON_PIO_DATA(PIXEL_INDEX_BASE,kj); //pixel idx
		    	IOWR_ALTERA_AVALON_PIO_DATA(SYNC_DATA_BASE, (1<<8)+(1<<9));

		    	IOWR_ALTERA_AVALON_PIO_DATA(SYNC_DATA_BASE,0);
				IOWR_ALTERA_AVALON_PIO_DATA(SYNC_DATA_BASE,2);
		    	IOWR_ALTERA_AVALON_PIO_DATA(SYNC_DATA_BASE, (1<<4)+(1<<8)+(1<<9));
				IOWR_ALTERA_AVALON_PIO_DATA(SYNC_DATA_BASE,0);
		    	IOWR_ALTERA_AVALON_PIO_DATA(SYNC_DATA_BASE,1);

			}

		}


		for (ki = 0; ki < MNIST_LABELS; ki++) {
			IOWR_ALTERA_AVALON_PIO_DATA(ACTIVATIONS_INDEX_BASE, ki);
	    	IOWR_ALTERA_AVALON_PIO_DATA(SYNC_DATA_BASE,0);
	    	IOWR_ALTERA_AVALON_PIO_DATA(SYNC_DATA_BASE,1);
	    	unsigned int a = IORD_ALTERA_AVALON_PIO_DATA(RESULTS_INPUT_BASE);
	    	activations[ki] = ConvertNumberToFloat(a);
		}
		time = alt_timestamp();

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

		// Set predict to the index of the greatest activation
		for (j = 0, predict = 0, max_activation = activations[0]; j < MNIST_LABELS; j++) {
			if (max_activation < activations[j]) {
				max_activation = activations[j];
				predict = j;
			}
		}

		if(label == predict){
			TP[label]+=1;
		}
		else{
			FN[label]+=1;
			FP[predict]+=1;
		}


		time = (time *1000 ) / freq; //get execution time (ms)
		avgtime += time;

	}

	//SHOW STATS
	alt_up_char_buffer_dev * char_buffer = NULL ;
	char_buffer = alt_up_char_buffer_open_dev ("/dev/video_character_buffer_with_dma_0");
	alt_up_char_buffer_init(char_buffer);
	alt_up_char_buffer_clear(char_buffer);

	time    = avgtime;
	avgtime = avgtime/nimages;
	char str[20];

	alt_up_char_buffer_string(char_buffer ,"Statistics: ",28,5);
	alt_up_char_buffer_string(char_buffer ,"Execution time:",28,15);
	sprintf(str, "%.3f s", time/1000);
	alt_up_char_buffer_string(char_buffer ,str,28+30,15);

	alt_up_char_buffer_string(char_buffer ,"Avg time per sample:",28,17);
	sprintf(str, "%.3f ms", avgtime);
	alt_up_char_buffer_string(char_buffer ,str,28+30,17);

	alt_up_char_buffer_string(char_buffer ,"label",10,22);
	alt_up_char_buffer_string(char_buffer ,"Precision",25,22);
	alt_up_char_buffer_string(char_buffer ,"Recall",40,22);
	alt_up_char_buffer_string(char_buffer ,"F1 Score",55,22);

	for (i = 0; i<10; i++){
		sprintf(str, "|%d ->", i);
		alt_up_char_buffer_string(char_buffer ,str,10,24+i);

		precision[i]=TP[i]/(TP[i]+FP[i]);
		sprintf(str, "%.2f", precision[i]);
		alt_up_char_buffer_string(char_buffer ,str,25,24+i);

		recall[i]=TP[i]/(TP[i]+FN[i]);
		sprintf(str, "%.2f", recall[i]);
		alt_up_char_buffer_string(char_buffer ,str,40,24+i);

		f1scores[i]=2*((precision[i]*recall[i])/(precision[i]+recall[i]));
		sprintf(str, "%.2f", f1scores[i]);
		alt_up_char_buffer_string(char_buffer ,str,55,24+i);
	}

	alt_up_char_buffer_string(char_buffer ,"press key[0] to restart ",28,48);

	free(pixels);
	return 1;
}

//-----------------------------
int main(){
	IOWR_ALTERA_AVALON_PIO_DATA(LEDR_BASE, 0);
	IOWR_ALTERA_AVALON_PIO_DATA(LEDG_BASE, 0);

    FAT_HANDLE hFat;
	hFat = Fat_Mount(FAT_SD_CARD, 0);

	if (hFat){
		printf("SD card mount success!\n");

	    const neural_network_t network =
	    {       //bias
	    		{-0.343634,0.732933,0.557800,0.275675,0.468390,1.817463,0.463807,0.824511,-1.144381,0.292256},

				{//weights
						{0.563585,0.193304,0.808740,0.585009,0.479873,0.350291,0.895962,0.822840,0.746605,0.174108,0.858943,0.710501,0.513534,0.303992,0.014984,0.091403,0.364452,0.147313,0.165899,0.988525,0.445692,0.119083,0.004669,0.008911,0.377880,0.531663,0.571184,0.601764,0.607166,0.166234,0.663045,0.450789,0.352123,0.057039,0.607680,0.783308,0.802592,0.519869,0.301890,0.880114,0.735211,0.959261,0.924994,0.542059,0.150823,0.465985,0.234807,0.861802,0.208849,0.779385,0.843606,0.996795,0.999695,0.611499,0.392438,0.266213,0.297281,0.840144,0.023743,0.376009,0.092676,0.677198,0.054016,0.003773,0.907928,0.259789,0.260820,0.576733,0.680089,0.817557,0.698897,0.465045,0.191605,0.721242,0.443534,0.438725,0.937587,0.738385,0.106085,0.597758,0.384379,0.734997,0.608966,0.572405,0.361339,0.151555,0.225104,0.425288,0.801732,0.506869,0.972913,0.721951,0.316732,0.116121,0.602576,0.441725,-0.004247,0.636689,0.448851,0.123858,0.919156,0.083091,0.790043,0.574232,0.208788,0.359620,0.023769,0.941500,0.670130,0.151547,0.877061,0.821680,0.582049,0.191351,0.177889,0.816969,0.473163,0.139552,0.475710,0.680893,0.338448,0.217743,0.496753,0.580432,0.631965,0.684680,0.521875,0.276555,0.444534,0.891484,0.045922,0.436625,0.548522,0.332522,0.057646,0.904168,0.883241,0.539362,0.345161,0.471711,0.374981,0.846980,0.316834,0.455281,0.259699,0.947228,0.234353,0.618149,0.454847,0.105858,0.690698,0.737587,0.397781,0.681259,1.114472,0.199477,1.081943,0.770886,0.274703,0.668428,0.242186,0.819293,0.582577,0.580463,0.128085,0.810651,0.118785,0.109452,0.743126,0.314066,0.940739,0.284230,0.320423,0.101743,0.628834,0.675045,0.546358,0.456375,0.664247,0.245724,0.291582,0.271737,0.326913,0.909214,0.911647,0.371285,0.368761,0.817074,0.259839,0.341642,0.536496,0.672875,0.561315,0.464461,0.049083,0.433738,0.201941,0.696157,0.283820,0.424151,0.203656,0.523330,0.399094,0.436408,-0.013618,0.392790,0.846611,0.658197,1.002156,0.370894,0.479115,0.292174,1.038183,0.487148,1.040825,0.287371,0.849853,0.313142,0.539627,0.914342,-0.017932,0.803083,0.252794,0.223820,0.770094,0.410604,0.197333,0.612675,0.561009,0.364843,0.303713,0.394808,0.453103,0.708365,0.746391,0.563898,0.700749,0.567279,0.507097,0.408075,1.026916,0.721903,0.926296,0.363526,0.427187,0.814438,0.872747,0.711472,0.363520,0.407610,0.050936,0.737832,0.612048,0.620072,0.690536,0.795757,0.113676,0.498564,0.708491,0.717982,0.432592,0.560641,-0.034387,0.501508,0.814187,0.390193,0.677006,0.466659,0.482511,0.387937,0.961002,0.901307,0.866729,0.321298,0.498591,0.293489,0.152990,0.556918,0.789017,0.544201,0.448255,0.408736,0.302957,0.462187,0.474490,0.097644,0.217536,0.628554,0.243623,0.694983,0.765776,0.670313,0.648654,0.370307,0.988809,0.166160,0.194052,0.995125,0.652412,0.559156,0.966209,0.243697,1.116844,0.885196,0.399092,0.020485,0.468948,0.226274,0.290280,0.920458,0.549930,0.659501,0.066379,0.420861,0.350606,0.553978,0.800526,0.451756,0.300299,0.627355,0.458275,0.493404,0.174820,-0.297375,0.414376,0.108430,0.680562,0.828104,0.526787,0.980449,0.849990,0.672405,0.889845,0.510170,0.344584,0.954976,0.630848,0.176496,0.373027,0.125728,0.689035,0.936973,0.714853,0.207857,0.421659,0.142201,0.795677,0.285183,0.826403,0.577980,0.058480,0.048749,0.283539,-0.117247,0.241741,0.727960,0.646036,0.450530,0.535021,0.578188,0.800629,0.343786,0.318134,0.367495,0.661000,0.802483,0.807334,0.517850,0.565237,0.861984,1.139431,0.416765,0.744272,0.634095,0.708194,0.461805,0.854463,-0.261934,-0.192178,-0.170849,-0.273440,0.380113,0.309357,0.272637,0.097789,1.049773,0.635425,0.701565,0.948207,0.908486,0.788253,0.743488,0.615208,0.361108,0.856435,0.206129,0.814622,0.438814,0.584591,0.799819,1.060771,0.406760,0.563363,0.842843,0.457192,-0.195625,-0.335644,0.233803,-0.243188,0.121762,0.167566,0.761978,0.091828,0.365943,0.524233,0.723798,1.061699,0.406111,0.431725,0.946142,0.837178,0.534223,0.841845,0.670313,0.405182,0.558056,0.303950,0.677374,1.012770,0.749505,0.643508,0.680488,0.133142,0.214822,0.261766,-0.220252,0.474006,0.002694,0.911427,0.376854,0.833187,0.885622,0.543111,0.534108,0.505455,0.005403,0.416530,0.082230,0.659929,0.855077,0.064452,0.786424,0.694203,0.955809,1.063779,0.608767,0.830011,0.526189,0.981575,0.730374,0.470767,-0.552549,0.199973,0.108746,0.448599,0.142051,0.484658,0.810416,0.267868,0.596276,0.404440,1.143340,0.680873,0.275285,0.916297,0.367739,0.694540,0.218622,0.155283,0.219505,0.569240,1.094178,0.324186,0.937971,0.557708,0.474624,1.098750,0.242195,0.706465,0.189965,0.285251,-0.135269,0.402366,0.114766,0.680171,0.148685,0.650917,0.550404,1.146830,0.118148,0.283825,0.173582,0.245027,0.155315,0.620317,0.885370,0.938340,0.172159,0.828800,0.779282,0.794547,0.956721,0.573970,1.136687,0.923518,0.254901,0.410780,0.097894,-0.018681,0.719150,0.471505,0.560562,0.453993,0.301905,0.510525,0.471335,0.123664,0.835834,0.208467,0.065880,0.388289,0.590218,0.070925,0.198368,0.155088,0.630305,0.469004,0.718763,0.875295,0.584249,0.764068,0.552293,1.029569,0.768580,0.247051,0.744432,0.639846,0.291975,0.896931,0.135496,0.778611,0.522600,0.440190,0.689684,0.755345,0.044977,0.275695,0.252689,0.406416,0.895187,0.710196,0.730210,0.894849,0.388356,0.430688,1.037448,0.546734,0.604710,0.688570,0.521035,0.968756,0.991391,0.935210,0.815099,0.159593,0.235383,0.244767,0.622723,0.757996,0.234895,0.456066,0.108224,0.878477,0.148352,0.481916,0.646623,0.439274,0.687616,0.090426,0.079559,0.073155,0.030242,0.374473,0.442286,0.913536,0.436338,0.613679,0.297130,0.910597,1.022514,0.451195,0.790937,0.613406,0.633864,0.531727,0.039128,0.410296,-0.092261,0.847817,0.377616,0.889611,0.664998,0.491970,0.671547,0.697735,0.372117,0.388623,0.049958,0.809203,0.163939,0.046390,0.374184,0.914127,0.397470,0.530181,0.716485,0.254956,0.656465,0.661291,0.780115,0.834871,0.497445,0.406261,0.754535,0.751656,0.419377,0.808663,-0.086452,0.488389,0.820757,0.366152,0.304326,0.286495,0.026712,0.612568,0.391186,0.018540,0.024479,0.415188,0.813445,0.896732,0.389356,0.704968,0.619278,0.878021,1.037093,0.781681,0.831382,0.283111,0.820556,0.339944,0.425325,0.361396,0.350694,0.620364,0.554400,0.289929,-0.016548,0.259484,0.340070,0.231748,0.771020,0.371746,0.869869,0.115043,0.722458,0.613400,0.856745,0.115194,0.412282,0.731208,0.589512,0.793790,0.056391,0.799743,0.697860,0.812434,0.656200,0.688845,0.464946,0.047657,0.501034,0.160602,0.178681,0.703135,0.778972,0.169567,0.713871,0.480941,0.081484,0.806543,0.855342,0.124635,0.307414,0.571729,0.462909,0.735198,0.140566,0.641372,0.881338,-0.109148,0.516376,0.524777,0.013564,-0.093981,0.139365,0.556946,0.725373,0.470775,0.322515,0.730043,0.193963,0.620974,0.226977,0.203684,0.971038,0.281747,0.022889,0.769219,0.151830,0.046968,0.009241,0.348395,0.639643,0.331184,0.391835,0.389434,0.062792,0.518088,0.670253,0.302332,0.602126,0.816486,0.484054,0.528841,0.018318,0.438144,0.248574,0.411246,0.846935,0.475782,0.085792,0.565810,0.709403,0.778466,0.864223,0.850551,0.293008,0.864376,0.644917,0.746849,0.828578,0.764580,0.867546,0.903846,0.912062,0.521623,0.439532,0.105282,0.347939,0.812858,0.417038,0.164252,-0.006676,0.652513,0.771180,0.059670,0.086295,0.378319,0.682361,0.807611,0.154088,0.142857,0.048463},
						{0.018555,0.978423,0.647633,0.053652,0.914151,0.637471,0.431715,0.021058,0.242592,0.391949,0.654012,0.781884,0.022819,0.826025,0.138432,0.634266,0.550401,0.983490,0.463424,0.214789,0.300699,0.782159,0.342540,0.480605,0.608020,0.102512,0.984436,0.755821,0.150884,0.456404,0.711631,0.998749,0.775506,0.641652,0.621204,0.946093,0.687644,0.289390,0.566718,0.289481,0.548063,0.711820,0.249189,0.550849,0.491797,0.951592,0.669757,0.765108,0.623194,0.445407,0.798634,0.745536,0.988922,0.204627,0.629475,0.909818,0.591296,0.918393,0.505539,0.489395,0.449720,0.889339,0.940384,0.182619,0.248991,0.197592,0.723077,0.932484,0.629593,0.069684,0.663815,0.076500,0.579386,0.545814,0.442686,0.062640,0.992314,0.122357,0.598287,0.049569,0.172669,0.660235,0.332438,0.315958,0.653584,0.410535,0.427602,0.221649,0.678516,0.166391,0.497164,0.504732,0.788913,0.413213,-0.028363,0.300772,0.368118,0.135568,0.812466,0.705483,0.843970,0.567682,0.580905,0.461798,0.683712,0.699905,0.938399,0.454516,0.355540,0.110731,0.902358,0.821223,0.454787,0.108676,0.869778,0.761449,0.040218,0.422121,0.938070,0.065442,0.172047,0.206432,0.106517,0.588723,0.229589,0.794008,0.698460,0.715494,0.623719,0.734014,0.567766,0.302416,0.837116,0.428727,1.043481,0.782728,0.177492,0.156223,0.387534,0.180947,0.580981,0.195624,0.960440,0.190805,0.383358,0.322261,0.135533,0.191754,-0.001317,0.690158,0.678502,0.859245,0.153660,0.670114,0.560418,0.788516,0.356167,0.401779,0.708092,0.335979,0.759332,0.320820,1.088921,0.956812,0.863347,0.039570,0.489623,0.631671,0.747215,0.756065,0.254884,0.909328,0.790856,0.189470,0.520777,-0.037715,0.361040,0.578667,0.538508,0.015751,0.793071,0.567832,0.772300,0.494180,0.486801,0.119475,0.312354,0.552928,0.265992,0.818400,0.129852,0.798540,0.745391,0.196217,0.152969,0.056818,0.670614,0.032306,0.345068,0.696120,0.795500,0.393057,0.870901,0.480838,0.054199,0.207326,0.626769,-0.017048,0.137505,0.088551,-0.049086,0.472632,-0.025386,0.107338,0.578555,0.095843,0.928081,0.308623,0.916148,0.230293,0.209014,0.176036,0.225590,0.844924,0.874843,0.887622,0.844395,0.608541,0.034296,0.136472,0.083233,0.184211,0.190097,-0.120787,0.611044,0.430190,0.469156,0.887701,0.538856,0.633814,0.784506,0.731554,0.650235,0.798337,0.109024,0.299907,0.174195,0.041178,0.853187,0.686699,0.711535,0.030273,0.645039,0.812587,0.735504,0.816207,0.110968,0.670498,0.276668,0.481507,0.669484,0.620179,-0.184351,0.594212,0.401569,0.236778,0.645326,0.783339,0.253755,0.724443,0.387970,0.154984,0.559505,0.688454,0.022883,-0.121640,0.659280,0.176324,0.548319,0.802850,0.852015,0.332624,0.538261,0.985403,0.848816,0.589342,0.112328,0.640563,0.172541,0.179880,-0.094445,0.407835,0.505350,0.522967,1.043162,1.584043,0.436276,0.808652,0.022759,0.678914,0.625837,0.217382,0.063238,0.509577,0.468590,0.589951,0.417422,0.015986,0.864131,0.139248,0.266779,0.287448,0.670117,0.519266,0.842811,0.447088,0.624397,0.263177,0.642453,0.547047,0.212133,0.940011,0.936015,0.811912,0.859052,0.167730,0.758438,0.222392,0.547202,-0.076411,0.547782,-0.037937,0.634039,0.946848,0.494749,0.707011,0.265938,0.821790,0.645839,0.596362,0.261388,0.806051,0.378874,0.638140,0.232009,-0.111313,0.259434,0.395567,0.412558,0.819730,1.021487,0.924254,0.552445,0.725851,0.259457,0.681617,0.220047,0.458822,-0.033087,0.936202,0.829631,0.422193,0.694512,0.333337,0.602361,0.498460,0.559342,0.039573,0.374446,0.200416,0.870175,0.756921,0.865465,0.455654,0.307213,-0.159316,0.380078,0.825472,1.272346,0.495813,0.840346,0.306926,0.563175,0.703629,0.450102,0.147612,0.228776,0.507593,0.829667,0.231557,0.297147,0.727008,0.866604,0.231082,0.190820,0.448238,0.889572,0.547646,-0.025135,0.899874,-0.041210,0.433770,-0.051233,-0.079659,0.262133,0.795586,1.099630,1.272738,0.266672,0.121142,0.119325,0.059962,0.374512,0.413680,0.349173,0.865907,0.566372,0.276141,0.472774,0.401648,0.951323,0.548356,0.835587,0.793839,0.793168,0.550045,0.792996,0.231153,0.827939,0.048334,0.599889,0.580537,0.411214,1.313120,1.064223,0.529287,0.281111,-0.051188,0.202248,0.602737,-0.041726,-0.083007,0.136813,0.761193,0.762370,0.762532,0.652932,0.297695,0.781426,0.921359,0.841979,0.237140,0.482368,-0.034128,-0.018300,-0.078036,0.195717,0.483053,0.189606,0.192685,0.464692,0.621889,1.271508,0.439560,0.084728,0.285661,0.052642,0.516551,-0.058371,0.771705,0.043804,-0.031392,0.148153,0.797147,0.244269,0.570568,0.346904,0.539952,0.663862,0.861859,0.906295,0.846776,-0.043178,-0.001292,0.370401,0.571006,0.804618,0.110990,1.020362,1.145131,0.787510,0.717908,0.542202,0.207312,0.029619,0.708035,0.777987,0.577260,0.490325,0.187655,0.075856,0.157095,0.593641,0.486858,0.355327,0.229929,0.477820,0.679761,0.905973,0.530069,0.745694,0.016210,0.347264,0.125409,0.786417,0.485275,0.478138,0.751451,0.692111,0.312024,0.249850,0.102766,0.186354,0.082853,0.798591,0.625047,0.700441,0.569601,0.636490,0.248820,0.764561,0.067430,0.721765,0.859048,0.339469,0.025299,0.430060,-0.124735,0.635910,0.444368,0.407627,0.554775,0.482101,0.837684,0.772020,0.817217,0.642746,0.842816,0.349296,0.574080,0.886876,0.079369,0.443413,0.036070,0.485146,0.257897,0.078635,0.252547,0.983119,0.507944,0.615528,0.070950,0.360308,0.928453,0.810401,0.250699,0.166621,0.224711,0.030940,0.957624,0.229394,0.270125,0.426813,0.264047,0.675984,0.735841,0.450008,0.631619,0.411158,0.354629,0.619734,0.220884,0.644805,0.113777,0.472038,0.766152,0.336197,0.057421,0.282543,0.694288,0.861149,0.555974,0.860132,0.143451,0.847267,1.010718,0.651772,0.553534,0.632803,0.598293,0.175521,0.787001,0.214333,0.297309,0.556332,0.525117,0.564847,0.821730,0.294820,0.918591,0.346227,0.710586,0.217113,0.064205,0.605863,0.353984,0.361279,0.668118,0.966564,0.214809,0.915326,0.493643,1.070025,0.696755,0.193884,0.227777,0.324617,0.399336,0.100175,0.052722,0.159994,0.359698,0.938056,1.018017,0.747022,0.800296,-0.000134,0.460309,0.542286,0.908093,0.288103,0.611582,0.747194,0.873836,0.912229,0.830103,0.605829,0.294726,0.784451,0.236956,0.637183,0.704449,0.360235,0.946022,0.144834,0.420128,0.021644,0.554981,0.280244,0.432687,0.493555,0.379363,0.881898,0.753477,0.028801,0.439063,0.013063,0.927485,0.078591,0.566297,0.963997,0.573901,0.878658,0.408704,0.403136,0.403164,0.627588,0.786508,0.913843,0.231136,-0.056654,0.621714,0.079462,0.547098,0.165109,0.572766,0.376630,0.165548,0.383805,0.344879,0.620234,0.809660,0.002239,0.105148,0.385381,0.267775,0.314196,0.825258,0.853739,0.311533,0.197668,0.564684,0.469069,0.993259,0.417291,0.337409,0.378344,0.320032,0.511235,0.795869,0.213487,0.251787,0.728750,0.217884,0.196861,0.457677,0.682626,0.040856,0.135707,0.195623,0.082545,0.315480,0.470455,0.627369,0.332452,0.079314,0.614517,0.142827,0.280709,0.824976,0.439070,0.994251,0.218419,0.101909,0.513426,0.259506,0.002025,0.565408,0.749389,0.079476,0.898346,0.279474,0.843428,0.493232,0.207637,0.298178,0.471153,0.306226,0.413553,0.763932,0.841388,0.389051,0.427473,0.724174,0.734916,0.091403,0.081729,0.753075,0.846980,0.637806,0.359629,0.475016,0.094383,0.415267,0.188328,0.343097,0.912127,0.229077,0.307277,0.149179,0.338898,0.181805,0.272067,0.204855,0.169183,0.334958,0.065350,0.501873,0.963580,0.809258,0.446455,0.912992,0.179266,0.481918},
						{0.699240,0.248604,0.936705,0.277444,0.299539,0.822443,0.819636,0.629902,0.795068,0.007202,0.260964,0.289529,0.952318,0.858068,0.517481,0.707512,0.256172,0.921049,0.491256,0.093539,0.825983,0.076693,0.282632,0.383862,0.643269,0.515091,0.431532,0.140721,0.974975,0.074618,0.005188,0.914792,0.411664,0.755332,0.187653,0.101478,0.424423,0.677655,0.523926,0.813562,0.832735,0.267776,0.500781,0.876628,0.160488,0.962676,0.529694,0.217029,0.065453,0.203604,0.197497,0.192862,0.199530,0.956786,0.593493,0.738792,0.294382,0.679830,0.786126,0.921321,0.927582,0.844134,0.259065,0.107694,0.066701,0.903625,0.144680,0.899686,0.375561,0.977402,0.411530,0.337534,0.648208,0.816542,0.327443,0.406411,0.665480,0.254335,0.469030,0.523707,0.805881,0.797667,0.746330,0.486679,0.827784,0.559038,0.278773,0.347166,0.053884,0.914271,0.772295,0.404771,0.355915,0.344221,0.494815,0.303236,1.153073,0.580132,0.694087,0.316221,0.949754,0.591745,0.711461,0.130951,0.392473,0.684088,0.368251,0.595982,0.790235,0.176153,0.322475,0.963195,0.709403,0.197546,0.862749,0.047262,0.738921,0.051181,0.176929,0.160503,0.611962,0.681186,0.692644,0.551596,0.535614,0.763289,0.538614,0.768029,0.384893,0.357781,0.768441,0.394168,0.178122,0.827359,0.762029,0.033240,0.239931,0.700079,0.469752,0.293798,0.344646,0.525101,0.811062,0.520567,0.068674,0.846750,0.622441,0.223059,0.662205,1.144467,1.006799,0.847119,0.778015,1.145110,0.800021,0.605434,1.105661,0.944114,0.272180,0.417541,0.069233,0.240228,0.451440,0.696306,0.318724,0.844838,0.739562,0.462462,0.359661,0.442091,0.584100,0.380657,0.627568,0.575383,0.575180,1.012085,0.454092,0.350775,0.440377,0.360980,0.469214,0.371682,0.524827,0.795912,0.541927,0.294907,0.603275,0.628876,0.681520,0.174211,0.815167,0.179391,0.077677,0.584844,0.005766,0.954009,0.537244,0.386504,0.017401,0.247851,0.913007,0.656780,0.375256,0.277907,0.813289,1.038337,0.712286,0.377579,0.470663,0.975127,0.187573,0.455112,0.571102,0.329825,0.295593,0.401576,0.119386,0.272612,0.492900,0.694098,0.353376,0.287148,0.666330,0.459271,0.582721,0.413202,0.925279,0.136931,0.736262,0.268167,0.456041,0.352423,0.346562,0.775571,0.913687,0.605968,0.490688,0.680055,0.570647,0.818321,0.259738,0.863499,0.337370,0.710914,0.372088,0.899176,0.366118,0.606715,0.470326,0.318164,0.805333,0.852068,0.063933,0.742920,0.363790,0.250222,0.871772,0.909252,0.363634,0.628684,0.276730,0.171482,0.738195,0.466266,0.389333,0.252484,0.674778,0.514450,0.508005,0.326640,0.700787,0.472451,0.481577,0.578374,0.115262,0.251863,0.801910,0.273772,0.300686,0.162613,0.368540,0.275614,0.962477,0.246445,0.258217,0.619298,0.650394,0.394961,0.210010,0.492130,0.126056,0.633953,-0.018426,0.449664,0.498577,0.194634,0.728164,0.437181,0.493397,0.671927,0.513242,0.511865,0.114026,0.627322,0.462180,0.310760,0.567791,0.764072,0.022643,-0.000344,0.545290,0.735296,0.231259,0.637785,0.498006,0.293662,-0.060148,0.055038,-0.092784,-0.222227,-0.005564,-0.480232,0.215572,0.008227,0.524060,0.456162,0.565619,0.793589,0.745922,0.622923,0.285084,0.048807,0.226638,0.741789,0.974514,0.487791,0.214911,0.800317,0.611690,0.321593,0.392623,0.704337,-0.142562,0.302296,-0.283831,0.016255,0.025381,-0.235091,0.445484,-0.071893,0.149627,-0.285354,0.312229,0.366141,-0.037306,0.696407,0.279903,0.281725,0.626758,0.857838,0.753495,0.949434,0.570814,0.894290,0.868861,0.615820,0.719446,0.706757,0.028974,0.428219,-0.349624,-0.147911,0.129814,0.184679,0.470231,0.722871,-0.066107,0.178259,0.248897,0.146225,0.113548,0.440419,0.491350,0.282668,0.543997,0.358938,0.624679,-0.001945,0.162153,1.102827,0.102473,0.411345,0.108180,0.412246,0.432009,0.245806,0.367606,0.023266,0.065666,0.432385,0.627760,0.623430,0.426898,0.568873,0.325595,0.932509,0.725836,0.296813,0.525705,0.345853,0.397944,0.621548,0.536570,0.178713,0.012750,0.477288,0.792226,0.693748,0.088429,0.512896,0.458446,0.964983,0.884424,0.710588,0.275791,0.206318,0.417961,0.538235,0.610146,0.097032,0.547088,0.296639,0.946587,0.837925,0.377479,0.245917,0.578623,0.664796,0.305267,0.364867,-0.126722,0.603398,0.531428,0.655578,1.154463,0.282776,1.003520,0.871147,0.628498,0.885667,0.562703,0.201219,1.007006,0.759367,0.581564,0.387001,0.645248,0.805942,0.648869,0.845722,0.547150,1.211265,0.874358,0.729815,0.152868,0.751765,0.524781,0.665313,0.648830,0.440726,0.297867,0.444890,0.648667,1.290127,0.968750,0.261764,0.215918,0.864954,0.418623,0.271082,1.064813,0.889721,0.658438,0.683103,0.666808,0.590748,0.845011,0.785215,1.048117,0.751054,0.626229,0.917168,0.726067,0.498620,0.531561,0.594673,0.719927,0.651762,0.896107,0.586287,0.572291,0.460460,0.238704,0.462263,0.180271,0.646258,0.825666,0.279364,0.933444,0.728412,1.138815,0.552670,0.541472,0.660921,0.803267,0.832144,0.651139,0.925168,0.722745,0.892142,0.192910,0.736473,0.409472,0.677697,0.412916,0.845518,0.924958,1.033411,1.013379,0.335471,0.232582,0.998420,0.427196,0.642887,0.686250,0.857398,0.889202,0.934896,0.652902,0.808024,0.984083,0.954801,0.492826,0.925908,0.745253,0.902124,0.383049,0.816540,0.653671,0.592964,0.592254,0.360200,0.409913,0.677395,0.881746,0.550123,0.503038,0.640854,0.787050,0.563409,0.347911,0.695393,0.947079,0.387440,0.344247,0.946694,0.703430,0.752903,0.456438,0.906360,0.813384,0.928328,0.662888,0.375272,0.709978,0.791486,0.476433,0.509762,0.463135,0.903958,0.461088,1.017492,0.508058,1.180617,0.971380,0.470634,0.336420,0.398656,0.657559,0.930781,0.900062,0.296074,0.850584,0.624755,0.996258,0.870659,0.616801,0.298461,0.513024,0.531696,0.959382,0.476932,0.360539,0.209699,0.820441,0.976053,0.594893,0.745420,0.499762,1.180166,0.458315,1.088983,0.903999,0.150401,0.582115,0.227362,0.375935,0.191402,0.785818,0.910268,0.404596,0.188181,0.872697,0.780602,0.608985,0.739189,0.660188,0.432588,0.261621,0.804154,-0.011280,0.205359,0.360216,0.411995,0.868272,0.663605,1.045791,0.815000,0.733387,0.990760,0.699506,0.731739,0.578795,0.071778,0.294748,0.014618,0.771462,0.990300,0.593354,0.333255,0.517423,0.166671,0.471354,0.167364,0.551795,0.477073,0.545949,0.547020,0.569088,0.588039,0.609434,0.211525,1.072372,0.903915,0.250462,0.384178,0.595319,0.175419,0.984591,0.472429,0.784515,0.612232,0.774712,0.612598,0.838197,0.269165,0.956569,0.788289,0.262740,0.097340,-0.013996,0.742231,0.739753,0.478781,0.163661,0.194536,0.464052,0.452135,0.414841,0.684651,0.720592,0.511927,0.848182,0.562055,0.895379,0.203291,0.257831,0.200961,0.442227,0.320383,0.216773,0.834742,0.783680,0.848304,0.373195,0.404603,0.903482,-0.013490,0.692423,0.190028,0.425097,0.145874,0.610420,0.244341,0.134678,0.839563,0.420614,0.776852,0.819719,0.497330,0.120078,0.420742,0.657662,0.799467,0.976021,0.967960,0.517448,0.087985,0.050722,0.576861,0.862514,0.592636,0.602716,0.579381,0.280878,0.781988,0.380873,0.875498,0.111109,0.955775,0.049839,0.405106,0.328078,0.342149,-0.021066,0.289758,0.186154,0.765141,0.483288,0.055649,0.886645,0.397336,0.568590,0.445235,0.706565,0.568560,0.585101,0.606220,0.195532,0.375225,0.776025,0.905797,0.564231,0.811985,0.798785,0.948220,0.770104,0.197044,0.629891,0.513738,0.842732,0.824232,0.728286,0.689885,0.617875,0.395911,0.877708,0.982412,0.531066,0.252980,0.600055,0.680776,0.640797,0.950682},
						{0.271310,0.510178,0.202643,0.875973,0.169866,0.149022,0.722098,0.691519,0.556810,0.622150,0.996460,0.999939,0.598176,0.513892,0.525192,0.045686,0.832636,0.892666,0.551103,0.256996,0.005341,0.933287,0.817499,0.099826,0.130833,0.854488,0.381603,0.762322,0.065249,0.326548,0.016175,0.075106,0.369823,0.006256,0.230109,0.574601,0.746437,0.354672,0.439386,0.114695,0.826903,0.394627,0.318797,0.932569,0.596120,0.564183,0.872683,0.584726,0.236333,0.321545,0.234053,0.626789,0.655416,0.657826,0.760430,0.499619,0.511490,0.175542,0.419874,0.470990,0.984374,0.887017,0.022119,0.073095,0.091975,0.387661,0.317763,0.934214,0.835698,0.571713,0.492276,0.383844,0.906587,0.514959,0.108556,0.854577,0.412330,0.746506,0.991679,0.227681,0.030124,0.465345,0.118381,0.355449,0.340251,0.943968,0.020416,0.942238,0.781133,0.790563,0.789276,0.770830,0.642252,0.087368,0.119505,0.666080,0.852199,0.290815,0.497189,0.944697,0.551037,0.561000,0.643391,0.955289,0.952578,0.885170,0.825065,0.557895,0.935158,0.691547,0.306635,0.990905,0.425123,0.940307,0.604819,0.492941,0.179225,0.779770,0.145271,0.461993,0.731010,0.496297,0.373256,0.518718,0.863879,0.583098,0.977052,0.698090,0.544414,0.225287,0.343637,0.406719,0.563522,-0.035670,0.715587,0.023241,0.905434,0.804400,0.208725,0.383607,0.447890,0.775018,0.875852,0.931027,0.597088,0.791331,0.473979,0.868981,0.789221,0.822438,1.080517,0.708716,0.312349,0.498921,0.759440,0.573524,0.637100,0.750417,0.762514,0.210652,0.218316,0.043442,0.860936,0.339626,0.154086,0.626812,0.197117,0.332094,0.760674,0.649028,0.952987,0.198882,0.765216,0.410765,0.727210,0.285464,0.449212,0.697192,0.702425,0.709389,0.589541,0.499220,0.669579,0.818327,0.359044,0.551626,0.440826,0.274449,0.678379,0.107605,0.874854,0.162924,0.860654,0.493552,0.054244,0.568904,0.414227,0.151127,0.394146,0.218977,0.524948,0.536410,0.359882,0.436603,0.628305,0.841622,0.360359,0.465746,0.761034,0.719084,0.277752,0.918100,0.694488,0.484960,0.481391,0.667014,0.661380,0.419234,-0.057947,0.445431,0.796580,0.366520,0.068241,0.340353,0.820853,0.111148,0.900760,0.647178,0.937821,0.485133,0.879517,0.238169,0.402232,0.668300,0.662629,0.500620,0.680613,0.541822,0.679756,0.522160,0.879364,0.606192,0.560865,0.976627,0.467402,0.857181,0.830046,0.047961,0.705346,0.434093,0.556858,0.765818,0.020862,0.736566,0.473327,0.362784,0.629486,0.830159,0.260575,1.035342,0.116958,0.273733,0.510350,0.090722,0.049198,0.021166,1.038090,0.826753,0.349027,0.951057,0.491479,0.646373,0.686364,0.686811,0.036619,0.004858,0.404550,0.347894,0.451701,0.847041,0.021348,0.412661,0.167424,0.810204,0.479779,0.472234,0.211112,0.327384,0.350865,0.023071,-0.169831,-0.076673,0.276040,0.464489,0.651268,0.417682,0.992724,0.303403,0.744849,0.875539,0.563008,0.819982,0.750534,0.543081,0.324578,0.679196,0.282818,0.481084,0.175993,0.471839,0.338259,0.471319,0.084303,0.440294,0.186301,0.106191,-0.240915,0.204205,0.332323,0.283535,0.371553,0.570072,0.781272,0.889718,0.416422,1.079019,0.984746,0.935016,0.855949,0.696999,0.311279,0.212571,0.293711,0.781847,0.192283,0.277769,0.074403,0.384626,0.330290,0.446737,0.075466,0.377156,0.284671,-0.176687,0.543597,-0.012439,0.398462,0.526781,0.444721,0.368624,0.931442,0.737643,0.510882,0.387582,0.434134,0.390248,0.426149,0.067844,0.251202,0.300421,0.860269,0.423080,0.214808,0.564841,0.910452,0.540201,0.601716,0.206459,0.607789,0.328411,0.539036,0.014252,0.402606,0.458291,0.445712,0.717245,0.264087,0.674372,0.396692,0.198603,0.599148,0.406856,0.557724,0.776532,-0.072842,0.523284,0.372677,0.660178,0.325600,0.492372,0.571215,0.407858,0.796642,0.170732,0.245741,0.544101,0.314772,0.531573,0.118849,0.431439,0.339180,0.031861,0.222330,0.584013,0.446235,0.871048,0.545904,0.400313,0.584927,0.966977,0.679258,0.485078,0.244907,0.620029,0.134373,0.486886,-0.010127,0.320854,0.165158,0.991808,0.998635,0.528116,0.859521,0.091709,0.542133,0.691306,0.433921,0.204276,0.407161,0.398730,0.257425,0.053083,0.859846,0.767588,0.591762,0.614260,0.567578,0.353065,0.296528,0.499669,0.578159,0.615367,0.806448,0.279405,0.439666,0.633847,0.013456,0.206389,0.545881,0.912653,0.841581,0.303151,0.845637,0.549162,0.129817,0.111378,-0.016376,0.161256,0.367981,0.639812,0.080496,0.797658,0.827199,0.064750,-0.142863,0.634556,0.799299,0.657294,0.535357,0.816296,0.184343,0.958688,0.373683,0.819429,0.816073,0.782115,0.205634,0.650483,0.827155,0.783231,0.882332,0.541487,0.361491,0.624133,0.204488,0.274226,-0.167918,0.239932,0.651053,0.226969,0.490605,0.902478,0.510218,0.870842,0.610121,0.881469,0.897241,0.825851,0.749164,0.616099,-0.009159,0.576792,0.425095,0.817358,0.273479,0.584848,0.623638,0.671101,1.165073,0.926086,0.482311,0.284710,0.471367,-0.008106,0.448934,-0.157723,-0.001324,0.239185,-0.120228,0.157558,0.597063,0.476119,0.594769,0.673130,0.856835,0.918687,0.741633,0.503021,0.660737,0.582220,0.366538,0.043002,0.156346,0.197421,0.166402,0.391642,0.749125,0.403682,0.932542,0.863463,0.549920,0.527608,0.049562,0.574496,0.060789,0.323179,0.712362,0.744598,0.934523,0.877005,0.913452,0.539779,0.526382,0.858885,0.255957,0.677083,0.007517,0.025759,0.179113,0.667087,0.214484,0.959888,0.893259,0.669517,0.893465,0.654431,0.982593,0.384759,0.272822,0.837173,0.130431,0.762529,0.095993,0.355378,0.635521,0.397305,0.456153,0.479161,0.574182,0.689259,0.284812,1.011508,1.011335,0.207771,0.810876,0.051664,0.794613,0.140134,0.833489,0.996854,0.086473,0.159541,0.501486,0.823752,0.724116,0.692264,0.414117,0.442402,0.490003,0.290119,0.321007,0.368089,0.544453,0.286921,0.870596,1.170187,0.595657,0.998209,0.596827,0.733916,0.503647,0.145459,0.406649,0.849605,0.653967,0.693258,0.192264,0.652985,0.543770,0.442943,0.791586,0.906152,1.011181,0.859355,0.317348,0.511212,0.554569,0.229576,0.250864,0.718835,0.120198,0.381050,0.509730,0.554618,0.529372,0.793271,0.801554,0.519022,0.432774,0.340358,0.792090,0.061724,0.772882,0.984557,0.697653,0.067385,0.287491,0.738719,1.140601,0.461044,0.697746,0.367406,0.411106,0.332221,0.448619,0.870580,0.423049,0.783695,0.350294,0.607169,0.392128,0.882876,0.300763,0.749788,0.764286,0.399142,0.309625,0.055796,0.539298,0.658543,0.503801,0.700430,0.553606,0.338572,0.403556,0.212019,0.213467,0.405861,0.649596,0.506219,1.016670,1.003364,0.874771,0.736987,0.968285,0.304888,0.917188,0.644167,0.999943,0.229795,0.125405,0.722865,0.844782,0.515653,0.223562,0.863003,0.181326,0.835028,0.192020,0.456618,0.433790,0.382305,0.818718,0.405612,0.294594,0.510573,0.344163,0.778742,0.451488,0.839614,1.046064,1.110829,0.754087,0.603324,0.856320,0.890063,0.056427,0.674525,0.812966,0.692436,0.228577,0.105596,0.029770,0.445098,0.861681,0.056773,0.461652,0.841609,0.726859,0.071413,0.309519,0.460734,0.866329,0.203073,0.661067,0.106535,0.340869,0.503602,0.184309,0.651469,0.106928,0.472376,0.527828,0.067601,0.840861,0.920186,0.496405,0.357001,0.818497,0.374575,0.100681,0.650495,0.634324,0.809595,0.318705,0.946074,0.322123,0.752922,0.282907,0.227424,0.286111,0.446514,0.416265,0.272586,0.550510,0.243662,0.879551,0.434594,0.046573,0.876360,0.387843,0.313509,0.220772,0.763759,0.936198,0.729011,0.745417,0.993363,0.434655,0.901994,0.615558,0.166326,0.829554,0.222816},
						{0.735466,0.712058,0.581195,0.986694,0.002289,0.946196,0.581378,0.458571,0.429273,0.358226,0.604297,0.598865,0.422133,0.012722,0.103148,0.601062,0.241646,0.111667,0.041597,0.385388,0.193152,0.033204,0.880581,0.262856,0.762322,0.566698,0.020325,0.234352,0.841609,0.849544,0.103824,0.705435,0.710835,0.052520,0.078292,0.875308,0.893906,0.319982,0.467019,0.730631,0.742215,0.120632,0.682917,0.495963,0.397433,0.224116,0.814090,0.095770,0.534154,0.749779,0.825287,0.843303,0.273598,0.550035,0.633839,0.357524,0.618091,0.970641,0.722800,0.899534,0.980931,0.666122,0.015880,0.551098,0.754415,0.263882,0.646362,0.445960,0.349592,-0.065342,0.462241,0.438749,0.404412,0.865567,0.299480,0.259110,0.710005,0.346884,0.307093,0.539374,0.989354,0.900908,0.217811,0.254616,0.298105,0.861629,0.121921,0.889948,0.230215,0.877740,0.526658,0.188809,0.842887,0.298173,0.593638,-0.135663,0.223591,0.422716,0.064635,0.462397,0.049310,0.075655,0.385700,0.161805,0.890014,0.970952,0.801386,0.160685,0.579330,0.928581,0.403318,0.857387,0.109317,0.270608,0.263283,0.671289,0.340433,0.989997,0.472082,0.558424,0.420150,0.518479,0.359995,0.062120,0.538089,0.636527,-0.199447,0.626920,0.036428,0.392028,0.082836,0.584111,0.652609,0.679925,0.902078,0.952763,0.673249,0.863648,0.963793,0.925562,0.021302,0.040437,0.863957,0.854602,0.679198,0.755684,0.553287,0.524425,0.174262,0.572297,0.892234,0.034293,0.437024,0.634649,0.309350,0.378645,0.588678,0.714167,0.741159,0.864167,0.147685,0.293371,0.399634,0.381103,0.246834,1.001775,0.377668,0.746089,0.526170,0.141636,0.171446,0.713763,0.871959,0.566633,0.586942,0.296839,0.740726,0.363544,0.210020,0.457365,0.266890,-0.117608,0.495735,0.037058,0.329040,0.249793,0.029409,0.606906,0.178729,1.068730,1.095159,1.108801,0.321286,0.264377,0.623051,0.119841,0.546057,0.324559,0.884403,0.817665,0.758569,0.706405,0.419191,0.166576,0.651073,0.774157,0.193907,0.224693,0.190767,0.036902,0.488172,0.145661,-0.194966,0.416881,0.042954,0.191627,0.470657,0.769145,0.643110,0.396596,0.759359,0.372985,0.048727,0.113978,0.061517,0.604642,0.552347,0.171472,0.827135,0.646559,0.235336,0.141682,0.596977,0.241115,0.625929,0.521861,0.569377,-0.035122,-0.242503,0.176562,-0.038566,-0.058111,0.646925,0.553291,0.427809,0.290710,0.339485,0.708045,0.846562,0.293171,0.637556,0.531150,0.554575,0.327763,0.311939,0.371858,0.547613,0.305716,0.485133,0.190199,0.386149,0.156227,0.696780,0.131205,0.235792,0.077062,0.154466,0.150561,0.437267,0.814191,0.535394,0.662571,0.109725,0.898422,0.776740,0.330563,-0.022421,-0.033758,0.684104,0.607105,0.858962,0.969454,0.301885,0.859817,0.873360,0.133180,0.656075,0.027036,0.636891,0.299039,0.258710,0.487690,1.041949,-0.084508,0.232368,-0.040982,0.277875,0.549886,0.520504,0.335007,0.841108,0.057437,0.440150,0.155804,0.482685,0.652576,0.870411,0.819128,0.513701,0.460293,0.586067,0.394077,0.237797,0.306419,0.788580,0.741448,0.606514,0.570917,1.201802,0.514507,0.773189,0.112218,-0.182167,0.644464,0.763818,0.716022,0.279955,0.532650,0.637557,0.584213,0.031145,0.402119,0.164252,0.289828,0.917657,0.376759,0.211279,0.213452,0.946459,0.050601,0.391386,0.433408,0.507125,0.387996,1.060886,0.665864,0.995709,1.139452,1.001976,0.126899,0.105073,0.621077,0.651476,0.853843,0.692664,0.497623,0.347552,0.429092,0.532739,0.703136,0.633627,0.177273,0.011106,0.306734,0.435987,0.591622,0.349377,0.269629,0.718287,0.568958,0.620136,0.762760,1.030319,0.972664,0.740454,1.166086,0.986399,0.154089,0.421737,0.648969,0.571927,0.467904,0.839816,0.615588,0.509305,0.653558,0.401969,0.187671,0.891856,0.043091,0.923219,0.596947,0.885055,0.012063,0.506369,0.484276,0.703846,0.466419,1.107238,0.469358,1.066203,0.536860,1.066402,0.932959,0.677746,0.163516,0.387544,0.417874,0.995306,1.184025,0.635346,1.001094,0.695874,1.020521,0.440266,0.610530,0.718828,0.347287,0.024736,0.403582,0.766863,0.429153,0.321764,0.179969,0.431275,0.973345,0.373268,0.792758,0.978724,0.635009,0.598895,0.935441,0.709175,0.484660,0.798145,0.815889,0.878880,0.914121,0.751427,0.882259,0.602569,0.594821,0.225181,0.139037,0.726572,0.575531,0.236548,0.713663,0.569922,0.401103,0.527165,0.291899,0.821025,0.703034,0.669909,0.948754,0.755563,1.176740,0.207808,0.903846,0.705561,0.391423,1.089620,0.536811,1.327367,0.459988,0.512201,0.414639,0.162729,0.311969,0.307113,0.321279,0.835363,0.818005,0.973984,0.569780,0.396222,0.468489,0.485490,0.461408,0.083203,0.385685,0.771974,0.660116,0.480722,0.508805,0.219973,0.673788,1.117903,0.479083,1.280289,0.937222,0.647306,0.872084,0.825456,0.765898,0.770792,0.816746,0.486438,0.157986,0.266859,0.367224,0.407281,0.175190,0.451919,0.050752,0.318168,0.731166,0.495401,0.040808,0.195615,0.621987,0.426139,0.841859,-0.002443,0.739711,0.321377,0.788874,0.773033,1.243185,0.757012,0.553628,0.531924,0.015504,0.726144,-0.016679,0.397251,-0.097541,0.643013,0.540222,0.733585,0.949491,0.795099,0.591605,0.356582,0.579641,0.596374,0.394055,0.054936,-0.036859,0.300099,0.381209,0.446811,0.059575,0.047827,0.672791,0.226761,0.211169,0.460756,-0.183324,-0.120166,0.365667,0.069079,0.721757,0.519549,0.301260,0.436902,0.407947,0.908353,0.938181,0.916684,0.168014,0.205346,0.132980,0.245824,0.773314,0.587964,-0.149739,0.001025,0.101936,0.432207,0.151368,0.501403,0.374896,0.458349,0.467106,0.258727,0.820549,0.602230,0.221452,0.308201,0.612388,0.639235,0.354628,0.793534,0.646713,0.849206,0.614334,0.277413,0.275704,0.240507,0.171090,0.062485,0.559951,-0.121929,0.729322,0.384609,-0.122929,0.010111,0.041468,0.702872,0.158876,0.631294,0.391800,0.503324,0.428746,0.296224,0.830797,0.541907,0.759134,0.231194,0.237968,0.688235,0.427077,0.006266,0.474136,0.365612,0.021851,0.928569,0.451245,0.838011,0.559307,0.744598,0.022312,0.129572,0.093656,0.833857,0.093030,0.316829,0.083481,0.357616,0.530195,0.553147,0.598293,0.205067,0.525762,0.504596,0.391625,0.695971,0.715926,0.221395,0.168066,0.692158,0.433332,0.386792,0.431989,0.768385,0.265550,0.885378,0.890444,0.275657,0.184805,0.762230,0.142806,0.146544,0.261433,0.704244,0.503896,0.718178,0.259358,0.269025,0.773351,1.143777,0.472605,0.554225,0.258895,0.151367,0.501795,0.758450,0.208349,0.985913,0.482559,0.173223,0.096561,0.224511,0.991268,0.168683,0.716435,0.290313,0.443658,0.775933,0.571992,0.888623,0.544256,0.387803,0.242084,0.313314,0.618193,0.446175,0.557039,0.073306,1.043276,0.741134,0.747169,0.658665,0.722607,0.895279,0.479480,0.567961,0.570971,0.408063,0.874111,0.498637,0.193230,0.990338,0.641235,0.751340,0.462143,0.535845,-0.037123,0.672868,0.357616,0.509642,0.017762,0.012744,0.550199,-0.012356,0.320848,0.411240,0.203437,0.373192,0.411496,0.247770,0.039160,0.043320,0.955292,0.316758,0.187384,0.757347,0.869411,0.467513,0.170530,0.167277,0.553026,0.606910,0.942567,0.666162,0.054331,0.701766,-0.036951,-0.021108,0.529974,0.517470,0.114310,0.750913,0.024114,0.684262,0.721496,0.153921,0.844908,0.736665,0.621099,0.468831,0.032105,0.017243,0.607044,0.004212,0.406201,0.639882,0.459456,0.420759,0.830832,0.555225,0.496260,0.831489,0.199358,0.807030,0.842311,0.577486,0.948034,0.244677,0.982766,0.602485,0.580426,0.146633,0.616570,0.096284,0.174462,0.548996,0.946435,0.701132,0.587542,0.025819,0.273324},
						{0.212592,0.475509,0.248543,0.983917,0.895230,0.466811,0.362438,0.763787,0.036775,0.061586,0.130955,0.965636,0.410901,0.494058,0.685781,0.191015,0.723289,0.616993,0.061525,0.059297,0.446303,0.520768,0.961486,0.035310,0.961394,0.233955,0.283059,0.977996,0.538102,0.909116,0.184515,0.065462,0.183203,0.401379,0.973570,0.254094,0.889689,0.646461,0.599439,0.925761,0.316514,0.742161,0.030747,0.882763,0.435618,0.145896,0.298547,0.343522,0.164522,0.879246,0.484748,0.097962,0.542497,0.173711,0.444044,0.466628,0.421003,0.422742,0.529496,0.253679,0.306281,0.896053,0.114730,0.304547,0.265929,0.074221,0.173150,0.834389,0.491807,0.046357,0.761657,0.305807,0.634927,0.878623,0.574676,0.707516,0.196845,0.289368,0.401355,0.170880,0.196100,0.609771,0.873470,0.158483,0.825159,0.511307,0.140171,0.454005,0.619472,0.167565,0.763513,0.520002,0.115317,0.464691,0.649293,0.633006,0.831838,0.239996,0.637942,0.574394,0.505448,0.643085,0.516455,0.812473,0.353000,0.437135,0.635004,0.338576,0.256488,0.465293,0.606903,0.065645,0.864223,0.883907,0.238370,0.453544,0.891514,0.922345,-0.046601,0.256581,0.647106,-0.091048,0.267890,0.347253,0.653979,0.137352,0.493082,0.809634,0.726551,0.427326,0.747321,0.143295,0.385235,0.306855,0.989529,1.024035,0.335893,0.429471,0.186662,0.412913,0.833064,0.245430,0.439717,0.697491,0.348304,0.228961,0.856515,0.852516,-0.014016,0.931843,0.803345,0.599228,0.282243,0.412288,0.740110,0.138081,0.374356,1.065113,0.310437,0.644404,0.665888,0.329392,0.952768,1.005530,0.414369,1.043880,0.690441,0.412401,0.264046,0.002166,0.020088,0.403665,0.744043,0.125721,0.210141,0.058223,0.237593,0.188229,0.975145,0.653450,0.479728,0.269613,0.836698,0.574789,0.547794,0.345327,0.591369,0.461184,0.362587,0.833601,1.051955,0.549440,1.037001,0.707392,0.212348,0.786433,0.727702,0.730876,0.851061,0.572929,0.687133,0.372244,-0.009197,0.629999,0.883928,0.242052,0.725520,0.519716,0.629164,0.560753,0.321726,0.435206,0.631863,0.676879,0.282724,0.426946,0.789228,0.568928,0.701485,0.552306,1.221544,1.264229,0.103734,0.637371,0.596245,0.778893,0.054549,-0.024950,0.116122,0.414924,0.488755,0.333769,0.804326,0.949115,0.778969,0.951746,0.557142,0.185650,0.366553,0.016291,0.363860,0.432567,0.917307,0.783991,0.501164,0.716347,0.681152,1.051546,1.430148,0.885591,0.265857,0.761593,0.490338,0.778505,0.729659,0.761406,0.718963,0.534793,0.384967,0.340686,0.134949,0.733483,0.293005,0.984678,0.764722,0.363906,0.561625,0.014185,0.475852,-0.125550,0.288089,0.346733,0.898320,0.616220,0.806258,1.102332,1.139455,0.582332,0.636845,0.978602,0.620646,0.504966,0.823925,0.808884,0.388312,0.060322,0.211687,0.733377,0.996927,0.443501,0.917663,0.504830,0.828668,0.788054,0.738191,0.165566,0.186496,0.519003,0.599890,0.265411,0.434518,0.620757,0.857830,0.891653,0.777743,0.864351,0.927506,0.621121,0.756603,0.848132,0.570324,0.177002,0.895069,0.425145,0.511873,0.778560,0.715958,0.810628,0.732172,0.991741,0.696659,0.800863,0.693188,0.508531,0.293409,0.285968,0.176663,0.259516,0.107168,-0.390546,0.603103,0.000766,0.732991,1.105274,0.243627,0.725851,0.027985,0.285083,0.455197,0.178906,0.532063,0.686355,0.975086,0.537986,0.354004,0.575203,0.682819,0.610027,0.761299,1.191761,0.147003,0.472344,0.226529,0.099798,0.148517,0.458849,0.098019,0.470732,-0.369605,0.124852,0.406463,0.066709,0.469624,0.015639,0.282601,0.670473,0.200803,0.236769,0.628392,0.536195,0.713536,0.816877,0.863664,0.332466,0.938124,0.956347,0.826082,0.458208,0.199491,0.034288,0.245857,0.044188,0.101456,0.510866,0.613455,-0.045077,-0.119031,-0.208311,0.472468,0.933574,0.589885,0.336071,0.224789,0.713649,0.730555,0.931218,0.915157,0.736411,0.586968,0.130338,0.644880,1.081584,0.682641,0.741461,0.386399,0.760849,0.411941,0.384004,-0.013889,0.462141,0.106020,0.412360,0.538717,0.181513,0.605286,0.319108,0.167178,0.543517,0.380094,0.560342,0.033778,0.966269,0.575819,0.610732,0.029564,0.012563,0.571775,0.569793,0.699887,0.249790,0.504074,0.541874,0.889277,0.634713,0.461179,0.009138,0.192672,0.590350,0.721814,0.063748,0.244069,0.248524,0.197246,0.711682,0.709941,0.457410,0.711075,0.592814,0.722649,0.670382,0.700439,0.812667,0.705607,0.211090,0.499153,0.065142,0.598683,0.384398,0.290321,0.412806,0.247773,0.401728,0.298029,0.579602,0.071111,0.211972,0.535146,0.371458,0.693881,0.672356,0.208433,0.235588,0.397099,0.348250,0.480964,0.589366,0.898068,0.314663,0.218550,0.973427,0.761242,0.685087,0.993985,0.126431,-0.115275,0.257735,0.243869,0.640945,0.724760,-0.046443,0.471375,0.603030,0.202992,0.840452,0.081826,0.744600,0.686559,0.817658,0.802408,0.829980,0.728066,0.096834,0.529714,0.930103,0.186928,0.237806,0.956457,0.642508,1.056305,0.734384,0.987604,0.731010,0.525826,0.431841,0.106519,0.096839,0.168415,0.419968,0.547672,0.629272,0.470528,0.775160,0.658278,0.473553,0.555395,0.485511,0.314824,0.820064,0.979510,-0.030520,0.669507,0.071974,0.031373,0.125091,0.323988,0.684515,0.113205,0.449170,0.570928,0.963469,0.497997,0.887627,0.808730,0.508975,0.632466,0.867525,0.533680,0.480293,0.454001,0.220310,0.427921,0.442603,0.477186,0.340918,0.935473,0.595508,0.820024,0.500639,0.180548,0.900051,0.068117,0.741497,0.475261,0.627016,0.558159,0.646129,1.142510,0.699626,0.516079,0.600695,0.587887,0.927627,0.312989,0.730334,0.754717,0.223269,0.746435,0.524048,0.874776,0.564950,0.412372,0.764698,0.314615,0.396265,0.189183,0.650910,0.829710,0.601742,0.816248,0.744319,0.312099,0.769103,0.379327,0.933572,0.364635,0.497509,0.864421,0.448025,0.463756,0.712725,0.781052,0.292218,0.368121,0.892465,0.066253,0.775235,0.201819,0.464870,0.611778,1.129991,0.734648,0.206698,0.340629,0.962732,0.775939,0.736930,0.889798,0.919643,0.100359,0.014843,0.481474,0.856026,0.648714,0.345382,0.527441,0.441687,0.510260,0.312854,0.723042,0.949810,0.186045,0.737167,0.644912,0.725092,0.600665,0.869336,0.794258,0.512535,0.328984,0.325137,0.862050,0.066220,0.464441,0.537827,0.454695,0.687094,0.317685,0.116937,0.185814,0.687403,0.230609,0.832105,0.506823,0.375800,0.567330,1.023228,0.315473,0.671673,0.786620,0.434956,0.330415,0.821058,0.489821,0.236174,0.496976,0.819247,0.947282,0.328686,0.768397,0.377594,0.838254,0.060549,0.148686,0.215125,0.713937,0.553502,0.262354,0.300768,0.516995,0.094321,0.876804,0.584257,0.797391,0.624514,0.653349,1.026016,0.526778,0.981779,0.374973,0.915471,0.827518,0.064912,0.508406,0.939125,0.624760,0.505767,0.681843,0.366976,0.951383,0.276925,0.182653,0.991882,0.965056,0.407245,0.838082,0.319967,0.777106,0.734181,0.922644,0.378525,0.377707,1.072221,0.449480,0.177357,0.530388,0.182598,0.972572,0.414127,0.871915,0.415837,0.631930,0.239663,0.399565,0.548529,0.927766,0.222751,0.376045,0.698935,0.842372,0.194372,0.935514,0.388412,0.295884,0.376992,0.368490,0.146047,0.485798,0.845778,0.690881,0.743956,0.515692,0.314962,0.088887,0.783328,0.514735,0.750063,0.548199,0.380839,0.024405,0.704028,0.540245,0.334533,0.668553,0.200690,0.411145,0.630055,0.109622,0.701071,0.019166,0.738426,0.207861,0.473936,0.483058,0.037279,0.113054,0.717833,0.762740,0.494994,0.908526,0.748850,0.703604,0.595429,0.393374,0.738245,0.487549,0.269292,0.104142,0.926313,0.656315,0.811040,0.931974,0.630634,0.552507,0.637135},
						{0.089785,0.518937,0.934568,0.453261,0.476852,0.537461,0.849696,0.266243,0.950621,0.614490,0.414747,0.309641,0.225018,0.731013,0.848509,0.968687,0.516037,0.591479,0.389996,0.888150,0.307718,0.021119,0.321421,0.528336,0.796503,0.188025,0.337138,0.683767,0.466628,0.536180,0.914304,0.995758,0.675652,0.647917,0.851308,1.015365,0.696723,0.859762,0.588768,0.701605,0.927826,0.777576,0.508615,0.249250,0.162996,0.800613,0.966006,0.766485,0.196372,0.509589,0.907170,0.840938,0.597949,0.991302,0.091708,0.565691,0.467940,0.338908,0.841915,0.937985,0.478228,0.951774,0.044967,0.487155,0.338997,0.262964,0.674686,0.545641,0.524273,0.830318,0.833688,0.430238,0.341886,0.941554,0.184402,0.928769,0.811916,0.405120,0.854257,0.517938,0.322397,0.977734,0.703787,0.781365,0.280465,0.677084,0.037341,0.110930,0.521822,0.655673,1.000867,0.077662,1.067355,0.330887,0.317834,0.873465,0.741955,0.879396,1.040765,0.230384,1.004892,1.038416,0.478172,1.119984,1.092118,0.975126,0.664664,0.875638,0.412959,0.714264,0.713235,0.589373,0.022401,0.013794,0.157483,0.902126,0.051509,0.253320,0.916122,0.285179,0.204619,0.219062,0.276470,0.864656,0.153520,0.298417,0.475790,0.791515,0.183070,0.676035,0.956060,0.337724,0.615017,0.351825,1.035530,1.057501,0.317373,0.592749,0.360492,0.424292,0.329875,0.141606,0.223496,0.556135,0.914316,0.943726,0.410244,0.113134,0.758868,0.857409,0.704151,-0.082613,0.497755,0.665049,0.467065,0.145239,0.531146,0.826254,0.442363,0.430628,1.124126,0.700011,0.975175,1.047174,0.966005,0.710102,0.901350,0.468826,0.491867,0.151891,0.584637,0.645246,-0.002043,0.716013,0.195370,0.172469,0.187237,0.533489,0.136469,0.486288,0.560362,0.104513,0.621439,0.136005,0.506709,0.711882,0.183937,0.260312,0.560460,0.790873,0.082076,0.447217,0.107717,0.292892,0.163601,0.113088,0.978114,0.226770,0.420339,0.954534,0.491256,0.899488,0.051523,0.142048,0.701476,0.348761,0.523091,0.015230,0.365435,0.056296,0.629999,0.080892,0.072769,0.354179,0.164109,0.352320,0.233839,0.178160,0.706330,0.712958,0.497702,0.042309,0.241059,0.481694,0.702974,0.222291,0.866823,0.493617,0.768651,-0.065533,0.554552,-0.024453,0.587201,0.596420,0.419575,0.143056,0.168527,0.486825,0.244272,0.328494,0.605906,-0.257829,0.299949,0.275831,0.466304,0.239462,0.576486,0.353355,0.492148,0.697683,0.135374,0.786695,0.869744,0.510550,0.359771,0.764293,0.235489,0.109224,0.741424,0.699886,0.527922,0.114995,0.065979,0.571279,0.367476,0.040890,0.315169,0.235647,0.048712,-0.001523,0.017867,0.041169,0.127686,0.288604,-0.042453,0.371654,0.400020,0.758165,0.113787,0.703140,0.738425,0.051284,0.298845,0.215730,0.048343,0.875494,0.179348,0.096877,0.742290,0.405589,0.904227,0.367045,0.054873,0.271128,0.426288,-0.124493,0.192012,-0.117915,0.252047,0.486156,0.152565,0.153883,0.387239,0.404797,0.176513,0.797227,0.775454,0.450879,0.161259,0.160249,0.631739,0.903651,0.029563,0.521690,0.469636,0.232910,0.777998,0.373687,0.273894,0.342081,0.828797,0.595233,0.035332,0.435276,-0.126085,0.517791,0.598804,0.006975,0.741893,0.732336,0.722674,0.613047,0.349858,0.540026,0.464597,0.912500,0.229743,0.943406,0.384649,0.133627,0.472563,0.818068,0.142730,0.617924,0.660036,0.507881,0.695696,1.049167,0.427466,0.898182,0.080574,0.318915,0.386586,0.449165,0.069412,0.172946,0.685204,0.284621,0.777919,0.912432,0.710693,0.840887,0.158146,0.349053,0.876400,0.866371,0.876046,0.328651,0.522744,0.806596,0.726777,0.333694,0.915754,0.831823,0.555250,0.644258,0.534705,0.250164,0.198373,0.230452,0.493823,0.265524,0.207825,0.822136,0.287460,0.907111,0.799314,0.874836,1.124017,0.118342,0.388754,0.086742,0.775487,0.427235,0.551005,0.849873,0.520759,0.540058,0.651300,0.789655,0.598125,0.568004,0.899474,0.727457,0.659867,0.428897,0.855584,0.657317,0.499119,0.641675,0.476121,0.209984,0.764718,0.356352,0.532080,0.935584,0.473340,0.971835,0.286702,0.800156,0.386653,0.401165,0.119552,0.924884,0.062500,0.670216,0.245849,0.457072,0.464920,0.408983,0.803217,0.906775,0.902457,0.438786,0.301917,1.019136,0.046818,0.494917,0.757798,0.352034,0.553809,0.641380,0.807973,0.230128,0.671392,0.456907,0.298718,0.587901,0.484145,0.626757,0.081889,0.528223,0.431275,0.579066,0.926049,1.081982,1.174164,0.852980,0.653117,0.909665,0.375794,1.137636,0.606455,0.481741,0.628666,0.095216,0.587390,0.197816,0.603745,0.530432,0.631003,0.477741,0.912181,0.250492,0.787325,0.692825,0.016877,0.801748,0.211381,0.236587,0.668008,0.126665,0.111407,0.315774,0.357901,0.839305,0.681368,1.275420,1.031424,0.269929,0.663604,0.730793,0.272622,0.235177,0.841945,0.802014,0.175706,0.863403,0.779887,0.630828,0.591632,0.014871,-0.023127,0.923729,0.641894,0.257300,0.108468,-0.025204,0.400190,-0.001395,0.559973,0.823784,0.516477,1.088890,0.828440,0.562287,0.674012,0.622452,0.722836,0.798752,0.346375,1.045530,0.226407,0.815186,0.398503,0.708117,0.462512,0.412484,0.418644,0.697691,0.440461,0.582807,0.775536,0.179360,0.407464,0.741116,0.343744,0.332598,-0.001992,0.699081,0.294999,0.924104,0.929235,0.939559,1.010127,1.049595,0.310180,0.815750,0.720170,0.460634,0.766767,0.741286,0.471471,0.682328,0.289075,0.304828,0.110490,0.694735,0.664967,0.153221,0.505844,0.246596,0.748647,0.150517,0.796422,0.529453,0.355099,0.381501,0.213926,0.895868,0.402061,0.877608,0.600378,0.698021,1.035343,0.630247,0.679931,0.499536,0.752137,0.123373,0.372260,0.825166,0.597388,0.442275,0.177533,0.685001,0.869917,0.358165,0.210272,0.153043,0.072652,0.787484,0.183014,0.350946,0.516138,0.432570,0.370637,0.559017,0.427108,1.149457,0.866285,1.370413,0.629448,0.817610,0.783711,0.724891,0.996418,0.698283,0.166351,0.399950,0.811406,0.967963,0.043453,0.201416,0.470714,0.910855,0.221900,0.982147,0.376054,0.391479,0.448153,0.670748,0.196282,0.064384,0.308444,0.026821,0.352176,0.290547,0.350271,0.378670,0.954066,0.169096,0.216755,0.516594,0.387093,0.161132,0.330145,0.804797,-0.036647,0.766815,0.169823,0.610667,0.564360,0.616962,0.388470,0.907193,0.489405,0.910370,0.858900,0.523892,0.414273,0.068853,0.654692,0.114405,0.417652,0.273298,-0.019376,0.730864,0.095135,0.651733,0.497684,0.261152,0.028538,0.535547,0.385584,0.372707,0.454733,0.594062,0.150552,0.360910,0.797663,0.065615,0.867702,0.179205,0.987603,0.144897,0.981955,0.816609,0.756299,0.201820,-0.039672,0.830953,0.630938,0.063657,-0.001269,0.307937,0.094256,0.479624,0.576081,0.667337,0.523424,0.927580,-0.013523,0.476865,0.342784,0.186638,0.830333,0.805431,0.828577,0.630696,0.002533,0.876186,0.816614,0.224951,0.068914,0.541157,0.903249,0.392140,0.582770,0.855197,0.737019,0.820608,0.168534,0.156929,0.626445,0.490389,0.165589,0.647259,0.797967,0.159621,0.704756,0.870413,0.848160,0.597190,0.556771,0.954415,0.942509,0.995788,0.620594,0.380261,0.735649,0.656389,0.761847,0.847765,0.837752,0.210002,0.236104,0.588089,0.925606,0.886571,0.085345,-0.002669,0.216392,0.821662,0.243827,0.576759,0.301484,0.961875,0.052554,0.439237,0.258719,0.572482,0.744102,0.147588,0.922239,0.923795,0.600574,0.052644,0.168432,0.146764,0.509690,0.202366,0.254005,0.787291,0.388112,0.680492,0.924069,0.121073,0.381018,0.645711,0.425476,0.735684,0.229641,0.847118,0.086727,0.426995,0.723918,0.900416,0.612200,0.923628,0.636006,0.103122,0.761589,0.489151},
						{0.074221,0.556261,0.920438,0.492538,0.996918,0.678640,0.978057,0.758324,0.744774,0.215094,0.780206,0.861263,0.675456,0.402060,0.190466,0.992279,0.370708,0.529862,0.731040,0.905667,0.105411,0.660970,0.052522,0.634297,0.326182,0.319407,0.491836,0.941679,0.921049,0.806574,0.748833,0.415937,0.561205,0.300393,0.213732,0.613366,0.896414,0.394110,0.975976,0.155503,0.883627,0.229795,0.046092,0.193839,0.044322,0.890659,0.822676,0.127496,0.884477,0.182698,0.330286,0.012103,0.338633,0.506424,0.271645,0.538743,0.443129,0.930906,0.411145,0.000582,0.058951,0.151157,0.330218,0.153229,0.916501,0.475895,0.080367,0.367859,0.760886,0.511850,0.774667,0.014124,0.905130,0.690020,0.650809,0.429285,0.905781,0.288178,0.574637,0.366296,0.699175,0.098880,0.641713,0.476394,0.016083,0.466384,0.805475,0.524438,0.239922,0.731183,0.077381,0.867119,0.574997,0.815521,0.344035,0.075578,0.826655,0.159570,0.430467,0.188948,0.506708,0.510877,0.183171,0.211164,0.511116,0.782788,0.330317,0.648663,0.283713,0.773430,0.159734,0.819147,0.733024,0.773705,0.429151,0.256985,0.513113,0.662256,0.525160,0.192419,0.080292,0.727926,0.446780,0.502425,0.293636,-0.028411,-0.035329,0.464527,0.311069,0.178231,0.829107,0.539051,-0.028585,0.179267,0.163840,0.114974,0.790763,0.454806,0.456018,0.952477,0.106662,0.806269,0.446948,0.128040,0.313865,0.497485,0.117913,0.340132,0.129411,0.502063,0.344015,0.398010,0.363938,0.408027,0.034947,-0.006225,0.191533,0.373873,-0.167053,0.439125,0.165346,0.426221,0.708169,0.035616,0.941510,0.571106,0.835684,0.647835,0.250069,0.443312,0.073741,0.496855,0.231481,0.169269,0.520150,0.535601,0.299188,0.377027,0.747097,0.587553,0.659350,0.229775,0.438200,0.492201,0.420584,0.549095,0.120077,0.217394,0.617986,0.400027,0.835991,0.600573,0.363855,0.314335,0.876504,0.144584,0.004365,0.982004,0.454533,0.280760,0.833284,0.320199,0.926062,0.502716,0.437539,0.939239,0.669155,0.820606,0.771928,0.350772,0.266550,0.632385,0.433315,0.344879,0.957825,0.517254,0.219679,0.874401,0.136507,0.707375,0.658124,0.516050,0.251361,0.595218,0.982012,0.890280,0.823484,0.439401,0.776040,0.144851,0.467938,0.926880,0.816340,0.455354,0.504106,1.312826,0.643247,0.419667,0.921339,0.924512,1.010224,0.696843,0.823358,0.841064,0.937119,0.248790,0.460508,0.854017,0.362562,0.164249,0.444350,0.708891,0.996692,0.361738,0.109184,1.013937,0.962259,0.383409,0.789178,0.330426,0.534205,0.732451,0.477546,0.206144,0.916084,0.526896,0.422571,0.715573,0.679259,0.974686,0.922572,0.598403,0.797781,0.664399,0.262350,0.544946,0.770655,0.234926,0.674450,0.972166,0.843597,0.880507,0.737869,0.602981,0.326219,0.796422,0.453265,0.646098,0.140236,0.211600,0.580852,0.610055,0.584330,0.915370,1.042692,1.191444,0.737969,1.154482,0.654184,0.938289,1.009926,1.098150,0.822987,0.107938,0.078440,0.901837,0.615821,0.176213,0.906219,0.288804,0.803366,0.750466,0.448979,0.715543,0.834418,0.490520,0.642691,0.438091,0.565304,0.416773,0.186587,0.100306,0.861908,0.495832,1.185787,0.763762,0.613524,1.003067,0.412934,0.734628,0.660870,0.481932,0.627219,0.694564,0.288759,0.835049,0.548094,0.438744,0.337345,0.778911,0.999870,0.385613,0.528666,0.382620,1.027208,0.831084,0.415685,0.555147,0.138211,0.360859,0.597982,0.469692,0.684777,0.963107,0.973210,0.348000,0.522277,0.530011,0.899054,0.106018,0.152454,0.781805,0.709260,0.024847,0.772298,0.109464,1.015791,0.822200,0.297217,0.805818,0.107874,0.304155,0.196993,0.473123,-0.016872,0.172975,-0.304335,-0.540145,-0.283862,0.559092,0.302160,0.802606,0.898299,0.760896,0.624375,0.804140,0.256244,0.132196,0.421187,0.813339,0.534825,0.227804,0.102844,0.281404,0.908984,0.644729,0.434942,0.836842,0.792599,0.629202,0.360074,0.164561,0.526445,-0.299676,-0.244689,0.246692,0.263102,0.553585,0.624238,0.575862,0.716581,1.035150,0.945269,0.880897,0.524508,0.524836,0.243693,0.610355,0.536634,0.948418,0.164761,0.209080,0.366918,0.448825,0.866860,0.054925,0.371496,0.471889,0.342451,0.524821,0.034109,-0.154124,0.036389,0.281453,0.327926,0.511437,0.209830,0.704109,0.545020,0.839994,0.535001,0.640166,0.398988,0.651824,0.550892,0.135048,0.960320,0.402568,0.932598,0.583451,0.001283,0.848365,0.803309,0.351618,0.841721,0.360035,0.533270,-0.102158,0.167856,0.236941,0.221030,0.415612,0.769430,0.446569,0.851856,1.113845,1.208320,0.397526,0.682973,0.756748,0.509883,0.598295,0.187584,-0.008379,0.603872,0.193404,0.111087,0.243099,0.887210,0.236490,0.533707,0.256906,0.283098,0.438569,-0.010881,0.142728,0.684673,0.384029,0.128585,0.699948,0.388042,0.840642,0.493133,0.475509,0.280179,0.473469,0.924140,0.652177,0.786352,0.694154,0.008848,0.810131,0.792868,0.270865,0.622565,0.611316,0.693737,0.293221,0.451809,0.666867,0.010508,0.386099,0.774894,-0.102986,0.101644,0.010158,0.640477,0.679385,0.936321,0.070901,0.581106,0.258786,0.526600,0.691973,0.065960,0.129131,0.250664,0.272457,0.214925,0.832315,0.946119,0.038016,0.658895,0.394890,0.932058,0.043595,0.192556,0.259953,0.694542,0.016412,0.113240,0.504688,0.167777,0.626764,0.488025,0.675667,0.516118,0.765616,0.069595,0.481327,0.149051,0.575304,0.327788,0.620984,0.416484,0.060845,0.809977,0.339152,0.037842,0.944063,0.288888,0.655362,0.102015,0.344863,0.474887,0.327949,0.638607,0.618172,-0.159662,0.201635,0.487915,0.558142,0.361949,0.361508,0.609717,0.497331,0.240714,0.272306,-0.198286,0.047125,0.019137,-0.079368,0.758815,0.423790,0.154480,0.659155,0.572807,0.404216,0.058349,0.874014,0.933632,0.109708,0.002291,0.299272,0.757524,-0.123284,0.379767,0.203445,-0.128258,0.366312,0.574741,0.298442,0.503813,0.176102,0.208563,-0.069535,0.517239,0.179610,0.189912,0.208156,0.240143,0.236508,0.348533,0.775994,0.323765,0.397870,0.580917,0.051850,0.397580,0.041639,0.386244,0.585180,0.310329,0.484954,0.816301,-0.055243,0.558155,0.445580,0.481442,0.508080,0.424566,-0.010329,0.741560,0.711246,0.449431,0.240423,0.287393,0.756759,0.616910,0.206473,0.404848,0.352351,0.609045,0.582049,0.571154,0.489853,0.746696,0.345172,0.475424,0.619219,0.133363,0.813638,0.257987,0.395285,0.461567,0.202892,0.664411,0.317111,0.377901,0.740368,0.542277,0.497141,0.112639,-0.035949,0.054062,0.311310,0.757759,0.755204,0.205483,0.867657,0.245965,0.265786,0.294870,0.312021,0.348458,0.238501,0.274023,1.002684,1.044827,0.958104,0.970427,0.541637,1.108374,0.386057,0.373089,0.913400,0.434455,0.269270,1.025160,0.205721,0.484782,0.753336,0.234326,-0.050134,0.480559,0.642506,-0.015659,0.935759,0.958797,0.090457,0.903836,0.336131,0.428524,0.467599,0.568605,0.314588,0.216765,0.512536,0.720197,0.988160,0.708386,1.102428,0.445765,0.949258,0.735592,1.103578,0.456240,1.180193,1.048298,0.318432,0.953830,0.407667,0.300587,0.177734,0.951846,0.425778,0.419291,0.758476,0.624043,0.383526,0.520463,0.911599,0.686144,0.962816,0.625281,0.431266,1.047222,0.879146,0.835190,0.910353,0.480349,0.529652,0.725792,0.892804,0.431299,0.397648,0.468894,0.710523,0.465373,0.942874,0.612389,0.363416,0.817404,0.409041,0.670186,0.654988,0.266488,0.622333,0.819208,0.429182,0.516477,0.449169,0.762685,0.035518,0.696685,0.679275,0.125182,0.877180,0.797867,0.661728,0.407642,0.130311,0.959637,0.929514,0.080865,0.526802,0.089893,0.405951,0.117437,0.208125,0.091708,0.683950,0.127964,0.705496},
						{0.037446,0.137761,0.771813,0.623066,0.634816,0.925901,0.504898,0.617573,0.610736,0.496658,0.165807,0.907620,0.744666,0.615920,0.645009,0.757714,0.934874,0.013337,0.980682,0.872127,0.112033,0.266182,0.832026,0.599963,0.748924,0.896268,0.313791,0.056398,0.621540,0.710776,0.098483,0.441237,0.978759,0.918790,0.192407,0.335544,0.775626,0.053347,0.291254,0.259191,0.131036,0.235592,0.123807,0.267732,0.653613,0.798454,0.685371,0.874368,0.813010,0.822269,0.773560,0.069577,0.991455,0.603168,0.723685,0.732841,0.380016,0.554521,0.762291,0.206487,0.387679,0.653559,0.874070,0.314737,0.692799,0.863346,0.582749,0.612847,0.624751,0.651996,0.594218,0.153711,0.023204,0.110565,0.701039,0.578399,0.193179,0.547459,0.594289,0.611738,0.119668,0.483551,0.648152,0.166509,0.744743,0.601642,0.781235,0.916765,0.211317,0.549820,0.370318,0.300466,0.885210,0.386591,0.859236,0.285326,0.384771,0.420729,0.571130,0.526752,0.063432,0.408419,0.873937,0.495711,0.133069,0.927839,0.742592,0.594376,0.438532,0.923232,0.509087,0.844325,0.489212,0.329783,0.827338,0.741147,0.885667,0.957459,0.117358,0.956421,0.907304,0.023050,0.097500,0.238900,0.608253,0.761382,0.715046,0.800924,0.586426,0.329293,0.356525,0.357709,0.336994,0.035689,0.867646,0.773917,0.062094,0.192724,0.140102,0.918726,0.739860,0.103397,0.076091,0.841564,0.326346,0.331743,0.515231,0.214579,0.172246,0.640018,0.646990,0.614625,0.367025,0.851866,0.740983,0.453226,0.869841,0.527595,0.757177,0.308592,0.719174,0.058665,0.704542,0.494284,0.577461,0.526444,0.256671,0.100732,0.176489,0.947049,0.761854,0.090982,0.700897,0.404269,0.401061,0.202938,0.836232,0.362000,0.414135,0.366441,0.876675,0.392108,0.903214,0.519436,1.052782,0.250081,0.670812,0.568608,0.393463,0.552238,0.842574,0.768801,0.868564,0.505931,0.631690,0.626929,0.016091,0.967725,0.782164,0.641174,0.593404,-0.031691,0.479078,0.543972,0.424119,0.602440,0.577922,0.306830,0.145808,0.199959,0.580666,0.696909,0.142713,0.598150,0.434647,0.373450,0.510573,0.519815,0.134240,0.690726,0.172834,0.508499,0.222159,0.249176,0.873559,0.152143,0.193925,0.784916,0.215145,0.558447,0.036170,0.326031,0.387436,0.792803,0.488425,0.764857,0.818070,0.753954,0.059861,0.263512,0.717005,0.528030,0.575001,0.591778,0.717904,0.834051,1.023946,0.780657,0.740625,0.252676,0.035578,0.773254,0.093216,0.741869,0.937629,0.136535,0.736416,0.883879,0.521596,0.909216,0.617497,0.736659,0.969315,0.423004,0.254350,0.363876,0.478451,0.366013,0.122578,0.494963,0.718078,0.541133,0.369841,0.513388,0.553793,0.735754,0.269138,0.889184,0.319996,0.010046,0.644333,0.926159,0.682244,0.604811,0.434313,0.915908,0.400797,0.884668,0.585947,0.311517,0.550071,0.672585,0.891042,0.427322,0.219589,0.256779,0.006145,0.414984,0.695379,0.898572,1.042993,0.261510,1.173285,0.791105,0.141896,0.179415,0.786082,0.804913,0.175599,0.046599,0.019678,0.028953,0.422002,0.776067,0.420327,0.538925,1.039787,0.779462,0.891613,0.475822,0.944162,0.626065,0.856851,0.671264,0.322104,0.302035,0.617840,0.260399,0.460042,0.771538,0.609713,0.510740,0.937127,1.006797,1.008015,0.764171,0.320992,0.063076,0.985407,0.038822,0.081662,0.931277,1.014717,0.494781,0.915180,0.454449,0.616566,0.466562,0.708021,0.800137,0.658263,0.423559,0.505278,0.509474,0.330916,0.769910,1.044342,0.687046,0.657433,0.302731,0.391821,0.676928,0.117999,0.608290,0.851495,0.468213,0.178900,0.052029,0.796954,0.446870,0.449280,0.031101,0.582025,0.376617,0.255525,0.878235,0.290362,1.130048,0.079430,0.878731,0.441662,0.075491,0.709203,0.568944,0.386447,0.655502,0.828934,0.362016,0.605320,0.313242,0.142215,0.712696,0.766301,0.391121,0.335937,0.018528,0.902642,0.895339,0.271263,0.740857,0.089917,0.555730,0.619934,0.708046,1.059295,0.715874,0.936401,0.284792,0.694421,0.776482,0.446614,0.393925,0.175438,0.646677,0.360001,-0.024792,0.621510,0.836802,0.641888,0.535209,0.917336,0.098215,0.692903,0.632025,0.636354,0.122578,0.011284,0.059749,0.766397,0.174588,0.211306,0.550464,0.526830,1.020101,1.021454,0.764632,0.010825,0.594259,0.504119,0.266837,-0.035926,0.185306,0.049344,0.077273,0.812518,0.025268,0.067126,0.823085,0.260271,0.614361,-0.000365,0.674463,0.030254,0.128881,0.122093,0.289128,0.464020,0.437835,0.798487,0.794816,0.996658,0.867984,0.471611,0.648778,0.457868,0.562893,0.116112,0.037555,0.655203,-0.017064,0.596879,0.245963,0.839909,0.522740,0.738168,0.085352,0.326945,0.681549,0.517806,0.389896,0.731223,0.308897,0.742695,0.587857,0.516199,0.636713,0.716916,0.915869,0.903002,0.918921,0.609005,0.693767,0.296412,0.359243,0.295872,0.690843,0.679695,0.271834,0.659066,0.360702,0.183027,0.907178,0.752194,0.283064,0.011512,0.774763,0.848848,0.386225,0.126224,0.277939,0.497402,0.637456,0.789710,0.686523,0.976841,0.672272,0.850308,0.730097,0.230230,0.798595,0.299455,0.635757,0.231803,0.153299,0.131974,0.581569,0.370089,0.462815,0.204322,0.392553,0.634979,0.226836,0.265602,0.392512,0.938559,0.022258,0.191919,0.810130,0.894926,0.697375,0.458174,1.088950,0.494899,0.441453,0.691886,0.056442,0.382101,0.860445,0.067447,0.404642,0.267951,0.761080,0.487516,0.561621,0.578665,0.563735,0.898802,0.423340,0.090793,0.635591,0.794427,0.342282,0.095489,0.675229,0.315735,0.439345,0.273583,0.574122,0.486501,0.419775,0.718413,0.295263,0.630572,0.631189,0.206255,0.608399,0.494033,0.667325,0.750397,0.583613,0.617473,0.552969,0.799412,0.201965,0.812081,0.210096,0.772758,0.327492,0.349494,0.238370,0.370532,0.025813,0.557218,0.280553,1.158644,0.531729,0.657261,0.392078,0.303140,0.657799,0.465297,0.669748,0.409160,0.455569,0.412132,0.171623,0.523718,0.143381,0.712861,0.900222,0.950216,0.494191,0.027238,0.926743,0.289588,0.521955,0.710436,0.930752,0.145929,0.623146,0.695053,0.396157,0.922699,0.473393,0.399451,0.275286,0.733176,0.393530,0.422440,1.104200,0.446591,0.312713,0.548172,0.941032,0.392964,0.445212,0.556834,0.270059,0.337210,0.323621,0.345492,0.657449,0.353576,0.707780,0.949767,0.086032,0.270917,0.870933,0.064332,0.414026,-0.188832,0.631285,0.441538,0.196183,0.686994,0.634475,0.912171,0.762519,0.989767,0.843056,0.432418,0.808730,0.961714,0.948934,0.876484,0.524908,0.769077,0.212555,0.155123,0.098806,0.462422,0.054201,0.075106,0.636067,0.583763,0.825722,0.195763,0.201834,0.410807,0.545065,0.094048,0.898208,0.754653,0.415852,0.632227,0.598834,0.737384,0.826384,0.776041,0.418364,0.661700,0.308857,0.342982,0.537825,0.190537,0.229002,0.201108,0.882615,0.879204,0.831751,0.641743,0.314920,0.182580,0.822669,0.882932,0.671674,0.817264,0.196393,0.158671,0.182312,0.679224,0.870270,0.136528,0.153308,0.155361,0.878267,0.522643,0.444640,0.481987,0.513188,0.193478,0.513636,0.503767,0.892123,0.606828,0.649286,0.278218,0.811396,0.429701,0.002594,0.267464,0.220454,0.515610,0.417987,0.910026,0.403597,0.738189,0.588246,0.575400,0.139433,0.298034,0.755947,0.851410,0.748820,0.750596,0.166693,0.246112,0.835941,0.467243,0.227105,0.874784,0.133991,0.480020,0.586413,0.932798,0.111393,0.779199,0.094089,0.906980,0.558367,0.745838,0.023710,0.373375,0.159315,0.634435,0.183056,0.687516,0.996160,0.236465,0.582912,0.132776,0.759242,0.357110,0.669737,0.384283,0.003704,0.240854,0.609151,0.690098,0.116303,0.640950,0.733696,0.747246,0.459700},
						{0.612384,0.574572,0.292459,0.119938,0.810724,0.368725,0.853969,0.269509,0.861690,0.710776,0.360485,0.099368,0.481947,0.043514,0.198761,0.797326,0.564501,0.825953,0.098117,0.375164,0.973754,0.257485,0.562090,0.498611,0.684347,0.002136,0.341288,0.193640,0.284677,0.795282,0.228370,0.586596,0.382610,0.352788,0.956516,0.332901,0.942125,0.485082,0.482600,0.892390,0.082910,0.821846,0.438824,0.309903,0.376532,0.057610,0.024977,0.645137,0.172590,0.117526,0.136822,0.719952,0.946745,0.144444,0.999023,0.347392,0.299905,0.747673,0.093417,0.674127,0.522885,0.369242,0.655107,0.810060,0.331749,0.207812,0.519979,0.394430,0.778569,0.601288,0.629658,0.274968,0.883995,0.257051,0.894930,0.935877,0.089938,0.345916,0.894572,0.779531,0.675315,0.471570,0.756859,0.018403,0.967071,0.164739,0.209814,0.352973,0.009209,0.518125,0.847677,0.897576,0.230274,0.324106,0.329424,0.700857,0.525366,0.093638,0.284176,0.504490,0.703373,0.548138,0.870248,0.898336,0.226754,0.697196,0.230906,0.979788,0.989180,0.051793,0.633869,0.977020,0.992157,0.377056,0.739524,0.175817,0.567564,0.963670,0.482484,0.839346,-0.019917,0.851365,0.872371,0.858153,0.276322,0.022907,0.196095,0.289893,0.554509,-0.086900,0.479730,0.357799,0.523891,0.884896,0.296583,0.026665,0.593688,0.723555,0.717761,0.637914,0.593432,0.842586,0.470389,0.793328,0.693950,0.326198,0.714378,0.294735,0.321065,0.578598,0.464647,0.098239,0.345054,0.710440,0.642663,0.161504,-0.043049,0.501681,-0.013223,0.019973,0.301600,0.001749,0.365135,0.718070,0.892343,0.510242,0.269041,0.627975,0.173193,0.006207,0.610356,0.439886,0.551916,0.825152,-0.100004,0.369137,-0.193711,0.766215,0.432113,0.302763,0.623988,0.370721,0.782412,0.993223,0.477023,0.534523,0.478978,0.407550,0.342608,0.272179,0.306668,0.147275,0.518898,0.339124,0.013788,0.761439,0.764940,0.832243,0.565723,0.488383,0.737292,0.070604,-0.167118,-0.048140,0.709831,0.115273,0.552703,0.648625,1.007979,0.971780,0.777522,0.794111,0.994880,0.624059,0.785463,0.462014,0.553607,0.583884,0.000435,0.347789,0.062890,0.315779,0.603316,0.519753,0.974111,0.859171,0.153605,0.662191,-0.003757,0.629896,0.223774,0.148209,0.272992,0.445510,0.410044,0.297384,0.416392,0.554313,1.105668,0.631090,0.957624,0.330801,0.592235,0.267062,0.641355,0.415496,0.436082,0.063140,0.114802,0.520974,0.784221,0.622511,0.634053,0.108542,0.257441,0.028376,0.698137,0.461227,0.686542,0.496786,0.596950,0.441363,0.749099,0.465200,0.186787,0.487301,0.718361,0.472156,0.181838,0.862838,0.518313,0.654103,0.749068,0.285213,0.623604,0.035424,0.721675,0.561516,0.000552,0.796076,0.888860,0.706240,0.334787,0.213689,0.369513,0.488256,0.564249,0.373651,0.610449,0.162482,0.689685,0.799061,0.678532,0.610396,0.716994,0.563263,0.469728,0.278625,0.694879,0.568165,0.668091,0.761849,0.505993,0.271858,0.074191,0.680922,0.192439,0.307739,0.931966,0.535720,0.774387,0.248165,0.678628,0.835401,0.873437,0.546813,0.915940,0.487623,0.818046,0.591987,0.225453,0.211445,0.387273,0.654408,0.751913,0.739009,0.871438,0.731889,0.651851,1.191520,0.210175,0.669634,0.134188,0.015235,0.655552,0.404547,0.111600,0.821861,0.672124,0.366436,0.611559,0.796554,0.488752,0.722080,0.783752,0.788568,0.499683,0.792489,0.512043,0.126670,0.848659,0.701841,0.960354,0.751268,0.802747,0.813524,0.796177,0.490735,0.449724,0.880108,0.729608,0.615919,0.351310,0.196047,0.622300,0.920972,0.738367,0.767227,0.894730,0.758224,0.696610,0.830287,0.818754,0.768396,0.657454,0.378821,0.455919,0.621229,0.406137,0.547114,0.654196,0.886880,1.071574,0.654243,1.030569,0.830836,0.657037,0.492599,-0.088679,0.669261,0.723420,0.116109,0.503747,0.156286,0.037151,0.325335,0.650355,0.540109,0.603274,0.459199,0.263149,0.474573,0.296468,0.580394,0.206531,0.551962,0.508885,0.703268,0.625176,0.979805,0.573776,0.829126,0.497002,0.577532,0.615813,-0.184443,0.065958,0.693112,0.421795,0.540871,0.173950,0.163911,0.503558,0.047624,0.639630,0.485154,0.647152,0.783677,0.973647,0.654826,0.337004,0.588712,0.356681,0.621322,0.476847,0.645278,0.826797,0.960483,0.406463,0.674766,0.043043,0.702665,0.546854,0.337957,0.268101,-0.030174,0.952223,0.652680,0.186128,0.438732,0.652168,0.973668,0.829670,0.319587,0.672236,0.643068,0.255534,0.411710,0.922077,0.293964,0.873170,0.276494,0.490065,0.470387,0.443938,0.392462,0.392317,0.205395,0.536963,-0.083644,0.069326,0.653372,0.727188,0.508292,0.823795,0.165017,0.470321,0.529957,0.361061,0.456070,0.677838,0.294879,0.705475,0.623978,0.328598,0.528016,0.419418,0.437209,0.814906,0.737737,0.542442,0.988436,0.696047,0.731936,0.863413,0.457107,0.172128,0.092263,0.143760,0.350983,0.395775,0.288004,0.861627,0.959991,0.744964,0.652534,0.376284,0.621898,0.416806,0.072030,0.030569,0.074741,0.435141,0.754150,0.826417,0.340649,0.583043,0.085412,0.406112,0.710948,0.278008,0.095062,-0.037319,0.519146,0.810865,0.308561,0.562776,0.288904,0.160841,0.928206,0.473181,0.289347,0.329814,0.129687,0.363809,0.024447,0.249122,0.215389,0.581976,0.741847,0.382226,0.174682,0.230637,0.444817,0.218918,0.145766,0.724753,0.462585,0.458858,0.540904,0.263671,0.204628,0.691371,0.200873,0.574508,0.077686,0.429752,0.226036,0.659625,0.583712,0.081484,0.470285,0.091187,0.027033,0.515595,0.517806,0.641765,0.326847,-0.140328,0.436953,-0.349387,0.321086,0.118125,0.457429,-0.014097,0.677834,0.124374,0.549982,0.554008,0.691248,0.015127,0.552906,0.586748,0.247928,0.389402,0.124735,0.310217,0.482248,0.663075,0.419453,0.598210,0.140113,0.244923,0.570274,0.434828,-0.102456,0.039426,0.207015,0.241298,0.047108,0.616006,0.479958,0.061898,-0.047752,-0.057595,0.348706,0.144466,-0.110270,0.656744,0.639734,0.547964,0.441083,0.342663,0.666166,0.700164,0.188208,0.872921,0.859582,0.911151,0.788266,0.192731,0.472089,0.758825,0.400558,0.264404,0.345100,0.047476,0.494531,0.110390,0.422357,0.291883,0.455470,0.531827,0.346597,-0.024408,0.646453,0.192199,0.423837,0.628035,0.099181,0.738754,0.804969,0.708482,0.724052,0.247963,0.870754,0.216312,0.620220,0.241784,0.623212,0.252237,0.848575,0.196971,0.714908,0.233587,0.619650,0.160505,0.579063,0.613294,0.167626,0.348162,0.699238,0.696891,0.739679,0.719968,0.652666,0.801362,0.533026,0.232847,0.117859,0.662299,0.189795,0.661367,0.813746,0.994090,0.011875,0.294841,0.735066,0.089865,0.173803,0.321097,0.751267,0.448065,0.711243,0.504598,0.448113,0.358271,0.364447,0.485044,0.180182,0.431054,0.557639,1.052788,0.639278,0.210137,0.581701,0.259683,0.681479,0.372908,0.618549,0.132633,0.773888,0.053207,0.844289,0.354897,0.361516,0.957080,0.914578,0.905094,0.345772,0.868027,1.069867,0.277779,0.228686,0.546268,0.720690,0.904461,1.012946,1.103559,0.586854,1.161880,0.198827,1.016512,0.132116,0.805966,0.134429,0.234762,0.520524,0.569994,0.209723,0.968535,0.031772,0.222977,0.940049,0.479876,0.973927,0.978991,1.071357,0.253999,0.493947,0.680008,0.973706,0.808622,0.311097,0.929126,0.318195,0.455744,0.256197,0.792933,0.865641,0.666386,0.018075,0.451547,0.879481,0.907254,0.710746,0.662648,0.150792,0.861324,0.085330,0.205413,0.388894,0.917287,0.367116,0.476959,0.602758,0.051480,0.415004,0.406939,0.266635,0.542084,0.279201,0.951709,0.612071,0.019796,0.669894,0.314525,0.259664,0.333547,0.198609,0.512162,0.839625,0.532121,0.513474}
	    		}
	    };

	    draw_menu();
	    int status;
	    int delay = 0;
	    unsigned short int flag = 0;
	    while(1){
			status = IORD_ALTERA_AVALON_PIO_DATA(SW_BASE);

			if (status == 0x20000 || status == 0x8000 || status == 0x10000 ){ // switch 17,16,15
				int record = status;
				short unsigned int nimages = 0;
				draw_menu2(nimages);
				while(IORD_ALTERA_AVALON_PIO_DATA(SW_BASE)!= 1){ //sw1
					if(IORD_ALTERA_AVALON_PIO_DATA(KEY_BASE) == 14 && (nimages<=80)){//key0
						nimages+=1;
						while(delay<1000000)
							delay++;
						delay=0;
						draw_menu2(nimages);
					}
					else if (IORD_ALTERA_AVALON_PIO_DATA(KEY_BASE) == 13 && (nimages<=70)){//key1
						nimages+=10;
						while(delay<1000000)
							delay++;
						delay=0;
						draw_menu2(nimages);
					}
					else if ((IORD_ALTERA_AVALON_PIO_DATA(KEY_BASE) == 11) && (nimages>0)){//key2
						nimages-=1;
						while(delay<1000000)
							delay++;
						delay=0;
						draw_menu2(nimages);
					}
					else if ((IORD_ALTERA_AVALON_PIO_DATA(KEY_BASE) == 7) && (nimages>9)){//key3
						nimages-=10;
						while(delay<1000000)
							delay++;
						delay=0;
						draw_menu2(nimages);
					}
					else
						;
				}
				if(record == 0x20000 ){//version1
					draw_menu3();
					fast_version(network,hFat,nimages+21);
					flag = 1;
				}
				else if(record == 0x10000){//version2
					draw_menu3();
					fast_version3(network,hFat,nimages+21);
					flag = 1;
				}
				else if(record == 0x8000){//version3
					draw_menu3();
					fast_version1(network,hFat,nimages+21);
					flag = 1;
				}
				else{
					;
				}

			}
			else if (status == 0x4000){ // switch 14
				step_version(network,hFat);
				flag = 1;
			}

			else{
				;
			}
			while ( flag == 1){
				if(IORD_ALTERA_AVALON_PIO_DATA(KEY_BASE) == 14){
					flag=0;
					draw_menu();
				}
			}
	    }


		FAT_FILE_HANDLE hFile;
		hFile =  Fat_FileOpen(hFat, "110.txt");
		char *rawimg = NULL;
		rawimg = (char *) malloc(sizeof(char)*41);//41 chars por linha
		if(!Fat_FileRead(hFile, rawimg, 1))
			printf("ERRO AO LER FICHEIRO\n");

		free(hFile);
//		listfiles(hFat);
	}

	else
		printf("Failed to mount the SDCARD!\r\nPlease insert the SDCARD into DE2-115\r\n");

	Fat_Unmount(hFat);
	printf("\n\n program finish, fat unmounted\n");
	return 0;
}
//----------- functions ----------------------
//--------------------------------------------
void displayimage(int *pixels,int predict,int label, float time){
	alt_up_char_buffer_dev * char_buffer = NULL ;
	char_buffer = alt_up_char_buffer_open_dev ("/dev/video_character_buffer_with_dma_0");
	alt_up_char_buffer_init(char_buffer);
	alt_up_char_buffer_clear(char_buffer);

	int i=0;
	int j= 0;
	int idx = 0;

	for ( i = 0; i<28; i++){
		for ( j = 0;j<28;j++){
			idx=i*28+j;
			if(*(pixels+idx)>128)
				alt_up_char_buffer_draw(char_buffer ,'1',j,i);
			else
				alt_up_char_buffer_draw(char_buffer ,'0',j,i);
		}
	}
	alt_up_char_buffer_string(char_buffer ,"predicted:",0,29);
	alt_up_char_buffer_string(char_buffer ,"label:",0,30);
	alt_up_char_buffer_draw(char_buffer ,predict+'0',12,29);
	alt_up_char_buffer_draw(char_buffer ,label+'0',12,30);

	alt_up_char_buffer_string(char_buffer ,"Next Sample: KEY[0]",35,15);
	alt_up_char_buffer_string(char_buffer ,"Exit:        KEY[1]",35,17);

	alt_up_char_buffer_string(char_buffer ,"Execution time for this sample:",35,31);
	char str[20];
	sprintf(str, "%f ms", time);
	alt_up_char_buffer_string(char_buffer ,str,35,33);
}
//--------------------------------------------
//------------------------------------
int readimage(FAT_HANDLE hFat, int *pixels,char *filename){
	int pixel_idx,aux,j,i = 0;

	FAT_FILE_HANDLE hFile;
	hFile =  Fat_FileOpen(hFat, filename);

	//memory allocation
	char *rawimg = NULL;
	rawimg = (char *) malloc(sizeof(char)*41);//41 chars por linha

	char *arr32,*arr2,*p= NULL;
	arr32 = (char *) malloc(sizeof(char)*32);
	arr2 = (char *) malloc(sizeof(char)*2);

	//copy file information to integer pixel values
	aux = 0;
	pixel_idx=0;
	j= 0;
	int k = 0;

	for (i = 0; i<49; i++){ //49 linhas
		if(!Fat_FileRead(hFile, rawimg, 41))
			printf("ERRO AO LER FICHEIRO\n");
		for (k = 0; k<39; k++){
			if (*(rawimg+k) != 32){						 //filter spaces
				memcpy(arr32+j, rawimg+k, sizeof(char)); //copy 32 hex string with no spaces
				aux+=1;
				if(aux == 2){							 //each pixel has 8bits, so its 2 hex chars
					aux = 0;
					memcpy(arr2, arr32+(j-1), sizeof(char)*2);
					*(pixels+pixel_idx) = strtol(arr2,&p,16); //convert hex 2 char string to integer value
					pixel_idx += 1;
				}
				j+=1;
			}
		}
		j=0;
		aux=0;
	}
	char n;
	Fat_FileRead(hFile, &n, 1);
	int label;
	label = atoi(&n);

	free(arr32);
	free(arr2);
	free(rawimg);
	free(hFile);
	//Fat_FileClose(hFile);
	return label;
}
//--------------------------------------------
bool listfiles(FAT_HANDLE hFat){
    bool bSuccess;
    int nCount = 0;
    FAT_BROWSE_HANDLE hBrowse;
    FILE_CONTEXT FileContext;

    bSuccess = Fat_FileBrowseBegin(hFat, &hBrowse);
    if (bSuccess){
        while(Fat_FileBrowseNext(&hBrowse, &FileContext)){
            if (FileContext.bLongFilename){
                alt_u16 *pData16;
                alt_u8 *pData8;
                pData16 = (alt_u16 *)FileContext.szName;
                pData8 = FileContext.szName;
                printf("[%d]", nCount);
                while(*pData16){
                    if (*pData8)
                        printf("%c", *pData8);
                    pData8++;
                    if (*pData8)
                        printf("%c", *pData8);
                    pData8++;
                    //
                    pData16++;
                }
                printf("\n");
            }else{
                printf("[%d]%s\n", nCount, FileContext.szName);
            }
            nCount++;
        }
    }
    return bSuccess;
}
//--------------------------------------------
void file_exists(FAT_HANDLE hFat, const char *filename){
	if (Fat_FileExist(hFat, filename) ){
		printf("\n--------- file exists -------- \n");

		FAT_FILE_HANDLE file_handle;
		file_handle = Fat_FileOpen(hFat, filename);

		unsigned int i = 0;
		i = Fat_FileSize(file_handle);

		printf("file size:%u\n",i);

		Fat_FileClose(file_handle);
	}
	else{
		printf("\n--------- file doesnt exist -------- \n");
	}
}
//--------------------------------------------
void draw_menu(void){
	alt_up_char_buffer_dev * char_buffer = NULL ;
	char_buffer = alt_up_char_buffer_open_dev ("/dev/video_character_buffer_with_dma_0");
	alt_up_char_buffer_init(char_buffer);
	alt_up_char_buffer_clear(char_buffer);

	alt_up_char_buffer_string(char_buffer ,"CPHS 2021 - RAFAEL VIEIRA",28,2);

	alt_up_char_buffer_string(char_buffer ,"MAIN MENU",35,8);

	int offset = 3;
	int xoffset = 18;
	alt_up_char_buffer_string(char_buffer ,"SW 17 - RUN VERSION 1 (no accelaration)",xoffset,10+offset*1);
	alt_up_char_buffer_string(char_buffer ,"SW 16 - RUN VERSION 2 (RTL only)",xoffset,10+offset*2);
	alt_up_char_buffer_string(char_buffer ,"SW 15 - RUN VERSION 3 (RTL paralelization)",xoffset,10+offset*3);
	alt_up_char_buffer_string(char_buffer ,"SW 14 - RUN VERSION 4 (step-by-step version)",xoffset,10+offset*4);
}
//--------------------------------------------
void draw_menu2(short unsigned int nimages){
	alt_up_char_buffer_dev * char_buffer = NULL ;
	char_buffer = alt_up_char_buffer_open_dev ("/dev/video_character_buffer_with_dma_0");
	alt_up_char_buffer_init(char_buffer);
	alt_up_char_buffer_clear(char_buffer);

	char str[20];

	alt_up_char_buffer_string(char_buffer ,"CONTROLS:",18,8);
	alt_up_char_buffer_string(char_buffer ,"ADD 1   - KEY[0]",18,10);
	alt_up_char_buffer_string(char_buffer ,"ADD 10  - KEY[1]",18,12);
	alt_up_char_buffer_string(char_buffer ,"SUBB 1  - KEY[2]",18,14);
	alt_up_char_buffer_string(char_buffer ,"SUBB 10 - KEY[3]",18,16);
	alt_up_char_buffer_string(char_buffer ,"CONFIRM - SW [0]",18,18);

	alt_up_char_buffer_string(char_buffer ,"Number of samples to classify (max 80):",25,25);
	sprintf(str, "%d", nimages);
	alt_up_char_buffer_string(char_buffer ,str,40,30);
}
//--------------- END ------------------------
void draw_menu3(void){
	alt_up_char_buffer_dev * char_buffer = NULL ;
	char_buffer = alt_up_char_buffer_open_dev ("/dev/video_character_buffer_with_dma_0");
	alt_up_char_buffer_init(char_buffer);
	alt_up_char_buffer_clear(char_buffer);

	int offset = 3;
	int xoffset = 28;
	alt_up_char_buffer_string(char_buffer ,"CLASSIFIER RUNNING",xoffset,10+offset*1);
}

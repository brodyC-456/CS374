#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

#include "ppm.h"

// Contains all the data we pass between threads
struct ThreadArgs {
    struct ppm *input;
    int starting_y;
    int height;
    int total_r;
    int total_g;
    int total_b;
};

void usage(){
    fprintf(stderr, "usage: gray_scaler num_threads input_file output_file\n");
    exit(1);
}

// Uses total rgb values and the number of pixels in the image to calculate their averages, then prints them.
void calc_and_print_averages(int total_r, int total_g, int total_b, int pixel_count){
    total_r = total_r / pixel_count;
    total_g = total_g / pixel_count;
    total_b = total_b / pixel_count;
    printf("Average R: %i\n", total_r);
    printf("Average G: %i\n", total_g);
    printf("Average B: %i\n", total_b);
}

// Uses PPM API to convert every pixel to grayscale
void process_rows(struct ppm *input, int starting_y, int height, int *total_r, int *total_g, int *total_b){
    for (int i = starting_y; i < starting_y + height; i++){
        for (int j = 0; j < input->width; j++){
            int pixel = ppm_get_pixel(input, j, i);
            int r = PPM_PIXEL_R(pixel);
            int g = PPM_PIXEL_G(pixel);
            int b = PPM_PIXEL_B(pixel);
            *total_r += r;
            *total_g += g;
            *total_b += b;
            int gray = (299*r + 587*g + 114*b) / 1000;
            int gray_pixel = PPM_PIXEL(gray, gray, gray);
            ppm_set_pixel(input, j, i, gray_pixel);
        }
    }
}

// Thread Running Function
void *run(void *args){
    struct ThreadArgs *a = (struct ThreadArgs *)args;
    process_rows(a->input, a->starting_y, a->height, &a->total_r, &a->total_g, &a->total_b);
    return NULL;
}


int main(int argc, char *argv[]){

    // Needs exactly 3 arguments
    if (argc != 4){
        usage();
    }

    // Setting up necessary variables
    int num_threads = atoi(argv[1]);
    char *output_file = argv[3];
    int total_r = 0;
    int total_g = 0;
    int total_b = 0;
    struct ppm *input_file = ppm_read(argv[2]);
    pthread_t threads[num_threads];
    struct ThreadArgs thread_args[num_threads];
    int height_of_threads = (input_file->height / num_threads);
    int remainder = (input_file->height % num_threads);
    int total_pixel_count = input_file->height * input_file->width;
    

    // Thread loop
    for (int i = 0; i < num_threads; i++){
        
        thread_args[i].starting_y = i * height_of_threads;
        thread_args[i].input = input_file;
        if (i != num_threads - 1){
            thread_args[i].height = height_of_threads;
            printf("Thread %i: %i %i\n", i, thread_args[i].starting_y, height_of_threads);
        }
        else{
            thread_args[i].height = height_of_threads + remainder;
            printf("Thread %i: %i %i\n", i, thread_args[i].starting_y, height_of_threads + remainder);
        }
        thread_args[i].total_r = 0;
        thread_args[i].total_g = 0;
        thread_args[i].total_b = 0;
        pthread_create(&threads[i], NULL, run, &thread_args[i]);
        
    }

    // Join Loop
    for (int i = 0; i < num_threads; i++){
        pthread_join(threads[i], NULL);
    }   

    // Close File
    ppm_write(input_file, output_file);
    ppm_free(input_file);

    // Get total rgb values
    for (int i = 0; i < num_threads; i++){
        total_r += thread_args[i].total_r;
        total_g += thread_args[i].total_g;
        total_b += thread_args[i].total_b;
    }

    // Print their averages
    calc_and_print_averages(total_r, total_g, total_b, total_pixel_count);

}
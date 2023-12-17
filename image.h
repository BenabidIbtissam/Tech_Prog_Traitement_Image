#include <stdio.h>
#include <stdlib.h>
#include <png.h>
#include <time.h>
#include <string.h>


void fill_randomly_Image(png_bytep *row_pointers, int width, int height){
   
   srand(time(NULL));
            for (int y = 0; y < height; y++) {
                png_bytep row = row_pointers[y];
                for (int x = 0; x < width * 4; x += 4) {
                    row[x] = rand() % 256;      // Red
                    row[x + 1] = rand() % 256;  // Green
                    row[x + 2] = rand() % 256;  // Blue
                    row[x + 3] = 255;           // Alpha
                }
            }
}


void fill_manualy_Image(png_bytep *row_pointers, int width, int height){
for (int y = 0; y < height; y++) {
                png_bytep row = row_pointers[y];
                for (int x = 0; x < width * 4; x += 4) {
                    printf("Enter RGBA values for pixel at position (%d, %d):\n", x / 4, y);
                    printf("Red (0-255): ");
                    scanf("%hhu", &row[x]);
                    printf("Green (0-255): ");
                    scanf("%hhu", &row[x + 1]);
                    printf("Blue (0-255): ");
                    scanf("%hhu", &row[x + 2]);
                    row[x + 3] = 255; // Alpha
                }
}
}


void fill_file_Image(png_bytep *row_pointers, int width, int height,const char *filename){

 FILE *file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Error: Cannot open file %s for reading.\n", filename);
        return;
    }
for (int y = 0; y < height; y++) {
        png_bytep row = row_pointers[y];
        for (int x = 0; x < width * 4; x += 4) { 
            int red, green, blue;
            if (fscanf(file, "%d %d %d", &red, &green, &blue) != 3) {
                fprintf(stderr, "Error: Insufficient data in input file.\n");
                fclose(file);
                return;
            }
            row[x] = (png_byte)red; 
            row[x + 1] = (png_byte)green; 
            row[x + 2] = (png_byte)blue; 
            row[x + 3] = 255; 
        }
    }

} 

void initialize_image(png_bytep *row_pointers, int width, int height) {
    
    printf("Select image initialization method:\n");
    printf("1. Random pixel values (R/G/B)\n");
    printf("2. Input pixel values manually from keyboard\n");
    printf("3. Input pixel values from a file\n");
    int choice;
    scanf("%d", &choice);
    switch (choice) {
        case 1: 
            fill_randomly_Image(row_pointers,  width,  height);
            break;
        case 2: 
            fill_manualy_Image(row_pointers,  width,  height);
            break;
        case 3: 
         char filename[100]; // Adjust the size according to your needs
            printf("Enter file name: ");
            scanf("%s", filename);
            fill_file_Image(row_pointers, width, height, filename); 
            break;
        default:
            printf("Invalid choice.\n");
    }
}

void extract_color_channel(png_bytep *row_pointers, int width, int height, char color_channel) {
    const char *color_str;
    switch (color_channel) {
        case 'R':
            color_str = "Red";
            break;
        case 'G':
            color_str = "Green";
            break;
        case 'B':
            color_str = "Blue";
            break;
        default:
            printf("Invalid color channel.\n");
            return;
    }
    char output_filename[100];
    sprintf(output_filename, "grayscale_%s.png", color_str);

    FILE *fp = fopen(output_filename, "wb");
    if (!fp) {
        fprintf(stderr, "Error: Cannot open file %s for writing.\n", output_filename);
        return;
    }

    png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) {
        fclose(fp);
        fprintf(stderr, "Error: png_create_write_struct failed.\n");
        return;
    }

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_write_struct(&png_ptr, NULL);
        fclose(fp);
        fprintf(stderr, "Error: png_create_info_struct failed.\n");
        return;
    }

    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_write_struct(&png_ptr, &info_ptr);
        fclose(fp);
        fprintf(stderr, "Error: Error during PNG creation.\n");
        return;
    }

    png_set_IHDR(png_ptr, info_ptr, width, height, 8, PNG_COLOR_TYPE_GRAY, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

    png_init_io(png_ptr, fp);
    png_set_compression_level(png_ptr, 9);

    png_write_info(png_ptr, info_ptr);

    for (int y = 0; y < height; y++) {
        png_bytep row = row_pointers[y];
        for (int x = 0; x < width * 4; x += 4) {
            png_byte color_value;
            switch (color_channel) {
                case 'R':
                    color_value = row[x];
                    break;
                case 'G':
                    color_value = row[x + 1]; 
                    break;
                case 'B':
                    color_value = row[x + 2]; 
                    break;
            }

            row[x / 4] = color_value; 
        }
    }

    png_write_image(png_ptr, row_pointers);
    png_write_end(png_ptr, NULL);

    png_destroy_write_struct(&png_ptr, &info_ptr);
    fclose(fp);

    printf("Grayscale image created from %s channel: %s\n", color_str, output_filename);
}

void apply_blur_filter(png_bytep *row_pointers, int width, int height ,png_structp png_ptr ,png_infop info_ptr ) {

    png_bytep *temp_row_pointers = (png_bytep *)malloc(sizeof(png_bytep) * height);
    for (int y = 0; y < height; y++) {
        temp_row_pointers[y] = (png_byte *)malloc(png_get_rowbytes(png_ptr, info_ptr));
        memcpy(temp_row_pointers[y], row_pointers[y], png_get_rowbytes(png_ptr, info_ptr));
    }

    float kernel[3][3] = {
        {1.0f / 9.0f, 1.0f / 9.0f, 1.0f / 9.0f},
        {1.0f / 9.0f, 1.0f / 9.0f, 1.0f / 9.0f},
        {1.0f / 9.0f, 1.0f / 9.0f, 1.0f / 9.0f}
    };

    for (int y = 1; y < height - 1; y++) {
        for (int x = 4; x < (width - 1) * 4; x += 4) {
            float sumR = 0.0f, sumG = 0.0f, sumB = 0.0f;
            for (int ky = -1; ky <= 1; ky++) {
                for (int kx = -4; kx <= 4; kx += 4) {
                    png_bytep neighbor_pixel = temp_row_pointers[y + ky] + x + kx;
                    sumR += kernel[ky + 1][kx / 4 + 1] * neighbor_pixel[0]; // Red channel
                    sumG += kernel[ky + 1][kx / 4 + 1] * neighbor_pixel[1]; // Green channel
                    sumB += kernel[ky + 1][kx / 4 + 1] * neighbor_pixel[2]; // Blue channel
                }
            }

            row_pointers[y][x] = (png_byte)sumR;
            row_pointers[y][x + 1] = (png_byte)sumG;
            row_pointers[y][x + 2] = (png_byte)sumB;
        }
    }

    // Free the temporary buffer
    for (int y = 0; y < height; y++) {
        free(temp_row_pointers[y]);
    }
    free(temp_row_pointers);
}


void create_image(const char *filename, int width, int height, int resolution) {
    FILE *fp = fopen(filename, "wb");
    if (!fp) {
        fprintf(stderr, "Error: Cannot open file %s for writing.\n", filename);
        return;
    }

    png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) {
        fclose(fp);
        fprintf(stderr, "Error: png_create_write_struct failed.\n");
        return;
    }

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_write_struct(&png_ptr, NULL);
        fclose(fp);
        fprintf(stderr, "Error: png_create_info_struct failed.\n");
        return;
    }

    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_write_struct(&png_ptr, &info_ptr);
        fclose(fp);
        fprintf(stderr, "Error: Error during PNG creation.\n");
        return;
    }

    png_set_IHDR(png_ptr, info_ptr, width, height, 8, PNG_COLOR_TYPE_RGBA, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);
    png_set_pHYs(png_ptr, info_ptr, resolution, resolution, PNG_RESOLUTION_METER);

    png_init_io(png_ptr, fp);
    png_set_compression_level(png_ptr, 9);

    png_write_info(png_ptr, info_ptr);

    png_bytep *row_pointers = (png_bytep *)malloc(sizeof(png_bytep) * height);
    for (int y = 0; y < height; y++) {
        row_pointers[y] = (png_byte *)malloc(png_get_rowbytes(png_ptr, info_ptr));
    }

    

    initialize_image(row_pointers, width, height);

    extract_color_channel(row_pointers, width, height, 'G');

    printf("Apply blur filter\n");
    printf("1. yes\n");
    printf("2. No\n");

    int choice;
    scanf("%d", &choice);
  
    if(choice == 1){
    apply_blur_filter(row_pointers, width, height , png_ptr , info_ptr);
    }
      



    // Write image data to file
    png_write_image(png_ptr, row_pointers);
    png_write_end(png_ptr, NULL);

    // Free memory
    for (int y = 0; y < height; y++) {
        free(row_pointers[y]);
    }
    free(row_pointers);

    png_destroy_write_struct(&png_ptr, &info_ptr);
    fclose(fp);

    printf("Image created: %s\n", filename);
}


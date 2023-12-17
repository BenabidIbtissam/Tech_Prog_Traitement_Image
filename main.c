#include <stdio.h>
#include <stdlib.h>
#include <png.h>
#include <time.h>
#include "image.h" // Assuming create_png_image is defined in image.h


int main() {
    const char *file_path = "created_image.png";
    int width, height, resolution;

    printf("Enter image width: ");
    scanf("%d", &width);

    printf("Enter image height: ");
    scanf("%d", &height);

    printf("Enter image resolution: ");
    scanf("%d", &resolution);

    create_image(file_path, width, height, resolution);


    return 0;
}
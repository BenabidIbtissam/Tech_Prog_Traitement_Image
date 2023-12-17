# Image Processing with PNG in C

This repository contains a simple C program that generates PNG images and applies various image processing operations like filling with random colors, manual input, file input, and basic filters like blur.

## Introduction

The program utilizes the libpng library to create PNG images with options to fill pixels randomly, manually input pixel values, or read pixel values from a file. It also includes functions to extract specific color channels and apply a simple blur filter to the generated images.

## Prerequisites

  1. C Compiler (e.g., GCC)
  2. libpng library
  3. [Optional] IDE or Text Editor (e.g., Visual Studio Code, Sublime Text)

## Setup and Usage
   
1. Build the program :

   ```bash
   image_processing

2. Run the program :
   
    ```bash
    ./image_processing
    
3. Follow the on-screen instructions:

    The program presents a menu to select image data sources, apply filters, and perform various operations on the generated images.


## Functions Available

   - Generate PNG images with random pixel values (RGB).
   - Input pixel values manually from the keyboard.
   - Input pixel values from a file.
   - Extract specific color channels (Red, Green, Blue) and create grayscale images.
   - Apply a simple blur filter to the generated images.

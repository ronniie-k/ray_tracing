#include<iostream>
#include<glm/glm.hpp>

#include"Log.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include<stb_image_write.h>

int main()
{
    unsigned char* data = new unsigned char[3];
    int width = 1;
    int height = 1;
    int channels = 3;
    *data = 0;
    *(data + 1) = 255;
    *(data + 2) = 0;
    stbi_write_png("output/test.png", width, height, channels, data, width * channels);
    return 0;
}
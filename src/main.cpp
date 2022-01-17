#include<iostream>
#include<glm/glm.hpp>

#include"Log.h"
#include"Texture/Image.h"

#include"Example/CubeExample.h"

int main()
{
    Image img(1280, 720, 3);
    img.data = new unsigned char[img.width * img.height * img.channels];

    CubeExample ce(img);
    ce.draw();

    return 0;
}
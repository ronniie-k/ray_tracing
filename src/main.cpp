#include<iostream>
#include<glm/glm.hpp>

#include"Log.h"
#include"Renderer/Renderer.h"

int main()
{
    Image img;
    img.width = 1280;
    img.height = 720;
    img.channels = 3;
    img.step = img.width * img.channels;
    img.aspectRatio = static_cast<float>(img.width) / img.height;

    Renderer r(img);
    r.draw();
    return 0;
}
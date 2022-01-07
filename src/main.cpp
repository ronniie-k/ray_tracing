#include<iostream>
#include<glm/glm.hpp>

#include"Log.h"
#include"Renderer/Renderer.h"
#include"Texture/Image.h"
#include"Model/Model.h"

int main()
{
    Image img(1280, 720, 3);
    img.data = new unsigned char[img.width * img.height * img.channels];

    Renderer r(img);
    r.draw();
    return 0;
}
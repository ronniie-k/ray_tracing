#include<iostream>
#include<glm/glm.hpp>
#include<chrono>

#include"Log.h"
#include"Texture/Image.h"
#include"Example/CubeExample.h"
#include"Example/CornellBoxExample.h"

using time_point_t = std::chrono::steady_clock::time_point;

int main()
{
    Image img(1280, 720, 3);
    img.data = new unsigned char[img.width * img.height * img.channels];

    time_point_t begin = std::chrono::steady_clock::now();

    //CubeExample ce(img);
    //ce.draw();

    CornellBoxExample cbe(img);
    cbe.draw();

    time_point_t end = std::chrono::steady_clock::now();
    Log::info("time taken: {} seconds", std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() / 1000.f);



    return 0;
}
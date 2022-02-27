#include<iostream>
#include<glm/glm.hpp>
#include<chrono>

#include"Log.h"
#include"Texture/Image.h"
#include"Example/CubeExample.h"
#include"Example/CornellBoxExample.h"
//#include"Example/BVHExample.h"

#include"Random.h"

using time_point_t = std::chrono::steady_clock::time_point;

int main()
{
    Image img(1280, 720, 3);
    img.data = new unsigned char[img.width * img.height * img.channels];

    time_point_t begin = std::chrono::high_resolution_clock::now();

    //CubeExample ce(img);
    //ce.draw();

    CornellBoxExample cbe(img);
    cbe.draw();

    //BVHExample bvh(img);
    //bvh.draw();

    time_point_t end = std::chrono::high_resolution_clock::now();
    Log::info("time taken: {} seconds", std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin).count() / 1000000000.f);



    return 0;
}

//link time, abx/avx/sse
//lto/ltc
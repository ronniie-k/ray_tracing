#include<iostream>
#include<glm/glm.hpp>
#include<chrono>

#include"Log.h"
#include"Texture/Image.h"
#include"Example/CubeExample.h"
#include"Example/CornellBoxExample.h"
#include"Example/SpheresExample.h"

#include"Instrumentor.h"

#include"Random.h"
#include"Timer.h"

using time_point_t = std::chrono::steady_clock::time_point;

int main()
{
    Image img(1280, 720, 3);
    img.data = new unsigned char[img.width * img.height * img.channels];

    SpheresExample se(img, 10, 250);

    se.draw();

    /*double avg = 0;
    int count = 0;

    for(int i = 0; i < 5; i++)
    {
        Log::info("warmup {}", i + 1);
        se.draw();
    }

    for(int i = 0; i < 25; i++)
    {
        Log::info("run: {}", i + 1);
        time_point_t begin = std::chrono::high_resolution_clock::now();
        se.draw();

        time_point_t end = std::chrono::high_resolution_clock::now();
        auto time = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin).count();
        avg = avg + ((time - avg) / ++count);
    }

    
    Log::info("avg time taken: {} seconds", avg / 1000000000.f);*/

    Timer::print();

    return 0;
}
#include<iostream>
#include<glm/glm.hpp>

#include<glad/glad.h>

#include<SFML/Graphics.hpp>

#include"Log.h"

int main()
{
    Log::get();

    Log::trace("{}", 5);
    Log::info("{}", 4);
    Log::warn("aaaaaa");
    Log::error("error");
    Log::critical("bad");

    return 0;
}
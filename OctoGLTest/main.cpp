#include <SDL2/SDL.h>

int main(int argc, const char * argv[])
{
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) return 1;

    printf("Hello world\n");

    return 0;
}

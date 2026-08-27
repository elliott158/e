#include <SDL2/SDL.h>
#include <cairo/cairo.h>
#include <stdio.h>

#define WIDTH  640
#define HEIGHT 480

#define DEFAULT_TEXT_SIZE 16

int main(void)
{
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    fprintf(stderr, "SDL_Init failed: %s\n", SDL_GetError());
    return 1;
  }

  SDL_Window *window = SDL_CreateWindow(
    "e",
    SDL_WINDOWPOS_CENTERED,
    SDL_WINDOWPOS_CENTERED,
    WIDTH,
    HEIGHT,
    0
  );

  SDL_Renderer *renderer = SDL_CreateRenderer(
    window, -1, SDL_RENDERER_ACCELERATED
  );

  SDL_Texture *texture = SDL_CreateTexture(
    renderer,
    SDL_PIXELFORMAT_ARGB8888,
    SDL_TEXTUREACCESS_STREAMING,
    WIDTH,
    HEIGHT
  );

  // cairo draws into this buffer
  unsigned int pixels[WIDTH * HEIGHT];

  cairo_surface_t *surface = cairo_image_surface_create_for_data(
    (unsigned char *)pixels,
    CAIRO_FORMAT_ARGB32,
    WIDTH,
    HEIGHT,
    WIDTH * 4
  );

  cairo_t *cr = cairo_create(surface);

  cairo_set_source_rgb(cr, 0.0, 0.0, 0.0);
  cairo_paint(cr);

  cairo_set_source_rgb(cr, 1.0, 1.0, 1.0);
  cairo_select_font_face(
    cr,
    "Sans",
    CAIRO_FONT_SLANT_NORMAL,
    CAIRO_FONT_WEIGHT_NORMAL
  );
  cairo_set_font_size(cr, DEFAULT_TEXT_SIZE);
  //setup done
  
  cairo_move_to(cr, 0, DEFAULT_TEXT_SIZE);
  cairo_show_text(cr, "// This is the scratch buffer. Put what you want here.");

  int running = 1;
  while (running) {
    SDL_Event event;

    // event handling
    SDL_StartTextInput();
    while (SDL_PollEvent(&event)) {
      switch (event.type) {

      case SDL_QUIT:
        running = 0;
        break;

      case SDL_TEXTINPUT:
        // does not handle modifiers
        cairo_show_text(cr, event.text.text);
        break;
      }
      
    }
        
    SDL_UpdateTexture(
      texture,
      NULL,
      pixels,
      WIDTH * sizeof(unsigned int)
    );

    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);

    SDL_Delay(16);
  }

  //destruction begins

  cairo_destroy(cr);
  cairo_surface_flush(surface);
  cairo_surface_destroy(surface);
  SDL_DestroyTexture(texture);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}


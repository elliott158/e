#include <SDL2/SDL.h>
#include <cairo/cairo.h>

#include <stdio.h>
#include <stdlib.h>

#include "common.h"

#define WIDTH  1280
#define HEIGHT 960
//window can be resized, but resolution stays constant

#define DEFAULT_TEXT_SIZE 32

typedef struct {
  char* items;
  size_t used;
  size_t capacity;
} Buffer;

void reset_font(cairo_t *cr) {
  cairo_set_operator(cr, CAIRO_OPERATOR_OVER);
  cairo_set_source_rgb(cr, 1.0, 1.0, 1.0);
  cairo_select_font_face(
    cr,
    "@cairo:monospace",
    CAIRO_FONT_SLANT_NORMAL,
    CAIRO_FONT_WEIGHT_NORMAL
  );
  cairo_set_font_size(cr, DEFAULT_TEXT_SIZE);
}

void print_buffer(Buffer xs) {
  for (int i=0;i<xs.used;++i) {
    printf("%c", xs.items[i]);
  }
  puts("\n");
}

Buffer buffer_init(size_t initial_capacity) {
  return (Buffer){
    .capacity = initial_capacity,
    .used = 0,
    .items = malloc(sizeof(char) * initial_capacity),
  };
}

void draw_buffer(cairo_t *cr, Buffer xs) {
  if (xs.used > 0) cairo_show_text(cr, xs.items);
}

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
  SDL_SetWindowResizable(window, SDL_TRUE);

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
  reset_font(cr);
  //setup done
  
  cairo_move_to(cr, 0, DEFAULT_TEXT_SIZE);

  Buffer working = buffer_init(256);
  
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
        da_append(working, event.text.text[0]);
        break;

      case SDL_KEYDOWN:
        switch (event.key.keysym.sym) {
        case SDLK_BACKSPACE:
          if (working.used > 0) {
            working.items[working.used - 1] = '\0';
            working.used--;
          }
          break;
        case SDLK_RETURN:
          da_append(working, '\n');
        default:
          break;
        }
      default:
        break;
      }      
    }
    
    cairo_set_source_rgb(cr, 0.0, 0.0, 0.0);
    cairo_paint(cr);
    
    cairo_set_source_rgb(cr, 1,1,1);
    cairo_move_to(cr,0,DEFAULT_TEXT_SIZE/1.5);

    draw_buffer(cr, working);
    
    cairo_surface_flush(surface);
    
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

  print_buffer(working);
  //destruction begins
  da_free(working);
  cairo_destroy(cr);
  cairo_surface_flush(surface);
  cairo_surface_destroy(surface);
  SDL_DestroyTexture(texture);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}


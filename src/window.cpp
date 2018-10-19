#include "window.h"

namespace kagami {
#if not defined(_DISABLE_SDL_)

  /* Under Construction */
  Message SDLInit(ObjectMap &p) {
    int result = SDL_Init(SDL_INIT_EVERYTHING);
    Message msg;
    if (result < 0) {
      msg = Message(kStrFalse);
    }
    else {
      msg = Message(kStrTrue);
    }
    return msg;
  }

  Message SDLQuit(ObjectMap &p) {
    SDL_Quit();
    return Message();
  }

  Message SDLCreateWindow(ObjectMap &p) {
    int w = stoi(p.Get<string>("width"));
    int h = stoi(p.Get<string>("height"));
    string title = p.Get<string>("title");
    Object ret;
    WindowBase win_base = make_shared<Window>();
    win_base->window = SDL_CreateWindow(title.c_str(),
      SDL_WINDOWPOS_CENTERED,
      SDL_WINDOWPOS_CENTERED,
      w, h, SDL_WINDOW_SHOWN);
    win_base->render = SDL_CreateRenderer(win_base->window, -1,
      SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    ret.Set(win_base, kTypeIdSDLWindow, kSDLWindowMethods, false);
    return Message().SetObject(ret);
  }
  
  Message SDLCreateTextureFormBMP(ObjectMap &p) {
    string image_path = p.Get<string>("path");
    Window &win = p.Get<Window>("win");
    Object ret;
#if defined(_ENABLE_DEBUGGING_)
    image_path = "C:\\workspace\\test.bmp";
#endif
    TextureBase texture_base = make_shared<Texture>();
    SDL_Surface *image = SDL_LoadBMP(image_path.c_str());
    texture_base->texture = SDL_CreateTextureFromSurface(win.render, image);
    SDL_FreeSurface(image);
    ret.Set(texture_base, kTypeIdSDLTexture, "", false);
    return Message().SetObject(ret);
  }
 

  Message SDLTestPresent(ObjectMap &p) {
    Texture &te = p.Get<Texture>("texture");
    Window &win = p.Get<Window>("win");
    SDL_RenderClear(win.render);
    SDL_RenderCopy(win.render, te.texture, NULL, NULL);
    SDL_RenderPresent(win.render);
    return Message();
  }

  Message SDLDelay(ObjectMap &p) {
    int time = stoi(p.Get<string>("time"));
    SDL_Delay(time);
    return Message();
  }

  void LoadSDLStuff() {
    using entry::AddEntry;
    using type::AddTemplate;

    /* For test only */
    AddTemplate(kTypeIdSDLWindow, ObjectPlanner(FakeCopy, ""));
    AddEntry(Entry(SDLCreateWindow, kCodeNormalParm, "width|height|title", "window"));

    AddTemplate(kTypeIdSDLTexture, ObjectPlanner(FakeCopy, ""));
    AddEntry(Entry(SDLCreateTextureFormBMP, kCodeNormalParm, "win|path", "LoadBMP"));

    AddEntry(Entry(SDLInit, kCodeNormalParm, "", "SDLInit"));
    AddEntry(Entry(SDLQuit, kCodeNormalParm, "", "SDLQuit"));
    AddEntry(Entry(SDLDelay, kCodeNormalParm, "time", "SDLDelay"));
    AddEntry(Entry(SDLTestPresent, kCodeNormalParm, "texture|win", "Present"));
  }
#endif
}
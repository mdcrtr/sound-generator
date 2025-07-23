#include "ui.h"

#include <assert.h>
#include <raylib.h>
#include <stdlib.h>

#include "microui/atlas.h"
#include "microui/microui.h"

typedef struct UiState {
  mu_Context *ctx;
  Texture2D texture;
} UiState;

static UiState state;

static int get_text_width(mu_Font font, const char *text, int len) {
  int res = 0;
  for (const char *p = text; *p && len--; p++) {
    if ((*p & 0xc0) == 0x80) {
      continue;
    }
    int chr = mu_min((unsigned char)*p, 127);
    res += atlas[ATLAS_FONT + chr].w;
  }
  return res;
}

static int get_text_height(mu_Font font) {
  return 18;
}

static void render_texture(mu_Rect rect, mu_Vec2 pos, mu_Color color) {
  Rectangle source = {rect.x, rect.y, rect.w, rect.h};
  Vector2 position = {pos.x, pos.y};
  Color *col = (Color *)&color;
  DrawTextureRec(state.texture, source, position, *col);
}

static void render_text(const char *text, mu_Vec2 pos, mu_Color color) {
  int i = 0;
  char c = 0;
  while ((c = text[i]) != 0) {
    if ((c & 0xc0) == 0x80) {
      continue;
    }
    mu_Rect source = atlas[ATLAS_FONT + c];
    render_texture(source, pos, color);
    pos.x += source.w;
    i++;
  }
}

static void mouse_input(void) {
  static const MouseButton in_btns[] = {MOUSE_BUTTON_LEFT, MOUSE_BUTTON_RIGHT,
                                        MOUSE_BUTTON_MIDDLE};
  static const int out_btns[] = {MU_MOUSE_LEFT, MU_MOUSE_RIGHT,
                                 MU_MOUSE_MIDDLE};

  mu_Context *ctx = state.ctx;
  int x = GetMouseX();
  int y = GetMouseY();

  mu_input_mousemove(ctx, x, y);
  mu_input_scroll(ctx, 0, GetMouseWheelMove() * -30);

  for (int i = 0; i < 3; i++) {
    if (IsMouseButtonPressed(in_btns[i])) {
      mu_input_mousedown(ctx, x, y, out_btns[i]);
    } else if (IsMouseButtonReleased(in_btns[i])) {
      mu_input_mouseup(ctx, x, y, out_btns[i]);
    }
  }
}

static void key_input(void) {
  static const KeyboardKey in_keys[] = {
      KEY_LEFT_SHIFT,    KEY_RIGHT_SHIFT, KEY_LEFT_CONTROL,
      KEY_RIGHT_CONTROL, KEY_LEFT_ALT,    KEY_RIGHT_ALT,
      KEY_ENTER,         KEY_KP_ENTER,    KEY_BACKSPACE};
  static const int out_keys[] = {
      MU_KEY_SHIFT, MU_KEY_SHIFT,  MU_KEY_CTRL,   MU_KEY_CTRL,     MU_KEY_ALT,
      MU_KEY_ALT,   MU_KEY_RETURN, MU_KEY_RETURN, MU_KEY_BACKSPACE};

  mu_Context *ctx = state.ctx;

  for (int i = 0; i < 9; i++) {
    if (IsKeyPressed(in_keys[i])) {
      mu_input_keydown(ctx, out_keys[i]);
    } else if (IsKeyReleased(in_keys[i])) {
      mu_input_keyup(ctx, out_keys[i]);
    }
  }
}

static void text_input(void) {
  char buf[256];
  for (int i = 0; i < 256; i++) {
    char c = GetKeyPressed();
    buf[i] = c;
    if (c == 0) break;
  }
  buf[255] = 0;
  mu_input_text(state.ctx, buf);
}

static void process_frame(void) {
  mu_Context *ctx = state.ctx;
  mu_begin(ctx);
  if (mu_begin_window(ctx, "Test Window", mu_rect(200, 20, 400, 400))) {
    mu_end_window(ctx);
  }
  mu_end(ctx);
}

void ui_init(void) {
  Color *pixels = (Color *)MemAlloc(ATLAS_WIDTH * ATLAS_HEIGHT * sizeof(Color));
  for (int i = 0; i < ATLAS_WIDTH * ATLAS_HEIGHT; i++) {
    pixels[i] = (Color){255, 255, 255, atlas_texture[i]};
  }

  Image image = {.data = pixels,
                 .width = ATLAS_WIDTH,
                 .height = ATLAS_HEIGHT,
                 .mipmaps = 1,
                 .format = PIXELFORMAT_UNCOMPRESSED_R8G8B8A8};

  state.texture = LoadTextureFromImage(image);
  MemFree(pixels);

  state.ctx = (mu_Context *)malloc(sizeof(mu_Context));

  mu_init(state.ctx);
  state.ctx->text_width = get_text_width;
  state.ctx->text_height = get_text_height;
}

void ui_free(void) {
  UnloadTexture(state.texture);
  free(state.ctx);
}

void ui_update(void) {
  mouse_input();
  key_input();
  text_input();
  process_frame();
}

void ui_draw(void) {
  BeginScissorMode(0, 0, GetScreenWidth(), GetScreenHeight());

  mu_Command *cmd = NULL;

  while (mu_next_command(state.ctx, &cmd)) {
    switch (cmd->type) {
      case MU_COMMAND_TEXT: {
        render_text(cmd->text.str, cmd->text.pos, cmd->text.color);
        break;
      }
      case MU_COMMAND_RECT: {
        mu_Rect rect = cmd->rect.rect;
        Color *col = (Color *)&cmd->rect.color;
        DrawRectangle(rect.x, rect.y, rect.w, rect.h, *col);
        break;
      }
      case MU_COMMAND_ICON: {
        mu_Rect source = atlas[cmd->icon.id];
        mu_Vec2 pos = {cmd->icon.rect.x, cmd->icon.rect.y};
        render_texture(source, pos, cmd->icon.color);
        break;
      }
      case MU_COMMAND_CLIP: {
        mu_Rect rect = cmd->rect.rect;
        EndScissorMode();
        BeginScissorMode(rect.x, rect.y, rect.w, rect.h);
        break;
      }
    }
  }

  EndScissorMode();
}

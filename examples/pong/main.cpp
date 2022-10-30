#include <fmt/core.h>

#include "abcgOpenGL.hpp"
#include "window.hpp"

int main(int argc, char **argv) {
  try {
    abcg::Application app(argc, argv);

    Window window;
    window.setOpenGLSettings({.samples = 4});
    window.setWindowSettings({.width = 1280,
                               .height = 720,
                               .showFPS = false,
                               .showFullscreenButton = false,
                               .title = "PONG!"});
    app.run(window);
  } catch (const abcg::Exception &exception) {
    fmt::print(stderr, "{}\n", exception.what());
    return -1;
  }
  return 0;
}
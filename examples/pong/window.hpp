#ifndef OPENGLWINDOW_HPP_
#define OPENGLWINDOW_HPP_

#include <imgui.h>

#include <random>

#include "abcgOpenGL.hpp"
#include "raquete.hpp"
#include "raquete_player2.hpp"
#include "bola.hpp"

class Window : public abcg::OpenGLWindow {
 protected:
  void onEvent(SDL_Event const &event) override;
  void onCreate() override;
  //void onUpdate() override;
  void onPaint() override;
  void onPaintUI() override;
  void onResize(glm::ivec2 const &size) override;
  void onDestroy() override;

 private:
  GLuint m_raqueteProgram{};
  GLuint m_raqueteProgram2{};
  GLuint m_bolaProgram{};

  glm::ivec2 m_viewportSize{};

  GameData m_gameData;

  RaqueteP1 m_raquete;
  RaqueteP2 m_raquete2;
  Bola m_bola;
  int directionX = 1; //-1 esquerda 1 direita
  int directionY = 0;
  int player1Score = 0;
  int player2Score = 0;

  abcg::Timer m_restartWaitTimer;

  ImFont* m_font{};

  std::default_random_engine m_randomEngine;

  void checkCollisions();
  void checkWinCondition();

  void restart();
  void update();
};

#endif
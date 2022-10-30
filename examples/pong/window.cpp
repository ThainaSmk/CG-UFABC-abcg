#include "window.hpp"

#include <iostream>
#include "glm/ext.hpp"
#include <cstdio>
using namespace std;

#include <imgui.h>

#include "abcgOpenGL.hpp"

void Window::onEvent(SDL_Event const &event) {

  if (event.type == SDL_KEYDOWN) {
    if (event.key.keysym.sym == SDLK_w && m_raquete.m_translation.y < 0.8f){      
      m_gameData.m_input.set(static_cast<size_t>(Input::Up));
    }
    if (event.key.keysym.sym == SDLK_s && m_raquete.m_translation.y > -0.8f){
      m_gameData.m_input.set(static_cast<size_t>(Input::Down));
    }
    if (event.key.keysym.sym == SDLK_UP && m_raquete2.m_translation.y < 0.8f){
      m_gameData.m_input.set(static_cast<size_t>(Input::UpR));
    }
    if (event.key.keysym.sym == SDLK_DOWN && m_raquete2.m_translation.y > -0.8f){
      m_gameData.m_input.set(static_cast<size_t>(Input::DownR));
    }
  }

  if (event.type == SDL_KEYUP) {
    if (event.key.keysym.sym == SDLK_w)
      m_gameData.m_input.reset(static_cast<size_t>(Input::Up));
    if (event.key.keysym.sym == SDLK_s)
      m_gameData.m_input.reset(static_cast<size_t>(Input::Down));
    if (event.key.keysym.sym == SDLK_UP)
      m_gameData.m_input.reset(static_cast<size_t>(Input::UpR));
    if (event.key.keysym.sym == SDLK_DOWN)
      m_gameData.m_input.reset(static_cast<size_t>(Input::DownR));
  }
}

void Window::onCreate() {
  auto const assetsPath{abcg::Application::getAssetsPath()};

  // Load a new font
  auto const filename{assetsPath + "Inconsolata-Medium.ttf"};
  m_font = ImGui::GetIO().Fonts->AddFontFromFileTTF(filename.c_str(), 60.0f);
  if (m_font == nullptr) {
    throw abcg::RuntimeError("Cannot load font file");
  }


  m_raqueteProgram = abcg::createOpenGLProgram({{.source = assetsPath + "raquete.vert",
                                  .stage = abcg::ShaderStage::Vertex},
                                  {.source = assetsPath + "raquete.frag",
                                  .stage = abcg::ShaderStage::Fragment}});
                                         
  m_raqueteProgram2 = abcg::createOpenGLProgram({{.source = assetsPath + "raquete.vert",
                                  .stage = abcg::ShaderStage::Vertex},
                                  {.source = assetsPath + "raquete.frag",
                                  .stage = abcg::ShaderStage::Fragment}});
                                         
  m_bolaProgram = abcg::createOpenGLProgram({{.source = assetsPath + "raquete.vert",
                                  .stage = abcg::ShaderStage::Vertex},
                                  {.source = assetsPath + "raquete.frag",
                                  .stage = abcg::ShaderStage::Fragment}});
  //Cor do background                                       
  abcg::glClearColor(0, 0, 0, 1);

#if !defined(__EMSCRIPTEN__)
  abcg::glEnable(GL_PROGRAM_POINT_SIZE);
#endif

  restart();
}

void Window::restart() {
  m_gameData.m_state = State::Playing;

  m_raquete.create(m_raqueteProgram);
  m_raquete2.create(m_raqueteProgram2);
  m_raquete2.m_translation = glm::vec2(0.8f, 0);
  directionX = -1;
  directionY = 0;

  m_bola.create(m_bolaProgram);
}

void Window::update() {

  float deltaTime{static_cast<float>(getDeltaTime())};

  if (m_gameData.m_state == State::RoxoWin &&
      m_restartWaitTimer.elapsed() > 5) {
    restart();
    return;
  }
  else if (m_gameData.m_state == State::RosaWin &&
      m_restartWaitTimer.elapsed() > 5) {
    restart();
    return;
  }
  else if (m_gameData.m_state == State::Score &&
    m_restartWaitTimer.elapsed() > 2) {
    restart();
    return;
  }
  
  if (m_gameData.m_state == State::Playing) {
    checkCollisions();
    checkWinCondition();
  }

  if (m_gameData.m_input[static_cast<size_t>(Input::Up)] && m_raquete.m_translation.y < 0.8f){
    m_raquete.setTranslation(glm::vec2(m_raquete.m_translation.x, m_raquete.m_translation.y + (0.8 * deltaTime)));
  }
  if (m_gameData.m_input[static_cast<size_t>(Input::Down)] && m_raquete.m_translation.y > -0.8f){
    m_raquete.setTranslation(glm::vec2(m_raquete.m_translation.x, m_raquete.m_translation.y - (0.8 * deltaTime)));
  }
  if (m_gameData.m_input[static_cast<size_t>(Input::UpR)] && m_raquete2.m_translation.y < 0.8f){
    m_raquete2.setTranslation(glm::vec2(m_raquete2.m_translation.x, m_raquete2.m_translation.y + (0.8 * deltaTime)));
  }
  if (m_gameData.m_input[static_cast<size_t>(Input::DownR)] && m_raquete2.m_translation.y > -0.8f){
    m_raquete2.setTranslation(glm::vec2(m_raquete2.m_translation.x, m_raquete2.m_translation.y - (0.8 * deltaTime)));
  }

  m_bola.update(deltaTime, directionX, directionY);
  
}

void Window::onPaint() {
  update();

  abcg::glClear(GL_COLOR_BUFFER_BIT);
  abcg::glViewport(0, 0, m_viewportSize.x, m_viewportSize.y);

  m_raquete.paint(m_gameData);
  m_raquete2.paint(m_gameData);
  m_bola.paint(m_gameData);
}

void Window::onPaintUI() {
  //abcg::Window::onPaintUI();

  {
    auto size{ImVec2(300, 300)};
    auto position{ImVec2((m_viewportSize.x - size.x/2.0f) / 2.0f,
                         (m_viewportSize.y - size.y/2.0f) / 2.0f)};
    ImGui::SetNextWindowPos(position);
    ImGui::SetNextWindowSize(size);
    ImGuiWindowFlags flags{ImGuiWindowFlags_NoBackground |
                           ImGuiWindowFlags_NoTitleBar |
                           ImGuiWindowFlags_NoInputs};
    ImGui::Begin(" ", nullptr, flags);
    ImGui::PushFont(m_font);

    if (m_gameData.m_state == State::Score) {
      ImGui::Text("%d x %d", player1Score, player2Score);
    } 
    if (m_gameData.m_state == State::RoxoWin) {
       ImGui::Text("Rosa");
       ImGui::Text("Ganhou!");
    }
    if (m_gameData.m_state == State::RosaWin) {
       ImGui::Text("Roxo");
       ImGui::Text("Ganhou!");
    }

    ImGui::PopFont();
    ImGui::End();

  }
}

void Window::onResize(glm::ivec2 const &size) {
  m_viewportSize = size;

  abcg::glClear(GL_COLOR_BUFFER_BIT);
}

void Window::onDestroy() {
  abcg::glDeleteProgram(m_raqueteProgram);
  abcg::glDeleteProgram(m_raqueteProgram2);
  abcg::glDeleteProgram(m_bolaProgram);

  
  m_raquete.destroy();
  m_raquete2.destroy();
  m_bola.destroy();
}

void Window::checkCollisions() {

  glm::vec4 bolaRoxa{0.54, 0, 0.76, 1};
  glm::vec4 bolaRosa{0.93, 0.15, 0.46, 1};

  float distanciaRaquete1 = m_raquete.m_translation.x + (m_bola.m_translation.x)*-1;
  float distanciaRaquete2 = m_raquete2.m_translation.x - (m_bola.m_translation.x);

  //colisão raquete 1
  if(distanciaRaquete1 >= -0.04f){

    if(m_bola.m_color == bolaRoxa && m_raquete.m_translation.y + 0.2f >= m_bola.m_translation.y && (m_raquete.m_translation.y - 0.2f < m_bola.m_translation.y)){
      if(m_raquete.m_translation.y == m_bola.m_translation.y){

        directionY = 0;
        directionX = directionX * -1;
      }
      else if(m_raquete.m_translation.y > m_bola.m_translation.y){

        directionY = -1;
        directionX = directionX * -1;
      }
      else if(m_raquete.m_translation.y < m_bola.m_translation.y){

        directionY = 1;
        directionX = directionX * -1;
      }
    }
  }
  //colisão raquete 2
  else if(distanciaRaquete2 <= 0.04f){

    if(m_bola.m_color == bolaRosa && m_raquete2.m_translation.y + 0.2f >= m_bola.m_translation.y && (m_raquete2.m_translation.y - 0.2f <= m_bola.m_translation.y)){
      if(m_raquete2.m_translation.y == m_bola.m_translation.y){

        directionY = 0;
        directionX = directionX * -1;
      }
      else if(m_raquete2.m_translation.y > m_bola.m_translation.y){

        directionY = -1;
        directionX = directionX * -1;
      }
      else if(m_raquete2.m_translation.y < m_bola.m_translation.y){

        directionY = 1;
        directionX = directionX * -1;
      }
    }
  }

  //colisão com eixo y
  else if(m_bola.m_translation.y <= -1.0f){
    directionY = 1;
  }
  else if(m_bola.m_translation.y >= 1.0f){
    directionY = -1;
  }

}

void Window::checkWinCondition() {

  if(m_bola.m_translation.x >= 0.9f){
    player1Score = player1Score + 1;
    directionX = 0;
    if(player1Score == 10){
      m_gameData.m_state = State::RoxoWin;
      player1Score = 0;
      player2Score = 0;
    }
    else{
      m_gameData.m_state = State::Score;
    }
    m_restartWaitTimer.restart();
  }
  else if(m_bola.m_translation.x <= -0.9f){
    player2Score = player2Score + 1;
    directionX = 0;
    if(player2Score == 10){
      m_gameData.m_state = State::RosaWin;
      player1Score = 0;
      player2Score = 0;
    }
    else{
      m_gameData.m_state = State::Score;
    }
    m_restartWaitTimer.restart();
  }

}
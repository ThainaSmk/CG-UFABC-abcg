#ifndef RAQUETEP2_HPP_
#define RAQUETEP2_HPP_

#include "abcgOpenGL.hpp"
#include "gamedata.hpp"

//class Bola;
class Window;

class RaqueteP2{
 public:
  void create(GLuint program);
  void paint(const GameData &gameData);
  void destroy();

  void setRotation(float rotation) { m_rotation = rotation; }
  void setTranslation(glm::vec2 translation) { m_translation = translation; }


 private:
  friend Window;

  GLuint m_program{};
  GLint m_translationLoc{};
  GLint m_colorLoc{};
  GLint m_scaleLoc{};
  GLint m_rotationLoc{};

  GLuint m_vao{};
  GLuint m_vbo{};
  GLuint m_ebo{};

  glm::vec4 m_color{0.54, 0, 0.76, 1};
  float m_rotation{};
  
  float m_scale{1.0f};
  glm::vec2 m_translation{glm::vec2(0)};

  glm::vec2 m_velocity{glm::vec2(0)};
  glm::vec2 m_position{glm::vec2(0)};

  abcg::Timer m_raqueteMoveTimer;
};

#endif
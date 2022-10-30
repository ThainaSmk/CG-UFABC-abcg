#include "bola.hpp"

#include <glm/gtx/fast_trigonometry.hpp>
#include <glm/gtx/rotate_vector.hpp>

#include <iostream>
using namespace std;

void Bola::create(GLuint program) {
  destroy();

  m_program = program;
  m_colorLoc = abcg::glGetUniformLocation(m_program, "color");
  m_rotationLoc = abcg::glGetUniformLocation(m_program, "rotation");
  m_scaleLoc = abcg::glGetUniformLocation(m_program, "scale");
  m_translationLoc = abcg::glGetUniformLocation(m_program, "translation");

  m_rotation = 0.0f;
  m_translation = glm::vec2(0, 0);
  m_velocity = glm::vec2(0);


  // clang-format off
  std::array<glm::vec2, 4> positions{
      // Bola
      glm::vec2{-01.5f, +01.5}, glm::vec2{-01.5f, -01.5f},
      glm::vec2{+01.5f, -01.5f}, glm::vec2{+01.5f, +01.5f},
    };

  // Normalize
  for (auto &position :positions) {
    position /= glm::vec2{15.0f, 15.0f};
  }

  std::array indices{0, 1, 2,
                     0, 2, 3};
  // clang-format on

  // Generate VBO
  abcg::glGenBuffers(1, &m_vbo);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
  abcg::glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions.data(),
               GL_STATIC_DRAW);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  // Generate EBO
  abcg::glGenBuffers(1, &m_ebo);
  abcg::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
  abcg::glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices.data(),
               GL_STATIC_DRAW);
  abcg::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  // Get location of attributes in the program
  GLint positionAttribute{glGetAttribLocation(m_program, "inPosition")};
  

  // Create VAO
  abcg::glGenVertexArrays(1, &m_vao);

  // Bind vertex attributes to current VAO
  abcg::glBindVertexArray(m_vao);

  abcg::glEnableVertexAttribArray(positionAttribute);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
  abcg::glVertexAttribPointer(positionAttribute, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  abcg::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);

  // End of binding to current VAO
  abcg::glBindVertexArray(0);
}

void Bola::paint(const GameData &gameData) {
  if (gameData.m_state != State::Playing) return;

  abcg::glUseProgram(m_program);

  abcg::glBindVertexArray(m_vao);

  abcg::glUniform1f(m_scaleLoc, m_scale);
  abcg::glUniform1f(m_rotationLoc, m_rotation);
  abcg::glUniform2fv(m_translationLoc, 1, &m_translation.x);

  abcg::glUniform4fv(m_colorLoc, 1, &m_color.r);
  abcg::glDrawElements(GL_TRIANGLES, 2 * 3, GL_UNSIGNED_INT, nullptr);

  abcg::glBindVertexArray(0);

  abcg::glUseProgram(0);
}

void Bola::destroy() {
  abcg::glDeleteBuffers(1, &m_vbo);
  abcg::glDeleteBuffers(1, &m_ebo);
  abcg::glDeleteVertexArrays(1, &m_vao);
}

void Bola::update(float deltaTime, int &directionX, int &directionY) {
    
    if(directionX == 1){
      setTranslation(glm::vec2(m_translation.x + (deltaTime * (1.2f)), (m_translation.y + (deltaTime * (1.2f) * directionY))));
      setCorBola(glm::vec4(0.93, 0.15, 0.46, 1));
    }
    else if(directionX == -1){
      setTranslation(glm::vec2(m_translation.x - (deltaTime * (1.2f)), (m_translation.y + (deltaTime * (1.2f) * directionY))));
      setCorBola(glm::vec4(0.54, 0, 0.76, 1));
    }
}

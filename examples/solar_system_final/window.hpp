#ifndef WINDOW_HPP_
#define WINDOW_HPP_

#include "abcgOpenGL.hpp"

#include "camera.hpp"
#include "model.hpp"

class Window : public abcg::OpenGLWindow {
protected:
  void onEvent(SDL_Event const &event) override;
  void onCreate() override;
  void onPaint() override;
  void onPaintUI() override;
  void onResize(glm::ivec2 const &size) override;
  void onDestroy() override;
  void onUpdate() override;

private:
  glm::ivec2 m_viewportSize{};

  GLuint m_VAO{};
  GLuint m_VBO{};
  GLuint m_EBO{};
  GLuint m_program{};
  GLuint m_skyProgram{};

  GLint m_viewMatrixLocation{};
  GLint m_projMatrixLocation{};
  GLint m_modelMatrixLocation{};
  GLint m_colorLocation{};
  GLint m_normalMatrixLocation{};

  Model m_skyModel;

  //Modelos dos planetas
  Model m_solModel;
  Model m_mercurioModel;
  Model m_venusModel;
  Model m_terraModel;
  Model m_luaModel;
  Model m_marteModel;
  Model m_jupiterModel;
  Model m_saturnoModel;
  Model m_aneisSaturnoModel; //Modelo do anel de saturno para usar outra textura
  Model m_uranoModel;
  Model m_netunoModel;

  // Sistema m_sistema;

  bool m_screenFocus{false};
  bool m_isFlashlightOn{true};
  bool m_gameOver{false};

// Mapping mode
  // 0: triplanar; 1: cylindrical; 2: spherical; 3: from mesh
  int m_mappingMode{};

// Light properties
  glm::vec4 m_Ia{1.0f};
  glm::vec4 m_Id{1.0f};
  glm::vec4 m_Is{1.0f};
  float m_lightCutOff{0.98f};
  float m_lightOuterCutOff{0.92f};
  float m_lightOff{2.00f};
  
// Material properties
  glm::vec4 m_Ka;
  glm::vec4 m_Kd;
  glm::vec4 m_Ks;
  float m_shininess{};

  Camera m_camera;
  float m_dollySpeed{};
  float m_truckSpeed{};
  float m_panSpeed{};

  std::vector<Vertex> m_vertices;
  std::vector<GLuint> m_indices;

  void renderSistema();
  void renderSkybox();
  void update();
  void initializeModels();
  void initializeGameObjects();
  void loadModel(Model m_model, std::string_view path);
};

#endif
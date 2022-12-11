#include "window.hpp"

#include <unordered_map>

// Explicit specialization of std::hash for Vertex
template <> struct std::hash<Vertex> {
  size_t operator()(Vertex const &vertex) const noexcept {
    auto const h1{std::hash<glm::vec3>()(vertex.position)};
    return h1;
  }
};

void Window::onEvent(SDL_Event const &event) {
  if (event.type == SDL_KEYDOWN) {
    if (event.key.keysym.sym == SDLK_UP || event.key.keysym.sym == SDLK_w)
      m_dollySpeed = 9.0f;
    if (event.key.keysym.sym == SDLK_DOWN || event.key.keysym.sym == SDLK_s)
      m_dollySpeed = -9.0f;
    if (event.key.keysym.sym == SDLK_LEFT || event.key.keysym.sym == SDLK_a)
      m_panSpeed = -4.0f;
    if (event.key.keysym.sym == SDLK_RIGHT || event.key.keysym.sym == SDLK_d)
      m_panSpeed = 4.0f;
    if (event.key.keysym.sym == SDLK_q)
      m_truckSpeed = -9.0f;
    if (event.key.keysym.sym == SDLK_e)
      m_truckSpeed = 9.0f;
    if (event.key.keysym.sym == SDLK_r) {
      m_camera.resetCameraPosition();
    }
  }
  if (event.type == SDL_KEYUP) {
    if ((event.key.keysym.sym == SDLK_UP || event.key.keysym.sym == SDLK_w) &&
        m_dollySpeed > 0)
      m_dollySpeed = 0.0f;
    if ((event.key.keysym.sym == SDLK_DOWN || event.key.keysym.sym == SDLK_s) &&
        m_dollySpeed < 0)
      m_dollySpeed = 0.0f;
    if ((event.key.keysym.sym == SDLK_LEFT || event.key.keysym.sym == SDLK_a) &&
        m_panSpeed < 0)
      m_panSpeed = 0.0f;
    if ((event.key.keysym.sym == SDLK_RIGHT ||
         event.key.keysym.sym == SDLK_d) &&
        m_panSpeed > 0)
      m_panSpeed = 0.0f;
    if (event.key.keysym.sym == SDLK_q && m_truckSpeed < 0)
      m_truckSpeed = 0.0f;
    if (event.key.keysym.sym == SDLK_e && m_truckSpeed > 0)
      m_truckSpeed = 0.0f;
  }
}

void Window::onCreate(){
  initializeModels();
  initializeGameObjects();
}

void Window::initializeModels() {
  auto const assetsPath{abcg::Application::getAssetsPath()};
  abcg::glClearColor(0, 0, 0, 1);

  // Enable depth buffering
  abcg::glEnable(GL_DEPTH_TEST);

  // Create programs
  m_program = abcg::createOpenGLProgram({{.source = assetsPath + "shaders/normalmapping.vert",
                                          .stage = abcg::ShaderStage::Vertex},
                                        {.source = assetsPath + "shaders/normalmapping.frag",
                                          .stage = abcg::ShaderStage::Fragment}});

  m_skyProgram = abcg::createOpenGLProgram({{.source = assetsPath + "shaders/skybox.vert",
                                          .stage = abcg::ShaderStage::Vertex},
                                        {.source = assetsPath + "shaders/skybox.frag",
                                          .stage = abcg::ShaderStage::Fragment}});


  //################################################################ Load models ################################################################
  //Sol
  m_solModel.loadObj(assetsPath + "models/sun.obj", false);
  m_solModel.setupVAO(m_program);

  //Mercúrio
  m_mercurioModel.loadObj(assetsPath + "models/mercury.obj", false);
  m_mercurioModel.setupVAO(m_program);

  //Vênus
  m_venusModel.loadObj(assetsPath + "models/venus.obj", false);
  m_venusModel.setupVAO(m_program);

  //Terra
  m_terraModel.loadObj(assetsPath + "models/earth.obj", false);
  m_terraModel.setupVAO(m_program);
  
  //Lua
  m_luaModel.loadObj(assetsPath + "models/moon.obj", false);
  m_luaModel.setupVAO(m_program);

  //Marte
  m_marteModel.loadObj(assetsPath + "models/mars.obj", false);
  m_marteModel.setupVAO(m_program);

  //Júpiter
  m_jupiterModel.loadObj(assetsPath + "models/jupiter.obj", false);
  m_jupiterModel.setupVAO(m_program);

  //Saturno
  m_saturnoModel.loadObj(assetsPath + "models/saturn.obj", false);
  m_saturnoModel.setupVAO(m_program);

  //Aneis de Saturno
  m_aneisSaturnoModel.loadObj(assetsPath + "models/saturnRing.obj", false);
  m_aneisSaturnoModel.setupVAO(m_program);

  //Urano
  m_uranoModel.loadObj(assetsPath + "models/uranus.obj", false);
  m_uranoModel.setupVAO(m_program);

  //Netuno
  m_netunoModel.loadObj(assetsPath + "models/neptune.obj", false);
  m_netunoModel.setupVAO(m_program);


  //###############################################################################################################################

  // Load cubemap
  m_skyModel.loadObj(assetsPath + "models/skybox.obj", false);
  m_skyModel.loadCubeTexture(assetsPath + "maps/cube/");
  m_skyModel.setupVAO(m_skyProgram);

  // Use material properties from the loaded model (they are the same)
  m_Ka = m_terraModel.getKa();
  m_Kd = m_terraModel.getKd();
  m_Ks = m_terraModel.getKs();
  m_shininess = m_terraModel.getShininess();
  m_mappingMode = 3;  // "From mesh" option
}

void Window::initializeGameObjects() {
  auto const assetsPath{abcg::Application::getAssetsPath()};
  m_camera.initializeCamera();
}

void Window::onPaint() {

  update();

  // Clear color buffer and depth buffer
  abcg::glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  abcg::glViewport(0, 0, m_viewportSize.x, m_viewportSize.y);

  renderSistema();
}

void Window::onPaintUI() { abcg::OpenGLWindow::onPaintUI(); }

void Window::onResize(glm::ivec2 const &size) {
  m_viewportSize = size;
  m_camera.computeProjectionMatrix(size);
}

void Window::renderSistema() {
  abcg::glUseProgram(m_program);

  // Get location of uniform variables (could be precomputed)
  m_modelMatrixLocation = abcg::glGetUniformLocation(m_program, "modelMatrix");
  m_viewMatrixLocation = abcg::glGetUniformLocation(m_program, "viewMatrix");
  m_projMatrixLocation = abcg::glGetUniformLocation(m_program, "projMatrix");
  m_normalMatrixLocation = abcg::glGetUniformLocation(m_program, "normalMatrix");
  
  GLint lightDirLoc{abcg::glGetUniformLocation(m_program, "lightDirWorldSpace")};
  GLint lightPosLoc{abcg::glGetUniformLocation(m_program, "lightPosWorldSpace")};
  GLint lightCutOffLoc{abcg::glGetUniformLocation(m_program, "lightCutOff")};
  GLint lightOuterCutOffLoc{abcg::glGetUniformLocation(m_program, "lightOuterCutOff")};

  GLint IaLoc{abcg::glGetUniformLocation(m_program, "Ia")};
  GLint IdLoc{abcg::glGetUniformLocation(m_program, "Id")};
  GLint IsLoc{abcg::glGetUniformLocation(m_program, "Is")};
  GLint KaLoc{abcg::glGetUniformLocation(m_program, "Ka")};
  GLint KdLoc{abcg::glGetUniformLocation(m_program, "Kd")};
  GLint KsLoc{abcg::glGetUniformLocation(m_program, "Ks")};
  GLint shininessLoc{abcg::glGetUniformLocation(m_program, "shininess")};

  GLint diffuseTexLoc{abcg::glGetUniformLocation(m_program, "diffuseTex")};
  GLint normalTexLoc{abcg::glGetUniformLocation(m_program, "normalTex")};
  GLint mappingModeLoc{abcg::glGetUniformLocation(m_program, "mappingMode")};

  // Set uniform variables that will be used for every scene object
  abcg::glUniformMatrix4fv(m_viewMatrixLocation, 1, GL_FALSE, &m_camera.m_viewMatrix[0][0]);
  abcg::glUniformMatrix4fv(m_projMatrixLocation, 1, GL_FALSE, &m_camera.m_projMatrix[0][0]);
  abcg::glUniform1i(diffuseTexLoc, 0);
  abcg::glUniform1i(normalTexLoc, 1);
  abcg::glUniform1i(mappingModeLoc, m_mappingMode);

  glm::vec4 lightDir(m_camera.m_at - m_camera.m_eye, 0.0f);
  glm::vec4 lightPos(m_camera.m_eye, 1.0f);
  abcg::glUniform4fv(lightDirLoc, 1, &lightDir.x);
  abcg::glUniform4fv(lightPosLoc, 1, &lightPos.x);
  abcg::glUniform1f(lightCutOffLoc, m_isFlashlightOn ? m_lightCutOff : m_lightOff);
  abcg::glUniform1f(lightOuterCutOffLoc, m_isFlashlightOn ? m_lightOuterCutOff : m_lightOff);
  
  abcg::glUniform4fv(IaLoc, 1, &m_Ia.x);
  abcg::glUniform4fv(IdLoc, 1, &m_Id.x);
  abcg::glUniform4fv(IsLoc, 1, &m_Is.x);
  abcg::glUniform4fv(KaLoc, 1, &m_Ka.x);
  abcg::glUniform4fv(KdLoc, 1, &m_Kd.x);
  abcg::glUniform4fv(KsLoc, 1, &m_Ks.x);
  abcg::glUniform1f(shininessLoc, m_shininess);

  //Draw planetas
  glm::mat4 modelMatrix{1.0f};
  modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0f, 0.0f, 0.0f));
  abcg::glUniformMatrix4fv(m_modelMatrixLocation, 1, GL_FALSE, &modelMatrix[0][0]);

  auto modelViewMatrix{glm::mat3(m_camera.m_viewMatrix * modelMatrix)};
  glm::mat3 normalMatrix{glm::inverseTranspose(modelViewMatrix)};
  abcg::glUniformMatrix3fv(m_normalMatrixLocation, 1, GL_FALSE, &normalMatrix[0][0]);

  m_solModel.render();
  m_mercurioModel.render();
  m_venusModel.render();
  m_terraModel.render();
  m_luaModel.render();
  m_marteModel.render();
  m_jupiterModel.render();
  
  modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0f, 0.5f, 0.0f));
  modelViewMatrix = glm::mat3(m_camera.m_viewMatrix * modelMatrix);
  normalMatrix = glm::inverseTranspose(modelViewMatrix);
  abcg::glUniformMatrix4fv(m_modelMatrixLocation, 1, GL_FALSE, &modelMatrix[0][0]);
  abcg::glUniformMatrix3fv(m_normalMatrixLocation, 1, GL_FALSE, &normalMatrix[0][0]);

  m_saturnoModel.render();
  m_aneisSaturnoModel.render();

  modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0f, -0.5f, 0.0f));
  modelViewMatrix = glm::mat3(m_camera.m_viewMatrix * modelMatrix);
  normalMatrix = glm::inverseTranspose(modelViewMatrix);
  abcg::glUniformMatrix4fv(m_modelMatrixLocation, 1, GL_FALSE, &modelMatrix[0][0]);
  abcg::glUniformMatrix3fv(m_normalMatrixLocation, 1, GL_FALSE, &normalMatrix[0][0]);

  m_uranoModel.render();
  m_netunoModel.render();
  
  abcg::glUseProgram(m_program);
}

void Window::onDestroy() {
  abcg::glDeleteProgram(m_program);
  abcg::glDeleteProgram(m_skyProgram);
}

void Window::update() {
  auto const deltaTime{gsl::narrow_cast<float>(getDeltaTime())};

  // Update LookAt camera
  m_camera.dolly(m_dollySpeed * deltaTime);
  m_camera.truck(m_truckSpeed * deltaTime);
  m_camera.pan(m_panSpeed * deltaTime);
}

void Window::onUpdate() {
  auto const deltaTime{gsl::narrow_cast<float>(getDeltaTime())};

  // Update LookAt camera
  m_camera.dolly(m_dollySpeed * deltaTime);
  m_camera.truck(m_truckSpeed * deltaTime);
  m_camera.pan(m_panSpeed * deltaTime);
}
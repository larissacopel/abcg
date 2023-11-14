#include "window.hpp"

#include <unordered_map>

float m_earthRotation{0.0f};
// Explicit specialization of std::hash for Vertex
template <> struct std::hash<Vertex> {
  size_t operator()(Vertex const &vertex) const noexcept {
    auto const h1{std::hash<glm::vec3>()(vertex.position)};
    return h1;
  }
};

void Window::onEvent(SDL_Event const &event) {

  if (event.type == SDL_KEYDOWN) {
        if (event.key.keysym.sym == SDLK_a || event.key.keysym.sym == SDLK_LEFT)
            m_earthRotation -= 0.05f;
        if (event.key.keysym.sym == SDLK_d || event.key.keysym.sym == SDLK_RIGHT)
            m_earthRotation += 0.05f;
    }

  if (event.type == SDL_KEYDOWN) {
    if (event.key.keysym.sym == SDLK_UP || event.key.keysym.sym == SDLK_i)
      m_dollySpeed = 1.0f;
    if (event.key.keysym.sym == SDLK_DOWN || event.key.keysym.sym == SDLK_k)
      m_dollySpeed = -1.0f;
    if (event.key.keysym.sym == SDLK_LEFT || event.key.keysym.sym == SDLK_j)
      m_panSpeed = -1.0f;
    if (event.key.keysym.sym == SDLK_RIGHT || event.key.keysym.sym == SDLK_l)
      m_panSpeed = 1.0f;
    if (event.key.keysym.sym == SDLK_u)
      m_truckSpeed = -1.0f;
    if (event.key.keysym.sym == SDLK_o)
      m_truckSpeed = 1.0f;
  }
  if (event.type == SDL_KEYUP) {
    if ((event.key.keysym.sym == SDLK_UP || event.key.keysym.sym == SDLK_i) &&
        m_dollySpeed > 0)
      m_dollySpeed = 0.0f;
    if ((event.key.keysym.sym == SDLK_DOWN || event.key.keysym.sym == SDLK_k) &&
        m_dollySpeed < 0)
      m_dollySpeed = 0.0f;
    if ((event.key.keysym.sym == SDLK_LEFT || event.key.keysym.sym == SDLK_j) &&
        m_panSpeed < 0)
      m_panSpeed = 0.0f;
    if ((event.key.keysym.sym == SDLK_RIGHT ||
         event.key.keysym.sym == SDLK_l) &&
        m_panSpeed > 0)
      m_panSpeed = 0.0f;
    if (event.key.keysym.sym == SDLK_u && m_truckSpeed < 0)
      m_truckSpeed = 0.0f;
    if (event.key.keysym.sym == SDLK_k && m_truckSpeed > 0)
      m_truckSpeed = 0.0f;
  }
}

void Window::onCreate() {
  auto const &assetsPath{abcg::Application::getAssetsPath()};

  abcg::glClearColor(0, 0, 0, 1);

  // Enable depth buffering
  abcg::glEnable(GL_DEPTH_TEST);

  // Create program
  m_program =
      abcg::createOpenGLProgram({{.source = assetsPath + "lookat.vert",
                                  .stage = abcg::ShaderStage::Vertex},
                                 {.source = assetsPath + "lookat.frag",
                                  .stage = abcg::ShaderStage::Fragment}});

  // Get location of uniform variables
  m_viewMatrixLocation = abcg::glGetUniformLocation(m_program, "viewMatrix");
  m_projMatrixLocation = abcg::glGetUniformLocation(m_program, "projMatrix");
  m_modelMatrixLocation = abcg::glGetUniformLocation(m_program, "modelMatrix");
  m_colorLocation = abcg::glGetUniformLocation(m_program, "color");

  // Load model
  loadModelFromFile(assetsPath + "esferas.obj");

  // Generate VBO
  abcg::glGenBuffers(1, &m_VBO);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
  abcg::glBufferData(GL_ARRAY_BUFFER,
                     sizeof(m_vertices.at(0)) * m_vertices.size(),
                     m_vertices.data(), GL_STATIC_DRAW);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  // Generate EBO
  abcg::glGenBuffers(1, &m_EBO);
  abcg::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
  abcg::glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                     sizeof(m_indices.at(0)) * m_indices.size(),
                     m_indices.data(), GL_STATIC_DRAW);
  abcg::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  // Create VAO
  abcg::glGenVertexArrays(1, &m_VAO);

  // Bind vertex attributes to current VAO
  abcg::glBindVertexArray(m_VAO);

  abcg::glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
  auto const positionAttribute{
      abcg::glGetAttribLocation(m_program, "inPosition")};
  abcg::glEnableVertexAttribArray(positionAttribute);
  abcg::glVertexAttribPointer(positionAttribute, 3, GL_FLOAT, GL_FALSE,
                              sizeof(Vertex), nullptr);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  abcg::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);

  // End of binding to current VAO
  abcg::glBindVertexArray(0);
}

void Window::loadModelFromFile(std::string_view path) {
  tinyobj::ObjReader reader;

  if (!reader.ParseFromFile(path.data())) {
    if (!reader.Error().empty()) {
      throw abcg::RuntimeError(
          fmt::format("Failed to load model {} ({})", path, reader.Error()));
    }
    throw abcg::RuntimeError(fmt::format("Failed to load model {}", path));
  }

  if (!reader.Warning().empty()) {
    fmt::print("Warning: {}\n", reader.Warning());
  }

  auto const &attributes{reader.GetAttrib()};
  auto const &shapes{reader.GetShapes()};

  m_vertices.clear();
  m_indices.clear();

  // A key:value map with key=Vertex and value=index
  std::unordered_map<Vertex, GLuint> hash{};

  // Loop over shapes
  for (auto const &shape : shapes) {
    // Loop over indices
    for (auto const offset : iter::range(shape.mesh.indices.size())) {
      // Access to vertex
      auto const index{shape.mesh.indices.at(offset)};

      // Vertex position
      auto const startIndex{3 * index.vertex_index};
      auto const vx{attributes.vertices.at(startIndex + 0)};
      auto const vy{attributes.vertices.at(startIndex + 1)};
      auto const vz{attributes.vertices.at(startIndex + 2)};

      Vertex const vertex{.position = {vx, vy, vz}};

      // If map doesn't contain this vertex
      if (!hash.contains(vertex)) {
        // Add this index (size of m_vertices)
        hash[vertex] = m_vertices.size();
        // Add this vertex
        m_vertices.push_back(vertex);
      }

      m_indices.push_back(hash[vertex]);
    }
  }
}

void Window::onPaint() {
  // Clear color buffer and depth buffer
  abcg::glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  abcg::glViewport(0, 0, m_viewportSize.x, m_viewportSize.y);

  abcg::glUseProgram(m_program);

  // Set uniform variables for viewMatrix and projMatrix
  // These matrices are used for every scene object
  abcg::glUniformMatrix4fv(m_viewMatrixLocation, 1, GL_FALSE,
                           &m_camera.getViewMatrix()[0][0]);
  abcg::glUniformMatrix4fv(m_projMatrixLocation, 1, GL_FALSE,
                           &m_camera.getProjMatrix()[0][0]);

  abcg::glBindVertexArray(m_VAO);

  // Escala Global

  const float globalScale{0.4f};
  const float globalDist{0.2f};

  // Constantes da órbita da Terra - Base para tudo
  const float semiMajorAxis = 0.2f + globalDist;  // Semi-eixo maior
  const float semiMinorAxis = 0.2f + globalDist;  // Semi-eixo menor

  // Posição da fonte de luz
  // abcg::glUniform3f(m_lightDirLocation, 0.0f, 0.0f, 0.0f);

  glm::mat4 model{1.0f};

  // Sun
  model = glm::mat4(1.0);
  model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
  model = glm::scale(model, glm::vec3(3.0f * globalScale / 10.0f));

  abcg::glUniformMatrix4fv(m_modelMatrixLocation, 1, GL_FALSE, &model[0][0]);
  abcg::glUniform4f(m_colorLocation, 1.0f, 1.0f, 1.0f, 1.0f);
  abcg::glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT,
                       nullptr);

  // Mercúrio

  float mercuZ = semiMajorAxis * cos(m_earthRotation * (365.0f/88.0f)); // Calcula a posição x com base no ângulo
  float mercuX = semiMinorAxis * sin(m_earthRotation * (365.0f/88.0f)); // Calcula a posição y com base no ângulo

  model = glm::mat4(1.0);
  model = glm::translate(model, glm::vec3(mercuX, 0.0f, mercuZ));
  model = glm::scale(model, glm::vec3(0.016f * globalScale));

  abcg::glUniformMatrix4fv(m_modelMatrixLocation, 1, GL_FALSE, &model[0][0]);
  abcg::glUniform4f(m_colorLocation, 0.788f, 0.788f, 0.788f, 1.0f);
  abcg::glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT,
                       nullptr);

  // Vênus

  float venusZ = 2 * semiMajorAxis * cos(m_earthRotation * (365.0f/255.0f)); // Calcula a posição x com base no ângulo
  float venusX = 2 * semiMinorAxis * sin(m_earthRotation * (365.0f/225.0f)); // Calcula a posição y com base no ângulo

  model = glm::mat4(1.0);
  model = glm::translate(model, glm::vec3(venusX, 0.0f, venusZ));
  model = glm::scale(model, glm::vec3(0.02f * globalScale));

  abcg::glUniformMatrix4fv(m_modelMatrixLocation, 1, GL_FALSE, &model[0][0]);
  abcg::glUniform4f(m_colorLocation, 0.878f, 0.6f, 0.122f, 1.0f);
  abcg::glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT,
                       nullptr);

  // Terra

  float earthZ = 3 * semiMajorAxis * cos(m_earthRotation); // Calcula a posição x com base no ângulo
  float earthX = 3 * semiMinorAxis * sin(m_earthRotation); // Calcula a posição y com base no ângulo

  model = glm::mat4(1.0);
  model = glm::translate(model, glm::vec3(earthX, 0.0f, earthZ));
  model = glm::scale(model, glm::vec3(0.02f * globalScale));

  abcg::glUniformMatrix4fv(m_modelMatrixLocation, 1, GL_FALSE, &model[0][0]);
  abcg::glUniform4f(m_colorLocation, 0.122f, 0.369f, 0.878f, 1.0f);
  abcg::glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT,
                       nullptr);

  // Marte

  float marsZ = 4 * semiMajorAxis * cos(m_earthRotation * (365.0f/687.0f)); // Calcula a posição x com base no ângulo
  float marsX = 4 * semiMinorAxis * sin(m_earthRotation * (365.0f/687.0f)); // Calcula a posição y com base no ângulo


  model = glm::mat4(1.0);
  model = glm::translate(model, glm::vec3(marsX, 0.0f, marsZ));
  model = glm::scale(model, glm::vec3(0.01f * globalScale));

  abcg::glUniformMatrix4fv(m_modelMatrixLocation, 1, GL_FALSE, &model[0][0]);
  abcg::glUniform4f(m_colorLocation, 0.839f, 0.251f, 0.0f, 1.0f);
  abcg::glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT,
                       nullptr);

  // Júpiter

  float jupiterZ = 5 * semiMajorAxis * cos(m_earthRotation * (365.0f/4332.0f)); // Calcula a posição x com base no ângulo
  float jupiterX = 5 * semiMinorAxis * sin(m_earthRotation * (365.0f/4332.0f)); // Calcula a posição y com base no ângulo

  model = glm::mat4(1.0);
  model = glm::translate(model, glm::vec3(jupiterX, 0.0f, jupiterZ));
  model = glm::scale(model, glm::vec3(0.22f * globalScale));

  abcg::glUniformMatrix4fv(m_modelMatrixLocation, 1, GL_FALSE, &model[0][0]);
  abcg::glUniform4f(m_colorLocation, 0.78f, 0.616f, 0.451f, 1.0f);
  abcg::glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT,
                       nullptr);
                  
  // Saturno

  float saturnZ = 6 * semiMajorAxis * cos(m_earthRotation * (365.0f/10760.0f)); // Calcula a posição x com base no ângulo
  float saturnX = 6 * semiMinorAxis * sin(m_earthRotation * (365.0f/10760.0f)); // Calcula a posição y com base no ângulo

  model = glm::mat4(1.0);
  model = glm::translate(model, glm::vec3(saturnX, 0.0f, saturnZ));
  model = glm::scale(model, glm::vec3(0.18f * globalScale));

  abcg::glUniformMatrix4fv(m_modelMatrixLocation, 1, GL_FALSE, &model[0][0]);
  abcg::glUniform4f(m_colorLocation, 0.878f, 0.729f, 0.514f, 1.0f);
  abcg::glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT,
                       nullptr);

  // Urano

  float uranusZ = 7 * semiMajorAxis * cos(m_earthRotation * (365.0f/30681.0f)); // Calcula a posição x com base no ângulo
  float uranusX = 7 * semiMinorAxis * sin(m_earthRotation * (365.0f/30681.0f)); // Calcula a posição y com base no ângulo

  model = glm::mat4(1.0);
  model = glm::translate(model, glm::vec3(uranusX, 0.0f, uranusZ));
  model = glm::scale(model, glm::vec3(0.08f * globalScale));

  abcg::glUniformMatrix4fv(m_modelMatrixLocation, 1, GL_FALSE, &model[0][0]);
  abcg::glUniform4f(m_colorLocation, 0.435f, 0.596f, 0.89f, 1.0f);
  abcg::glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT,
                       nullptr);

  // Netuno

  float neptuZ = 8 * semiMajorAxis * cos(m_earthRotation * (365.0f/60190.0f)); // Calcula a posição x com base no ângulo
  float neptuX = 8 * semiMinorAxis * sin(m_earthRotation * (365.0f/60190.0f)); // Calcula a posição y com base no ângulo

  model = glm::mat4(1.0);
  model = glm::translate(model, glm::vec3(neptuX, 0.0f, neptuZ));
  model = glm::scale(model, glm::vec3(0.16f * globalScale));

  abcg::glUniformMatrix4fv(m_modelMatrixLocation, 1, GL_FALSE, &model[0][0]);
  abcg::glUniform4f(m_colorLocation, 1.0f, 1.0f, 1.0f, 1.0f);
  abcg::glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT,
                       nullptr);


  abcg::glBindVertexArray(0);

  // Draw ground
  // m_ground.paint();

  abcg::glUseProgram(0);
}

void Window::onPaintUI() { abcg::OpenGLWindow::onPaintUI(); }

void Window::onResize(glm::ivec2 const &size) {
  m_viewportSize = size;
  m_camera.computeProjectionMatrix(size);
}

void Window::onDestroy() {
  abcg::glDeleteProgram(m_program);
  abcg::glDeleteBuffers(1, &m_EBO);
  abcg::glDeleteBuffers(1, &m_VBO);
  abcg::glDeleteVertexArrays(1, &m_VAO);
}

void Window::onUpdate() {
  auto const deltaTime{gsl::narrow_cast<float>(getDeltaTime())};

  // if (m_input.isCursorGrabbed()) {
  //   // Calcula a diferença entre a posição atual do mouse e a anterior
  //   auto [x, y] = m_input.getMousePosition();
  //   glm::vec2 delta = glm::vec2{x, y} - m_lastMousePosition;

  //   // Atualiza a câmera baseada na diferença
  //   m_camera.mouseUpdate(delta);

  //   // Move o mouse de volta ao centro da tela
  //   m_input.setMousePosition({m_windowSize.x / 2, m_windowSize.y / 2});
  //   m_lastMousePosition = {m_windowSize.x / 2, m_windowSize.y / 2};
  // }
  
  // Update LookAt camera
  m_camera.dolly(m_dollySpeed * deltaTime);
  m_camera.truck(m_truckSpeed * deltaTime);
  m_camera.pan(m_panSpeed * deltaTime);
}
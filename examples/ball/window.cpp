#include "window.hpp"

#include <glm/gtx/fast_trigonometry.hpp>

void Window::onEvent(SDL_Event const &event) {
  // Keyboard events
  if (event.type == SDL_KEYDOWN) {
    if (event.key.keysym.sym == SDLK_LEFT || event.key.keysym.sym == SDLK_a)
      m_gameData.m_input.set(gsl::narrow<size_t>(Input::Left));
    if (event.key.keysym.sym == SDLK_RIGHT || event.key.keysym.sym == SDLK_d)
      m_gameData.m_input.set(gsl::narrow<size_t>(Input::Right));
  }
  if (event.type == SDL_KEYUP) {
    restart();
    if (event.key.keysym.sym == SDLK_LEFT || event.key.keysym.sym == SDLK_a)
      m_gameData.m_input.reset(gsl::narrow<size_t>(Input::Left));
    if (event.key.keysym.sym == SDLK_RIGHT || event.key.keysym.sym == SDLK_d)
      m_gameData.m_input.reset(gsl::narrow<size_t>(Input::Right));
  }

  // Mouse events
  
  if (event.type == SDL_MOUSEMOTION) {
    if (m_gameData.m_state == State::Playing){
      restart();
    }
    glm::ivec2 mousePosition;
    SDL_GetMouseState(&mousePosition.x, &mousePosition.y);

    glm::vec2 direction{mousePosition.x - m_viewportSize.x / 2,
                        -(mousePosition.y - m_viewportSize.y / 2)};

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

  // Create program to render the other objects
  m_objectsProgram =
      abcg::createOpenGLProgram({{.source = assetsPath + "objects.vert",
                                  .stage = abcg::ShaderStage::Vertex},
                                 {.source = assetsPath + "objects.frag",
                                  .stage = abcg::ShaderStage::Fragment}});

  abcg::glClearColor(0, 0, 0, 1);

#if !defined(__EMSCRIPTEN__)
  abcg::glEnable(GL_PROGRAM_POINT_SIZE);
#endif

  // Start pseudo-random number generator
  m_randomEngine.seed(
      std::chrono::steady_clock::now().time_since_epoch().count());

    restart();
    restartBall();
}

void Window::restart() {
  glm::ivec2 mousePosition;
  SDL_GetMouseState(&mousePosition.x, &mousePosition.y);

  m_gameData.m_state = State::Playing;

  m_base.create(m_objectsProgram, mousePosition, m_viewportSize);
}

void Window::restartBall() {
  glm::ivec2 mousePosition;
  SDL_GetMouseState(&mousePosition.x, &mousePosition.y);

  m_gameData.m_state = State::Playing;

  m_balls.create(m_objectsProgram, 1);
}

void Window::onUpdate() {
  auto const deltaTime{gsl::narrow_cast<float>(getDeltaTime())};

  // Wait 5 seconds before restarting
  if (m_gameData.m_state != State::Playing &&
      m_restartWaitTimer.elapsed() > 5) {
    restart();
    restartBall();
    return;
  }

  m_base.update(m_gameData, deltaTime);
  m_balls.update(m_base, deltaTime);

  if (m_gameData.m_state == State::Playing) {
    checkCollisionsWalls();
    // checkGameOverCondition();
  }
}

void Window::onPaint() {
  abcg::glClear(GL_COLOR_BUFFER_BIT);
  abcg::glViewport(0, 0, m_viewportSize.x, m_viewportSize.y);

  m_base.paint(m_gameData);
  m_balls.paint();
}

void Window::onPaintUI() {
  abcg::OpenGLWindow::onPaintUI();

  {
    auto const size{ImVec2(300, 85)};
    auto const position{ImVec2((m_viewportSize.x - size.x) / 4.0f,
                               (m_viewportSize.y - size.y) / 4.0f)};
    ImGui::SetNextWindowPos(position);
    ImGui::SetNextWindowSize(size);
    ImGuiWindowFlags const flags{ImGuiWindowFlags_NoBackground |
                                 ImGuiWindowFlags_NoTitleBar |
                                 ImGuiWindowFlags_NoInputs};
    ImGui::Begin(" ", nullptr, flags);
    ImGui::PushFont(m_font);

    if (m_gameData.m_state == State::GameOver) {
      ImGui::Text("Game Over!");
    } else if (m_gameData.m_state == State::Win) {
      ImGui::Text("*You Win!*");
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
  abcg::glDeleteProgram(m_objectsProgram);

  m_balls.destroy();
  m_base.destroy();
}

void Window::checkGameOverCondition() {
  for (auto const &ball : m_balls.m_balls) {
    const float ballSizeWithMargin = ball.m_scale / 3.5f;

    if (ball.m_translation.y - ballSizeWithMargin < -1.0f) {
      m_gameData.m_state = State::GameOver;
      m_restartWaitTimer.restart();
      break;
    }
  }
}

void Window::checkCollisionsWalls() {
  auto const deltaTime{gsl::narrow_cast<float>(getDeltaTime())};

  for (auto &ball : m_balls.m_balls) {
    const float v = 3.0;

    // Atualiza a posição da bola
    ball.m_translation -= m_base.m_velocity * deltaTime * v;
    ball.m_rotation = glm::wrapAngle(
        ball.m_rotation + ball.m_angularVelocity * deltaTime);
    ball.m_translation += ball.m_velocity * deltaTime * v;

    const float ballSizeWithMargin = ball.m_scale / 3.5f;

    // Wrap-around nas laterais e na parte superior
    if (ball.m_translation.x - ballSizeWithMargin < -1.0f) {
        ball.m_translation.x = -1.0f + ballSizeWithMargin; // Reposiciona para a borda esquerda
        ball.m_velocity.x = -ball.m_velocity.x; // Inverte a direção X
    }

    if (ball.m_translation.x + ballSizeWithMargin > 1.0f) {
        ball.m_translation.x = 1.0f - ballSizeWithMargin; // Reposiciona para a borda direita
        ball.m_velocity.x = -ball.m_velocity.x; // Inverte a direção X
    }

    if (ball.m_translation.y + ballSizeWithMargin > 1.0f) {
        ball.m_translation.y = 1.0f - ballSizeWithMargin; // Reposiciona para a borda superior
        ball.m_velocity.y = -ball.m_velocity.y; // Rebate na parte superior
    }

    // Desaparecer na parte inferior com uma margem
    if (ball.m_translation.y - ballSizeWithMargin < -1.0f) {
        m_gameData.m_state = State::GameOver;
        m_restartWaitTimer.restart();
        // Reseta a bola quando leva um ponto
        // ball.m_translation = glm::vec2(0.0f, 0.0f); 
    }

    // Colisão com a base (retângulo)
    if (ball.m_translation.x - ballSizeWithMargin < m_base.m_translation.x &&
        ball.m_translation.x + ballSizeWithMargin > m_base.m_translation.x &&
        ball.m_translation.y - ballSizeWithMargin < m_base.m_translation.y &&
        ball.m_translation.y + ballSizeWithMargin > m_base.m_translation.y &&
        ball.m_velocity.y < 0.0f) {
        ball.m_velocity.y = -ball.m_velocity.y;
    }
  }
}
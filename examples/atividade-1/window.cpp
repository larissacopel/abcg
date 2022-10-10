#include "window.hpp"

void Window::onCreate() {
  auto const &windowSettings{getWindowSettings()};
  fmt::print("Initial window size: {}x{}\n", windowSettings.width,
             windowSettings.height);
}

void Window::onPaint() {
  // Set the clear color
  abcg::glClearColor(m_clearColor.at(0), m_clearColor.at(1), m_clearColor.at(2),
                     m_clearColor.at(3));
  // Clear the color buffer
  abcg::glClear(GL_COLOR_BUFFER_BIT);
}

void Window::onPaintUI() {
  // Parent class will show fullscreen button and FPS meter
  abcg::OpenGLWindow::onPaintUI();

  // Our own ImGui widgets go below
  {
    // Window begin
    ImGui::Begin("Formulário");

    ImGui::PushItemWidth(ImGui::GetFontSize() * -12);

    // Static text
    ImGui::TextWrapped(
        "Esse formulário tem como objetivo realizar o cadastro de "
        "novos clientes. Todas as informações deverão ser preenchidas "
        "para prosseguir com o envio.\n");

    // Input text
    static char name[32] = "";
    ImGui::Text("\nNome:");
    ImGui::InputText("##edit", name, IM_ARRAYSIZE(name));

    // InputTextWithHint
    static char str1[128] = "";
    ImGui::Text("\nCPF (XXX.XXX.XX-XX):");
    ImGui::InputTextWithHint("", "XXX.XXX.XXX-XX", str1, IM_ARRAYSIZE(str1));

    // Combo box
    const char *item_names[] = {"Feminino", "Masculino",
                                "Prefiro não informar"};
    static int item_type = 1;
    ImGui::Text("\nGênero:");
    ImGui::Combo("", &item_type, item_names, IM_ARRAYSIZE(item_names),
                 IM_ARRAYSIZE(item_names));

    // InputInt3
    static int nascimento[3] = {1, 1, 1};
    ImGui::Text("\nNascimento:");
    ImGui::InputInt3("", nascimento);
    // Button
    ImGui::Spacing();
    if (ImGui::Button("Enviar!", ImVec2(-1, 50))) {
      m_showAnotherWindow = true;
    }

    if (m_showAnotherWindow == true) {
      ImGui::Begin("Envio", &m_showAnotherWindow);
      ImGui::Text("Enviando...");

      if (ImGui::Button("Close Me"))
        m_showAnotherWindow = false;
      ImGui::End();
    }

    // Window end
    ImGui::End();
  }
}

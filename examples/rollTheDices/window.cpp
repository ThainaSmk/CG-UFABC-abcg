#include "window.hpp"

static std::size_t currentIndex{};
std::vector comboItems{"D4", "D6", "D8", "D12", "D20", "D100"};
std::string text_valor_rolado = " ";
int qtd_dados_rolados = 1;

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
  // Get size of application's window
  auto const appWindowWidth{gsl::narrow<float>(getWindowSettings().width)};
  auto const appWindowHeight{gsl::narrow<float>(getWindowSettings().height)};

  // Parent class will show fullscreen button and FPS meter
  //abcg::OpenGLWindow::onPaintUI();

  {
    ImGui::SetNextWindowSize(ImVec2(appWindowWidth, appWindowHeight));
    ImGui::SetNextWindowPos(ImVec2(0, 0));

    auto const flags{ImGuiWindowFlags_NoResize |
                     ImGuiWindowFlags_NoCollapse};
    ImGui::Begin("Roll the dices", nullptr, flags);

    {
      ImGui::InputInt("Quantidade de dados para rolar", &qtd_dados_rolados);
    }

    if (ImGui::BeginCombo("Tipo do dado", comboItems.at(currentIndex))) {
      for (auto index{0U}; index < comboItems.size(); ++index) {
        bool const isSelected{currentIndex == index};
        if (ImGui::Selectable(comboItems.at(index), isSelected))
          currentIndex = index;

        // Set the initial focus when opening the combo (scrolling + keyboard
        // navigation focus)
        if (isSelected)
          ImGui::SetItemDefaultFocus();
      }
      ImGui::EndCombo();
    }

    // "botão de rolagem
    {
      if (ImGui::Button("Rolar!", ImVec2(-1, 50))) {
        int range_min = 1;
        int range_max = 100;
        std::string aux_valores_rolados = "";
        int soma_valores_rolados = 0;
        int valor_rolado;

        fmt::print("Entrou na rolagem: {}\n", comboItems.at(currentIndex));
        
        if (comboItems.at(currentIndex) == "D4") {
          range_max = 4;
        } else if (comboItems.at(currentIndex) == "D6") {
          range_max = 6;
        } else if (comboItems.at(currentIndex) == "D8") {
          range_max = 8;
        } else if (comboItems.at(currentIndex) == "D12") {
          range_max = 12;
        } else if (comboItems.at(currentIndex) == "D20") {
          range_max = 20;
        } else if (comboItems.at(currentIndex) == "D100") {
          range_max = 100;
        }

        if (qtd_dados_rolados > 1) {
          for (int i = 0; i < qtd_dados_rolados; i++) {
            valor_rolado = range_min + (std::rand() % (range_max - range_min + 1));
            if (aux_valores_rolados == "") {
              aux_valores_rolados = std::to_string(valor_rolado);
            } else {
              aux_valores_rolados = aux_valores_rolados + " + " + std::to_string(valor_rolado);
            }
            soma_valores_rolados = soma_valores_rolados + valor_rolado;
          }
          text_valor_rolado = "Resultado: " + aux_valores_rolados + " = " + std::to_string(soma_valores_rolados);
        } else {
          valor_rolado = range_min + (std::rand() % (range_max - range_min + 1));
          text_valor_rolado = "Resultado: " + std::to_string(valor_rolado);
        }
      }
    }

    {
            // Center text
        ImGui::SetCursorPosX(
            (appWindowWidth - ImGui::CalcTextSize(text_valor_rolado.c_str()).x) / 2);
        ImGui::Text("%s", text_valor_rolado.c_str());
        ImGui::Spacing();
    } 
  }
}


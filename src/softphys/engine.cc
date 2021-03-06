#include "softphys/engine.h"

#include <iostream>

#include "softphys/physics/physics_loader.h"
#include "softphys/model/modelset_loader.h"

namespace softphys
{
// Hidden callbacks
namespace callbacks
{
Window* ToWindow(GLFWwindow* glfw_window)
{
  return static_cast<Window*>(glfwGetWindowUserPointer(glfw_window));
}

void ResizeCallback(GLFWwindow* glfw_window, int width, int height)
{
  auto window = ToWindow(glfw_window);
  window->Resize(width, height);
}

void WindowPosCallback(GLFWwindow* glfw_window, int xpos, int ypos)
{
  auto window = ToWindow(glfw_window);
  window->Move(xpos, ypos);
}

void CursorPosCallback(GLFWwindow* glfw_window, double xpos, double ypos)
{
  auto window = ToWindow(glfw_window);
  window->MouseMove(xpos, ypos);
}

void KeyCallback(GLFWwindow* glfw_window, int key, int scancode, int action, int mods)
{
  auto window = ToWindow(glfw_window);
  window->Keyboard(key, action, mods);
}

// @window  The window that received the event.
// @button  The mouse button that was pressed or released.
// @action  One of GLFW_PRESS or GLFW_RELEASE.
// @mods  Bit field describing which modifier keys were held down.
void MouseButtonCallback(GLFWwindow* glfw_window, int button, int action, int mods)
{
  Mouse::Button mouse_button;
  switch (button)
  {
  case GLFW_MOUSE_BUTTON_LEFT:
    mouse_button = Mouse::Button::LeftButton;
    break;
  case GLFW_MOUSE_BUTTON_RIGHT:
    mouse_button = Mouse::Button::RightButton;
    break;
  case GLFW_MOUSE_BUTTON_MIDDLE:
    mouse_button = Mouse::Button::MiddleButton;
    break;
  default:
    return;
  }

  Mouse::Status mouse_status;
  switch (action)
  {
  case GLFW_PRESS:
    mouse_status = Mouse::Status::Pressed;
    break;
  case GLFW_RELEASE:
    mouse_status = Mouse::Status::Released;
    break;
  default:
    return;
  }

  auto window = ToWindow(glfw_window);
  window->MouseButton(mouse_button, mouse_status, mods);
}
}

Engine::Engine()
  : start_time_(Clock::now())
{
  glfwInit();
}

Engine::~Engine()
{
  glfwTerminate();
}

void Engine::LoadModels(const std::string& filename)
{
  model::ModelsetLoader loader;
  modelset_ = loader.LoadFromJson(filename);
}

void Engine::LoadPhysics(const std::string& filename)
{
  physics::PhysicsLoader loader(this);
  physics_ = loader.LoadFromJson(filename);
}

FontFace& Engine::LoadFont(const std::string& font)
{
  return font_faces_.Load(font);
}

bool Engine::ShouldClose()
{
  for (int i = 0; i < windows_.size(); i++)
  {
    auto window = windows_[i];

    if (window->ShouldClose())
    {
      for (int j = i; j < windows_.size() - 1; j++)
        windows_[j] = windows_[j + 1];
      windows_.pop_back();
      i--;
    }
  }

  return windows_.empty();
}

void Engine::Run()
{
  for (const auto& window : windows_)
  {
    window->MakeCurrent();
    window->Initialize();
  }

  while (!ShouldClose())
  {
    for (const auto& window : windows_)
    {
      window->MakeCurrent();
      window->Display();
      window->SwapBuffers();
    }

    glfwPollEvents();
  }
}

double Engine::GetTime()
{
  return Duration(Clock::now() - start_time_).count();
}

std::shared_ptr<model::Model> Engine::GetModelFromPhysicsObject(std::shared_ptr<physics::Object> object) const
{
  const auto& it = model_from_physics_object_.find(object);
  if (it == model_from_physics_object_.cend())
    return nullptr;
  return it->second;
}

std::shared_ptr<model::Model> Engine::GetModel(const std::string& name)
{
  return modelset_->GetModel(name);
}

void Engine::LinkPhysicsToModel(std::shared_ptr<physics::Object> physics_object, std::shared_ptr<model::Model> model)
{
  model_from_physics_object_[physics_object] = model;
}
}

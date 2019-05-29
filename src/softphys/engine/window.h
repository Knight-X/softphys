#ifndef SOFTPHYS_ENGINE_WINDOW_H_
#define SOFTPHYS_ENGINE_WINDOW_H_

#include <memory>

#include "softphys/engine/mouse.h"
#include "softphys/engine/mouse_event.h"
#include "softphys/engine/camera.h"

struct GLFWwindow;

namespace softphys
{
class Engine;

class Window
{
  friend class Engine;

public:
  Window() = delete;
  Window(Engine* engine, int x, int y, int width, int height);
  virtual ~Window();

  Window(const Window& rhs) = delete;
  Window& operator = (const Window& rhs) = delete;

  Window(Window&& rhs) = default;
  Window& operator = (Window&& rhs) = default;

  auto Width() const noexcept
  {
    return width_;
  }

  auto Height() const noexcept
  {
    return height_;
  }

  bool ShouldClose();

  virtual void Resize(int width, int height);
  virtual void Move(int x, int y);
  virtual void MouseMove(double x, double y);
  virtual void MouseButton(int button, int action, int mods);

  virtual void Initialize();
  virtual void Display();

protected:
  auto PointerToWindow() const
  {
    return window_;
  }

  auto GetEngine() const
  {
    return engine_;
  }

private:
  void MakeCurrent();
  void SwapBuffers();

  GLFWwindow* window_;
  Engine* engine_;

  int x_;
  int y_;
  int width_;
  int height_;
};
}

#endif // MODERNGL_WINDOW_H_

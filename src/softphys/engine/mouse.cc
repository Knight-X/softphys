#include "softphys/engine/mouse.h"

namespace softphys
{
Mouse::Mouse()
  : x_(0.), y_(0.),
  buttons_(static_cast<int>(Button::NumButtons), Status::Released)
{
}
}

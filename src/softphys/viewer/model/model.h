#ifndef SOFTPHYS_VIEWER_MODEL_MODEL_H_
#define SOFTPHYS_VIEWER_MODEL_MODEL_H_

#include <vector>

#include "softphys/viewer/gl_vertex_array.h"
#include "softphys/viewer/gl_buffer.h"

namespace softphys
{
namespace viewer
{
class Model
{
public:
  Model();
  virtual ~Model();

  virtual void Draw();

private:
};
}
}

#endif // SOFTPHYS_VIEWER_MODEL_MODEL_H_

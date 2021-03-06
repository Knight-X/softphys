#ifndef SOFTPHYS_PHYSICS_OBJECT_RIGID_BODY_H_
#define SOFTPHYS_PHYSICS_OBJECT_RIGID_BODY_H_

#include "softphys/physics/object/object.h"

#include <vector>

#include "softphys/data/eigen.h"

namespace softphys
{
namespace physics
{
class RigidBody : public Object
{
public:
  RigidBody();
  ~RigidBody();

  bool IsRigidBody() const noexcept override
  {
    return true;
  }

  virtual bool IsSphere() const noexcept
  {
    return false;
  }

  virtual bool IsCube() const noexcept
  {
    return false;
  }

  virtual Matrix3d MassInverse() const noexcept
  {
    return Matrix3d::Identity() / mass_;
  }

  virtual Matrix3d InertiaInverse() const noexcept
  {
    return inertia_inverse_;
  }

  virtual void ApplyForce(const Vector3d& force)
  {
    force_ += force;
  }

  virtual void ApplyTorque(const Vector3d& torque)
  {
    torque_ += torque;
  }

  void SetPosition(const Eigen::Vector3d& position)
  {
    position_ = position;
  }

  void SetOrientation(const Eigen::Quaterniond& orientation)
  {
    orientation_ = orientation;
  }

  const auto& GetCom()
  {
    return com_;
  }

  auto GetMass()
  {
    return mass_;
  }

  const auto& GetPosition()
  {
    return position_;
  }

  const auto& GetOrientation()
  {
    return orientation_;
  }

  void SetMomentum(const Vector3d& p)
  {
    momentum_ = p;
  }

  void SetVelocity(const Vector3d& v)
  {
    momentum_ = mass_ * v;
  }

  void SetAngularMomentum(const Vector3d& l)
  {
    angular_momentum_ = l;
  }

  bool IsFixed() const noexcept override
  {
    return false;
  }

  virtual void Simulate(double time) override;

  Eigen::Vector3d GetVelocity() const
  {
    return momentum_ / mass_;
  }

  Eigen::Vector3d GetAngularVelocity() const
  {
    return inertia_inverse_ * angular_momentum_;
  }

protected:
  void SetMass(double mass)
  {
    mass_ = mass;
  }

  void SetCom(const Vector3d& com)
  {
    com_ = com;
  }

  void SetInertia(const Matrix3d& inertia)
  {
    inertia_ = inertia;
    inertia_inverse_ = inertia_.inverse();
  }

  void SetDiagonalInertia(const Vector3d& d)
  {
    inertia_.setZero();
    inertia_(0, 0) = d(0);
    inertia_(1, 1) = d(1);
    inertia_(2, 2) = d(2);

    inertia_inverse_.setZero();
    inertia_inverse_(0, 0) = 1. / d(0);
    inertia_inverse_(1, 1) = 1. / d(1);
    inertia_inverse_(2, 2) = 1. / d(2);
  }

private:
  // property
  double mass_ = 0.;
  Matrix3d inertia_{ Matrix3d::Identity() };
  Vector3d com_{ 0., 0., 0. };

  // Inverse matrix cache
  Matrix3d inertia_inverse_{ Matrix3d::Identity() };

  // Status (linear)
  Vector3d position_{ 0., 0., 0. };
  Vector3d momentum_{ 0., 0., 0. };

  // Status (rotation)
  Quaterniond orientation_{ Quaterniond::Identity() };
  Vector3d angular_momentum_{ 0., 0., 0. };

  // Simulation
  Vector3d force_{ 0., 0., 0. };
  Vector3d torque_{ 0., 0., 0. };
};
}
}

#endif // SOFTPHYS_PHYSICS_OBJECT_RIGID_BODY_H_

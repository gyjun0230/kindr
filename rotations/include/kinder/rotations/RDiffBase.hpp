/*
 * Copyright (c) 2013, Christian Gehring, Hannes Sommer, Paul Furgale, Remo Diethelm
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the Autonomous Systems Lab, ETH Zurich nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL Christian Gehring, Hannes Sommer, Paul Furgale,
 * Remo Diethelm BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY,
 * OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
 * GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
*/

#ifndef KINDER_ROTATIONS_RDIFFBASE_HPP_
#define KINDER_ROTATIONS_RDIFFBASE_HPP_


#include "kinder/common/common.hpp"

namespace kinder {
namespace rotations {
//! Internal stuff (only for developers)
namespace internal {

/*! \brief Addition traits for angular velocities
 *  \class RDiffAdditionTraits
 *  (only for advanced users)
 */
template<typename LeftAndRight_>
class RDiffAdditionTraits {
 public:
//  inline static LeftAndRight_ add(const LeftAndRight_& lhs, const LeftAndRight_& rhs);
//  inline static LeftAndRight_ subtract(const LeftAndRight_& lhs, const LeftAndRight_& rhs);
};

/*! \class get_scalar
 *  \brief Gets the primitive of the angular velocity.
 */
template<typename Position_>
class get_scalar {
 public:
//  typedef PrimType Scalar;
};

} // namespace internal

/*! \class RDiffBase
 * \brief Interface for an angular velocity.
 *
 * This class defines the generic interface for an angular velocity.
 * More precisely an interface to store and access the angular velocity of a rigid body is provided.
 * \tparam Derived_ the derived class that should implement the angular velocity.
 * \ingroup rotations
 *
 */
template<typename Derived_>
class RDiffBase {
 public:
  /*! \brief Default constructor.
   *
    *  Creates a time derivative of a rotation with zero coefficients.
    */
  RDiffBase() = default;

  /*! \brief Constructor from derived time derivative of a rotation.
   *
   *  This constructor has been deleted because the abstract class does not contain any data.
   */
  RDiffBase(const Derived_&) = delete; // on purpose!!

  /*! \brief Gets the derived time derivative of a rotation.
   *  (only for advanced users)
   *  \returns the derived time derivative of a rotation
   */
  operator Derived_& () {
    return static_cast<Derived_&>(*this);
  }

  /*! \brief Gets the derived time derivative of a rotation.
   *  (only for advanced users)
   *  \returns the derived time derivative of a rotation
   */
  operator const Derived_& () const {
    return static_cast<const Derived_&>(*this);
  }

  /*! \brief Gets the derived time derivative of a rotation.
   *  (only for advanced users)
   *  \returns the derived time derivative of a rotation
   */
  const Derived_& derived() const {
    return static_cast<const Derived_&>(*this);
  }


  /*! \brief Sets the time derivative of a rotation to zero.
   *  \returns reference
   */
  Derived_& setZero();

  /*! \brief Addition of two time derivatives.
   *  \returns sum of the two time derivatives.
   */
  template<typename OtherDerived_>
  Derived_ operator +(const RDiffBase<OtherDerived_>& other) const {
    return internal::RDiffAdditionTraits<RDiffBase<Derived_>>::add(this->derived(), other.derived()); // todo: 1. ok? 2. may be optimized
  }

  /*! \brief Subtraction of two time derivatives.
   *  \returns result of the subtraction of the two time derivatives.
   */
  template<typename OtherDerived_>
  Derived_ operator -(const RDiffBase<OtherDerived_>& other) const {
    return internal::RDiffAdditionTraits<RDiffBase<Derived_>>::subtract(this->derived(), other.derived()); // todo: 1. ok? 2. may be optimized
  }

  /*! \brief Addition and assignment.
   *  \returns sum of the two time derivatives.
   */
  template<typename OtherDerived_>
  Derived_& operator +=(const RDiffBase<OtherDerived_>& other);

  /*! \brief Subtraction and assignment.
   *  \returns result of the subtraction of the two time derivatives.
   */
  template<typename OtherDerived_>
  Derived_& operator -=(const RDiffBase<OtherDerived_>& other);

};

/*! \class AngularVelocity3Base
 * \brief Interface for an angular velocity of a rigid body in 3D-space.
 *
 * This class defines the generic interface for an angular velocity in 3D-space.
 * More precisely an interface to store and access the three components of the angular velocity of a rigid body is provided.

 * \tparam Derived_ the derived class that should implement the angular velocity.
 *
 *  \ingroup rotations
 */
template<typename Derived_>
class AngularVelocity3Base : public RDiffBase<Derived_> {
 public:
  /*! \brief The primitive type of an angular velocity.
   */
  typedef typename internal::get_scalar<Derived_>::Scalar Scalar;

  /*! \returns the x-coordinate of the 3D angular velocity
   */
  inline const Scalar& x() const;

  /*! \returns the x-coordinate of the 3D angular velocity
   */
  inline Scalar& x();

  /*! \returns the y-coordinate of the 3D angular velocity
   */
  inline const Scalar& y() const;

  /*! \returns the y-coordinate of the 3D angular velocity
   */
  inline Scalar& y();

  /*! \returns the z-coordinate of the 3D angular velocity
   */
  inline Scalar& z() const;

  /*! \returns the z-coordinate of the 3D angular velocity
   */
  inline Scalar& z();

};


namespace internal {

template<typename LeftAndRight_>
class RDiffAdditionTraits<RDiffBase<LeftAndRight_>> {
 public:
  /*! \returns the sum of two angular velocities
   * \param lhs left-hand side
   * \param rhs right-hand side
   */
  inline static LeftAndRight_ add(const RDiffBase<LeftAndRight_>& lhs, const RDiffBase<LeftAndRight_>& rhs) {
    return LeftAndRight_(typename LeftAndRight_::Implementation(lhs.derived().toImplementation() + rhs.derived().toImplementation()));
  }
  /*! \returns the subtraction of two angular velocities
   * \param lhs left-hand side
   * \param rhs right-hand side
   */
  inline static LeftAndRight_ subtract(const RDiffBase<LeftAndRight_>& lhs, const RDiffBase<LeftAndRight_>& rhs) {
    return LeftAndRight_(typename LeftAndRight_::Implementation(lhs.derived().toImplementation() - rhs.derived().toImplementation()));
  }
};

} // namespace internal

} // namespace rotations
} // namespace kinder




#endif /* KINDER_ROTATIONS_RDIFFBASE_HPP_ */
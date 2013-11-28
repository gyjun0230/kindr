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

#include <iostream>

#include <gtest/gtest.h>
#include <rm/common/assert_macros_eigen.hpp>
#include <rm/quaternions/QuaternionEigen.hpp>
#include <rm/rotations/RotationEigen.hpp>

namespace quat = kinder::quaternions::eigen_implementation;
namespace rot = kinder::rotations::eigen_implementation;

TEST (RotationsTest, testDEBUG ) {
  Eigen::Vector3d test1(1.0,2.0,3.0);
  Eigen::Vector3d test2(1.0,2.0,3.0);

  quat::UnitQuaternionD(1,0,0,0);
  KINDER_ASSERT_MATRIX_NEAR_DBG(std::runtime_error, test1, test2, 1e-6, "Hello");

}



template <typename QuaternionImplementation>
struct QuaternionsTest  { //public ::testing::Test  {
  typedef typename QuaternionImplementation::Scalar Scalar;
  typedef Eigen::Matrix<Scalar, 3, 1> Vector3;

  QuaternionImplementation zero = QuaternionImplementation(quat::Quaternion<Scalar>(Eigen::Quaterniond(0, 0, 0, 0).cast<Scalar>()));
  QuaternionImplementation identity = QuaternionImplementation(quat::Quaternion<Scalar>(Eigen::Quaterniond(1, 0, 0, 0).cast<Scalar>()));
  QuaternionImplementation halfX =    QuaternionImplementation(quat::Quaternion<Scalar>(Eigen::Quaterniond(0, 1, 0, 0).cast<Scalar>()));
  QuaternionImplementation halfY =    QuaternionImplementation(quat::Quaternion<Scalar>(Eigen::Quaterniond(0, 0, 1, 0).cast<Scalar>()));
  QuaternionImplementation halfZ =    QuaternionImplementation(quat::Quaternion<Scalar>(Eigen::Quaterniond(0, 0, 0, 1).cast<Scalar>()));
  QuaternionImplementation generic =  QuaternionImplementation(quat::Quaternion<Scalar>(Eigen::Quaterniond(1, 2, 3, 4).cast<Scalar>()));
  QuaternionImplementation genericInverse =  QuaternionImplementation(quat::Quaternion<Scalar>(Eigen::Quaterniond(   0.033333333333333, -0.066666666666667, -0.1, -0.133333333333333).cast<Scalar>()));

  QuaternionsTest() : X(Vector3::UnitX()), Y(Vector3::UnitY()), Z(Vector3::UnitZ()) {}
  Vector3 X, Y, Z;
  virtual ~QuaternionsTest(){}
};


typedef ::testing::Types<
    std::pair<quat::QuaternionD, quat::UnitQuaternionD>,
	std::pair<quat::QuaternionF, quat::UnitQuaternionF>
> TypePairs;


template <typename QuaternionImplementationPair>
struct QuaternionsPairsTest : public ::testing::Test  {

  typedef typename QuaternionImplementationPair::first_type QuaternionImplementation;
  typedef typename QuaternionImplementationPair::second_type UnitQuaternionImplementation;

   QuaternionsTest<QuaternionImplementation> quat;
   QuaternionsTest<UnitQuaternionImplementation> uquat;
   QuaternionsPairsTest() {}
};

TYPED_TEST_CASE(QuaternionsPairsTest, TypePairs);

TYPED_TEST (QuaternionsPairsTest, testQuaternionConstructorAndConversion ) {

	typedef typename TestFixture::QuaternionImplementation Quaternion;
	typedef typename TestFixture::UnitQuaternionImplementation UnitQuaternion;
	typedef typename TestFixture::UnitQuaternionImplementation::Scalar UnitQuaternionScalar;
	typedef typename TestFixture::QuaternionImplementation::Scalar QuaternionScalar;

	// default constructor quaternion
	Quaternion quat;
	ASSERT_EQ(quat, TestFixture::quat.zero);
	ASSERT_EQ(quat.w(),QuaternionScalar(0));
	ASSERT_EQ(quat.x(),QuaternionScalar(0));
	ASSERT_EQ(quat.y(),QuaternionScalar(0));
	ASSERT_EQ(quat.z(),QuaternionScalar(0));

	// default constructor of unit quaternion
	UnitQuaternion uquat;
	ASSERT_EQ(uquat, TestFixture::uquat.identity);
	ASSERT_EQ(uquat.w(),UnitQuaternionScalar(1));
	ASSERT_EQ(uquat.x(),UnitQuaternionScalar(0));
	ASSERT_EQ(uquat.y(),UnitQuaternionScalar(0));
	ASSERT_EQ(uquat.z(),UnitQuaternionScalar(0));

	Quaternion quat3(UnitQuaternionScalar(10),UnitQuaternionScalar(11),UnitQuaternionScalar(12),UnitQuaternionScalar(13));
	ASSERT_EQ(quat3.w(),QuaternionScalar(10));
	ASSERT_EQ(quat3.x(),QuaternionScalar(11));
	ASSERT_EQ(quat3.y(),QuaternionScalar(12));
	ASSERT_EQ(quat3.z(),QuaternionScalar(13));

	Quaternion uquat2(UnitQuaternionScalar(10),UnitQuaternionScalar(11),UnitQuaternionScalar(12),UnitQuaternionScalar(13));
	ASSERT_EQ(uquat2.w(),UnitQuaternionScalar(10));
	ASSERT_EQ(uquat2.x(),UnitQuaternionScalar(11));
	ASSERT_EQ(uquat2.y(),UnitQuaternionScalar(12));
	ASSERT_EQ(uquat2.z(),UnitQuaternionScalar(13));

	Quaternion quat4(uquat2);
	ASSERT_EQ(quat4.w(),QuaternionScalar(10));
	ASSERT_EQ(quat4.x(),QuaternionScalar(11));
	ASSERT_EQ(quat4.y(),QuaternionScalar(12));
	ASSERT_EQ(quat4.z(),QuaternionScalar(13));

	Quaternion quat5 = uquat2;
	ASSERT_EQ(quat5.w(),QuaternionScalar(10));
	ASSERT_EQ(quat5.x(),QuaternionScalar(11));
	ASSERT_EQ(quat5.y(),QuaternionScalar(12));
	ASSERT_EQ(quat5.z(),QuaternionScalar(13));


	UnitQuaternion uquat3(Quaternion(4,16,32,64).toUnitQuaternion());
	ASSERT_NEAR(uquat3.w(),UnitQuaternionScalar(0.0544735),1e-6);
	ASSERT_NEAR(uquat3.x(),UnitQuaternionScalar(0.217894),1e-6);
	ASSERT_NEAR(uquat3.y(),UnitQuaternionScalar(0.435788),1e-6);
	ASSERT_NEAR(uquat3.z(),UnitQuaternionScalar(0.871576),1e-6);


}

TYPED_TEST (QuaternionsPairsTest, testQuaternionInversion ) {

	typedef typename TestFixture::QuaternionImplementation Quaternion;
	typedef typename TestFixture::UnitQuaternionImplementation UnitQuaternion;
	typedef typename TestFixture::UnitQuaternionImplementation::Scalar UnitQuaternionScalar;
	typedef typename TestFixture::QuaternionImplementation::Scalar QuaternionScalar;

	ASSERT_NEAR(TestFixture::quat.generic.inverted().w(), TestFixture::quat.genericInverse.w(),1e-6);
	ASSERT_NEAR(TestFixture::quat.generic.inverted().x(), TestFixture::quat.genericInverse.x(),1e-6);
	ASSERT_NEAR(TestFixture::quat.generic.inverted().y(), TestFixture::quat.genericInverse.y(),1e-6);
	ASSERT_NEAR(TestFixture::quat.generic.inverted().z(), TestFixture::quat.genericInverse.z(),1e-6);
}



TEST (RotationsTest, DISABLED_testQuaternionVarious ) {

  rot::RotationQuaternionPD rquat1(0,0,0,1);
  rot::RotationQuaternionPD rquat2(rquat1);
  rot::RotationQuaternionPD rquat3;
  rquat3 = rquat1;
  std::cout << rquat1 << std::endl;
  std::cout << rquat2 << std::endl;
  std::cout << rquat3 << std::endl;
  std::cout << rquat1.conjugated() << std::endl;
  std::cout << rquat1.inverted() << std::endl;
  std::cout << rquat1*rquat2 << std::endl;
  std::cout << (rquat1==rquat2) << std::endl;
  std::cout << rquat3.setIdentity() << std::endl;
  std::cout << std::endl;

  quat::UnitQuaternion<double> uquat1(0,1,0,0);
  quat::UnitQuaternion<double> uquat2(uquat1);
  quat::UnitQuaternion<double> uquat3;
  uquat3 = uquat1;
  std::cout << uquat1 << std::endl;
  std::cout << uquat2 << std::endl;
  std::cout << uquat3 << std::endl;
  std::cout << uquat1.conjugated() << std::endl;
  std::cout << uquat1.inverted() << std::endl;
  std::cout << uquat1*uquat2 << std::endl;
  std::cout << (uquat1==uquat2) << std::endl;
  std::cout << std::endl;

  quat::Quaternion<double> quat1(1,2,3,4);
  quat::Quaternion<double> quat2(quat1);
  quat::Quaternion<double> quat3;
  quat3 = quat1;
  std::cout << quat1 << std::endl;
  std::cout << quat2 << std::endl;
  std::cout << quat3 << std::endl;
  std::cout << quat1.conjugated() << std::endl;
  std::cout << quat1.inverted() << std::endl;
  std::cout << quat1*quat2 << std::endl;
  std::cout << (quat1==quat2) << std::endl;
  std::cout << std::endl;

  std::cout << rquat1.toUnitQuaternion()*uquat2 << std::endl;
  std::cout << rquat1.toUnitQuaternion()*quat2 << std::endl;
  std::cout << uquat1*quat2 << std::endl;
  std::cout << (rquat1.toUnitQuaternion()==uquat2) << std::endl;
  std::cout << (rquat1.toUnitQuaternion()==quat2) << std::endl;
  std::cout << (uquat1==quat2) << std::endl;
  std::cout << std::endl;

  rquat1 = rquat2; // allowed
  rquat1 = uquat2; // allowed
//rquat1 = quat2; // not allowed
  uquat1 = rquat2; // allowed
  uquat1 = uquat2; // allowed
//uquat1 = quat2; // not allowed
  quat1  = rquat2; // allowed
  quat1  = uquat2; // allowed
  quat1  = quat2; // allowed

  rquat1(rquat2); // allowed
  rquat1(uquat2); // allowed
  rquat1(quat2.normalized()); // allowed, checks length in debug mode
  uquat1(rquat2); // allowed
  uquat1(uquat2); // allowed
  uquat1(quat2.normalized()); // allowed, checks length in debug mode
  quat1(rquat2); // allowed
  quat1(uquat2); // allowed
  quat1(quat2); // allowed

  uquat1 = quat2.toUnitQuaternion();

}




TEST (RotationsTest, DISABLED_testQuaternionInverseConjugate ) {

  quat::QuaternionD q1(1,2,3,4);
  std::cout << q1 << std::endl;
  std::cout << q1.conjugated() << std::endl;
  std::cout << q1 << std::endl;
  q1.conjugate();
  std::cout << q1 << std::endl;
  std::cout << std::endl;
  std::cout << q1 << std::endl;
  std::cout << q1.inverted() << std::endl;
  std::cout << q1 << std::endl;
  q1.invert();
  std::cout << q1 << std::endl;
  std::cout << std::endl;


  quat::UnitQuaternionD uq1(q1.toUnitQuaternion());
  std::cout << uq1 << std::endl;
  std::cout << uq1.conjugated() << std::endl;
  std::cout << uq1 << std::endl;
  uq1.conjugate();
  std::cout << uq1 << std::endl;
  std::cout << std::endl;
  std::cout << uq1 << std::endl;
  std::cout << uq1.inverted() << std::endl;
  std::cout << uq1 << std::endl;
  uq1.invert();
  std::cout << uq1 << std::endl;
  std::cout << std::endl;

}





















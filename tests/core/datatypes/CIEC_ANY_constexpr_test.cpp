/*******************************************************************************
 * Copyright (c) 2026 Franz Höpfinger
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Franz Höpfinger - initial implementation
 *******************************************************************************/
#include <boost/test/unit_test.hpp>
#include <numbers>
#include "forte/datatypes/forte_bool.h"
#include "forte/datatypes/forte_int.h"
#include "forte/datatypes/forte_real.h"
#include "forte/datatypes/forte_lreal.h"
#include "forte/datatypes/forte_time.h"
#include "forte/datatypes/forte_ltime.h"

using namespace forte;

BOOST_AUTO_TEST_SUITE(CIEC_ANY_constexpr_test)

BOOST_AUTO_TEST_CASE(Constexpr_Initialization_test) {
  // Verify that these types can be initialized in a constexpr context
  constexpr forte::CIEC_BOOL b(true);
  static_assert(static_cast<bool>(b) == true, "CIEC_BOOL constexpr failed");

  constexpr forte::CIEC_INT i(42);
  static_assert(static_cast<forte::TForteInt16>(i) == 42, "CIEC_INT constexpr failed");

  constexpr forte::TForteFloat fVal = std::numbers::pi_v<float>;
  constexpr forte::CIEC_REAL r(fVal);
  static_assert(static_cast<forte::TForteFloat>(r) == fVal, "CIEC_REAL constexpr failed");

  constexpr forte::TForteDFloat dfVal = std::numbers::e;
  constexpr forte::CIEC_LREAL lr(dfVal);
  static_assert(static_cast<forte::TForteDFloat>(lr) == dfVal, "CIEC_LREAL constexpr failed");

  // Test construction from integer types
  constexpr forte::CIEC_REAL rFromInt(forte::CIEC_INT(123));
  static_assert(static_cast<forte::TForteFloat>(rFromInt) == 123.0f, "CIEC_REAL from CIEC_INT constexpr failed");

  constexpr forte::CIEC_LREAL lrFromDInt(forte::CIEC_DINT(456));
  static_assert(static_cast<forte::TForteDFloat>(lrFromDInt) == 456.0, "CIEC_LREAL from CIEC_DINT constexpr failed");
}

BOOST_AUTO_TEST_CASE(Constexpr_Default_Constructor_test) {
  constexpr forte::CIEC_BOOL b;
  static_assert(static_cast<bool>(b) == false, "CIEC_BOOL default constexpr failed");

  constexpr forte::CIEC_INT i;
  static_assert(static_cast<forte::TForteInt16>(i) == 0, "CIEC_INT default constexpr failed");
}

BOOST_AUTO_TEST_CASE(Constexpr_Arithmetic_test) {
  constexpr forte::CIEC_INT i1(10);
  constexpr forte::CIEC_INT i3 = -i1;
  static_assert(static_cast<forte::TForteInt16>(i3) == -10, "CIEC_INT unary minus constexpr failed");

  // Note: operator+ for CIEC_INT is currently NOT constexpr as it's often handled via functions or not defined as
  // operator
}

BOOST_AUTO_TEST_CASE(Constexpr_Duration_Arithmetic_test) {
  constexpr forte::CIEC_TIME t1(1000);
  constexpr forte::CIEC_TIME t2(2000);
  constexpr forte::CIEC_TIME t3 = t1 + t2;
  static_assert(static_cast<forte::TForteInt64>(t3) == 3000, "CIEC_TIME operator+ constexpr failed");

  constexpr forte::CIEC_TIME t4 = t2 - t1;
  static_assert(static_cast<forte::TForteInt64>(t4) == 1000, "CIEC_TIME operator- constexpr failed");

  static_assert(t1 < t2, "CIEC_TIME operator< constexpr failed");
  static_assert(t2 > t1, "CIEC_TIME operator> constexpr failed");
  static_assert(t1 <= t2, "CIEC_TIME operator<= constexpr failed");
  static_assert(t2 >= t1, "CIEC_TIME operator>= constexpr failed");
  static_assert(t1 == forte::CIEC_TIME(1000), "CIEC_TIME operator== constexpr failed");
  static_assert(t1 != t2, "CIEC_TIME operator!= constexpr failed");
}

BOOST_AUTO_TEST_CASE(Constexpr_Equals_test) {
  constexpr forte::CIEC_INT i1(42);
  constexpr forte::CIEC_INT i2(42);
  constexpr forte::CIEC_INT i3(43);

  // same-type equality
  static_assert(i1.equals(i2) == true, "CIEC_ANY::equals constexpr failed");
  static_assert(i1.equals(i3) == false, "CIEC_ANY::equals constexpr failed");

  // cross-type equality: differing type IDs must not be equal in constexpr context
  constexpr forte::CIEC_SINT s1(42);
  constexpr forte::CIEC_UINT u1(42);

  static_assert(i1.equals(s1) == false, "CIEC_ANY::equals constexpr failed for differing type IDs (INT vs SINT)");
  static_assert(s1.equals(i1) == false, "CIEC_ANY::equals constexpr failed for differing type IDs (SINT vs INT)");

  static_assert(i1.equals(u1) == false, "CIEC_ANY::equals constexpr failed for differing type IDs (INT vs UINT)");
  static_assert(u1.equals(i1) == false, "CIEC_ANY::equals constexpr failed for differing type IDs (UINT vs INT)");
}

BOOST_AUTO_TEST_CASE(Constexpr_Assignment_test) {
  constexpr auto i = []() {
    forte::CIEC_INT i1(10);
    forte::CIEC_INT i2(20);
    i1 = i2;
    return i1;
  }();
  static_assert(static_cast<forte::TForteInt16>(i) == 20, "CIEC_INT assignment constexpr failed");

  constexpr auto r = []() {
    forte::CIEC_REAL r1(1.0f);
    forte::CIEC_INT i1(42);
    r1 = i1;
    return r1;
  }();
  static_assert(static_cast<forte::TForteFloat>(r) == 42.0f, "CIEC_REAL assignment from CIEC_INT constexpr failed");
}

BOOST_AUTO_TEST_CASE(Constexpr_LTIME_test) {
  constexpr forte::CIEC_LTIME lt1(1000);
  constexpr forte::CIEC_LTIME lt2(2000);
  constexpr forte::CIEC_LTIME lt3 = lt1 + lt2;
  static_assert(static_cast<forte::TForteInt64>(lt3) == 3000, "CIEC_LTIME operator+ constexpr failed");

  static_assert(lt1 < lt2, "CIEC_LTIME operator< constexpr failed");
}

BOOST_AUTO_TEST_CASE(Constinit_test) {
  constinit static auto seventeen = static_cast<forte::TForteInt16>(17_INT);
  BOOST_TEST(seventeen == 17);
}

BOOST_AUTO_TEST_SUITE_END()

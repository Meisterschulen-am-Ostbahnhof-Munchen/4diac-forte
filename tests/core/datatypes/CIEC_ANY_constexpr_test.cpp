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
#include "forte/datatypes/forte_sint.h"
#include "forte/datatypes/forte_int.h"
#include "forte/datatypes/forte_dint.h"
#include "forte/datatypes/forte_uint.h"
#include "forte/datatypes/forte_real.h"
#include "forte/datatypes/forte_lreal.h"
#include "forte/datatypes/forte_time.h"
#include "forte/datatypes/forte_ltime.h"
#include "forte/datatypes/forte_array_fixed.h"

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

BOOST_AUTO_TEST_CASE(Constexpr_Compare_test) {
  constexpr forte::CIEC_INT i1(10);
  constexpr forte::CIEC_INT i2(20);
  static_assert(i1.compare(i2) == std::strong_ordering::less, "CIEC_INT::compare constexpr failed");
  static_assert(i2.compare(i1) == std::strong_ordering::greater, "CIEC_INT::compare constexpr failed");
  static_assert(i1.compare(i1) == std::strong_ordering::equal, "CIEC_INT::compare constexpr failed");

  constexpr forte::CIEC_REAL r1(1.0f);
  constexpr forte::CIEC_REAL r2(2.0f);
  static_assert(r1.compare(r2) == std::partial_ordering::less, "CIEC_REAL::compare constexpr failed");

  constexpr forte::CIEC_TIME t1(1000);
  constexpr forte::CIEC_TIME t2(2000);
  static_assert(t1.compare(t2) == std::strong_ordering::less, "CIEC_TIME::compare constexpr failed");
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

constexpr auto create_assignment_i() {
  forte::CIEC_INT i1(10);
  forte::CIEC_INT i2(20);
  i1 = i2;
  return i1;
}

constexpr auto create_assignment_r() {
  forte::CIEC_REAL r1(1.0f);
  forte::CIEC_INT i1(42);
  r1 = i1;
  return r1;
}

BOOST_AUTO_TEST_CASE(Constexpr_Assignment_test) {
  static constexpr auto i = create_assignment_i();
  static_assert(static_cast<forte::TForteInt16>(i) == 20, "CIEC_INT assignment constexpr failed");

  static constexpr auto r = create_assignment_r();
  static_assert(static_cast<forte::TForteFloat>(r) == 42.0f, "CIEC_REAL assignment from CIEC_INT constexpr failed");
}

BOOST_AUTO_TEST_CASE(Constexpr_LTIME_test) {
  constexpr forte::CIEC_LTIME lt1(1000);
  constexpr forte::CIEC_LTIME lt2(2000);
  constexpr forte::CIEC_LTIME lt3 = lt1 + lt2;
  static_assert(static_cast<forte::TForteInt64>(lt3) == 3000, "CIEC_LTIME operator+ constexpr failed");

  static_assert(lt1 < lt2, "CIEC_LTIME operator< constexpr failed");
}

BOOST_AUTO_TEST_CASE(Constexpr_REAL_LREAL_test) {
  constexpr forte::CIEC_REAL r1(1.0f);
  constexpr forte::CIEC_REAL r2(2.0f);
  static_assert(static_cast<forte::TForteFloat>(r1) == 1.0f, "CIEC_REAL constexpr init failed");
  static_assert(static_cast<forte::TForteFloat>(r2) == 2.0f, "CIEC_REAL constexpr init failed");

  static_assert(r1 < r2, "CIEC_REAL operator< constexpr failed");
  static_assert(r2 > r1, "CIEC_REAL operator> constexpr failed");
  static_assert(r1 == forte::CIEC_REAL(1.0f), "CIEC_REAL operator== constexpr failed");

  constexpr forte::CIEC_LREAL lr1(10.0);
  constexpr forte::CIEC_LREAL lr2(20.0);
  static_assert(static_cast<forte::TForteDFloat>(lr1) == 10.0, "CIEC_LREAL constexpr init failed");
  static_assert(static_cast<forte::TForteDFloat>(lr2) == 20.0, "CIEC_LREAL constexpr init failed");

  static_assert(lr1 < lr2, "CIEC_LREAL operator< constexpr failed");
  static_assert(lr2 > lr1, "CIEC_LREAL operator> constexpr failed");
  static_assert(lr1 == forte::CIEC_LREAL(10.0), "CIEC_LREAL operator== constexpr failed");
}

BOOST_AUTO_TEST_CASE(Constexpr_Array_test) {
  // MSVC cannot evaluate virtual operator[] overrides in constexpr context
#if !defined(_MSC_VER) || (_MSC_VER >= 1951)
  constexpr CIEC_ARRAY_FIXED<CIEC_INT, 0, 2> arr1{CIEC_INT(1), CIEC_INT(2), CIEC_INT(3)};
  static_assert(static_cast<TForteInt16>(arr1[0]) == 1, "CIEC_ARRAY_FIXED constexpr access failed");
  static_assert(static_cast<TForteInt16>(arr1[2]) == 3, "CIEC_ARRAY_FIXED constexpr access failed");

  constexpr CIEC_ARRAY_FIXED<CIEC_INT, 0, 2> arr2{CIEC_INT(1), CIEC_INT(2), CIEC_INT(3)};
  static_assert(arr1 == arr2, "CIEC_ARRAY_FIXED constexpr equality failed");

  constexpr CIEC_ARRAY_FIXED<CIEC_INT, 0, 2> arr3{CIEC_INT(1), CIEC_INT(2), CIEC_INT(4)};
  static_assert(arr1 != arr3, "CIEC_ARRAY_FIXED constexpr inequality failed");
#endif
}

constexpr auto create_init_list_0_5() {
  CIEC_ARRAY_FIXED<CIEC_INT, 0, 5> a;
  a = {CIEC_INT(1), CIEC_INT(2), CIEC_INT(3), CIEC_INT(4), CIEC_INT(5), CIEC_INT(6)};
  return a;
}

BOOST_AUTO_TEST_CASE(Constexpr_FixedArray_InitializerListAssignment_0To5) {
  static constexpr auto arr = create_init_list_0_5();
  static_assert(arr.getLowerBound() == 0, "lower bound failed");
  static_assert(arr.getUpperBound() == 5, "upper bound failed");
#if !defined(_MSC_VER) || (_MSC_VER >= 1951)
  static_assert(static_cast<TForteInt16>(arr[0]) == 1, "[0] failed");
  static_assert(static_cast<TForteInt16>(arr[1]) == 2, "[1] failed");
  static_assert(static_cast<TForteInt16>(arr[2]) == 3, "[2] failed");
  static_assert(static_cast<TForteInt16>(arr[3]) == 4, "[3] failed");
  static_assert(static_cast<TForteInt16>(arr[4]) == 5, "[4] failed");
  static_assert(static_cast<TForteInt16>(arr[5]) == 6, "[5] failed");
#endif
}

constexpr auto create_init_list_2_7() {
  CIEC_ARRAY_FIXED<CIEC_INT, 2, 7> a;
  a = {CIEC_INT(100), CIEC_INT(200), CIEC_INT(300), CIEC_INT(400), CIEC_INT(500), CIEC_INT(600)};
  return a;
}

BOOST_AUTO_TEST_CASE(Constexpr_FixedArray_InitializerListAssignment_2To7) {
  static constexpr auto arr = create_init_list_2_7();
  static_assert(arr.getLowerBound() == 2, "lower bound failed");
  static_assert(arr.getUpperBound() == 7, "upper bound failed");
#if !defined(_MSC_VER) || (_MSC_VER >= 1951)
  static_assert(static_cast<TForteInt16>(arr[2]) == 100, "[2] failed");
  static_assert(static_cast<TForteInt16>(arr[3]) == 200, "[3] failed");
  static_assert(static_cast<TForteInt16>(arr[4]) == 300, "[4] failed");
  static_assert(static_cast<TForteInt16>(arr[5]) == 400, "[5] failed");
  static_assert(static_cast<TForteInt16>(arr[6]) == 500, "[6] failed");
  static_assert(static_cast<TForteInt16>(arr[7]) == 600, "[7] failed");
#endif
}

constexpr auto create_init_list_1_5() {
  CIEC_ARRAY_FIXED<CIEC_INT, 1, 5> a;
  a = {CIEC_INT(11), CIEC_INT(22), CIEC_INT(33), CIEC_INT(44), CIEC_INT(55)};
  return a;
}

BOOST_AUTO_TEST_CASE(Constexpr_FixedArray_InitializerListAssignment_1To5) {
  static constexpr auto arr = create_init_list_1_5();
  static_assert(arr.getLowerBound() == 1, "lower bound failed");
  static_assert(arr.getUpperBound() == 5, "upper bound failed");
#if !defined(_MSC_VER) || (_MSC_VER >= 1951)
  static_assert(static_cast<TForteInt16>(arr[1]) == 11, "[1] failed");
  static_assert(static_cast<TForteInt16>(arr[2]) == 22, "[2] failed");
  static_assert(static_cast<TForteInt16>(arr[3]) == 33, "[3] failed");
  static_assert(static_cast<TForteInt16>(arr[4]) == 44, "[4] failed");
  static_assert(static_cast<TForteInt16>(arr[5]) == 55, "[5] failed");
#endif
}

constexpr auto create_init_list_minus1_1() {
  CIEC_ARRAY_FIXED<CIEC_INT, -1, 1> a;
  a = {CIEC_INT(-10), CIEC_INT(-20), CIEC_INT(-30)};
  return a;
}

BOOST_AUTO_TEST_CASE(Constexpr_FixedArray_InitializerListAssignment_Minus1To1) {
  static constexpr auto arr = create_init_list_minus1_1();
  static_assert(arr.getLowerBound() == -1, "lower bound failed");
  static_assert(arr.getUpperBound() == 1, "upper bound failed");
#if !defined(_MSC_VER) || (_MSC_VER >= 1951)
  static_assert(static_cast<TForteInt16>(arr[-1]) == -10, "[-1] failed");
  static_assert(static_cast<TForteInt16>(arr[0]) == -20, "[0] failed");
  static_assert(static_cast<TForteInt16>(arr[1]) == -30, "[1] failed");
#endif
}

constexpr auto create_init_list_minus5_5() {
  CIEC_ARRAY_FIXED<CIEC_INT, -5, 5> a;
  a = {CIEC_INT(1), CIEC_INT(2), CIEC_INT(3), CIEC_INT(4),  CIEC_INT(5), CIEC_INT(6),
       CIEC_INT(7), CIEC_INT(8), CIEC_INT(9), CIEC_INT(10), CIEC_INT(11)};
  return a;
}

BOOST_AUTO_TEST_CASE(Constexpr_FixedArray_InitializerListAssignment_Minus5To5) {
  static constexpr auto arr = create_init_list_minus5_5();
  static_assert(arr.getLowerBound() == -5, "lower bound failed");
  static_assert(arr.getUpperBound() == 5, "upper bound failed");
#if !defined(_MSC_VER) || (_MSC_VER >= 1951)
  static_assert(static_cast<TForteInt16>(arr[-5]) == 1, "[-5] failed");
  static_assert(static_cast<TForteInt16>(arr[-4]) == 2, "[-4] failed");
  static_assert(static_cast<TForteInt16>(arr[-3]) == 3, "[-3] failed");
  static_assert(static_cast<TForteInt16>(arr[-2]) == 4, "[-2] failed");
  static_assert(static_cast<TForteInt16>(arr[-1]) == 5, "[-1] failed");
  static_assert(static_cast<TForteInt16>(arr[0]) == 6, "[0] failed");
  static_assert(static_cast<TForteInt16>(arr[1]) == 7, "[1] failed");
  static_assert(static_cast<TForteInt16>(arr[2]) == 8, "[2] failed");
  static_assert(static_cast<TForteInt16>(arr[3]) == 9, "[3] failed");
  static_assert(static_cast<TForteInt16>(arr[4]) == 10, "[4] failed");
  static_assert(static_cast<TForteInt16>(arr[5]) == 11, "[5] failed");
#endif
}

BOOST_AUTO_TEST_CASE(Constexpr_FixedArray_CopyConstructor_0To5) {
  constexpr CIEC_ARRAY_FIXED<CIEC_INT, 0, 5> source{CIEC_INT(1), CIEC_INT(2), CIEC_INT(3),
                                                    CIEC_INT(4), CIEC_INT(5), CIEC_INT(6)};
  constexpr CIEC_ARRAY_FIXED<CIEC_INT, 0, 5> dest(source);
#if !defined(_MSC_VER) || (_MSC_VER >= 1951)
  static_assert(static_cast<TForteInt16>(dest[0]) == 1, "[0] failed");
  static_assert(static_cast<TForteInt16>(dest[5]) == 6, "[5] failed");
  static_assert(dest == source, "equality failed");
#endif
}

BOOST_AUTO_TEST_CASE(Constexpr_FixedArray_CopyConstructor_2To7) {
  constexpr CIEC_ARRAY_FIXED<CIEC_INT, 2, 7> source{CIEC_INT(100), CIEC_INT(200), CIEC_INT(300),
                                                    CIEC_INT(400), CIEC_INT(500), CIEC_INT(600)};
  constexpr CIEC_ARRAY_FIXED<CIEC_INT, 2, 7> dest(source);
#if !defined(_MSC_VER) || (_MSC_VER >= 1951)
  static_assert(static_cast<TForteInt16>(dest[2]) == 100, "[2] failed");
  static_assert(static_cast<TForteInt16>(dest[7]) == 600, "[7] failed");
  static_assert(dest == source, "equality failed");
#endif
}

BOOST_AUTO_TEST_CASE(Constexpr_FixedArray_CopyConstructor_Minus5To5) {
  constexpr CIEC_ARRAY_FIXED<CIEC_INT, -5, 5> source{CIEC_INT(1), CIEC_INT(2),  CIEC_INT(3), CIEC_INT(4),
                                                     CIEC_INT(5), CIEC_INT(6),  CIEC_INT(7), CIEC_INT(8),
                                                     CIEC_INT(9), CIEC_INT(10), CIEC_INT(11)};
  constexpr CIEC_ARRAY_FIXED<CIEC_INT, -5, 5> dest(source);
#if !defined(_MSC_VER) || (_MSC_VER >= 1951)
  static_assert(static_cast<TForteInt16>(dest[-5]) == 1, "[-5] failed");
  static_assert(static_cast<TForteInt16>(dest[0]) == 6, "[0] failed");
  static_assert(static_cast<TForteInt16>(dest[5]) == 11, "[5] failed");
  static_assert(dest == source, "equality failed");
#endif
}

constexpr auto create_copy_assignment_minus1_1() {
  CIEC_ARRAY_FIXED<CIEC_INT, -1, 1> d;
  constexpr CIEC_ARRAY_FIXED<CIEC_INT, -1, 1> s{CIEC_INT(-10), CIEC_INT(-20), CIEC_INT(-30)};
  d = s;
  return d;
}

BOOST_AUTO_TEST_CASE(Constexpr_FixedArray_CopyAssignment_Minus1To1) {
  static constexpr auto dest = create_copy_assignment_minus1_1();
#if !defined(_MSC_VER) || (_MSC_VER >= 1951)
  static_assert(static_cast<TForteInt16>(dest[-1]) == -10, "[-1] failed");
  static_assert(static_cast<TForteInt16>(dest[0]) == -20, "[0] failed");
  static_assert(static_cast<TForteInt16>(dest[1]) == -30, "[1] failed");
#endif
}

BOOST_AUTO_TEST_CASE(Constinit_test) {
  constinit static auto seventeen = static_cast<forte::TForteInt16>(CIEC_INT(17));
  BOOST_TEST(seventeen == 17);
}

constexpr auto create_bracket_0_5() {
  CIEC_ARRAY_FIXED<CIEC_INT, 0, 5> a;
  a[0] = CIEC_INT(100);
  a[1] = CIEC_INT(200);
  a[2] = CIEC_INT(300);
  a[3] = CIEC_INT(400);
  a[4] = CIEC_INT(500);
  a[5] = CIEC_INT(600);
  return a;
}

BOOST_AUTO_TEST_CASE(Constexpr_FixedArray_BracketOperator_0To5) {
#if !defined(_MSC_VER) || (_MSC_VER >= 1951)
  static constexpr auto arr = create_bracket_0_5();
  static_assert(static_cast<TForteInt16>(arr[0]) == 100, "a[0] failed");
  static_assert(static_cast<TForteInt16>(arr[1]) == 200, "a[1] failed");
  static_assert(static_cast<TForteInt16>(arr[2]) == 300, "a[2] failed");
  static_assert(static_cast<TForteInt16>(arr[3]) == 400, "a[3] failed");
  static_assert(static_cast<TForteInt16>(arr[4]) == 500, "a[4] failed");
  static_assert(static_cast<TForteInt16>(arr[5]) == 600, "a[5] failed");
#endif
}

constexpr auto create_bracket_2_7() {
  CIEC_ARRAY_FIXED<CIEC_INT, 2, 7> a;
  a[2] = CIEC_INT(11);
  a[3] = CIEC_INT(22);
  a[4] = CIEC_INT(33);
  a[5] = CIEC_INT(44);
  a[6] = CIEC_INT(55);
  a[7] = CIEC_INT(66);
  return a;
}

BOOST_AUTO_TEST_CASE(Constexpr_FixedArray_BracketOperator_2To7) {
#if !defined(_MSC_VER) || (_MSC_VER >= 1951)
  static constexpr auto arr = create_bracket_2_7();
  static_assert(static_cast<TForteInt16>(arr[2]) == 11, "a[2] failed");
  static_assert(static_cast<TForteInt16>(arr[3]) == 22, "a[3] failed");
  static_assert(static_cast<TForteInt16>(arr[4]) == 33, "a[4] failed");
  static_assert(static_cast<TForteInt16>(arr[5]) == 44, "a[5] failed");
  static_assert(static_cast<TForteInt16>(arr[6]) == 55, "a[6] failed");
  static_assert(static_cast<TForteInt16>(arr[7]) == 66, "a[7] failed");
#endif
}

constexpr auto create_bracket_minus1_1() {
  CIEC_ARRAY_FIXED<CIEC_INT, -1, 1> a;
  a[-1] = CIEC_INT(1);
  a[0] = CIEC_INT(2);
  a[1] = CIEC_INT(3);
  return a;
}

BOOST_AUTO_TEST_CASE(Constexpr_FixedArray_BracketOperator_Minus1To1) {
#if !defined(_MSC_VER) || (_MSC_VER >= 1951)
  static constexpr auto arr = create_bracket_minus1_1();
  static_assert(static_cast<TForteInt16>(arr[-1]) == 1, "a[-1] failed");
  static_assert(static_cast<TForteInt16>(arr[0]) == 2, "a[0] failed");
  static_assert(static_cast<TForteInt16>(arr[1]) == 3, "a[1] failed");
#endif
}

constexpr auto create_arr_minus5_5() {
  CIEC_ARRAY_FIXED<CIEC_INT, -5, 5> a;
  a[-5] = CIEC_INT(10);
  a[-4] = CIEC_INT(20);
  a[-3] = CIEC_INT(30);
  a[-2] = CIEC_INT(40);
  a[-1] = CIEC_INT(50);
  a[0] = CIEC_INT(60);
  a[1] = CIEC_INT(70);
  a[2] = CIEC_INT(80);
  a[3] = CIEC_INT(90);
  a[4] = CIEC_INT(100);
  a[5] = CIEC_INT(110);
  return a;
}

BOOST_AUTO_TEST_CASE(Constexpr_FixedArray_BracketOperator_Minus5To5) {
#if !defined(_MSC_VER) || (_MSC_VER >= 1951)
  static constexpr auto arr = create_arr_minus5_5();
  static_assert(static_cast<TForteInt16>(arr[-5]) == 10, "a[-5] failed");
  static_assert(static_cast<TForteInt16>(arr[-4]) == 20, "a[-4] failed");
  static_assert(static_cast<TForteInt16>(arr[-3]) == 30, "a[-3] failed");
  static_assert(static_cast<TForteInt16>(arr[-2]) == 40, "a[-2] failed");
  static_assert(static_cast<TForteInt16>(arr[-1]) == 50, "a[-1] failed");
  static_assert(static_cast<TForteInt16>(arr[0]) == 60, "a[0] failed");
  static_assert(static_cast<TForteInt16>(arr[1]) == 70, "a[1] failed");
  static_assert(static_cast<TForteInt16>(arr[2]) == 80, "a[2] failed");
  static_assert(static_cast<TForteInt16>(arr[3]) == 90, "a[3] failed");
  static_assert(static_cast<TForteInt16>(arr[4]) == 100, "a[4] failed");
  static_assert(static_cast<TForteInt16>(arr[5]) == 110, "a[5] failed");
#endif
}

BOOST_AUTO_TEST_SUITE_END()

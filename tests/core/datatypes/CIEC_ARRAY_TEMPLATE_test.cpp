/*******************************************************************************
 * Copyright (c) 2022 Primetals Technologies Austria GmbH
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Martin Melik Merkumians
 *      - initial implementation and rework communication infrastructure
 *******************************************************************************/
#include <boost/test/unit_test.hpp>
#include "forte_boost_output_support.h"

#include "forte/datatypes/forte_array.h"
#include "forte/datatypes/forte_array_fixed.h"
#include "forte/datatypes/forte_array_variable.h"
#include "forte/datatypes/forte_bool.h"
#include "forte/datatypes/forte_int.h"
#include "forte/datatypes/forte_dint.h"
#include "forte/typelib.h"
#include "forte/datatypes/forte_array_dynamic.h"

using namespace forte::literals;

namespace forte::test {
  BOOST_AUTO_TEST_SUITE(CIEC_ARRAY_TEMPLATE_function_test)

  /* Static length tests */

  BOOST_AUTO_TEST_CASE(InstantiateTemplateWithLimitsArray) {
    CIEC_ARRAY_FIXED<CIEC_INT, 0, 10> intArray;

    BOOST_CHECK(intArray.size() == 11);
    BOOST_CHECK(intArray.getLowerBound() == 0);
    BOOST_CHECK(intArray.getUpperBound() == 10);
    BOOST_TEST(intArray.isIndexInRange(0) == true);
    BOOST_TEST(intArray.isIndexInRange(10) == true);
    BOOST_TEST(intArray.isIndexInRange(5) == true);
    BOOST_TEST(intArray.isIndexInRange(-1) == false);
    BOOST_TEST(intArray.isIndexInRange(11) == false);
    BOOST_CHECK(intArray.getDataTypeID() == CIEC_ANY::e_ARRAY);
    BOOST_CHECK(intArray.getElementDataTypeID() == CIEC_ANY::e_INT);

    BOOST_CHECK(static_cast<CIEC_INT::TValueType>(intArray[0]) == 0);
    BOOST_CHECK(static_cast<CIEC_INT::TValueType>(intArray[1]) == 0);
    BOOST_CHECK(static_cast<CIEC_INT::TValueType>(intArray[2]) == 0);
    BOOST_CHECK(static_cast<CIEC_INT::TValueType>(intArray[3]) == 0);
    BOOST_CHECK(static_cast<CIEC_INT::TValueType>(intArray[4]) == 0);
    BOOST_CHECK(static_cast<CIEC_INT::TValueType>(intArray[5]) == 0);
    BOOST_CHECK(static_cast<CIEC_INT::TValueType>(intArray[6]) == 0);
    BOOST_CHECK(static_cast<CIEC_INT::TValueType>(intArray[7]) == 0);
    BOOST_CHECK(static_cast<CIEC_INT::TValueType>(intArray[8]) == 0);
    BOOST_CHECK(static_cast<CIEC_INT::TValueType>(intArray[9]) == 0);
    BOOST_CHECK(static_cast<CIEC_INT::TValueType>(intArray[10]) == 0);
  }

  BOOST_AUTO_TEST_CASE(InitializerListStaticArray) {
    CIEC_ARRAY_FIXED<CIEC_INT, 0, 10> intArray = {CIEC_INT(1), CIEC_INT(2),  CIEC_INT(3), CIEC_INT(4),
                                                  CIEC_INT(5), CIEC_INT(6),  CIEC_INT(7), CIEC_INT(8),
                                                  CIEC_INT(9), CIEC_INT(10), CIEC_INT(11)};

    BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(intArray[0]), 1);
    BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(intArray[1]), 2);
    BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(intArray[2]), 3);
    BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(intArray[3]), 4);
    BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(intArray[4]), 5);
    BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(intArray[5]), 6);
    BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(intArray[6]), 7);
    BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(intArray[7]), 8);
    BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(intArray[8]), 9);
    BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(intArray[9]), 10);
    BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(intArray[10]), 11);
  }

  BOOST_AUTO_TEST_CASE(InitializeNegativeRangesStaticArray) {
    CIEC_ARRAY_FIXED<CIEC_INT, -10, 0> intArray = {CIEC_INT(1), CIEC_INT(2),  CIEC_INT(3), CIEC_INT(4),
                                                   CIEC_INT(5), CIEC_INT(6),  CIEC_INT(7), CIEC_INT(8),
                                                   CIEC_INT(9), CIEC_INT(10), CIEC_INT(11)};

    BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(intArray[-10]), 1);
    BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(intArray[-9]), 2);
    BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(intArray[-8]), 3);
    BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(intArray[-7]), 4);
    BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(intArray[-6]), 5);
    BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(intArray[-5]), 6);
    BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(intArray[-4]), 7);
    BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(intArray[-3]), 8);
    BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(intArray[-2]), 9);
    BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(intArray[-1]), 10);
    BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(intArray[0]), 11);
  }

  BOOST_AUTO_TEST_CASE(StaticArrayIndexAccesWithIECTypes) {
    CIEC_ARRAY_FIXED<CIEC_INT, -10, 0> intArray = {CIEC_INT(1), CIEC_INT(2),  CIEC_INT(3), CIEC_INT(4),
                                                   CIEC_INT(5), CIEC_INT(6),  CIEC_INT(7), CIEC_INT(8),
                                                   CIEC_INT(9), CIEC_INT(10), CIEC_INT(11)};

    BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(intArray[CIEC_INT(-10)]), 1);
    BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(intArray[CIEC_INT(-9)]), 2);
    BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(intArray[CIEC_INT(-8)]), 3);
    BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(intArray[CIEC_INT(-7)]), 4);
    BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(intArray[CIEC_INT(-6)]), 5);
    BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(intArray[CIEC_INT(-5)]), 6);
    BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(intArray[CIEC_INT(-4)]), 7);
    BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(intArray[CIEC_INT(-3)]), 8);
    BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(intArray[CIEC_INT(-2)]), 9);
    BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(intArray[CIEC_INT(-1)]), 10);
    BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(intArray[CIEC_INT(0)]), 11);
  }

  BOOST_AUTO_TEST_CASE(WriteElementsStaticArray) {
    CIEC_ARRAY_FIXED<CIEC_INT, 0, 10> intArray;

    intArray[0] = CIEC_INT(1);
    intArray[1] = CIEC_INT(2);
    intArray[2] = CIEC_INT(3);
    intArray[3] = CIEC_INT(4);
    intArray[4] = CIEC_INT(5);
    intArray[5] = CIEC_INT(6);
    intArray[6] = CIEC_INT(7);
    intArray[7] = CIEC_INT(8);
    intArray[8] = CIEC_INT(9);
    intArray[9] = CIEC_INT(10);
    intArray[10] = CIEC_INT(11);

    BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(intArray[0]), 1);
    BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(intArray[1]), 2);
    BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(intArray[2]), 3);
    BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(intArray[3]), 4);
    BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(intArray[4]), 5);
    BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(intArray[5]), 6);
    BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(intArray[6]), 7);
    BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(intArray[7]), 8);
    BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(intArray[8]), 9);
    BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(intArray[9]), 10);
    BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(intArray[10]), 11);
  }

  BOOST_AUTO_TEST_CASE(WriteElementsStaticArrayWithIECTypeIndices) {
    CIEC_ARRAY_FIXED<CIEC_INT, 0, 10> intArray;

    intArray[CIEC_UINT(0)] = CIEC_INT(1);
    intArray[CIEC_UINT(1)] = CIEC_INT(2);
    intArray[CIEC_UINT(2)] = CIEC_INT(3);
    intArray[CIEC_UINT(3)] = CIEC_INT(4);
    intArray[CIEC_UINT(4)] = CIEC_INT(5);
    intArray[CIEC_UINT(5)] = CIEC_INT(6);
    intArray[CIEC_UINT(6)] = CIEC_INT(7);
    intArray[CIEC_UINT(7)] = CIEC_INT(8);
    intArray[CIEC_UINT(8)] = CIEC_INT(9);
    intArray[CIEC_UINT(9)] = CIEC_INT(10);
    intArray[CIEC_UINT(10)] = CIEC_INT(11);

    BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(intArray[0]), 1);
    BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(intArray[1]), 2);
    BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(intArray[2]), 3);
    BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(intArray[3]), 4);
    BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(intArray[4]), 5);
    BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(intArray[5]), 6);
    BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(intArray[6]), 7);
    BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(intArray[7]), 8);
    BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(intArray[8]), 9);
    BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(intArray[9]), 10);
    BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(intArray[10]), 11);
  }

  BOOST_AUTO_TEST_CASE(StaticToStaticCopyConstructor) {
    CIEC_ARRAY_FIXED<CIEC_INT, 0, 10> intArraySource = {CIEC_INT(1), CIEC_INT(2),  CIEC_INT(3), CIEC_INT(4),
                                                        CIEC_INT(5), CIEC_INT(6),  CIEC_INT(7), CIEC_INT(8),
                                                        CIEC_INT(9), CIEC_INT(10), CIEC_INT(11)};

    CIEC_ARRAY_FIXED<CIEC_INT, 0, 10> intArray(intArraySource);

    BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(intArray[0]), 1);
    BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(intArray[1]), 2);
    BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(intArray[2]), 3);
    BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(intArray[3]), 4);
    BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(intArray[4]), 5);
    BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(intArray[5]), 6);
    BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(intArray[6]), 7);
    BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(intArray[7]), 8);
    BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(intArray[8]), 9);
    BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(intArray[9]), 10);
    BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(intArray[10]), 11);
  }

  BOOST_AUTO_TEST_CASE(StaticToStaticDifferentTypeSameSizeCopyConstructor) {
    CIEC_ARRAY_FIXED<CIEC_INT, 0, 10> intArraySource = {CIEC_INT(1), CIEC_INT(2),  CIEC_INT(3), CIEC_INT(4),
                                                        CIEC_INT(5), CIEC_INT(6),  CIEC_INT(7), CIEC_INT(8),
                                                        CIEC_INT(9), CIEC_INT(10), CIEC_INT(11)};

    CIEC_ARRAY_FIXED<CIEC_DINT, 0, 10> intArray(intArraySource);

    BOOST_CHECK_EQUAL(static_cast<CIEC_DINT::TValueType>(intArray[0]), 1);
    BOOST_CHECK_EQUAL(static_cast<CIEC_DINT::TValueType>(intArray[1]), 2);
    BOOST_CHECK_EQUAL(static_cast<CIEC_DINT::TValueType>(intArray[2]), 3);
    BOOST_CHECK_EQUAL(static_cast<CIEC_DINT::TValueType>(intArray[3]), 4);
    BOOST_CHECK_EQUAL(static_cast<CIEC_DINT::TValueType>(intArray[4]), 5);
    BOOST_CHECK_EQUAL(static_cast<CIEC_DINT::TValueType>(intArray[5]), 6);
    BOOST_CHECK_EQUAL(static_cast<CIEC_DINT::TValueType>(intArray[6]), 7);
    BOOST_CHECK_EQUAL(static_cast<CIEC_DINT::TValueType>(intArray[7]), 8);
    BOOST_CHECK_EQUAL(static_cast<CIEC_DINT::TValueType>(intArray[8]), 9);
    BOOST_CHECK_EQUAL(static_cast<CIEC_DINT::TValueType>(intArray[9]), 10);
    BOOST_CHECK_EQUAL(static_cast<CIEC_DINT::TValueType>(intArray[10]), 11);
  }

  BOOST_AUTO_TEST_CASE(StaticToStaticDifferentTypeDifferentSizeWindowCopyConstructor) {
    CIEC_ARRAY_FIXED<CIEC_INT, 0, 10> intArraySource = {CIEC_INT(1), CIEC_INT(2),  CIEC_INT(3), CIEC_INT(4),
                                                        CIEC_INT(5), CIEC_INT(6),  CIEC_INT(7), CIEC_INT(8),
                                                        CIEC_INT(9), CIEC_INT(10), CIEC_INT(11)};

    CIEC_ARRAY_FIXED<CIEC_DINT, 3, 5> intArray(intArraySource);

    BOOST_CHECK_EQUAL(static_cast<CIEC_DINT::TValueType>(intArray[3]), 4);
    BOOST_CHECK_EQUAL(static_cast<CIEC_DINT::TValueType>(intArray[4]), 5);
    BOOST_CHECK_EQUAL(static_cast<CIEC_DINT::TValueType>(intArray[5]), 6);
  }

  BOOST_AUTO_TEST_CASE(StaticToStaticDifferentTypeMovedSizeWindowCopyConstructor) {
    CIEC_ARRAY_FIXED<CIEC_INT, 0, 10> intArraySource = {CIEC_INT(1), CIEC_INT(2),  CIEC_INT(3), CIEC_INT(4),
                                                        CIEC_INT(5), CIEC_INT(6),  CIEC_INT(7), CIEC_INT(8),
                                                        CIEC_INT(9), CIEC_INT(10), CIEC_INT(11)};

    CIEC_ARRAY_FIXED<CIEC_DINT, 3, 12> dintArray(intArraySource);

    BOOST_CHECK_EQUAL(static_cast<CIEC_DINT::TValueType>(dintArray[3]), 4);
    BOOST_CHECK_EQUAL(static_cast<CIEC_DINT::TValueType>(dintArray[4]), 5);
    BOOST_CHECK_EQUAL(static_cast<CIEC_DINT::TValueType>(dintArray[5]), 6);
    BOOST_CHECK_EQUAL(static_cast<CIEC_DINT::TValueType>(dintArray[6]), 7);
    BOOST_CHECK_EQUAL(static_cast<CIEC_DINT::TValueType>(dintArray[7]), 8);
    BOOST_CHECK_EQUAL(static_cast<CIEC_DINT::TValueType>(dintArray[8]), 9);
    BOOST_CHECK_EQUAL(static_cast<CIEC_DINT::TValueType>(dintArray[9]), 10);
    BOOST_CHECK_EQUAL(static_cast<CIEC_DINT::TValueType>(dintArray[10]), 11);
    BOOST_CHECK_EQUAL(static_cast<CIEC_DINT::TValueType>(dintArray[11]), 0);
    BOOST_CHECK_EQUAL(static_cast<CIEC_DINT::TValueType>(dintArray[12]), 0);
  }

  /* Variable length tests */

  BOOST_AUTO_TEST_CASE(InstantiateSizeOnlyVariableArray) {
    CIEC_ARRAY_VARIABLE<CIEC_BOOL> boolArray(0, 10);
    BOOST_CHECK_EQUAL(boolArray.size(), 11);
    BOOST_CHECK_EQUAL(boolArray.getLowerBound(), 0);
    BOOST_CHECK_EQUAL(boolArray.getUpperBound(), 10);
    BOOST_TEST(boolArray.isIndexInRange(0) == true);
    BOOST_TEST(boolArray.isIndexInRange(10) == true);
    BOOST_TEST(boolArray.isIndexInRange(5) == true);
    BOOST_TEST(boolArray.isIndexInRange(-1) == false);
    BOOST_TEST(boolArray.isIndexInRange(11) == false);
    BOOST_CHECK_EQUAL(boolArray.getDataTypeID(), CIEC_ANY::e_ARRAY);
    BOOST_CHECK_EQUAL(boolArray.getElementDataTypeID(), CIEC_ANY::e_BOOL);
  }

  BOOST_AUTO_TEST_CASE(InstantiateVariableArrayFromInitList) {
    CIEC_ARRAY_VARIABLE<CIEC_BOOL> boolArray = {true_BOOL, false_BOOL, false_BOOL, true_BOOL};
    BOOST_CHECK_EQUAL(boolArray.size(), 4);
    BOOST_CHECK_EQUAL(boolArray.getLowerBound(), 0);
    BOOST_CHECK_EQUAL(boolArray.getUpperBound(), 3);
    BOOST_TEST(boolArray.isIndexInRange(0) == true);
    BOOST_TEST(boolArray.isIndexInRange(3) == true);
    BOOST_TEST(boolArray.isIndexInRange(2) == true);
    BOOST_TEST(boolArray.isIndexInRange(-1) == false);
    BOOST_TEST(boolArray.isIndexInRange(11) == false);
    BOOST_CHECK_EQUAL(boolArray.getDataTypeID(), CIEC_ANY::e_ARRAY);
    BOOST_CHECK_EQUAL(boolArray.getElementDataTypeID(), CIEC_ANY::e_BOOL);
  }

  BOOST_AUTO_TEST_CASE(VariableArrayAccess) {
    CIEC_ARRAY_VARIABLE<CIEC_BOOL> boolArray = {true_BOOL, false_BOOL, false_BOOL, true_BOOL};
    BOOST_CHECK_EQUAL(boolArray[0], true_BOOL);
    BOOST_CHECK_EQUAL(boolArray[1], false_BOOL);
    BOOST_CHECK_EQUAL(boolArray[2], false_BOOL);
    BOOST_CHECK_EQUAL(boolArray[3], true_BOOL);
  }

  BOOST_AUTO_TEST_CASE(VariableArrayAccessIECTypes) {
    CIEC_ARRAY_VARIABLE<CIEC_BOOL> boolArray = {true_BOOL, false_BOOL, false_BOOL, true_BOOL};
    BOOST_CHECK_EQUAL(boolArray[CIEC_INT(0)], true_BOOL);
    BOOST_CHECK_EQUAL(boolArray[CIEC_INT(1)], false_BOOL);
    BOOST_CHECK_EQUAL(boolArray[CIEC_INT(2)], false_BOOL);
    BOOST_CHECK_EQUAL(boolArray[CIEC_INT(3)], true_BOOL);
  }

  /* copy tests */

  BOOST_AUTO_TEST_CASE(CopyFromStaticToVariableArray) {
    CIEC_ARRAY_FIXED<CIEC_INT, 0, 10> intArraySource = {CIEC_INT(1), CIEC_INT(2),  CIEC_INT(3), CIEC_INT(4),
                                                        CIEC_INT(5), CIEC_INT(6),  CIEC_INT(7), CIEC_INT(8),
                                                        CIEC_INT(9), CIEC_INT(10), CIEC_INT(11)};

    CIEC_ARRAY_VARIABLE<CIEC_INT> intArray(intArraySource);

    BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(intArray[0]), 1);
    BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(intArray[1]), 2);
    BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(intArray[2]), 3);
    BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(intArray[3]), 4);
    BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(intArray[4]), 5);
    BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(intArray[5]), 6);
    BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(intArray[6]), 7);
    BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(intArray[7]), 8);
    BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(intArray[8]), 9);
    BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(intArray[9]), 10);
    BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(intArray[10]), 11);
  }

  BOOST_AUTO_TEST_CASE(CopyFromVariableToStaticArray) {
    CIEC_ARRAY_VARIABLE<CIEC_BOOL> boolArraySource = {true_BOOL, false_BOOL, false_BOOL, true_BOOL};
    CIEC_ARRAY_FIXED<CIEC_BOOL, 0, 3> boolArray(boolArraySource);

    BOOST_CHECK_EQUAL(boolArray[0], true_BOOL);
    BOOST_CHECK_EQUAL(boolArray[1], false_BOOL);
    BOOST_CHECK_EQUAL(boolArray[2], false_BOOL);
    BOOST_CHECK_EQUAL(boolArray[3], true_BOOL);
  }

  BOOST_AUTO_TEST_CASE(CopyFromPlainOldToStaticArray) {
    CIEC_ARRAY_DYNAMIC arraySource(5, "INT"_STRID);

    BOOST_TEST(arraySource.getLowerBound() == 0);
    BOOST_TEST(arraySource.getUpperBound() == 4);
    BOOST_TEST(arraySource.isIndexInRange(0) == true);
    BOOST_TEST(arraySource.isIndexInRange(4) == true);
    BOOST_TEST(arraySource.isIndexInRange(3) == true);
    BOOST_TEST(arraySource.isIndexInRange(-1) == false);
    BOOST_TEST(arraySource.isIndexInRange(11) == false);

    static_cast<CIEC_INT &>(arraySource[0]) = CIEC_INT(1);
    static_cast<CIEC_INT &>(arraySource[1]) = CIEC_INT(-32259);
    static_cast<CIEC_INT &>(arraySource[2]) = CIEC_INT(256);
    static_cast<CIEC_INT &>(arraySource[3]) = CIEC_INT(-32259);
    static_cast<CIEC_INT &>(arraySource[4]) = CIEC_INT(256);

    CIEC_ARRAY_FIXED<CIEC_INT, 0, 4> arrayDestination(arraySource);
    BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(arrayDestination[0]), 1);
    BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(arrayDestination[1]), -32259);
    BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(arrayDestination[2]), 256);
    BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(arrayDestination[3]), -32259);
    BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(arrayDestination[4]), 256);
  }

  BOOST_AUTO_TEST_CASE(CopyFromPlainOldToVariableArray) {
    CIEC_ARRAY_DYNAMIC arraySource(5, "INT"_STRID);

    static_cast<CIEC_INT &>(arraySource[0]) = CIEC_INT(1);
    static_cast<CIEC_INT &>(arraySource[1]) = CIEC_INT(-32259);
    static_cast<CIEC_INT &>(arraySource[2]) = CIEC_INT(256);
    static_cast<CIEC_INT &>(arraySource[3]) = CIEC_INT(-32259);
    static_cast<CIEC_INT &>(arraySource[4]) = CIEC_INT(256);

    CIEC_ARRAY_VARIABLE<CIEC_INT> arrayDestination(arraySource);
    BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(arrayDestination[0]), 1);
    BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(arrayDestination[1]), -32259);
    BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(arrayDestination[2]), 256);
    BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(arrayDestination[3]), -32259);
    BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(arrayDestination[4]), 256);
  }

  BOOST_AUTO_TEST_CASE(CopyFromVariableToPlainOldArray) {
    CIEC_ARRAY_VARIABLE<CIEC_INT> arraySource(0, 5);
    (arraySource[0]) = CIEC_INT(1);
    (arraySource[1]) = CIEC_INT(-32259);
    (arraySource[2]) = CIEC_INT(256);
    (arraySource[3]) = CIEC_INT(-32259);
    (arraySource[4]) = CIEC_INT(256);

    CIEC_ARRAY_DYNAMIC arrayDestination(arraySource);
    BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(static_cast<CIEC_INT &>(arrayDestination[0])), 1);
    BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(static_cast<CIEC_INT &>(arrayDestination[1])), -32259);
    BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(static_cast<CIEC_INT &>(arrayDestination[2])), 256);
    BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(static_cast<CIEC_INT &>(arrayDestination[3])), -32259);
    BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(static_cast<CIEC_INT &>(arrayDestination[4])), 256);
  }

  BOOST_AUTO_TEST_CASE(InitializerListAssignmentArray0To5) {
    CIEC_ARRAY_VARIABLE<CIEC_INT> intArray(0, 5);
    intArray = {10_INT, 20_INT, 30_INT, 40_INT, 50_INT, 60_INT};

    BOOST_CHECK_EQUAL(intArray.getLowerBound(), 0);
    BOOST_CHECK_EQUAL(intArray.getUpperBound(), 5);
    BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(intArray[0]), 10);
    BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(intArray[1]), 20);
    BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(intArray[2]), 30);
    BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(intArray[3]), 40);
    BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(intArray[4]), 50);
    BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(intArray[5]), 60);
  }

  BOOST_AUTO_TEST_CASE(InitializerListAssignmentArray2To7) {
    CIEC_ARRAY_VARIABLE<CIEC_INT> intArray(2, 7);
    intArray = {100_INT, 200_INT, 300_INT, 400_INT, 500_INT, 600_INT};

    BOOST_CHECK_EQUAL(intArray.getLowerBound(), 2);
    BOOST_CHECK_EQUAL(intArray.getUpperBound(), 7);
    BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(intArray[2]), 100);
    BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(intArray[3]), 200);
    BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(intArray[4]), 300);
    BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(intArray[5]), 400);
    BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(intArray[6]), 500);
    BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(intArray[7]), 600);
  }

  BOOST_AUTO_TEST_CASE(InitializerListAssignmentArray1To5) {
    CIEC_ARRAY_VARIABLE<CIEC_INT> intArray(1, 5);
    intArray = {11_INT, 22_INT, 33_INT, 44_INT, 55_INT};

    BOOST_CHECK_EQUAL(intArray.getLowerBound(), 1);
    BOOST_CHECK_EQUAL(intArray.getUpperBound(), 5);
    BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(intArray[1]), 11);
    BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(intArray[2]), 22);
    BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(intArray[3]), 33);
    BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(intArray[4]), 44);
    BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(intArray[5]), 55);
  }

  BOOST_AUTO_TEST_CASE(InitializerListAssignmentArrayMinus1To1) {
    CIEC_ARRAY_VARIABLE<CIEC_INT> intArray(-1, 1);
    intArray = {-10_INT, -20_INT, -30_INT};

    BOOST_CHECK_EQUAL(intArray.getLowerBound(), -1);
    BOOST_CHECK_EQUAL(intArray.getUpperBound(), 1);
    BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(intArray[-1]), -10);
    BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(intArray[0]), -20);
    BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(intArray[1]), -30);
  }

  BOOST_AUTO_TEST_CASE(InitializerListAssignmentArrayMinus5To5) {
    CIEC_ARRAY_VARIABLE<CIEC_INT> intArray(-5, 5);
    intArray = {1_INT, 2_INT, 3_INT, 4_INT, 5_INT, 6_INT, 7_INT, 8_INT, 9_INT, 10_INT, 11_INT};

    BOOST_CHECK_EQUAL(intArray.getLowerBound(), -5);
    BOOST_CHECK_EQUAL(intArray.getUpperBound(), 5);
    BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(intArray[-5]), 1);
    BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(intArray[-4]), 2);
    BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(intArray[-3]), 3);
    BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(intArray[-2]), 4);
    BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(intArray[-1]), 5);
    BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(intArray[0]), 6);
    BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(intArray[1]), 7);
    BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(intArray[2]), 8);
    BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(intArray[3]), 9);
    BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(intArray[4]), 10);
    BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(intArray[5]), 11);
  }

  BOOST_AUTO_TEST_CASE(VariableToVariableCopyAssignmentArray0To5) {
    CIEC_ARRAY_VARIABLE<CIEC_INT> source(0, 5);
    source[0] = 10_INT;
    source[1] = 20_INT;
    source[2] = 30_INT;
    source[3] = 40_INT;
    source[4] = 50_INT;
    source[5] = 60_INT;

    CIEC_ARRAY_VARIABLE<CIEC_INT> destination(0, 5);
    destination = source;

    BOOST_CHECK_EQUAL(destination.getLowerBound(), 0);
    BOOST_CHECK_EQUAL(destination.getUpperBound(), 5);
    BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(destination[0]), 10);
    BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(destination[1]), 20);
    BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(destination[2]), 30);
    BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(destination[3]), 40);
    BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(destination[4]), 50);
    BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(destination[5]), 60);
  }

  BOOST_AUTO_TEST_CASE(VariableToVariableCopyAssignmentArray2To7) {
    CIEC_ARRAY_VARIABLE<CIEC_INT> source(2, 7);
    source[2] = 100_INT;
    source[3] = 200_INT;
    source[4] = 300_INT;
    source[5] = 400_INT;
    source[6] = 500_INT;
    source[7] = 600_INT;

    CIEC_ARRAY_VARIABLE<CIEC_INT> destination(2, 7);
    destination = source;

    BOOST_CHECK_EQUAL(destination.getLowerBound(), 2);
    BOOST_CHECK_EQUAL(destination.getUpperBound(), 7);
    BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(destination[2]), 100);
    BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(destination[3]), 200);
    BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(destination[4]), 300);
    BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(destination[5]), 400);
    BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(destination[6]), 500);
    BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(destination[7]), 600);
  }

  BOOST_AUTO_TEST_CASE(VariableToVariableCopyAssignmentArray1To5) {
    CIEC_ARRAY_VARIABLE<CIEC_INT> source(1, 5);
    source[1] = 11_INT;
    source[2] = 22_INT;
    source[3] = 33_INT;
    source[4] = 44_INT;
    source[5] = 55_INT;

    CIEC_ARRAY_VARIABLE<CIEC_INT> destination(1, 5);
    destination = source;

    BOOST_CHECK_EQUAL(destination.getLowerBound(), 1);
    BOOST_CHECK_EQUAL(destination.getUpperBound(), 5);
    BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(destination[1]), 11);
    BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(destination[2]), 22);
    BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(destination[3]), 33);
    BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(destination[4]), 44);
    BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(destination[5]), 55);
  }

  BOOST_AUTO_TEST_CASE(VariableToVariableCopyAssignmentArrayMinus1To1) {
    CIEC_ARRAY_VARIABLE<CIEC_INT> source(-1, 1);
    source[-1] = -10_INT;
    source[0] = -20_INT;
    source[1] = -30_INT;

    CIEC_ARRAY_VARIABLE<CIEC_INT> destination(-1, 1);
    destination = source;

    BOOST_CHECK_EQUAL(destination.getLowerBound(), -1);
    BOOST_CHECK_EQUAL(destination.getUpperBound(), 1);
    BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(destination[-1]), -10);
    BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(destination[0]), -20);
    BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(destination[1]), -30);
  }

  BOOST_AUTO_TEST_CASE(VariableToVariableCopyAssignmentArrayMinus5To5) {
    CIEC_ARRAY_VARIABLE<CIEC_INT> source(-5, 5);
    source[-5] = 1_INT;
    source[-4] = 2_INT;
    source[-3] = 3_INT;
    source[-2] = 4_INT;
    source[-1] = 5_INT;
    source[0] = 6_INT;
    source[1] = 7_INT;
    source[2] = 8_INT;
    source[3] = 9_INT;
    source[4] = 10_INT;
    source[5] = 11_INT;

    CIEC_ARRAY_VARIABLE<CIEC_INT> destination(-5, 5);
    destination = source;

    BOOST_CHECK_EQUAL(destination.getLowerBound(), -5);
    BOOST_CHECK_EQUAL(destination.getUpperBound(), 5);
    BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(destination[-5]), 1);
    BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(destination[-4]), 2);
    BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(destination[-3]), 3);
    BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(destination[-2]), 4);
    BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(destination[-1]), 5);
    BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(destination[0]), 6);
    BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(destination[1]), 7);
    BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(destination[2]), 8);
    BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(destination[3]), 9);
    BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(destination[4]), 10);
    BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(destination[5]), 11);
  }

  BOOST_AUTO_TEST_SUITE_END() // CIEC_ARRAY_TEMPLATE_function_test
} // namespace forte::test

/*******************************************************************************
 * Copyright (c) 2022 Primetals Technologies Austria GmbH
 *               2023 Martin Erich Jobst
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
 *    Martin Jobst
 *      - refactored for common assignment operators
 *      - refactored array type structure
 *******************************************************************************/
#pragma once

#include <stddef.h>
#include <inttypes.h>
#include <initializer_list>

#include "forte/datatypes/forte_array.h"
#include "forte/datatypes/forte_any_int.h"
#include "forte/iec61131_cast_helper.h"
#include "forte/iec61131_functions/func_EQ.h"

namespace forte {
  /** \brief A common supertype for all typed CIEC_ARRAY variants, providing the minimal interface an array must provide
   */
  template<typename T>
  class CIEC_ARRAY_COMMON : public CIEC_ARRAY {
      friend constexpr bool operator==(const CIEC_ARRAY_COMMON &paLeft, const CIEC_ARRAY_COMMON &paRight) {
        if (paLeft.getLowerBound() != paRight.getLowerBound() || paLeft.getUpperBound() != paRight.getUpperBound()) {
          return false;
        }

        for (intmax_t i = paLeft.getLowerBound(), end = paLeft.getUpperBound(); i <= end; ++i) {
          if (!static_cast<const T &>(paLeft[i]).equals(static_cast<const T &>(paRight[i]))) {
            return false;
          }
        }
        return true;
      }

      friend constexpr bool operator!=(const CIEC_ARRAY_COMMON &paLeft, const CIEC_ARRAY_COMMON &paRight) {
        return !(paLeft == paRight);
      }

    public:
      using value_type = T;
      using pointer = value_type *;
      using const_pointer = const value_type *;
      using reference = value_type &;
      using const_reference = const value_type &;

      using CIEC_ARRAY::at;
      using CIEC_ARRAY::operator[];
      using CIEC_ARRAY::operator=;

      [[nodiscard]] constexpr reference at(intmax_t index) override = 0;

      [[nodiscard]] constexpr reference operator[](intmax_t index) override = 0;

      [[nodiscard]] constexpr const_reference at(intmax_t index) const override = 0;

      [[nodiscard]] constexpr const_reference operator[](intmax_t index) const override = 0;

      [[nodiscard]] constexpr reference at(const CIEC_ANY_INT &index) override {
        const intmax_t indexValue = index.getSignedValue();
        return at(indexValue);
      }

      [[nodiscard]] constexpr reference operator[](const CIEC_ANY_INT &index) override {
        const intmax_t indexValue = index.getSignedValue();
        return operator[](indexValue);
      }

      [[nodiscard]] constexpr const_reference at(const CIEC_ANY_INT &index) const override {
        const intmax_t indexValue = index.getSignedValue();
        return at(indexValue);
      }

      [[nodiscard]] constexpr const_reference operator[](const CIEC_ANY_INT &index) const override {
        const intmax_t indexValue = index.getSignedValue();
        return operator[](indexValue);
      }

      constexpr CIEC_ARRAY_COMMON &operator=(std::initializer_list<T> paSource) {
        if (size()) { // check if initialized
          intmax_t begin = getLowerBound();
          intmax_t end = std::min(getUpperBound(), getLowerBound() + static_cast<intmax_t>(paSource.size()) - 1);
          for (intmax_t i = begin; i <= end; ++i) {
            static_cast<T &>((*this)[i]).setValue(paSource.begin()[i - begin]);
          }
        }
        return *this;
      }

      constexpr ~CIEC_ARRAY_COMMON() override = default;

    protected:
      constexpr CIEC_ARRAY_COMMON() = default;
  };

  static_assert(std::is_copy_assignable_v<CIEC_ARRAY_COMMON<CIEC_ANY>>);
  static_assert(std::is_destructible_v<CIEC_ARRAY_COMMON<CIEC_ANY>>);
} // namespace forte

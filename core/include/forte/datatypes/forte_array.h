/*******************************************************************************
 * Copyright (c) 2022, 2025 Primetals Technologies Austria GmbH,
 *                          Martin Erich Jobst
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Martin Melik Merkumians
 *      - initial implementation and rework communication infrastructure
 *   Martin Jobst - refactored for common assignment operators
 *                - refactored array type structure
 *                - add support for setting bounds
 *                - added lower and upper bound with dimension
 *   Markus Meingast, Alois Zoitl  - migrated data type toString to std::string
 *******************************************************************************/
#pragma once

#include <cstddef>
#include <cinttypes>

#include "forte/datatypes/forte_any_derived.h"
#include "forte/datatypes/forte_any_int.h"
#include "forte/stringid.h"
#include "forte/util/devlog.h"

namespace forte {
  /** \brief A common supertype for all CIEC_ARRAY variants, providing the minimal interface an array must provide
   */
  class CIEC_ARRAY : public CIEC_ANY_DERIVED {
      friend constexpr bool operator==(const CIEC_ARRAY &paLeft, const CIEC_ARRAY &paRight) {
        if (paLeft.getLowerBound() != paRight.getLowerBound() || paLeft.getUpperBound() != paRight.getUpperBound()) {
          return false;
        }

        for (intmax_t i = paLeft.getLowerBound(), end = paLeft.getUpperBound(); i <= end; ++i) {
          if (!paLeft[i].equals(paRight[i])) {
            return false;
          }
        }
        return true;
      }

      friend constexpr bool operator!=(const CIEC_ARRAY &paLeft, const CIEC_ARRAY &paRight) {
        return !(paLeft == paRight);
      }

    public:
      using value_type = CIEC_ANY;
      using pointer = value_type *;
      using const_pointer = const value_type *;
      using reference = value_type &;
      using const_reference = const value_type &;

      [[nodiscard]] virtual constexpr intmax_t getLowerBound() const = 0;

      [[nodiscard]] virtual constexpr intmax_t getUpperBound() const = 0;

      [[nodiscard]] virtual constexpr intmax_t getLowerBound(intmax_t paDimension) const = 0;

      [[nodiscard]] virtual constexpr intmax_t getUpperBound(intmax_t paDimension) const = 0;

      [[nodiscard]] virtual constexpr size_t size() const = 0;

      [[nodiscard]] virtual constexpr EDataTypeID getElementDataTypeID() const = 0;

      [[nodiscard]] virtual constexpr StringId getElementTypeNameID() const = 0;

      [[nodiscard]] virtual constexpr reference at(intmax_t index) = 0;

      [[nodiscard]] virtual constexpr reference operator[](intmax_t index) = 0;

      [[nodiscard]] virtual constexpr const_reference at(intmax_t index) const = 0;

      [[nodiscard]] virtual constexpr const_reference operator[](intmax_t index) const = 0;

      bool isIndexInRange(const intmax_t index) const {
        return getLowerBound() <= index && index <= getUpperBound();
      }

      bool isIndexInRange(const CIEC_ANY_INT &index) const {
        return isIndexInRange(index.getSignedValue());
      }

      CIEC_ARRAY &operator=(const CIEC_ARRAY &paSource) {
        assignDynamic(paSource, paSource.getLowerBound(), paSource.getUpperBound());
        return *this;
      }

      [[nodiscard]] virtual constexpr reference at(const CIEC_ANY_INT &index) {
        const intmax_t indexValue = index.getSignedValue();
        return at(indexValue);
      }

      [[nodiscard]] virtual constexpr reference operator[](const CIEC_ANY_INT &index) {
        const intmax_t indexValue = index.getSignedValue();
        return operator[](indexValue);
      }

      [[nodiscard]] virtual constexpr const_reference at(const CIEC_ANY_INT &index) const {
        const intmax_t indexValue = index.getSignedValue();
        return at(indexValue);
      }

      [[nodiscard]] virtual constexpr const_reference operator[](const CIEC_ANY_INT &index) const {
        const intmax_t indexValue = index.getSignedValue();
        return operator[](indexValue);
      }

      [[nodiscard]] constexpr std::partial_ordering compare(const CIEC_ARRAY &paOther) const {
        if (getElementDataTypeID() != paOther.getElementDataTypeID() || getLowerBound() != paOther.getLowerBound() ||
            getUpperBound() != paOther.getUpperBound()) {
          return std::partial_ordering::unordered;
        }

        for (intmax_t i = getLowerBound(), end = getUpperBound(); i <= end; ++i) {
          auto res = (*this)[i].compare(paOther[i]);
          if (res != std::partial_ordering::equivalent) {
            return res;
          }
        }
        return std::partial_ordering::equivalent;
      }

      [[nodiscard]] constexpr std::partial_ordering compare(const CIEC_ANY &paOther) const override {
        if (paOther.getDataTypeID() == e_ARRAY) {
          return compare(static_cast<const CIEC_ARRAY &>(paOther));
        }
        return std::partial_ordering::unordered;
      }

      [[nodiscard]] EDataTypeID getDataTypeID() const final {
        return e_ARRAY;
      }

      StringId getTypeNameID() const override;

      void setValue(const CIEC_ANY &paValue) override;

      [[nodiscard]] virtual bool hasVariableBounds() const = 0;

      virtual void setBounds(intmax_t paLowerBound, intmax_t paUpperBound) = 0;

      virtual void setBounds(const CIEC_ARRAY &paArray) = 0;

      void reset() override;

      [[nodiscard]] bool equals(const CIEC_ANY &paOther) const override {
        if (paOther.getDataTypeID() == e_ARRAY) {
          return *this == static_cast<const CIEC_ARRAY &>(paOther);
        }
        return false;
      }

      void toString(std::string &paTargetBuf) const override;

      constexpr ~CIEC_ARRAY() override = default;

    protected:
      constexpr CIEC_ARRAY() = default;

      static void findNextNonBlankSpace(const char **paRunner) {
        while (' ' == **paRunner) {
          (*paRunner)++;
        }
      }

      static const intmax_t cmCollapseMaxSize = 100;

      constexpr static void
      checkBounds(const intmax_t paIndex, const intmax_t paLowerBound, const intmax_t paUpperBound, bool paValid) {
        if (paIndex < paLowerBound || paIndex > paUpperBound || !paValid) {
          // do not use if consteval { because it needs minimum /std:c++23preview
          if (std::is_constant_evaluated()) {
#if __cpp_exceptions
            throw std::out_of_range("Array index out of bounds");
#else
            struct ConstexprAssert {
                static void array_index_out_of_bounds() {
                }
            };
            ConstexprAssert::array_index_out_of_bounds();
#endif
          } else {
#if __cpp_exceptions
            throw std::out_of_range("Array access to index " + std::to_string(paIndex) + " is out of bounds from " +
                                    std::to_string(paLowerBound) + " to " + std::to_string(paUpperBound));
#else
            DEVLOG_ERROR("Array access to index %" PRIdMAX " is out of bounds from %" PRIdMAX " to %" PRIdMAX "\n",
                         paIndex, paLowerBound, paUpperBound);
            std::abort();
#endif
          }
        }
      }

    private:
      void toCollapsedString(std::string &paTargetBuf) const;

      void toCollapsedElementString(const CIEC_ANY &paElement, size_t paCount, std::string &paTargetBuf) const;

      template<typename U>
      constexpr void assignDynamic(const U &paArray, intmax_t sourceLowerBound, intmax_t sourceUpperBound) {
        if (size() && paArray.size()) { // check if initialized
          intmax_t begin = std::max(getLowerBound(), sourceLowerBound);
          intmax_t end = std::min(getUpperBound(), sourceUpperBound);
          for (intmax_t i = begin; i <= end; ++i) {
            (*this)[i].setValue(paArray[i]);
          }
        }
      }
  };

  static_assert(std::is_copy_assignable_v<CIEC_ARRAY>);
  static_assert(std::is_destructible_v<CIEC_ARRAY>);

  template<>
  struct CDataTypeTrait<CIEC_ARRAY> {
      static constexpr CIEC_ANY::EDataTypeID scmDataTypeId = CIEC_ANY::e_ARRAY;
      static constexpr StringId scmDataTypeName{};
  };
} // namespace forte

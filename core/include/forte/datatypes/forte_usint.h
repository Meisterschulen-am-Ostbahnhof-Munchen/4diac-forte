/*******************************************************************************
 * Copyright (c) 2005 Profactor GmbH, ACIN, fortiss GmbH,
 *                          Primetals Technologies Austria GmbH,
 *                          HR Agrartechnik GmbH, Martin Erich Jobst
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Thomas Strasser, Ingomar Müller, Alois Zoitl, Gerhard Ebenhofer,
 *     Ingo Hegny, Monika Wenger, Martin Melik-Merkumians
 *      - initial implementation and rework communication infrastructure
 *   Martin Melik Merkumians - make TForteUInt8 constructor explicit,
 *        removed built-in type operator=
 *   Martin Jobst - add user-defined literal
 *   Alois Zoitl  - migrated data type toString to std::string
 *   Franz Höpfinger - add constexpr
 *******************************************************************************/

#pragma once

#include "forte/datatypes/forte_any_unsigned.h"
#include <limits>

namespace forte {
  /*!\ingroup COREDTS CIEC_USINT represents the usint data type according to IEC 61131.
   */
  class CIEC_USINT final : public CIEC_ANY_UNSIGNED {
      DECLARE_FIRMWARE_DATATYPE(USINT)

    public:
      using TValueType = TForteUInt8;
      [[deprecated("Please use the corresponding numeric_limits template")]]
      static constexpr size_t scmBitLength = 8U;
      [[deprecated("Please use the corresponding numeric_limits template")]]
      static constexpr TValueType scmMinVal = std::numeric_limits<TValueType>::min();
      [[deprecated("Please use the corresponding numeric_limits template")]]
      static constexpr TValueType scmMaxVal = std::numeric_limits<TValueType>::max();

      constexpr CIEC_USINT() = default;

      constexpr CIEC_USINT(const CIEC_USINT &paValue) : CIEC_ANY_UNSIGNED() {
        setValueSimple(paValue);
      }

      constexpr explicit CIEC_USINT(const CIEC_ANY_INT &paValue) : CIEC_ANY_UNSIGNED() {
        setValueSimple(paValue);
      }

      constexpr explicit CIEC_USINT(const TValueType paValue) {
        setTUINT8(paValue);
      }

      constexpr ~CIEC_USINT() override = default;

      constexpr CIEC_USINT &operator=(const CIEC_USINT &paValue) {
        setValueSimple(paValue);
        return *this;
      }

      template<typename T,
               std::enable_if_t<std::is_same_v<typename mpl::implicit_cast_t<T, CIEC_USINT>, CIEC_USINT>, int> = 0>
      constexpr CIEC_USINT &operator=(const T &paValue) {
        setValueSimple(paValue);
        return *this;
      }

      /*! \brief Converts CIEC_UDINT to elementary unsigned 16 bit integer
       *
       *   Conversion operator for converting CIEC_UDINT to elementary unsigned 16 bit integer
       */
      constexpr explicit operator TForteUInt8() const {
        return getTUINT8();
      }

      constexpr EDataTypeID getDataTypeID() const override {
        return e_USINT;
      }

      using CIEC_ANY_UNSIGNED::compare;

      constexpr std::strong_ordering compare(const CIEC_USINT &paOther) const {
        return static_cast<TValueType>(*this) <=> static_cast<TValueType>(paOther);
      }

      constexpr std::strong_ordering operator<=>(const CIEC_USINT &paOther) const {
        return compare(paOther);
      }

      constexpr bool operator==(const CIEC_USINT &paOther) const {

        return compare(paOther) == std::strong_ordering::equal;
      }
  };

  constexpr CIEC_USINT operator""_USINT(unsigned long long int paValue) {
    return CIEC_USINT(static_cast<CIEC_USINT::TValueType>(paValue));
  }

  template<>
  struct CDataTypeTrait<CIEC_USINT> {
      static constexpr CIEC_ANY::EDataTypeID scmDataTypeId = CIEC_ANY::e_USINT;
      static const StringId scmDataTypeName;
  };
} // namespace forte

namespace std {
  template<>
  struct numeric_limits<forte::CIEC_USINT> : public forte::templates::numeric_limits<forte::CIEC_USINT> {
      static constexpr size_t bitLength = 8U;
  };
} // namespace std

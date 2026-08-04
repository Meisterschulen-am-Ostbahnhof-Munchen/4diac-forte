/*******************************************************************************
 * Copyright (c) 2008 nxtControl GmbH, ACIN, Profactor GmbH, fortiss GmbH,
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
 *   Stanislav Meduna, Alois Zoitl, Gerhard Ebenhofer, Martin Melik Merkumians,
 *     Monika Wenger
 *      - initial implementation and rework communication infrastructure
 *   Martin Melik Merkumians - make TForteUInt64 constructor explicit,
 *      removed built-in type operator=, removed operator++, update timebase to
 *      nanoseconds
 *   Martin Jobst - add user-defined literal
 *   Alois Zoitl  - migrated data type toString to std::string
 *   Franz Höpfinger - add constexpr
 *******************************************************************************/

#pragma once

#include "forte/datatypes/forte_any_date.h"

namespace forte {
  /*!\ingroup COREDTS CIEC_TIME_OF_DAY represents the time data types according to IEC 61131.
   */

  class CIEC_TIME_OF_DAY final : public CIEC_ANY_DATE {
      DECLARE_FIRMWARE_DATATYPE(TIME_OF_DAY)
    public:
      [[deprecated("Please use the corresponding numeric_limits template")]]
      constexpr static size_t scmBitLength = 64U;
      [[deprecated("Please use the corresponding numeric_limits template")]]
      static constexpr TValueType scmMinVal = std::numeric_limits<TValueType>::min();
      [[deprecated("Please use the corresponding numeric_limits template")]]
      static constexpr TValueType scmMaxVal = std::numeric_limits<TValueType>::max();

      constexpr CIEC_TIME_OF_DAY() = default;

      constexpr CIEC_TIME_OF_DAY(const CIEC_TIME_OF_DAY &paValue) : CIEC_ANY_DATE() {
        setValueSimple(paValue);
      }

      constexpr explicit CIEC_TIME_OF_DAY(const TValueType paValue) {
        setTUINT64(paValue);
      }

      constexpr ~CIEC_TIME_OF_DAY() override = default;

      constexpr CIEC_TIME_OF_DAY &operator=(const CIEC_TIME_OF_DAY &paValue) {
        // Simple value assignment - no self assignment check needed
        setValueSimple(paValue);
        return *this;
      }

      /*! \brief Converts CIEC_TIME_OF_DAY to elementary 64 bit unsigned integer
       *
       *   Conversion operator for converting CIEC_TIME_OF_DAY to elementary 64 bit unsigned integer
       */
      constexpr explicit operator TForteUInt64() const {
        return getTUINT64();
      }

      constexpr EDataTypeID getDataTypeID() const override {
        return e_TIME_OF_DAY;
      }

      using CIEC_ANY_DATE::compare;

      constexpr std::strong_ordering compare(const CIEC_TIME_OF_DAY &paOther) const {
        return static_cast<TForteUInt64>(*this) <=> static_cast<TForteUInt64>(paOther);
      }

      constexpr std::strong_ordering operator<=>(const CIEC_TIME_OF_DAY &paOther) const {
        return compare(paOther);
      }

      constexpr bool operator==(const CIEC_TIME_OF_DAY &paOther) const {

        return compare(paOther) == std::strong_ordering::equal;
      }

      /*! \brief Converts string value to data type value
       *
       *   This command implements a conversion function from IEC 61131
       *   data type (string format) to a C++ conform type.
       *   This function is necessary for communication with a proper engineering system.
       *   \param paValue Pointer to the given String
       *   \return Can be the following response:
       *   \return number of bytes taken used from the buffer
       *        -1 on on error
       */
      int fromString(const char *paValue) override;
      /*! \brief Converts data type value to string
       *
       *   This command implements a conversion function from C++ data type
       *   to IEC 61131 conform data type (string format).
       *   This function is necessary for communication with a proper engineering system.
       *   \param paTargetBuf Reference to the buffer String
       */
      void toString(std::string &paTargetBuf) const override;
  };

  constexpr CIEC_TIME_OF_DAY operator""_TIME_OF_DAY(unsigned long long int paValue) {
    return CIEC_TIME_OF_DAY(static_cast<CIEC_TIME_OF_DAY::TValueType>(paValue));
  }

  template<>
  struct CDataTypeTrait<CIEC_TIME_OF_DAY> {
      static constexpr CIEC_ANY::EDataTypeID scmDataTypeId = CIEC_ANY::e_TIME_OF_DAY;
      static const StringId scmDataTypeName;
  };
} // namespace forte

namespace std {
  template<>
  struct numeric_limits<forte::CIEC_TIME_OF_DAY> : public forte::templates::numeric_limits<forte::CIEC_TIME_OF_DAY> {
      static constexpr size_t bitLength = 64U;
  };
} // namespace std

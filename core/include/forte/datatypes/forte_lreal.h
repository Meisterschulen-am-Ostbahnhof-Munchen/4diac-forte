/*******************************************************************************
 * Copyright (c) 2005 Profactor GmbH, ACIN,
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
 *   Thomas Strasser, Ingomar Müller, Alois Zoitl,
 *     Ingo Hegny, Monika Wenger, Martin Melik Merkumians
 *                - initial implementation and rework communication infrastructure
 *   Martin Melik Merkumians - make TForteDFloat constructor explicit,
 *                  adds implicit cast constructor and operator=, removed built-in
 *                  type operator=, added castable CIEC types operator=
 *   Martin Jobst - add equals function
 *                - add user-defined literal
 *   Alois Zoitl  - migrated data type toString to std::string
 *   Franz Höpfinger - add constexpr
 *******************************************************************************/

#pragma once

#include "forte/datatypes/forte_any_real.h"
#include "forte/datatypes/forte_real.h"
#include "forte/datatypes/forte_dint.h"
#include "forte/datatypes/forte_int.h"
#include "forte/datatypes/forte_sint.h"
#include "forte/datatypes/forte_udint.h"
#include "forte/datatypes/forte_uint.h"
#include "forte/datatypes/forte_usint.h"

#include <limits>

namespace forte {
  /*!\ingroup COREDTS CIEC_LREAL represents the lreal data type according to IEC 61131.
   */
  class CIEC_LREAL final : public CIEC_ANY_REAL {
      DECLARE_FIRMWARE_DATATYPE(LREAL)

    public:
      using TValueType = TForteDFloat;
      [[deprecated("Please use the corresponding numeric_limits template")]]
      constexpr static size_t scmBitLength = 64U;
      [[deprecated("Please use the corresponding numeric_limits template")]]
      static constexpr TValueType scmMinVal = std::numeric_limits<TValueType>::min();
      [[deprecated("Please use the corresponding numeric_limits template")]]
      static constexpr TValueType scmMaxVal = std::numeric_limits<TValueType>::max();

      constexpr CIEC_LREAL() = default;

      /* The following constructors and assignment operators use direct static_casts
       * instead of calling the virtual setValue() to enable constexpr support.
       * This is semantically equivalent to the original setValue() logic for these
       * numeric types, as they did not implement additional range or overflow checks.
       */

      constexpr CIEC_LREAL(const CIEC_LREAL &paValue) : CIEC_ANY_REAL() {
        setValueSimple(paValue);
      }

      constexpr CIEC_LREAL(const CIEC_REAL &paValue) : CIEC_ANY_REAL() {
        setTDFLOAT(static_cast<TForteFloat>(paValue));
      }

      constexpr explicit CIEC_LREAL(const CIEC_DINT &paValue) : CIEC_ANY_REAL() {
        setTDFLOAT(static_cast<TForteDFloat>(static_cast<TForteInt32>(paValue)));
      }

      constexpr explicit CIEC_LREAL(const CIEC_INT &paValue) : CIEC_ANY_REAL() {
        setTDFLOAT(static_cast<TForteDFloat>(static_cast<TForteInt16>(paValue)));
      }

      constexpr explicit CIEC_LREAL(const CIEC_SINT &paValue) : CIEC_ANY_REAL() {
        setTDFLOAT(static_cast<TForteDFloat>(static_cast<TForteInt8>(paValue)));
      }

      constexpr explicit CIEC_LREAL(const CIEC_UDINT &paValue) : CIEC_ANY_REAL() {
        setTDFLOAT(static_cast<TForteDFloat>(static_cast<TForteUInt32>(paValue)));
      }

      constexpr explicit CIEC_LREAL(const CIEC_UINT &paValue) : CIEC_ANY_REAL() {
        setTDFLOAT(static_cast<TForteDFloat>(static_cast<TForteUInt16>(paValue)));
      }

      constexpr explicit CIEC_LREAL(const CIEC_USINT &paValue) : CIEC_ANY_REAL() {
        setTDFLOAT(static_cast<TForteDFloat>(static_cast<TForteUInt8>(paValue)));
      }

      constexpr explicit CIEC_LREAL(const TValueType paValue) {
        setTDFLOAT(paValue);
      }

      constexpr ~CIEC_LREAL() override = default;

      constexpr CIEC_LREAL &operator=(const CIEC_LREAL &paValue) {
        // Simple value assignment - no self assignment check needed
        setValueSimple(paValue);
        return *this;
      }

      constexpr CIEC_LREAL &operator=(const CIEC_REAL &paValue) {
        setTDFLOAT(static_cast<TForteFloat>(paValue));
        return *this;
      }

      constexpr CIEC_LREAL &operator=(const CIEC_DINT &paValue) {
        setTDFLOAT(static_cast<TForteDFloat>(static_cast<TForteInt32>(paValue)));
        return *this;
      }

      constexpr CIEC_LREAL &operator=(const CIEC_INT &paValue) {
        setTDFLOAT(static_cast<TForteDFloat>(static_cast<TForteInt16>(paValue)));
        return *this;
      }

      constexpr CIEC_LREAL &operator=(const CIEC_SINT &paValue) {
        setTDFLOAT(static_cast<TForteDFloat>(static_cast<TForteInt8>(paValue)));
        return *this;
      }

      constexpr CIEC_LREAL &operator=(const CIEC_UDINT &paValue) {
        setTDFLOAT(static_cast<TForteDFloat>(static_cast<TForteUInt32>(paValue)));
        return *this;
      }

      constexpr CIEC_LREAL &operator=(const CIEC_UINT &paValue) {
        setTDFLOAT(static_cast<TForteDFloat>(static_cast<TForteUInt16>(paValue)));
        return *this;
      }

      constexpr CIEC_LREAL &operator=(const CIEC_USINT &paValue) {
        setTDFLOAT(static_cast<TForteDFloat>(static_cast<TForteUInt8>(paValue)));
        return *this;
      }

      constexpr CIEC_LREAL operator-() const {
        return CIEC_LREAL(-1.0 * static_cast<TValueType>(*this));
      }

      /*! \brief Converts CIEC_LREAL to elementary 64 bit float
       *
       *   Conversion operator for converting CIEC_LREAL to elementary 64 bit float
       */

      constexpr explicit operator TForteDFloat() const {
        return getTDFLOAT();
      }

      constexpr EDataTypeID getDataTypeID() const override {
        return e_LREAL;
      }

      void setValue(const CIEC_ANY &paValue) override;

      void reset() override {
        setTDFLOAT(0.0);
      }

      /*! \brief Converts string value to data type value
       *
       *   This command implements a conversion function from IEC 61131
       *   data type (string format) to a C++ conform type.
       *   This function is necessary for communication with a proper engineering system.
       *   \param paValue string buffer
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

      [[nodiscard]] bool equals(const CIEC_ANY &paOther) const override {
        if (paOther.getDataTypeID() == e_LREAL) {
          return getTDFLOAT() == static_cast<const CIEC_LREAL &>(paOther).getTDFLOAT();
        }
        return false;
      }

      /*! \brief Casts TForteDFloat values to CIEC data types
       *  \param paSrcValue Source value for the cast
       *  \param paDestValue Destination CIEC object of the cast
       */
      static void castLRealData(const CIEC_LREAL &paSrcValue, CIEC_ANY &paDestValue);
  };

  consteval inline CIEC_LREAL operator""_LREAL(unsigned long long int paValue) {
    return CIEC_LREAL(static_cast<CIEC_LREAL::TValueType>(paValue));
  }

  consteval inline CIEC_LREAL operator""_LREAL(long double paValue) {
    return CIEC_LREAL(static_cast<CIEC_LREAL::TValueType>(paValue));
  }

  template<>
  struct CDataTypeTrait<CIEC_LREAL> {
      static constexpr CIEC_ANY::EDataTypeID scmDataTypeId = CIEC_ANY::e_LREAL;
      static const StringId scmDataTypeName;
  };
} // namespace forte

namespace std {
  template<>
  struct numeric_limits<forte::CIEC_LREAL> : public forte::templates::numeric_limits<forte::CIEC_LREAL> {
      static constexpr size_t bitLength = 64U;
  };
} // namespace std

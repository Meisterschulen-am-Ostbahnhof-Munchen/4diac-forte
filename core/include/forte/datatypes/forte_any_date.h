/*******************************************************************************
 * Copyright (c) 2005 Profactor GmbH, ACIN, nxtControl GmbH, fortiss GmbH
 *                          HR Agrartechnik GmbH
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Thomas Strasser, Ingomar Müller, Martin Melik Merkumians, Alois Zoitl,
 *    Monika Wenger, Stansilav Meduna
 *      - initial implementation and rework communication infrastructure
 *    Franz Höpfinger - add constexpr
 *******************************************************************************/

#pragma once

#include "forte/datatypes/forte_any_elementary.h"
#include "forte/arch/forte_architecture_time.h"

namespace forte {
  /*!\ingroup COREDTS IIEC_ANY_DATE represents any date data types according to IEC 61131.
   */
  class CIEC_ANY_DATE : public CIEC_ANY_ELEMENTARY {
    public:
      typedef TForteUInt64 TValueType;

      constexpr std::strong_ordering compare(const CIEC_ANY_DATE &paOther) const {
        return getTUINT64() <=> paOther.getTUINT64();
      }

      constexpr std::partial_ordering compare(const CIEC_ANY &paOther) const override {
        if (paOther.getDataTypeID() == getDataTypeID()) {
          return compare(static_cast<const CIEC_ANY_DATE &>(paOther));
        }
        return std::partial_ordering::unordered;
      }

      constexpr ~CIEC_ANY_DATE() override = default;

      void reset() override {
        setTUINT64(0);
      }

      /*! Retrieve the current timezone
       *
       * Can be sed to adjust mktime()-related values
       *
       * @return Number of minutes west (negative) or east (positive), not taking DST into consideration
       */
      static TForteInt32 getTimeZoneOffset();

      /*! set the value of the data according to the given values
       *
       */
      bool setDateAndTime(struct tm &paTM, unsigned int paMilliSec);

      /*! retrieve the time struct for the current value stored in this Data variable
       */
      struct tm *getTimeStruct(struct tm *const paTimeStruct) const;

      /*! Get the milliseconds part of the data value
       */
      unsigned int getMilliSeconds() const;

      template<typename T>
        requires std::is_base_of_v<CIEC_ANY_DATE, T>
      friend constexpr bool operator==(const T &paLeft, const T &paRight) {
        return static_cast<typename T::TValueType>(paLeft) == static_cast<typename T::TValueType>(paRight);
      }

      template<typename T>
        requires std::is_base_of_v<CIEC_ANY_DATE, T>
      friend constexpr std::strong_ordering operator<=>(const T &paLeft, const T &paRight) {
        return static_cast<typename T::TValueType>(paLeft) <=> static_cast<typename T::TValueType>(paRight);
      }

    protected:
      constexpr CIEC_ANY_DATE() = default;

    private:
      static TForteInt32 smTimeZoneOffset;
  };

  template<>
  struct is_generic_datatype<CIEC_ANY_DATE> : std::true_type {};
} // namespace forte

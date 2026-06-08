/*******************************************************************************
 * Copyright (c) 2005 Profactor GmbH, ACIN, fortiss GmbH
 *                          HR Agrartechnik GmbH
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Thomas Strasser, Alois Zoitl, Monika Wenger,
 *    Martin Melik Merkumians
 *      - initial implementation and rework communication infrastructure
 *    Franz Höpfinger - add constexpr
 *******************************************************************************/

#pragma once

#include "forte/datatypes/forte_any_num.h"

namespace forte {
  /*!\ingroup COREDTS CIEC_ANY_REAL represents any real data types according to IEC 61131.

   */
  class CIEC_ANY_REAL : public CIEC_ANY_NUM {
    public:
      constexpr ~CIEC_ANY_REAL() override = default;

      virtual constexpr std::partial_ordering compare(const CIEC_ANY_REAL &paOther) const {
        (void) paOther;
        return std::partial_ordering::unordered;
      }

      constexpr std::partial_ordering compare(const CIEC_ANY &paOther) const override {
        if (paOther.getDataTypeID() == getDataTypeID()) {
          return compare(static_cast<const CIEC_ANY_REAL &>(paOther));
        }
        return std::partial_ordering::unordered;
      }

      constexpr std::partial_ordering operator<=>(const CIEC_ANY_REAL &paOther) const {
        return compare(paOther);
      }

      constexpr bool operator==(const CIEC_ANY_REAL &paOther) const {

        return compare(paOther) == std::partial_ordering::equivalent;
      }

    protected:
      constexpr CIEC_ANY_REAL() = default;
  };

  template<>
  struct is_generic_datatype<CIEC_ANY_REAL> : std::true_type {};
} // namespace forte

/*******************************************************************************
 * Copyright (c) 2005 Profactor GmbH, ACIN,
 *                          Johannes Kepler University Linz,
 *                          Primetals Technologies Austria GmbH,
 *                          HR Agrartechnik GmbH
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Thomas Strasser, Ingomar Müller, Alois Zoitl, Monika Wenger,
 *   Martin Melik Merkumians
 *     - initial implementation and rework communication infrastructure
 *   Ernst Blecha - add multibit partial access
 *   Martin Melik Merkumians - changes multibit access index from template
 *      parameter to call argument
 *   Franz Höpfinger - add constexpr
 *******************************************************************************/

#pragma once

#include "forte/datatypes/forte_any_elementary.h"

namespace forte {
  /*!\ingroup COREDTS IIEC_ANY_BIT represents any bit data types according to IEC 61131.
   */
  class CIEC_ANY_BIT : public CIEC_ANY_ELEMENTARY {
    public:
      constexpr ~CIEC_ANY_BIT() override = default;

      virtual constexpr std::strong_ordering compare(const CIEC_ANY_BIT &paOther) const {
        return getLargestUInt() <=> paOther.getLargestUInt();
      }

      constexpr std::partial_ordering compare(const CIEC_ANY &paOther) const override {
        if (paOther.getDataTypeID() == getDataTypeID()) {
          return compare(static_cast<const CIEC_ANY_BIT &>(paOther));
        }
        return std::partial_ordering::unordered;
      }

      constexpr std::strong_ordering operator<=>(const CIEC_ANY_BIT &paOther) const {
        return compare(paOther);
      }

      constexpr bool operator==(const CIEC_ANY_BIT &paOther) const {

        return compare(paOther) == std::strong_ordering::equal;
      }

      void reset() override {
        setTUINT64(0);
      }

    protected:
      constexpr CIEC_ANY_BIT() = default;
  };

  template<>
  struct is_generic_datatype<CIEC_ANY_BIT> : std::true_type {};
} // namespace forte

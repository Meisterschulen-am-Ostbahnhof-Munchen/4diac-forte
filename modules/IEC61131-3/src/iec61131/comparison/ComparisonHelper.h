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

#pragma once

#include <compare>
#include "forte/datatypes/forte_any_elementary_variant.h"
#include "forte/util/devlog.h"
#include "forte/funcbloc.h"

namespace forte::iec61131::comparison {
  class ComparisonHelper {
    public:
      static std::partial_ordering compare(const forte::CIEC_ANY_ELEMENTARY_VARIANT &paIn1,
                                           const forte::CIEC_ANY_ELEMENTARY_VARIANT &paIn2,
                                           const CFunctionBlock &paFB) {
        const auto result = forte::CIEC_ANY_ELEMENTARY_VARIANT::compare(paIn1, paIn2);
        if (result == std::partial_ordering::unordered) {
          if (paIn1.unwrap().getDataTypeID() != paIn2.unwrap().getDataTypeID()) {
            DEVLOG_ERROR("Comparing incompatible types %s and %s in %s!\n", paIn1.unwrap().getTypeNameID().data(),
                         paIn2.unwrap().getTypeNameID().data(),
                         paFB.getFullQualifiedApplicationInstanceName('.').c_str());
          }
        }
        return result;
      }
  };
} // namespace forte::iec61131::comparison

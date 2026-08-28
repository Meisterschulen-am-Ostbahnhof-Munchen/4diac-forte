/*******************************************************************************
 * Copyright (c) 2026 HR Agrartechnik GmbH
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Franz Höpfinger
 *      - initial API and implementation and/or initial documentation
 *******************************************************************************/

#pragma once

#include "forte/genfb.h"
#include "forte/datatypes/forte_any_magnitude_variant.h"
#include "forte/datatypes/forte_bool.h"

namespace forte::SafeArithmetic::arithmetic {
  class GEN_SAFE_ADD final : public CGenFunctionBlock<CFunctionBlock> {
      DECLARE_GENERIC_FIRMWARE_FB(GEN_SAFE_ADD)

    private:
    protected:
      size_t getGenEOOffset() override {
        return 1;
      }

      // OUT and LIMIT_HIT are fixed data outputs (not part of the variable-arity IN1..INn inputs).
      size_t getGenDOOffset() override {
        return 2;
      }

      CEventConnection *getEOConUnchecked(TPortId) override;
      void createGenInputData() override;
      CIEC_ANY *getDI(size_t) override;
      CIEC_ANY *getDO(size_t) override;
      CDataConnection *getDOConUnchecked(const TPortId paIndex) override;

    private:
      static const TEventID scmEventREQID = 0;
      static const TEventID scmEventCNFID = 0;

      void executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) override;

      void readInputData(TEventID paEI) override;
      void writeOutputData(TEventID paEO) override;

      bool createInterfaceSpec(const char *paConfigString, SFBInterfaceSpec &paInterfaceSpec) override;

      CEventConnection conn_CNF;
      std::unique_ptr<CIEC_ANY_MAGNITUDE_VARIANT[]> mGenDIs;
      std::vector<StringId> mDINames;
      CIEC_ANY_MAGNITUDE_VARIANT var_OUT;
      CIEC_BOOL var_LIMIT_HIT;
      COutDataConnection<CIEC_ANY_MAGNITUDE_VARIANT> conn_OUT;
      COutDataConnection<CIEC_BOOL> conn_LIMIT_HIT;

    public:
      GEN_SAFE_ADD(const StringId paInstanceNameId, CFBContainer &paContainer);
      ~GEN_SAFE_ADD() override = default;
  };
} // namespace forte::SafeArithmetic::arithmetic

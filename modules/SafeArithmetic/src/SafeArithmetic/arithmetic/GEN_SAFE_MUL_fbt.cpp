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

#include "forte/SafeArithmetic/arithmetic/GEN_SAFE_MUL_fbt.h"
#include "forte/SafeArithmetic/arithmetic/safe_arithmetic_ops.h"

#include "forte/util/string_utils.h"

using namespace forte::literals;

namespace {
  static const forte::StringId eventInputNames[] = {"REQ"_STRID};
  static const forte::StringId eventOutputNames[] = {"CNF"_STRID};
  static const forte::StringId dataOutputNames[] = {"OUT"_STRID, "LIMIT_HIT"_STRID};

} // namespace

namespace forte::SafeArithmetic::arithmetic {
  DEFINE_GENERIC_FIRMWARE_FB(GEN_SAFE_MUL, "SafeArithmetic::arithmetic::GEN_SAFE_MUL"_STRID)

  GEN_SAFE_MUL::GEN_SAFE_MUL(const StringId paInstanceNameId, CFBContainer &paContainer) :
      CGenFunctionBlock<CFunctionBlock>(paContainer, paInstanceNameId),
      var_LIMIT_HIT(false),
      conn_CNF(*this, 0),
      conn_OUT(*this, 0, var_OUT),
      conn_LIMIT_HIT(*this, 1, var_LIMIT_HIT) {
  }

  void GEN_SAFE_MUL::executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) {
    if (paEIID == scmEventREQID) {
      var_OUT = mGenDIs[0];
      bool limitHit = false;
      for (size_t i = 1; i < getFBInterfaceSpec().getNumDIs(); ++i) {
        var_OUT = std::visit(
            [&limitHit](auto &&paOUT, auto &&paIN) -> CIEC_ANY_NUM_VARIANT {
              using T = std::decay_t<decltype(paOUT)>;
              using U = std::decay_t<decltype(paIN)>;
              using deductedType = typename mpl::get_mul_operator_result_type<T, U>::type;
              if constexpr (!std::is_same<deductedType, mpl::NullType>::value) {
                bool stepLimitHit = false;
                auto result = safe_mul(paOUT, paIN, stepLimitHit);
                limitHit = limitHit || stepLimitHit;
                return result;
              }
              DEVLOG_ERROR("Multiplying incompatible types %s and %s\n", paOUT.getTypeNameID().data(),
                           paIN.getTypeNameID().data());
              return paOUT;
            },
            static_cast<CIEC_ANY_NUM_VARIANT::variant &>(var_OUT),
            static_cast<CIEC_ANY_NUM_VARIANT::variant &>(mGenDIs[i]));
      }
      var_LIMIT_HIT = CIEC_BOOL(limitHit);
      sendOutputEvent(scmEventCNFID, paECET);
    }
  }

  void GEN_SAFE_MUL::readInputData(TEventID) {
    for (TPortId i = 0; i < getFBInterfaceSpec().getNumDIs(); ++i) {
      readData(i, mGenDIs[i], mGenDIConns[i]);
    }
  }

  void GEN_SAFE_MUL::writeOutputData(TEventID) {
    writeData(getFBInterfaceSpec().getNumDIs() + 0, var_OUT, conn_OUT);
    writeData(getFBInterfaceSpec().getNumDIs() + 1, var_LIMIT_HIT, conn_LIMIT_HIT);
  }

  bool GEN_SAFE_MUL::createInterfaceSpec(const char *paConfigString, SFBInterfaceSpec &paInterfaceSpec) {
    const char *pcPos = strrchr(paConfigString, '_');
    if (pcPos == nullptr) {
      return false;
    }

    pcPos++;
    // we have an underscore and it is the last underscore, followed by the arity
    unsigned int numDIs = static_cast<unsigned int>(util::strtoul(pcPos, nullptr, 10));
    DEVLOG_DEBUG("DIs: %d;\n", numDIs);

    if (numDIs < 2) {
      return false;
    }

    generateGenericInterfacePointNameArray("IN", mDINames, numDIs);

    paInterfaceSpec.mEINames = eventInputNames;
    paInterfaceSpec.mEONames = eventOutputNames;
    paInterfaceSpec.mDINames = mDINames;
    paInterfaceSpec.mDONames = dataOutputNames;

    return true;
  }

  CEventConnection *GEN_SAFE_MUL::getEOConUnchecked(TPortId paEONum) {
    return (paEONum == 0) ? &conn_CNF : nullptr;
  }

  void GEN_SAFE_MUL::createGenInputData() {
    mGenDIs = std::make_unique<CIEC_ANY_NUM_VARIANT[]>(getFBInterfaceSpec().getNumDIs());
  }

  CIEC_ANY *GEN_SAFE_MUL::getDI(size_t paDINum) {
    return &mGenDIs[paDINum];
  }

  CIEC_ANY *GEN_SAFE_MUL::getDO(size_t paDONum) {
    switch (paDONum) {
      case 0: return &var_OUT;
      case 1: return &var_LIMIT_HIT;
    }
    return nullptr;
  }

  CDataConnection *GEN_SAFE_MUL::getDOConUnchecked(const TPortId paDONum) {
    switch (paDONum) {
      case 0: return &conn_OUT;
      case 1: return &conn_LIMIT_HIT;
    }
    return nullptr;
  }
} // namespace forte::SafeArithmetic::arithmetic

/*************************************************************************
 *** FORTE Library Element
 ***
 *** This file was generated using the 4DIAC FORTE Export Filter 3.1.100.202604012115!
 ***
 *** Name: F_LEN_ARRAY
 *** Description: Returns the length of the dimension of the given array
 *** Version:
 ***     1.1: 2026-04-11/Franz Höpfinger - Demmler Andreas Fahrzeugbau - make same as F_nn_BOUND
 ***     1.0: 2026-03-25/Martin Melik Merkumians -  -
 *************************************************************************/

#pragma once

#include "forte/simplefb.h"
#include "forte/datatypes/ANY_DERIVED_dtp.h"
#include "forte/datatypes/forte_any_int_variant.h"
#include "forte/forte_st_util.h"

namespace forte::eclipse4diac::utils::arrays {
  class FORTE_F_LEN_ARRAY final : public CSimpleFB {
      DECLARE_FIRMWARE_FB(FORTE_F_LEN_ARRAY)

    private:
      static const TEventID scmEventCNFID = 0;
      static const TEventID scmEventREQID = 0;

      CIEC_ANY *getVarInternal(size_t) override;

      void alg_REQ(void);

      void enterStateREQ(CEventChainExecutionThread *const paECET);

      void executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) override;

      void readInputData(TEventID paEIID) override;
      void writeOutputData(TEventID paEIID) override;
      void setInitialValues() override;

    public:
      FORTE_F_LEN_ARRAY(StringId paInstanceNameId, CFBContainer &paContainer);

      CIEC_ANY_DERIVED_VARIANT var_ARR;
      CIEC_ANY_INT_VARIANT var_DIM;

      CIEC_ANY_INT_VARIANT var_OUT;

      CEventConnection conn_CNF;

      CDataConnection *conn_ARR;
      CDataConnection *conn_DIM;

      COutDataConnection<CIEC_ANY_INT_VARIANT> conn_OUT;

      CIEC_ANY *getDI(size_t) override;
      CIEC_ANY *getDO(size_t) override;
      CEventConnection *getEOConUnchecked(TPortId) override;
      CDataConnection **getDIConUnchecked(TPortId) override;
      CDataConnection *getDOConUnchecked(TPortId) override;

      void evt_REQ(const CIEC_ANY_DERIVED_VARIANT &paARR, const CIEC_ANY_INT_VARIANT &paDIM, COutputParameter<CIEC_ANY_INT_VARIANT> paOUT) {
        COutputGuard guard_OUT(paOUT);
        var_ARR = paARR;
        var_DIM = paDIM;
        executeEvent(scmEventREQID, nullptr);
        paOUT->setValue(var_OUT.unwrap());
      }

      void operator()(const CIEC_ANY_DERIVED_VARIANT &paARR, const CIEC_ANY_INT_VARIANT &paDIM, COutputParameter<CIEC_ANY_INT_VARIANT> paOUT) {
        evt_REQ(std::forward<const CIEC_ANY_DERIVED_VARIANT &>(paARR), std::forward<const CIEC_ANY_INT_VARIANT &>(paDIM), std::forward<COutputParameter<CIEC_ANY_INT_VARIANT>>(paOUT));
      }
  };
}


/*************************************************************************
 *** FORTE Library Element
 ***
 *** This file was generated using the 4DIAC FORTE Export Filter 3.1.100.202604172003!
 ***
 *** Name: GPIOChip_QX
 *** Description: Output service interface function block for boolean output data
 *** Version:
 ***     3.0: 2025-04-14/Patrick Aigner -  - changed package
 ***     1.2: 2015-06-10/Monika Wenger - fortiss GmbH -
 ***     1.1: 2014-08-30/Alois Zoitl - fortiss GmbH -
 ***     1.0: 2014-08-26/Waldemar Eisenmenger - fortiss GmbH -
 *************************************************************************/

#pragma once

#include "forte/funcbloc.h"
#include "forte/datatypes/forte_bool.h"
#include "forte/datatypes/forte_string.h"
#include "forte/datatypes/forte_uint.h"
#include "forte/forte_st_util.h"

namespace forte::eclipse4diac::io::gpiochip {
  class FORTE_GPIOChip_QX final : public CFunctionBlock {
      DECLARE_FIRMWARE_FB(FORTE_GPIOChip_QX)

    private:
      static const TEventID scmEventINITOID = 0;
      static const TEventID scmEventCNFID = 1;
      static const TEventID scmEventINITID = 0;
      static const TEventID scmEventREQID = 1;

      void executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) override;

      void readInputData(TEventID paEIID) override;
      void writeOutputData(TEventID paEIID) override;
      void setInitialValues() override;

    public:
      FORTE_GPIOChip_QX(StringId paInstanceNameId, CFBContainer &paContainer);

      CIEC_BOOL var_QI;
      CIEC_STRING var_PARAMS;
      CIEC_BOOL var_OUT;
      CIEC_UINT var_ChipNumber;
      CIEC_UINT var_LineNumber;
      CIEC_UINT var_ReadWriteMode;
      CIEC_UINT var_BiasMode;
      CIEC_BOOL var_ActiveLow;

      CIEC_BOOL var_QO;
      CIEC_STRING var_STATUS;

      CEventConnection conn_INITO;
      CEventConnection conn_CNF;

      CDataConnection *conn_QI;
      CDataConnection *conn_PARAMS;
      CDataConnection *conn_OUT;
      CDataConnection *conn_ChipNumber;
      CDataConnection *conn_LineNumber;
      CDataConnection *conn_ReadWriteMode;
      CDataConnection *conn_BiasMode;
      CDataConnection *conn_ActiveLow;

      COutDataConnection<CIEC_BOOL> conn_QO;
      COutDataConnection<CIEC_STRING> conn_STATUS;

      CIEC_ANY *getDI(size_t) override;
      CIEC_ANY *getDO(size_t) override;
      CEventConnection *getEOConUnchecked(TPortId) override;
      CDataConnection **getDIConUnchecked(TPortId) override;
      CDataConnection *getDOConUnchecked(TPortId) override;

      void evt_INIT(const CIEC_BOOL &paQI, const CIEC_STRING &paPARAMS, const CIEC_BOOL &paOUT, const CIEC_UINT &paChipNumber, const CIEC_UINT &paLineNumber, const CIEC_UINT &paReadWriteMode, const CIEC_UINT &paBiasMode, const CIEC_BOOL &paActiveLow, CAnyBitOutputParameter<CIEC_BOOL> paQO, COutputParameter<CIEC_STRING> paSTATUS) {
        COutputGuard guard_QO(paQO);
        COutputGuard guard_STATUS(paSTATUS);
        var_QI = paQI;
        var_PARAMS = paPARAMS;
        var_OUT = paOUT;
        var_ChipNumber = paChipNumber;
        var_LineNumber = paLineNumber;
        var_ReadWriteMode = paReadWriteMode;
        var_BiasMode = paBiasMode;
        var_ActiveLow = paActiveLow;
        executeEvent(scmEventINITID, nullptr);
        *paQO = var_QO;
        *paSTATUS = var_STATUS;
      }

      void evt_REQ(const CIEC_BOOL &paQI, const CIEC_STRING &paPARAMS, const CIEC_BOOL &paOUT, const CIEC_UINT &paChipNumber, const CIEC_UINT &paLineNumber, const CIEC_UINT &paReadWriteMode, const CIEC_UINT &paBiasMode, const CIEC_BOOL &paActiveLow, CAnyBitOutputParameter<CIEC_BOOL> paQO, COutputParameter<CIEC_STRING> paSTATUS) {
        COutputGuard guard_QO(paQO);
        COutputGuard guard_STATUS(paSTATUS);
        var_QI = paQI;
        var_PARAMS = paPARAMS;
        var_OUT = paOUT;
        var_ChipNumber = paChipNumber;
        var_LineNumber = paLineNumber;
        var_ReadWriteMode = paReadWriteMode;
        var_BiasMode = paBiasMode;
        var_ActiveLow = paActiveLow;
        executeEvent(scmEventREQID, nullptr);
        *paQO = var_QO;
        *paSTATUS = var_STATUS;
      }
  };
}


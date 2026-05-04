/*************************************************************************
 *** FORTE Library Element
 ***
 *** This file was generated using the 4DIAC FORTE Export Filter 3.1.100.202604172003!
 ***
 *** Name: GPIOChip_IX
 *** Description: Input service interface function block for boolean input data
 *** Version:
 ***     3.0: 2025-04-14/Patrick Aigner -  - changed package
 ***     1.2: 2015-06-10/Monika Wenger - fortiss GmbH -
 ***     1.1: 2014-08-30/Alois Zoitl - fortiss GmbH -
 ***     1.0: 2014-08-26/Waldemar Eisenmenger - fortiss GmbH -
 *************************************************************************/

#include "forte/eclipse4diac/io/gpiochip/GPIOChip_IX_fbt.h"

#include "forte/datatypes/forte_bool.h"
#include "forte/datatypes/forte_uint.h"
#include "forte/forte_st_util.h"

using namespace std::literals;
using namespace forte::literals;

namespace forte::eclipse4diac::io::gpiochip {
  namespace {
    constexpr std::string_view TypeHash =""sv;

    const auto cEventInputNames = std::array{"INIT"_STRID, "REQ"_STRID};
    const auto cEventInputTypeIds = std::array{"EInit"_STRID, "Event"_STRID};
    const auto cEventOutputNames = std::array{"INITO"_STRID, "CNF"_STRID, "IND"_STRID};
    const auto cEventOutputTypeIds = std::array{"EInit"_STRID, "Event"_STRID, "Event"_STRID};
    const auto cDataInputNames = std::array{"QI"_STRID, "PARAMS"_STRID, "ChipNumber"_STRID, "LineNumber"_STRID, "ReadWriteMode"_STRID, "BiasMode"_STRID, "ActiveLow"_STRID};
    const auto cDataOutputNames = std::array{"QO"_STRID, "STATUS"_STRID, "IN"_STRID};
    const SFBInterfaceSpec cFBInterfaceSpec = {
        .mEINames = cEventInputNames,
        .mEITypeNames = cEventInputTypeIds,
        .mEONames = cEventOutputNames,
        .mEOTypeNames = cEventOutputTypeIds,
        .mDINames = cDataInputNames,
        .mDONames = cDataOutputNames,
        .mDIONames = {},
        .mSocketNames = {},
        .mPlugNames = {},
    };
  }

  DEFINE_FIRMWARE_FB(FORTE_GPIOChip_IX, "eclipse4diac::io::gpiochip::GPIOChip_IX"_STRID, TypeHash)

  FORTE_GPIOChip_IX::FORTE_GPIOChip_IX(const StringId paInstanceNameId, CFBContainer &paContainer) :
      CFunctionBlock(paContainer, cFBInterfaceSpec, paInstanceNameId),
      var_QI(0_BOOL),
      var_PARAMS(""_STRING),
      var_ChipNumber(0_UINT),
      var_LineNumber(0_UINT),
      var_ReadWriteMode(0_UINT),
      var_BiasMode(0_UINT),
      var_ActiveLow(0_BOOL),
      var_QO(0_BOOL),
      var_STATUS(""_STRING),
      var_IN(0_BOOL),
      conn_INITO(*this, 0),
      conn_CNF(*this, 1),
      conn_IND(*this, 2),
      conn_QI(nullptr),
      conn_PARAMS(nullptr),
      conn_ChipNumber(nullptr),
      conn_LineNumber(nullptr),
      conn_ReadWriteMode(nullptr),
      conn_BiasMode(nullptr),
      conn_ActiveLow(nullptr),
      conn_QO(*this, 0, var_QO),
      conn_STATUS(*this, 1, var_STATUS),
      conn_IN(*this, 2, var_IN) {
  };

  void FORTE_GPIOChip_IX::setInitialValues() {
    CFunctionBlock::setInitialValues();
    var_QI = 0_BOOL;
    var_PARAMS = ""_STRING;
    var_ChipNumber = 0_UINT;
    var_LineNumber = 0_UINT;
    var_ReadWriteMode = 0_UINT;
    var_BiasMode = 0_UINT;
    var_ActiveLow = 0_BOOL;
    var_QO = 0_BOOL;
    var_STATUS = ""_STRING;
    var_IN = 0_BOOL;
  }

  void FORTE_GPIOChip_IX::executeEvent(const TEventID paEIID, CEventChainExecutionThread *const paECET) {
    switch(paEIID) {
      case scmEventINITID:
        #error add code for INIT event!
        /*
          do not forget to send output event, calling e.g.
            sendOutputEvent(scmEventCNFID, paECET);
        */
        break;
      case scmEventREQID:
        #error add code for REQ event!
        /*
          do not forget to send output event, calling e.g.
            sendOutputEvent(scmEventCNFID, paECET);
        */
        break;
    }
  }

  void FORTE_GPIOChip_IX::readInputData(const TEventID paEIID) {
    switch(paEIID) {
      case scmEventINITID: {
        readData(0, var_QI, conn_QI);
        readData(1, var_PARAMS, conn_PARAMS);
        readData(2, var_ChipNumber, conn_ChipNumber);
        readData(3, var_LineNumber, conn_LineNumber);
        readData(4, var_ReadWriteMode, conn_ReadWriteMode);
        readData(5, var_BiasMode, conn_BiasMode);
        readData(6, var_ActiveLow, conn_ActiveLow);
        break;
      }
      case scmEventREQID: {
        readData(0, var_QI, conn_QI);
        break;
      }
      default:
        break;
    }
  }

  void FORTE_GPIOChip_IX::writeOutputData(const TEventID paEIID) {
    switch(paEIID) {
      case scmEventINITOID: {
        writeData(7, var_QO, conn_QO);
        writeData(8, var_STATUS, conn_STATUS);
        break;
      }
      case scmEventCNFID: {
        writeData(7, var_QO, conn_QO);
        writeData(8, var_STATUS, conn_STATUS);
        writeData(9, var_IN, conn_IN);
        break;
      }
      case scmEventINDID: {
        writeData(7, var_QO, conn_QO);
        writeData(8, var_STATUS, conn_STATUS);
        writeData(9, var_IN, conn_IN);
        break;
      }
      default:
        break;
    }
  }

  CIEC_ANY *FORTE_GPIOChip_IX::getDI(const size_t paIndex) {
    switch(paIndex) {
      case 0: return &var_QI;
      case 1: return &var_PARAMS;
      case 2: return &var_ChipNumber;
      case 3: return &var_LineNumber;
      case 4: return &var_ReadWriteMode;
      case 5: return &var_BiasMode;
      case 6: return &var_ActiveLow;
    }
    return nullptr;
  }

  CIEC_ANY *FORTE_GPIOChip_IX::getDO(const size_t paIndex) {
    switch(paIndex) {
      case 0: return &var_QO;
      case 1: return &var_STATUS;
      case 2: return &var_IN;
    }
    return nullptr;
  }

  CEventConnection *FORTE_GPIOChip_IX::getEOConUnchecked(const TPortId paIndex) {
    switch(paIndex) {
      case 0: return &conn_INITO;
      case 1: return &conn_CNF;
      case 2: return &conn_IND;
    }
    return nullptr;
  }

  CDataConnection **FORTE_GPIOChip_IX::getDIConUnchecked(const TPortId paIndex) {
    switch(paIndex) {
      case 0: return &conn_QI;
      case 1: return &conn_PARAMS;
      case 2: return &conn_ChipNumber;
      case 3: return &conn_LineNumber;
      case 4: return &conn_ReadWriteMode;
      case 5: return &conn_BiasMode;
      case 6: return &conn_ActiveLow;
    }
    return nullptr;
  }

  CDataConnection *FORTE_GPIOChip_IX::getDOConUnchecked(const TPortId paIndex) {
    switch(paIndex) {
      case 0: return &conn_QO;
      case 1: return &conn_STATUS;
      case 2: return &conn_IN;
    }
    return nullptr;
  }

}
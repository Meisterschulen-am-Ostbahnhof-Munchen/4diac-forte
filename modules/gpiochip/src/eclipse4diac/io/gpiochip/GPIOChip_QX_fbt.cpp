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

#include "forte/eclipse4diac/io/gpiochip/GPIOChip_QX_fbt.h"

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
    const auto cEventOutputNames = std::array{"INITO"_STRID, "CNF"_STRID};
    const auto cEventOutputTypeIds = std::array{"EInit"_STRID, "Event"_STRID};
    const auto cDataInputNames = std::array{"QI"_STRID, "PARAMS"_STRID, "OUT"_STRID, "ChipNumber"_STRID, "LineNumber"_STRID, "ReadWriteMode"_STRID, "BiasMode"_STRID, "ActiveLow"_STRID};
    const auto cDataOutputNames = std::array{"QO"_STRID, "STATUS"_STRID};
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

  DEFINE_FIRMWARE_FB(FORTE_GPIOChip_QX, "eclipse4diac::io::gpiochip::GPIOChip_QX"_STRID, TypeHash)

  FORTE_GPIOChip_QX::FORTE_GPIOChip_QX(const StringId paInstanceNameId, CFBContainer &paContainer) :
      CFunctionBlock(paContainer, cFBInterfaceSpec, paInstanceNameId),
      var_QI(0_BOOL),
      var_PARAMS(""_STRING),
      var_OUT(0_BOOL),
      var_ChipNumber(0_UINT),
      var_LineNumber(0_UINT),
      var_ReadWriteMode(0_UINT),
      var_BiasMode(0_UINT),
      var_ActiveLow(0_BOOL),
      var_QO(0_BOOL),
      var_STATUS(""_STRING),
      conn_INITO(*this, 0),
      conn_CNF(*this, 1),
      conn_QI(nullptr),
      conn_PARAMS(nullptr),
      conn_OUT(nullptr),
      conn_ChipNumber(nullptr),
      conn_LineNumber(nullptr),
      conn_ReadWriteMode(nullptr),
      conn_BiasMode(nullptr),
      conn_ActiveLow(nullptr),
      conn_QO(*this, 0, var_QO),
      conn_STATUS(*this, 1, var_STATUS) {
  };

  void FORTE_GPIOChip_QX::setInitialValues() {
    CFunctionBlock::setInitialValues();
    var_QI = 0_BOOL;
    var_PARAMS = ""_STRING;
    var_OUT = 0_BOOL;
    var_ChipNumber = 0_UINT;
    var_LineNumber = 0_UINT;
    var_ReadWriteMode = 0_UINT;
    var_BiasMode = 0_UINT;
    var_ActiveLow = 0_BOOL;
    var_QO = 0_BOOL;
    var_STATUS = ""_STRING;
  }

  void FORTE_GPIOChip_QX::executeEvent(const TEventID paEIID, CEventChainExecutionThread *const paECET) {
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

  void FORTE_GPIOChip_QX::readInputData(const TEventID paEIID) {
    switch(paEIID) {
      case scmEventINITID: {
        readData(0, var_QI, conn_QI);
        readData(1, var_PARAMS, conn_PARAMS);
        readData(3, var_ChipNumber, conn_ChipNumber);
        readData(4, var_LineNumber, conn_LineNumber);
        readData(5, var_ReadWriteMode, conn_ReadWriteMode);
        readData(6, var_BiasMode, conn_BiasMode);
        readData(7, var_ActiveLow, conn_ActiveLow);
        break;
      }
      case scmEventREQID: {
        readData(0, var_QI, conn_QI);
        readData(2, var_OUT, conn_OUT);
        break;
      }
      default:
        break;
    }
  }

  void FORTE_GPIOChip_QX::writeOutputData(const TEventID paEIID) {
    switch(paEIID) {
      case scmEventINITOID: {
        writeData(8, var_QO, conn_QO);
        writeData(9, var_STATUS, conn_STATUS);
        break;
      }
      case scmEventCNFID: {
        writeData(8, var_QO, conn_QO);
        writeData(9, var_STATUS, conn_STATUS);
        break;
      }
      default:
        break;
    }
  }

  CIEC_ANY *FORTE_GPIOChip_QX::getDI(const size_t paIndex) {
    switch(paIndex) {
      case 0: return &var_QI;
      case 1: return &var_PARAMS;
      case 2: return &var_OUT;
      case 3: return &var_ChipNumber;
      case 4: return &var_LineNumber;
      case 5: return &var_ReadWriteMode;
      case 6: return &var_BiasMode;
      case 7: return &var_ActiveLow;
    }
    return nullptr;
  }

  CIEC_ANY *FORTE_GPIOChip_QX::getDO(const size_t paIndex) {
    switch(paIndex) {
      case 0: return &var_QO;
      case 1: return &var_STATUS;
    }
    return nullptr;
  }

  CEventConnection *FORTE_GPIOChip_QX::getEOConUnchecked(const TPortId paIndex) {
    switch(paIndex) {
      case 0: return &conn_INITO;
      case 1: return &conn_CNF;
    }
    return nullptr;
  }

  CDataConnection **FORTE_GPIOChip_QX::getDIConUnchecked(const TPortId paIndex) {
    switch(paIndex) {
      case 0: return &conn_QI;
      case 1: return &conn_PARAMS;
      case 2: return &conn_OUT;
      case 3: return &conn_ChipNumber;
      case 4: return &conn_LineNumber;
      case 5: return &conn_ReadWriteMode;
      case 6: return &conn_BiasMode;
      case 7: return &conn_ActiveLow;
    }
    return nullptr;
  }

  CDataConnection *FORTE_GPIOChip_QX::getDOConUnchecked(const TPortId paIndex) {
    switch(paIndex) {
      case 0: return &conn_QO;
      case 1: return &conn_STATUS;
    }
    return nullptr;
  }

}
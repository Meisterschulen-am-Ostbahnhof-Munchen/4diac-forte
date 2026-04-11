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

#include "forte/eclipse4diac/utils/arrays/F_LEN_ARRAY_fbt.h"

#include "forte/datatypes/ANY_DERIVED_dtp.h"
#include "forte/datatypes/forte_any_int_variant.h"
#include "forte/datatypes/forte_dint.h"
#include "forte/forte_st_util.h"
#include "forte/iec61131_functions/func_LOWER_BOUND.h"
#include "forte/iec61131_functions/func_SUB.h"
#include "forte/iec61131_functions/func_UPPER_BOUND.h"

using namespace std::literals;
using namespace forte::literals;

namespace forte::eclipse4diac::utils::arrays {
  namespace {
    constexpr std::string_view TypeHash =""sv;

    const auto cEventInputNames = std::array{"REQ"_STRID};
    const auto cEventOutputNames = std::array{"CNF"_STRID};
    const auto cDataInputNames = std::array{"ARR"_STRID, "DIM"_STRID};
    const auto cDataOutputNames = std::array{"OUT"_STRID};
    const SFBInterfaceSpec cFBInterfaceSpec = {
        .mEINames = cEventInputNames,
        .mEITypeNames = {},
        .mEONames = cEventOutputNames,
        .mEOTypeNames = {},
        .mDINames = cDataInputNames,
        .mDONames = cDataOutputNames,
        .mDIONames = {},
        .mSocketNames = {},
        .mPlugNames = {},
    };
  }

  DEFINE_FIRMWARE_FB(FORTE_F_LEN_ARRAY, "eclipse4diac::utils::arrays::F_LEN_ARRAY"_STRID, TypeHash)

  FORTE_F_LEN_ARRAY::FORTE_F_LEN_ARRAY(const StringId paInstanceNameId, CFBContainer &paContainer) :
      CSimpleFB(paContainer, cFBInterfaceSpec, paInstanceNameId, {}),
      var_ARR(CIEC_ANY_DERIVED_VARIANT()),
      var_DIM(1_DINT),
      var_OUT(CIEC_ANY_INT_VARIANT()),
      conn_CNF(*this, 0),
      conn_ARR(nullptr),
      conn_DIM(nullptr),
      conn_OUT(*this, 0, var_OUT) {
  }

  void FORTE_F_LEN_ARRAY::setInitialValues() {
    CSimpleFB::setInitialValues();
    var_ARR = CIEC_ANY_DERIVED_VARIANT();
    var_DIM = 1_DINT;
    var_OUT = CIEC_ANY_INT_VARIANT();
  }

  void FORTE_F_LEN_ARRAY::executeEvent(const TEventID paEIID, CEventChainExecutionThread *const paECET) {
    switch(paEIID) {
      case scmEventREQID:
        enterStateREQ(paECET);
        break;
      default:
        break;
    }
  }

  void FORTE_F_LEN_ARRAY::enterStateREQ(CEventChainExecutionThread *const paECET) {
    alg_REQ();
    sendOutputEvent(scmEventCNFID, paECET);
  }

  void FORTE_F_LEN_ARRAY::readInputData(const TEventID paEIID) {
    switch(paEIID) {
      case scmEventREQID: {
        readData(0, var_ARR, conn_ARR);
        readData(1, var_DIM, conn_DIM);
        break;
      }
      default:
        break;
    }
  }

  void FORTE_F_LEN_ARRAY::writeOutputData(const TEventID paEIID) {
    switch(paEIID) {
      case scmEventCNFID: {
        writeData(2, var_OUT, conn_OUT);
        break;
      }
      default:
        break;
    }
  }

  CIEC_ANY *FORTE_F_LEN_ARRAY::getDI(const size_t paIndex) {
    switch(paIndex) {
      case 0: return &var_ARR;
      case 1: return &var_DIM;
    }
    return nullptr;
  }

  CIEC_ANY *FORTE_F_LEN_ARRAY::getDO(const size_t paIndex) {
    switch(paIndex) {
      case 0: return &var_OUT;
    }
    return nullptr;
  }

  CEventConnection *FORTE_F_LEN_ARRAY::getEOConUnchecked(const TPortId paIndex) {
    switch(paIndex) {
      case 0: return &conn_CNF;
    }
    return nullptr;
  }

  CDataConnection **FORTE_F_LEN_ARRAY::getDIConUnchecked(const TPortId paIndex) {
    switch(paIndex) {
      case 0: return &conn_ARR;
      case 1: return &conn_DIM;
    }
    return nullptr;
  }

  CDataConnection *FORTE_F_LEN_ARRAY::getDOConUnchecked(const TPortId paIndex) {
    switch(paIndex) {
      case 0: return &conn_OUT;
    }
    return nullptr;
  }

  CIEC_ANY *FORTE_F_LEN_ARRAY::getVarInternal(size_t) {
    return nullptr;
  }

  void FORTE_F_LEN_ARRAY::alg_REQ(void) {

    #line 2 "F_LEN_ARRAY.fbt"
    var_OUT = func_SUB(func_UPPER_BOUND<CIEC_ANY_INT_VARIANT>(var_ARR, var_DIM), func_LOWER_BOUND<CIEC_ANY_INT_VARIANT>(var_ARR, var_DIM));
  }

}
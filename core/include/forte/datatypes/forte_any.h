/*******************************************************************************
 * Copyright (c) 2005 Profactor GmbH, ACIN, nxtcontrol GmbH, fortiss GmbH
 *                          TU Vienna/ACIN, Martin Erich Jobst,
 *                          Martin Melik Merkumians,
 *                          Primetals Technologies Austria
 *                          HR Agrartechnik GmbH
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Thomas Strasser, Ingomar Müller, Alois Zoitl, Gerhard Ebenhofer,
 *     Ingo Hegny, Martin Melik Merkumians, Stanislav Meduna, Monika Wenger
 *                - initial implementation and rework communication infrastructure
 *   Martin Melik Merkumians - templated cast factory function
 *   Martin Jobst - add equals function
 *                - add support for data types with different size
 *   Martin Melik Merkumians - Add specialized numeric_limits for IEC types
 *   Alois Zoitl  - migrated data type toString to std::string
 *   Franz Höpfinger - add constexpr
 *******************************************************************************/

#pragma once

#include <cstddef>
#include <bit>
#include <compare>
#include <span>
#include <limits>
#include <type_traits>
#include <cassert>
#include "forte/typelib.h"
#include "forte/iec61131_cast_helper.h"
#include "forte/config/forte_config.h"

/*!\ingroup COREDTS  CIEC_ANY represents the IEC_ANY data type according to IEC 61131.
 */

/*!\ingroup CORE \defgroup COREDTS Core Data Types
 * \brief Implementation of the core IEC 61131-3 data types.
 *
 */

namespace forte {
  class CIEC_ANY_REAL;
  class CIEC_ANY_BIT;

  class CIEC_ANY {
    public:
      /*! \ingroup COREDTS\brief Data type ID's are chosen according to IEC 61499-1 : Function Block
       *  -- Part 1 Architecture - Annex F (informative) Information exchange/F.3
       *  Transfer syntaxes (page 89).
       */
      enum EDataTypeID {
        e_ANY,
        e_BOOL,
        e_SINT,
        e_INT,
        e_DINT,
        e_LINT,
        e_USINT,
        e_UINT,
        e_UDINT,
        e_ULINT,
        e_BYTE,
        e_WORD,
        e_DWORD,
        e_LWORD,
        e_DATE,
        e_TIME_OF_DAY,
        e_DATE_AND_TIME,
        e_TIME,
        e_CHAR,
        e_WCHAR,
        e_LDATE,
        e_LTIME_OF_DAY,
        e_LDATE_AND_TIME,
        e_LTIME, // until here mem-copiable data types
        e_REAL,
        e_LREAL,
        e_STRING,
        e_WSTRING,
        e_DerivedData,
        e_DirectlyDerivedData,
        e_EnumeratedData,
        e_SubrangeData,
        e_ARRAY, // according to the compliance profile
        e_STRUCT,
        e_External = 256, // Base for CIEC_ANY based types outside of the forte base
        e_Max = 65535 // Guarantees at least 16 bits - otherwise gcc will optimizes on some platforms
      };

      typedef TForteUInt64 TLargestUIntValueType;
      typedef TForteInt64 TLargestIntValueType;

      // Enforce type width and properties for canonical storage
      static_assert(sizeof(TLargestUIntValueType) == 8, "TLargestUIntValueType must be 64-bit");
      static_assert(sizeof(TLargestIntValueType) == 8, "TLargestIntValueType must be 64-bit");
      static_assert(sizeof(TLargestUIntValueType) == sizeof(TLargestIntValueType),
                    "Largest integer types must have the same size for UAnyData");

      // Ensure canonical storage types remain integral and correctly signed
      static_assert(std::is_integral_v<TLargestUIntValueType> && std::is_unsigned_v<TLargestUIntValueType>,
                    "TLargestUIntValueType must be an unsigned integral type");
      static_assert(std::is_integral_v<TLargestIntValueType> && std::is_signed_v<TLargestIntValueType>,
                    "TLargestIntValueType must be a signed integral type");

      // Enforce character type contracts used by getChar8()/getChar16()
      static_assert(std::is_unsigned_v<TForteChar> && sizeof(TForteChar) == 1,
                    "TForteChar must be an unsigned 8-bit character type");
      static_assert(std::is_unsigned_v<TForteWChar> && sizeof(TForteWChar) == 2,
                    "TForteWChar must be an unsigned 16-bit character type");

      // Enforce float type contracts used by bitCastFloatToUInt32/bitCastDFloatToUInt64
      static_assert(sizeof(TForteFloat) == 4, "TForteFloat must be a 32-bit float");
      static_assert(sizeof(TForteDFloat) == 8, "TForteDFloat must be a 64-bit float");

      template<typename U, typename T>
      static auto cast(const T paFromCast) -> typename mpl::implicit_or_explicit_cast_t<T, U> {
        U oToCast;
        // If interacting with integers, add or remove sign extension
        if constexpr (std::is_base_of_v<CIEC_ANY_BIT, T> && std::is_base_of_v<CIEC_ANY_INT, U>) {
          oToCast.setValueSimple(
              U(static_cast<typename U::TValueType>(static_cast<typename T::TValueType>(paFromCast))));
        } else if constexpr (std::is_base_of_v<CIEC_ANY_INT, T> && std::is_base_of_v<CIEC_ANY_BIT, U>) {
          typename T::TValueType fromValue = static_cast<typename T::TValueType>(paFromCast);
          typename std::make_unsigned_t<typename T::TValueType> fromValueUnsigned =
              static_cast<std::make_unsigned_t<typename T::TValueType>>(fromValue);
          typename U::TValueType toValue = static_cast<typename U::TValueType>(fromValueUnsigned);
          oToCast.setValueSimple(U(toValue));
        } else if constexpr (std::is_base_of_v<CIEC_ANY_BIT, T> && // special cast binary to bool
                             std::is_base_of_v<CIEC_ANY_BIT, U>) {
          if constexpr (std::is_base_of_v<CIEC_BOOL, U>) { // reinterpret C/C++ bool to binary transfer
            oToCast.setValueSimple(U(static_cast<typename T::TValueType>(paFromCast) % 2 == 1 ? true : false));
          } else {
            oToCast.setValueSimple(
                U(static_cast<typename U::TValueType>(static_cast<typename T::TValueType>(paFromCast))));
          }
        } else if constexpr (std::is_base_of_v<CIEC_ANY_REAL, T>) {
          specialCast(paFromCast, oToCast);
        } else if constexpr (std::is_base_of_v<CIEC_ANY_REAL, U>) {
          oToCast.setValue(paFromCast);
        } else {
          oToCast.setValueSimple(paFromCast);
        }
        return oToCast;
      }

      constexpr CIEC_ANY() = default;

      constexpr virtual ~CIEC_ANY() = default;

      /*! \brief Set method for data type member value
       *
       *  The data type value is set through the copy assignment
       *
       */
      virtual constexpr void setValue(const CIEC_ANY &paValue) {
        setValueSimple(paValue);
      }

      /*! \brief Reset the value for to the default initial value
       */
      virtual void reset() = 0;

      /**
       * @brief Unwrap ANY value if inside a container
       * @return The unwrapped value or this value if not in a container
       */
      [[nodiscard]] virtual constexpr CIEC_ANY &unwrap() {
        return *this;
      }

      [[nodiscard]] virtual constexpr const CIEC_ANY &unwrap() const {
        return *this;
      }

      /*! \brief Get the allocation size of the data type object
       *
       * @return The allocation size in bytes
       */
      virtual constexpr size_t getSizeof() const {
        return sizeof(CIEC_ANY);
      }

      /*! \brief Makes a clone of the data type object
       *
       *   With this command a clone object of the actual data type object is created.
       *   This clone object is necessary for establishing data-connections.
       *   Pure virtual function implementation.
       */
      virtual CIEC_ANY *clone(TForteByte *paDataBuf) const = 0;

      /*! \brief Get data type id method
       *
       *   With this command the data type of the actual object can be identified.
       *   The data type identifier is unique for each data type within FORTE.
       */
      virtual constexpr EDataTypeID getDataTypeID() const {
        return e_ANY;
      }

      virtual StringId getTypeNameID() const;

      /*! \brief Get the pointer to the union char array
       *
       *  This command returns the pointer to the union char array
       *  \return Returns TForteByte*
       */

      /*! \brief Get a byte from the canonical scalar storage by its significance index.
       *
       * @param paByteIndex The significance index (0 = Least Significant Byte, 1 = LSB+1, ...)
       * @return The byte value at the given significance index.
       */
      TForteByte getScalarByte(size_t paByteIndex) const {
        assert(paByteIndex < sizeof(TLargestUIntValueType));
        size_t shift = paByteIndex;
#if defined(__ARMEL__) && !defined(__VFP_FP__)
        // On little-endian ARM with FPA floating point (mixed-endian), 64-bit doubles (LREAL)
        // are stored as two little-endian 32-bit words, but the words themselves are in big-endian order.
        // This means significance indices 0..3 (LSB word) are at bit positions 32..63, and vice-versa.
        if (e_LREAL == getDataTypeID()) {
          shift = (paByteIndex < 4) ? (paByteIndex + 4) : (paByteIndex - 4);
        }
#endif
        return static_cast<TForteByte>((mLargestUInt >> (shift * 8)) & 0xFF);
      }

      /*! \brief Set a byte in the canonical scalar storage by its significance index.
       *
       * @param paByteIndex The significance index (0 = Least Significant Byte, 1 = LSB+1, ...)
       * @param paValue The byte value to set.
       */
      void setScalarByte(size_t paByteIndex, TForteByte paValue) {
        assert(paByteIndex < sizeof(TLargestUIntValueType));
        size_t shift = paByteIndex;
#if defined(__ARMEL__) && !defined(__VFP_FP__)
        // On little-endian ARM with FPA floating point (mixed-endian), 64-bit doubles (LREAL)
        // are stored as two little-endian 32-bit words, but the words themselves are in big-endian order.
        // This means significance indices 0..3 (LSB word) are at bit positions 32..63, and vice-versa.
        if (e_LREAL == getDataTypeID()) {
          shift = (paByteIndex < 4) ? (paByteIndex + 4) : (paByteIndex - 4);
        }
#endif
        TLargestUIntValueType mask = ~(static_cast<TLargestUIntValueType>(0xFF) << (shift * 8));
        mLargestUInt = (mLargestUInt & mask) | (static_cast<TLargestUIntValueType>(paValue) << (shift * 8));
      }

      /*! \brief Get a safe span-view of the 8-byte canonical scalar storage.
       *
       *  \return Returns a span covering the internal scalar storage.
       */
      std::span<std::byte, sizeof(TLargestIntValueType)> getScalarByteView() {
        return std::span<std::byte, sizeof(TLargestIntValueType)>(getDataPtr(), sizeof(TLargestIntValueType));
      }

      /*! \brief Get a safe const span-view of the 8-byte canonical scalar storage.
       *
       *  \return Returns a const span covering the internal scalar storage.
       */
      std::span<const std::byte, sizeof(TLargestIntValueType)> getConstScalarByteView() const {
        return std::span<const std::byte, sizeof(TLargestIntValueType)>(getConstDataPtr(),
                                                                        sizeof(TLargestIntValueType));
      }

      /*! \brief Get a raw pointer to the internal 8-byte scalar storage.
       *
       *  IMPORTANT: This only points to the scalar field (8 bytes). For complex types (STRING, ARRAY),
       *  use the type-specific accessors.
       *
       *  \return Returns std::byte*
       */
      std::byte *getDataPtr() {
        return std::bit_cast<std::byte *>(&mLargestUInt);
      }

      /*! \brief Get a raw const pointer to the internal 8-byte scalar storage.
       *
       *  IMPORTANT: This only points to the scalar field (8 bytes). For complex types (STRING, ARRAY),
       *  use the type-specific accessors.
       *
       *  \return Returns const std::byte*
       */

      const std::byte *getConstDataPtr() const {
        return std::bit_cast<const std::byte *>(&mLargestUInt);
      }

      /*! \brief Converts string value to data type value
       *
       *   This command implements a conversion function from IEC 61131
       *   data type (string format) to a C++ conform type.
       *   This function is necessary for communication with a proper engineering system.
       *   Pure virtual function implementation.
       *   \param paValue string buffer
       *   \return number of bytes taken used from the buffer
       *        -1 on on error
       */
      virtual int fromString(const char *paValue) = 0;

      /*! \brief Converts data type value to string
       *
       *   This command implements a conversion function from C++ data type
       *   to IEC61131 conform data type (string format).
       *   This function is necessary for communication with a proper engineering system.
       *   Pure virtual function implementation.
       *   \param paValue buffer for storing the string representation
       */
      virtual void toString(std::string &paTargetBuf) const = 0;

      /*! \brief Compare for equality
       *
       * \param paOther The other value
       * \return whether the values are equal
       * \note This method does not perform implicit or explicit type promotion when comparing for equality
       *       (e.g., <code>CIEC_INT(0).equals(CIEC_SINT(0))</code> yields <code>false</code>).
       */
      [[nodiscard]] virtual constexpr bool equals(const CIEC_ANY &paOther) const {
        if (getDataTypeID() == paOther.getDataTypeID()) {
          return mLargestUInt == paOther.mLargestUInt;
        }
        return false;
      }

      [[nodiscard]] virtual constexpr std::partial_ordering compare(const CIEC_ANY &paOther) const {
        (void) paOther;
        return std::partial_ordering::unordered;
      }

      /*! \brief determine whether we can cast the source to the destination and what kind of cast it is
       */
      static bool isCastable(EDataTypeID paSource, EDataTypeID paDestination, bool &paUpCast, bool &paDownCast);
      static bool isCastable(EDataTypeID paSource, EDataTypeID paDestination) {
        bool bUpCast, bDownCast;
        return isCastable(paSource, paDestination, bUpCast, bDownCast);
      }

      /*! \brief perform special cast operation that can not be directly handled by the data types
       */
      static void specialCast(const CIEC_ANY &paSrcValue, CIEC_ANY &paDstValue);

      virtual size_t getIECMemorySize() const {
        const EDataTypeID typeId = getDataTypeID();
        return typeId > (sizeof(csmDataLengthLookup) / sizeof(size_t)) ? 0 : csmDataLengthLookup[typeId];
      }

      virtual CIEC_ANY *getVar(const std::span<const StringId> paNameList) {
        if (paNameList.empty()) {
          return this;
        }
        return nullptr;
      }

#ifdef FORTE_SUPPORT_CUSTOM_SERIALIZABLE_DATATYPES
      /*! \brief the following methods have to be implemented if a custom datatype is added to the forte which is not
       * supported by the default seralize mechanism. */
      /*! \brief returns the required size for serialization */
      virtual unsigned int getRequiredSerializationSize() const {
        return 0;
      }

      /*! \brief returns the tag of the datatype for serialization */
      virtual TForteByte getTag() const {
        return 0xFF;
      }

      /*! \brief serialize the custom type */
      virtual int serializeCustomType(TForteByte *, int) const {
        return -1;
      }

      /*! \brief deserialize the tag */
      virtual bool deserializeTag(const TForteByte) {
        return false;
      }

      /*! \brief deserialize the custom type*/
      virtual int deserializeCustomType(const TForteByte *, int) {
        return -1;
      }

#endif

    protected:
      // Bit-pattern preservation helpers to maintain standard compliance and avoid UB.
      static constexpr TLargestUIntValueType bitCastSignedToUInt64(TLargestIntValueType paValue) {
        return static_cast<TLargestUIntValueType>(paValue);
      }
      static constexpr TLargestIntValueType bitCastUInt64ToSigned(TLargestUIntValueType paValue) {
        return static_cast<TLargestIntValueType>(paValue);
      }
      static constexpr TForteUInt32 bitCastFloatToUInt32(TForteFloat paValue) {
        return std::bit_cast<TForteUInt32>(paValue);
      }
      static constexpr TForteFloat bitCastUInt32ToFloat(TForteUInt32 paValue) {
        return std::bit_cast<TForteFloat>(paValue);
      }
      static constexpr TLargestUIntValueType bitCastDFloatToUInt64(TForteDFloat paValue) {
        return std::bit_cast<TLargestUIntValueType>(paValue);
      }
      static constexpr TForteDFloat bitCastUInt64ToDFloat(TLargestUIntValueType paValue) {
        return std::bit_cast<TForteDFloat>(paValue);
      }

      // Canonical storage helpers to maintain data invariants.
      // All scalar writes MUST funnel through these helpers to ensure consistency.
      constexpr void setUnsignedCanonical(TLargestUIntValueType paValue) {
        mLargestUInt = paValue;
      }
      constexpr void setSignedCanonical(TLargestIntValueType paValue) {
        mLargestUInt = bitCastSignedToUInt64(paValue);
      }

      constexpr void setFloatCanonical(TForteFloat paValue) {
        // Explicitly zero-extend the 32-bit float bit pattern into the 64-bit canonical field
        mLargestUInt = static_cast<TLargestUIntValueType>(bitCastFloatToUInt32(paValue));
      }

      constexpr void setDFloatCanonical(TForteDFloat paValue) {
        mLargestUInt = bitCastDFloatToUInt64(paValue);
      }

      /*! \brief copy the data value
       *
       * To be used for efficiently implementing assignment operators where it is
       * known that this can be done safely.
       */
      constexpr void setValueSimple(const CIEC_ANY &paValue) {
        mLargestUInt = paValue.mLargestUInt;
      }

      /*! \brief Set Method for complex datatypes
       *  A virtual function for datatypes who can't be copied by simple assignment
       *
       *  IMPORTANT: The following set* methods maintain the invariant that mLargestUInt
       *  is the canonical storage for all scalar types (integer, bool, char, float).
       *  When adding new scalar types or modification methods, ensure this canonical field is updated.
       */

      constexpr void setTBOOL8(bool src) {
        setUnsignedCanonical(static_cast<TLargestUIntValueType>(src));
      }

      constexpr void setTUINT32(TForteUInt32 src) { // also used for TForteDWord
        setUnsignedCanonical(static_cast<TLargestUIntValueType>(src));
      }

      constexpr void setTUINT16(TForteUInt16 src) { // also used for TForteWord
        setUnsignedCanonical(static_cast<TLargestUIntValueType>(src));
      }

      constexpr void setTUINT8(TForteUInt8 src) { // also used for TForteByte
        setUnsignedCanonical(static_cast<TLargestUIntValueType>(src));
      }

      constexpr void setTINT32(TForteInt32 src) {
        setSignedCanonical(static_cast<TLargestIntValueType>(src));
      }

      constexpr void setTINT16(TForteInt16 src) {
        setSignedCanonical(static_cast<TLargestIntValueType>(src));
      }

      constexpr void setTINT8(TForteInt8 src) {
        setSignedCanonical(static_cast<TLargestIntValueType>(src));
      }

      constexpr void setChar(TForteChar src) {
        setUnsignedCanonical(static_cast<TLargestUIntValueType>(src));
      }

      constexpr void setChar16(TForteWChar src) {
        setUnsignedCanonical(static_cast<TLargestUIntValueType>(src));
      }

      constexpr void setTFLOAT(TForteFloat src) {
        setFloatCanonical(src);
      }

      constexpr void setTDFLOAT(TForteDFloat src) {
        setDFloatCanonical(src);
      }

      constexpr void setTUINT64(TForteUInt64 src) { // also used for LWORD
        setUnsignedCanonical(static_cast<TLargestUIntValueType>(src));
      }

      constexpr void setTINT64(TForteInt64 src) {
        setSignedCanonical(static_cast<TLargestIntValueType>(src));
      }

      constexpr bool getTBOOL8() const {
        return (mLargestUInt != 0);
      }

      constexpr TForteUInt32 getTUINT32() const { // also used for TForteDWord
        return static_cast<TForteUInt32>(mLargestUInt);
      }

      constexpr TForteUInt16 getTUINT16() const { // also used for TForteWord
        return static_cast<TForteUInt16>(mLargestUInt);
      }

      constexpr TForteUInt8 getTUINT8() const { // also used for TForteByte
        return static_cast<TForteUInt8>(mLargestUInt);
      }

      constexpr TForteInt32 getTINT32() const {
        return static_cast<TForteInt32>(bitCastUInt64ToSigned(mLargestUInt));
      }

      constexpr TForteInt16 getTINT16() const {
        return static_cast<TForteInt16>(bitCastUInt64ToSigned(mLargestUInt));
      }

      constexpr TForteInt8 getTINT8() const {
        return static_cast<TForteInt8>(bitCastUInt64ToSigned(mLargestUInt));
      }

      constexpr TForteChar getChar8() const {
        // TForteChar is defined as an unsigned 8-bit character type (see static_assert below);
        // mLargestUInt is the canonical store for all unsigned/char types.
        return static_cast<TForteChar>(mLargestUInt);
      }

      constexpr TForteWChar getChar16() const {
        // TForteWChar is defined as a 16-bit character type (see static_assert below);
        // mLargestUInt is the canonical store for all unsigned/char types.
        return static_cast<TForteWChar>(mLargestUInt);
      }

      constexpr TForteUInt64 getTUINT64() const { // also used for LWORD
        return static_cast<TForteUInt64>(mLargestUInt);
      }

      constexpr TForteInt64 getTINT64() const {
        return bitCastUInt64ToSigned(mLargestUInt);
      }

      //!< get-Methods are Big/Little Endian independent
      constexpr TForteFloat getTFLOAT() const {
        return bitCastUInt32ToFloat(static_cast<TForteUInt32>(mLargestUInt));
      }

      constexpr TForteDFloat getTDFLOAT() const {
        return bitCastUInt64ToDFloat(mLargestUInt);
      }

      std::byte *getGenData() {
        return std::bit_cast<std::byte *>(static_cast<uintptr_t>(mLargestUInt));
      }

      const std::byte *getGenData() const {
        return std::bit_cast<const std::byte *>(static_cast<uintptr_t>(mLargestUInt));
      }

      void setGenData(std::byte *paGenData) {
        static_assert(sizeof(uintptr_t) <= sizeof(TLargestUIntValueType),
                      "TLargestUIntValueType must be large enough to safely hold a pointer for getGenData/setGenData "
                      "round-tripping. Please adjust TLargestUIntValueType size.");
        mLargestUInt = std::bit_cast<uintptr_t>(paGenData);
      }

      static StringId parseTypeName(const char *paValue, const char *paHashPos);

    public:
      constexpr TLargestUIntValueType getLargestUInt() const {
        return mLargestUInt;
      }

      constexpr void setLargestUInt(TLargestUIntValueType paVal) {
        setUnsignedCanonical(paVal);
      }

      constexpr TLargestIntValueType getLargestInt() const {
        return bitCastUInt64ToSigned(mLargestUInt);
      }

      constexpr void setLargestInt(TLargestIntValueType paVal) {
        setSignedCanonical(paVal);
      }

      CIEC_ANY(const CIEC_ANY &) = delete;
      CIEC_ANY &operator=(const CIEC_ANY &paValue) = delete;

    private:
      // Canonical 64-bit storage holding the data value of our IEC data type.
      // mLargestUInt is the single source of truth for ALL scalar types (integer, bool, char, float).
      // IMPORTANT: This field MUST always hold the canonical value for all scalar types.
      // When writing to this field, ensure its bit pattern is correct (e.g., via std::bit_cast for floats).
      // For types whose payload does not fit in 64 bits (e.g. STRING, ARRAY), this field holds a pointer (at runtime).
      // The union has been removed to fix MSVC constexpr tracking issues.
      TLargestUIntValueType mLargestUInt = 0;
      static_assert(sizeof(uintptr_t) <= sizeof(TLargestUIntValueType),
                    "TLargestUIntValueType must be large enough to safely hold a pointer for getGenData/setGenData "
                    "round-tripping. Please adjust TLargestUIntValueType size.");

      constexpr static size_t csmDataLengthLookup[] = {0, 1, 1, 2, 4, 8, 1, 2, 4, 8, 1, 2, 4, 8, 8,
                                                       8, 8, 8, 8, 8, 8, 8, 1, 2, 4, 8, 0, 0, 0};
  };

  namespace templates {
    template<typename T, typename = typename std::enable_if_t<std::is_base_of_v<CIEC_ANY, T>>>
    struct numeric_limits {
        static constexpr bool is_specialized = true;

        static T min() noexcept {
          return T(std::numeric_limits<typename T::TValueType>::min());
        }

        static T max() noexcept {
          return T(std::numeric_limits<typename T::TValueType>::max());
        }

        static T lowest() noexcept {
          return min();
        }

        static constexpr int digits = std::numeric_limits<typename T::TValueType>::digits;
        static constexpr int digits10 = std::numeric_limits<typename T::TValueType>::digits10;
        static constexpr int max_digits10 = std::numeric_limits<typename T::TValueType>::max_digits10;

        static constexpr bool is_signed = std::numeric_limits<typename T::TValueType>::is_signed;
        static constexpr bool is_integer = std::numeric_limits<typename T::TValueType>::is_integer;
        static constexpr bool is_exact = std::numeric_limits<typename T::TValueType>::is_exact;
        static constexpr int radix = std::numeric_limits<typename T::TValueType>::radix;

        static T epsilon() noexcept {
          return T(std::numeric_limits<typename T::TValueType>::epsilon());
        }

        static T round_error() noexcept {
          return T(std::numeric_limits<typename T::TValueType>::round_error());
        }

        static constexpr int min_exponent = std::numeric_limits<typename T::TValueType>::min_exponent;
        static constexpr int min_exponent10 = std::numeric_limits<typename T::TValueType>::min_exponent;
        static constexpr int max_exponent = std::numeric_limits<typename T::TValueType>::max_exponent;
        static constexpr int max_exponent10 = std::numeric_limits<typename T::TValueType>::max_exponent10;

        static constexpr bool has_infinity = std::numeric_limits<typename T::TValueType>::has_infinity;
        static constexpr bool has_quiet_NaN = std::numeric_limits<typename T::TValueType>::has_quiet_NaN;
        static constexpr bool has_signaling_NaN = std::numeric_limits<typename T::TValueType>::has_signaling_NaN;
        static constexpr std::float_denorm_style has_denorm = std::numeric_limits<typename T::TValueType>::has_denorm;
        static constexpr bool has_denorm_loss = std::numeric_limits<typename T::TValueType>::has_denorm_loss;

        static T infinity() noexcept {
          return T(std::numeric_limits<typename T::TValueType>::infinity());
        }

        static T quiet_NaN() noexcept {
          return T(std::numeric_limits<typename T::TValueType>::quiet_NaN());
        }

        static T signaling_NaN() noexcept {
          return T(std::numeric_limits<typename T::TValueType>::signaling_NaN());
        }

        static T denorm_min() noexcept {
          return T(std::numeric_limits<typename T::TValueType>::denorm_min());
        }

        static constexpr bool is_iec559 = std::numeric_limits<typename T::TValueType>::is_iec559;
        static constexpr bool is_bounded = std::numeric_limits<typename T::TValueType>::is_bounded;
        static constexpr bool is_modulo = std::numeric_limits<typename T::TValueType>::is_modulo;

        static constexpr bool traps = std::numeric_limits<typename T::TValueType>::traps;
        static constexpr bool tinyness_before = std::numeric_limits<typename T::TValueType>::tinyness_before;
        static constexpr std::float_round_style round_style = std::numeric_limits<typename T::TValueType>::round_style;
    };

  } // namespace templates

  template<typename T, typename = typename std::enable_if_t<std::is_base_of_v<CIEC_ANY, T>>>
  struct CDataTypeTrait {
      static constexpr CIEC_ANY::EDataTypeID scmDataTypeId = CIEC_ANY::e_Max;
      static constexpr StringId scmDataTypeName{};
  };

  /*!
   * \brief A type trait marking generic data types
   * \tparam T The data type
   */
  template<typename T>
  struct is_generic_datatype : std::false_type {};

  template<>
  struct is_generic_datatype<CIEC_ANY> : std::true_type {};

  /*!
   * \brief Helper variable template for is_generic_datatype
   * \tparam T The data type
   */
  template<typename T>
  constexpr bool is_generic_datatype_v = is_generic_datatype<T>::value;

  /*!
   * \brief A concept for generic data types
   * \tparam T The data type
   */
  template<typename T>
  concept generic_datatype = is_generic_datatype_v<std::remove_cvref_t<T>>;
} // namespace forte

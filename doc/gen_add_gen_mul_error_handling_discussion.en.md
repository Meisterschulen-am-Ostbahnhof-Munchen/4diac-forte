# Discussion Basis: Incompatible-Type Error Handling (F_ADD/F_MUL vs. GEN_ADD/GEN_MUL)

**Date:** 2026-08-28
**Context:** Introduction of `GEN_MUL` (generic multiplication FB) with a shared base class
`CGenArithBase<TVariant>` for `GEN_ADD`/`GEN_MUL`, including unit tests for all four arithmetic FB
variants (`F_ADD`, `F_MUL`, `GEN_ADD_2/3/4`, `GEN_MUL_2/3/4`).

## Observation

While testing the error path for incompatible operand types (e.g. `DINT + REAL`), it turned out that the
**fixed 2-input FBs** (`F_ADD`, `F_MUL`) and the **generic, variable-arity FBs** (`GEN_ADD`, `GEN_MUL`)
behave differently, even though both rely on the same underlying mechanism
(`mpl::get_add_operator_result_type`/`get_mul_operator_result_type` + `DEVLOG_ERROR`).

### F_ADD / F_MUL (fixed, 2 inputs)

File: `modules/IEC61131-3/src/iec61131/arithmetic/F_ADD_fbt.cpp` (analogous for `F_MUL_fbt.cpp`)

```cpp
var_OUT = std::visit(
    [](auto &&paIN1, auto &&paIN2) -> CIEC_ANY_MAGNITUDE_VARIANT {
      ...
      if constexpr (!std::is_same<deductedType, mpl::NullType>::value) {
        return func_ADD(paIN1, paIN2);
      }
      DEVLOG_ERROR("Adding incompatible types %s and %s\n", ...);
      return CIEC_ANY_MAGNITUDE_VARIANT();   // <-- freshly default-constructed
    },
    ...);
```

**Result:** on incompatible types, `OUT` is set to a freshly default-constructed
`CIEC_ANY_MAGNITUDE_VARIANT`/`CIEC_ANY_NUM_VARIANT`. Since `SINT` is the first alternative in the
`std::variant`, `OUT` effectively ends up as `SINT#0` (observed as `0` in our tests via `CIEC_DINT`).

Confirmed by `F_ADD_IncompatibleTypeTests`/`F_MUL_IncompatibleTypeTests`
(`tests/modules/IEC61131-3/F_ADD_tester.cpp`, `F_MUL_tester.cpp`).

### GEN_ADD / GEN_MUL (generic, 2..n inputs)

File: `modules/IEC61131-3/src/iec61131/arithmetic/GEN_ADD_fbt.cpp` (analogous for `GEN_MUL_fbt.cpp`)

```cpp
var_OUT = std::visit(
    [](auto &&paOUT, auto &&paIN) -> CIEC_ANY_MAGNITUDE_VARIANT {
      ...
      if constexpr (!std::is_same<deductedType, mpl::NullType>::value) {
        return func_ADD(paOUT, paIN);
      }
      DEVLOG_ERROR("Adding incompatible types %s and %s\n", ...);
      return paOUT;   // <-- previous accumulator value is kept
    },
    ...);
```

**Result:** the accumulator (`var_OUT`) is left **unchanged** on an incompatible fold step — it retains the
value from the last successfully processed operand.

Confirmed by `GenAdd2IncompatibleTypeTests`/`GenMul2IncompatibleTypeTests`
(`tests/modules/IEC61131-3/GEN_ADD_2_tester.cpp`, `GEN_MUL_2_tester.cpp`).

## Why is it like this?

For `GEN_ADD`/`GEN_MUL`, this behavior is a direct consequence of the fold loop: the lambda's return value
becomes the next accumulator value. "Leave unchanged" is the natural fallback for a fold pattern over `>=2`
operands. For `F_ADD`/`F_MUL` there is no accumulator in the proper sense — `var_OUT` is assigned directly
from the `std::visit` result of the two operands, and the original author chose an explicit reset to a
fresh (default) value here, instead of e.g. leaving `var_OUT` unchanged or passing through `var_IN1`.

There is no indication that this divergence was intentionally designed as different behavior for "2
operands" vs. "n operands" — both code paths evolved independently (different authors/time periods, see
copyright headers), and the structural similarity suggests this simply was never harmonized.

## Why does this matter?

- **Inconsistent behavior** between two FB families that, from a user's perspective, solve the same task
  (arithmetic operation over generic numeric types).
- **Silent error correction vs. silent data loss:** `GEN_ADD`/`GEN_MUL` "swallow" the incompatible operand
  without resetting the result — in a chain with several inputs this can create a false impression of
  correctness (the result looks plausible but is incomplete). `F_ADD`/`F_MUL`, by contrast, consistently
  reset to a (possibly surprising) default value of `0`, which is easier to recognize as "something went
  wrong," but also overwrites an otherwise meaningful output.
- **Safety relevance in control logic:** if `OUT` is used further in a running control loop, a sudden,
  unnoticed jump to `0` (F_ADD/F_MUL) or freezing at a stale intermediate value (GEN_ADD/GEN_MUL) could be
  differently critical depending on the application.
- In both cases the error is only logged (`DEVLOG_ERROR`), not reported back to the calling network as an
  FB error state (`STATE_ERROR` or similar) — a user who doesn't monitor the logs may not notice the type
  error at all.

## Discussion questions

1. Should the behavior of `F_ADD`/`F_MUL` and `GEN_ADD`/`GEN_MUL` be unified?
2. If so, which behavior is the "correct" one?
   - Keep the accumulator/previous value (current GEN_ behavior)?
   - Reset to a default value (current F_ behavior)?
   - Something else, e.g. putting the FB into an explicit error state instead of just logging?
3. Is "just log and keep running" acceptable at all for safety-relevant IEC 61499 applications, or should
   an incompatible type lead to a hard failure (e.g. no `CNF` event, or a dedicated error event output)?
4. Does this also affect other generic/fixed FB pairs in the `arithmetic` module (`F_DIV`/a future
   `GEN_DIV`, `F_MOD`, `F_EXPT`, …) that use the same `std::visit` pattern?

## Affected / relevant files

- `modules/IEC61131-3/src/iec61131/arithmetic/F_ADD_fbt.cpp`
- `modules/IEC61131-3/src/iec61131/arithmetic/F_MUL_fbt.cpp`
- `modules/IEC61131-3/src/iec61131/arithmetic/GEN_ADD_fbt.cpp`
- `modules/IEC61131-3/src/iec61131/arithmetic/GEN_MUL_fbt.cpp`
- `modules/IEC61131-3/include/forte/iec61131/arithmetic/genarithbase_fbt.h`
- Tests: `tests/modules/IEC61131-3/F_ADD_tester.cpp`, `F_MUL_tester.cpp`, `GEN_ADD_2_tester.cpp`,
  `GEN_MUL_2_tester.cpp`

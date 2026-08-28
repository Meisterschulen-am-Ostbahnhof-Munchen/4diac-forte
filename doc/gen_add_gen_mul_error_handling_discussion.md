# Diskussionsgrundlage: Fehlerverhalten bei inkompatiblen Typen (F_ADD/F_MUL vs. GEN_ADD/GEN_MUL)

**Stand:** 2026-08-28
**Kontext:** Einführung von `GEN_MUL` (generische Multiplikations-FB) mit gemeinsamer Basisklasse
`CGenArithBase<TVariant>` für `GEN_ADD`/`GEN_MUL`, inkl. Unit-Tests für alle vier arithmetischen FB-Varianten
(`F_ADD`, `F_MUL`, `GEN_ADD_2/3/4`, `GEN_MUL_2/3/4`).

## Beobachtung

Beim Testen des Fehlerpfads für inkompatible Operandentypen (z. B. `DINT + REAL`) wurde festgestellt, dass sich
die **festen 2-Input-FBs** (`F_ADD`, `F_MUL`) und die **generischen, variablen-Arity-FBs** (`GEN_ADD`, `GEN_MUL`)
unterschiedlich verhalten, obwohl beide denselben zugrunde liegenden Mechanismus nutzen
(`mpl::get_add_operator_result_type`/`get_mul_operator_result_type` + `DEVLOG_ERROR`).

### F_ADD / F_MUL (fest, 2 Eingänge)

Datei: `modules/IEC61131-3/src/iec61131/arithmetic/F_ADD_fbt.cpp` (analog `F_MUL_fbt.cpp`)

```cpp
var_OUT = std::visit(
    [](auto &&paIN1, auto &&paIN2) -> CIEC_ANY_MAGNITUDE_VARIANT {
      ...
      if constexpr (!std::is_same<deductedType, mpl::NullType>::value) {
        return func_ADD(paIN1, paIN2);
      }
      DEVLOG_ERROR("Adding incompatible types %s and %s\n", ...);
      return CIEC_ANY_MAGNITUDE_VARIANT();   // <-- frisch default-konstruiert
    },
    ...);
```

**Ergebnis:** `OUT` wird bei inkompatiblen Typen auf einen frisch default-konstruierten
`CIEC_ANY_MAGNITUDE_VARIANT`/`CIEC_ANY_NUM_VARIANT` gesetzt. Da `SINT` die erste Alternative im `std::variant`
ist, landet `OUT` faktisch bei `SINT#0` (in unseren Tests über `CIEC_DINT` ausgelesen als `0`).

Bestätigt durch `F_ADD_IncompatibleTypeTests`/`F_MUL_IncompatibleTypeTests`
(`tests/modules/IEC61131-3/F_ADD_tester.cpp`, `F_MUL_tester.cpp`).

### GEN_ADD / GEN_MUL (generisch, 2..n Eingänge)

Datei: `modules/IEC61131-3/src/iec61131/arithmetic/GEN_ADD_fbt.cpp` (analog `GEN_MUL_fbt.cpp`)

```cpp
var_OUT = std::visit(
    [](auto &&paOUT, auto &&paIN) -> CIEC_ANY_MAGNITUDE_VARIANT {
      ...
      if constexpr (!std::is_same<deductedType, mpl::NullType>::value) {
        return func_ADD(paOUT, paIN);
      }
      DEVLOG_ERROR("Adding incompatible types %s and %s\n", ...);
      return paOUT;   // <-- bisheriger Akkumulator bleibt erhalten
    },
    ...);
```

**Ergebnis:** Der Akkumulator (`var_OUT`) bleibt bei einem inkompatiblen Faltschritt **unverändert** —
er behält den Wert des zuletzt erfolgreich verarbeiteten Operanden.

Bestätigt durch `GenAdd2IncompatibleTypeTests`/`GenMul2IncompatibleTypeTests`
(`tests/modules/IEC61131-3/GEN_ADD_2_tester.cpp`, `GEN_MUL_2_tester.cpp`).

## Warum ist das so?

Bei `GEN_ADD`/`GEN_MUL` ist das Verhalten eine direkte Konsequenz der Fold-Schleife: der Rückgabewert der
Lambda-Funktion wird der nächste Akkumulatorwert. „Unverändert lassen“ ist der natürliche Fallback beim
Fold-Muster mit `>=2` Operanden. Bei `F_ADD`/`F_MUL` gibt es keinen Akkumulator im eigentlichen Sinne —
`var_OUT` wird direkt aus dem `std::visit`-Ergebnis der zwei Operanden zugewiesen, und der ursprüngliche
Autor hat sich hier für einen expliziten Reset auf einen frischen (Default-)Wert entschieden statt z. B.
`var_OUT` unverändert zu lassen oder `var_IN1` durchzureichen.

Es gibt keinen Hinweis darauf, dass diese Divergenz absichtlich als unterschiedliches Verhalten für
"2 Operanden" vs. "n Operanden" gedacht war — beide Codepfade sind unabhängig voneinander entstanden
(unterschiedliche Autoren/Zeiträume, siehe Copyright-Header), und die Ähnlichkeit im Aufbau legt nahe,
dass hier einfach nicht harmonisiert wurde.

## Warum ist das relevant?

- **Inkonsistentes Verhalten** zwischen zwei FB-Familien, die aus Anwendersicht dieselbe Aufgabe lösen
  (arithmetische Operation über generische numerische Typen).
- **Stille Fehlerkorrektur vs. stiller Datenverlust:** `GEN_ADD`/`GEN_MUL` "verschlucken" den inkompatiblen
  Operanden, ohne das Ergebnis zurückzusetzen — das kann in einer Kette mit mehreren Eingängen einen falschen
  Eindruck von Korrektheit erzeugen (Ergebnis sieht plausibel aus, ist aber unvollständig). `F_ADD`/`F_MUL`
  setzen dagegen konsequent auf einen (evtl. überraschenden) Default-Wert `0` zurück, was leichter als "hier
  lief etwas schief" erkennbar ist, aber auch eine bestehende sinnvolle Ausgabe überschreibt.
- **Sicherheitsrelevanz in Steuerungslogik:** Wird `OUT` in einer laufenden Regelung weiterverwendet, könnte
  ein plötzlicher, unbemerkter Sprung auf `0` (F_ADD/F_MUL) oder das Einfrieren auf einem veralteten
  Zwischenwert (GEN_ADD/GEN_MUL) je nach Anwendungsfall unterschiedlich kritisch sein.
- In beiden Fällen wird der Fehler nur geloggt (`DEVLOG_ERROR`), nicht als FB-Fehlerzustand (`STATE_ERROR`
  o. ä.) an das aufrufende Netzwerk zurückgemeldet — ein Anwender, der die Logs nicht auswertet, bemerkt den
  Typfehler u. U. gar nicht.

## Diskussionsfragen

1. Soll das Verhalten zwischen `F_ADD`/`F_MUL` und `GEN_ADD`/`GEN_MUL` vereinheitlicht werden?
2. Falls ja: welches Verhalten ist das "richtige"?
   - Akkumulator/vorherigen Wert beibehalten (aktuelles GEN_-Verhalten)?
   - Auf Default-Wert zurücksetzen (aktuelles F_-Verhalten)?
   - Etwas anderes, z. B. den FB in einen expliziten Fehlerzustand versetzen statt nur zu loggen?
3. Ist das momentane "nur loggen und weiterlaufen" für sicherheitsrelevante IEC-61499-Anwendungen überhaupt
   akzeptabel, oder sollte ein inkompatibler Typ zu einem harten Fehler (z. B. kein `CNF`-Event, oder ein
   `EO`-Fehlerausgang) führen?
4. Betrifft das auch andere generische/feste FB-Paare im `arithmetic`-Modul (`F_DIV`/zukünftiges `GEN_DIV`,
   `F_MOD`, `F_EXPT`, …), die denselben `std::visit`-Musteransatz verwenden?

## Betroffene/relevante Dateien

- `modules/IEC61131-3/src/iec61131/arithmetic/F_ADD_fbt.cpp`
- `modules/IEC61131-3/src/iec61131/arithmetic/F_MUL_fbt.cpp`
- `modules/IEC61131-3/src/iec61131/arithmetic/GEN_ADD_fbt.cpp`
- `modules/IEC61131-3/src/iec61131/arithmetic/GEN_MUL_fbt.cpp`
- `modules/IEC61131-3/include/forte/iec61131/arithmetic/genarithbase_fbt.h`
- Tests: `tests/modules/IEC61131-3/F_ADD_tester.cpp`, `F_MUL_tester.cpp`, `GEN_ADD_2_tester.cpp`,
  `GEN_MUL_2_tester.cpp`

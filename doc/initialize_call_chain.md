# `initialize()` Call Chain in Function Blocks

## Überblick

`CFunctionBlock::initialize()` wird entweder beim **Start der FORTE-Instanz** (Device-Hochlauf, rekursiv über den Container-Baum) oder beim **dynamischen Anlegen** eines FB/Adapters über die Type Library aufgerufen.

---

## Einstiegspunkte

### 1. Device-Start — `forteinstance.cpp:35`

```
C4diacFORTEInstance::startupNewDevice()
  └→ mActiveDevice->initialize()
       └→ CFBContainer::initialize()       (fbcontainer.cpp:47)
            └→ CFunctionBlock::initialize() (für alle statisch konfigurierten FBs)
                 └→ CFBContainer::initialize() (rekursiv für innere Container)
```

Beim Start von FORTE wird `startupNewDevice()` aufgerufen. Das Device ist selbst ein `CFBContainer`, dessen `initialize()` rekursiv alle enthaltenen Funktionsblöcke initialisiert.

### 2. Dynamisches Erstellen eines FB — `typelib.cpp:166`

```
typelib::createFB()
  └→ newFB->initialize()
```

Wird aufgerufen, wenn ein MGM-Befehl (`CREATE FB`) eine neue FB-Instanz zur Laufzeit erzeugt.

### 3. Dynamisches Erstellen eines Adapters — `typelib.cpp:135`

```
typelib::createAdapter()
  └→ newAdapter->initialize()
```

Analoger Pfad für Adapter-Instanzen.

---

## Implementierung

### `CFunctionBlock::initialize()` — `core/src/funcbloc.cpp:53`

```cpp
bool CFunctionBlock::initialize() {
  if (!CFBContainer::initialize()) { return false; }  // Kinder initialisieren
  setupEventMonitoringData();
  setupInputConnectionTrackingData();
  return true;
}
```

### `CFBContainer::initialize()` — `core/src/fbcontainer.cpp:47`

```cpp
bool CFBContainer::initialize() {
  for (auto it : getChildren()) {
    if (it->isFB()) {
      if (!static_cast<CFunctionBlock *>(it)->initialize()) {
        return false;
      }
    }
  }
  return true;
}
```

---

## Vererbungshierarchie

Unterklassen überschreiben `initialize()` und rufen die Basisklasse auf:

| Klasse | Datei | Besonderheit |
|--------|-------|--------------|
| `CFunctionBlock` | `core/src/funcbloc.cpp` | Basis-Implementierung |
| `CCompositeFB` | `core/src/cfb.cpp` | ruft `CFunctionBlock::initialize()` |
| `CGenFunctionBlock<T>` | `core/include/forte/genfb.h` | delegiert an `T::initialize()` |
| `DEV_MGR` | `stdfblib/system/src/DEV_MGR.cpp` | ruft `CCommFB::initialize()` |
| `RMT_DEV` | `stdfblib/system/src/RMT_DEV.cpp` | ruft `CDevice::initialize()` + `MGR.initialize()` |
| `RMT_RES` | `stdfblib/system/src/RMT_RES.cpp` | ruft `CResource::initialize()` |
| `Config_EMB_RES` | `stdfblib/system/src/Config_EMB_RES.cpp` | ruft `CResource::initialize()` |
| `FORTE_E_TimeOut` | `stdfblib/events/src/E_TimeOut_fbt.cpp` | ruft `CEventSourceFB::initialize()` |
| `FakeTimeDev` | `stdfblib/system/src/FakeTimeDev.cpp` | ruft `CDevice::initialize()` + `MGR.initialize()` |

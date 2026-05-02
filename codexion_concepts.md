# Codexion — Concepts Reference

## 🔴 FIFO vs EDF — Kifash Khdamin?

### FIFO (First In First Out)
Bssit — li t9lb awwl, yakhod dongle awwl.

- Coder 1 t9lb f 100ms
- Coder 2 t9lb f 105ms
- → Coder 1 yakhod awwl, Coder 2 ysber

### EDF (Earliest Deadline First)
Li deadline dyalh 9rib aktar, yakhod awwl.

```
deadline = last_compile_start + time_to_burnout
```

**Mtal:**
- Coder 1: last compile kan f 0ms, burnout = 800ms → deadline = **800ms**
- Coder 2: last compile kan f 500ms, burnout = 800ms → deadline = **1300ms**
- → Coder 1 yakhod awwl l'dongle (deadline dyalh 9rib, proche de mourir 😅)

---

## 🟡 Monitor Thread — Sh7al o Kifash?

**Sh7al monitor thread kaykun?**
→ Wa7d (1) thread — hiya monitor unique

**Kifash khadma:** Monitor kaydur f loop permanente, f kol iteration katcheck:

```
kol X milliseconds (ex: 1ms):
  for kol coder:
    current_time - last_compile_start > time_to_burnout?
      → burned out! print message, stop simulation
```

**Imta kaye9der coder yburnout?** Coder burnout ila ma bda9ch compile men dak lwaqt:

```
[simulation start] ← last_compile_start = 0
   ↓
time_to_burnout (ex: 800ms) ta3da...
   ↓
ma khd9ch dongles o ma bda9ch compile
   ↓
BURNED OUT ← monitor ykshf o y9ul
```

**Reset dial timer:** Kl ma coder **bda compile** (machi khda dongle, bda compile!):

```
last_compile_start = current_time  ← reset timer
```

---

## 🟢 Burnout Rule bltafsil

```
SIMULATION_START → last_compile_start[i] = now

Loop dial coder i:
  1. T9lb dongle left + dongle right
  2. Bda compile  ← ← ← last_compile_start = now (RESET)
  3. Debug
  4. Refactor
  5. Rja3 l step 1
```

Monitor kaye9ra:

```c
if ((now - last_compile_start[i]) > time_to_burnout)
    → BURNOUT
```

> **9a3ida muhimma:** Log dyalha lazm tban f **less than 10ms** men l7adtha l7qi9iya.

---

## 🔵 EDF Priority Queue — Kifash t9der Timplimentiha?

EDF kay7taj **min-heap** — li deadline dyalh s9ira fi qimma:

```
Coder requests dongle:
  1. Add (coder_id, deadline) f heap
  2. Li fi qimma (min deadline) = li yakhdo dongle
```

**Dongle cooldown** kymchi qbl maye9dr coder yakhdo:

```
dongle released f T=500ms
cooldown = 100ms
→ dongle available f T=600ms
```

---

## ✅ Lkhlass — Steps bach tsali

```
1. Parse args → struct t_params
2. Init dongles (mutex + cond + queue per dongle)
3. Init coders (thread per coder)
4. Launch monitor thread
5. Coders loop: take_dongles → compile → debug → refactor
6. Monitor loop: check burnout every ~1ms
7. Stop signal → join threads → free → exit
```

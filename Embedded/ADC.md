# ADC — Interview Notes

Short answers you can actually say out loud.

---

## What is an ADC?

An ADC converts an analog voltage into a digital number the CPU can read.

A temperature sensor gives you 1.2 V. The CPU can't read volts — it reads numbers. The ADC turns
1.2 V into something like 1489.

---

## Resolution

Resolution is how many bits the output has. That decides how many steps you get.

| Bits | Steps | Step size at 3.3 V |
|---|---|---|
| 8 | 256 | 12.9 mV |
| 10 | 1024 | 3.2 mV |
| 12 | 4096 | 0.81 mV |
| 16 | 65536 | 0.05 mV |

One step is called **1 LSB** (Least Significant Bit):

```
LSB = Vref / 2^bits

12-bit, Vref = 3.3 V  ->  3.3 / 4096 = 0.806 mV
```

So a 12-bit ADC can't tell the difference between 1.0000 V and 1.0005 V. It just doesn't have the
resolution.

---

## Vref — the reference voltage

The ADC measures **relative to Vref**, not in absolute volts.

- Input = 0 V → code 0
- Input = Vref → code max (4095 for 12-bit)

So if Vref drifts, every reading drifts. That's why accurate systems use a dedicated reference chip
instead of just tying Vref to the 3.3 V supply rail.

**Input above Vref just clips** at the max code. It does not wrap around.

---

## Converting the code back to volts

```c
voltage = (raw * Vref) / 4096.0;     // 12-bit
```

Small nuance if they push you: strictly it's `raw / 2^n`, so full-scale code 4095 maps to
`4095/4096 × Vref`, which is one LSB below Vref. A lot of code uses `/4095` instead. The error is
tiny (0.02%), but knowing why is a good answer.

---

## Sampling rate and Nyquist

Sampling rate = how many conversions per second.

**Nyquist:** you must sample at more than **2×** the highest frequency in your signal.

Audio up to 20 kHz → sample at 40 kHz minimum, in practice 44.1 kHz.

If you sample too slow you get **aliasing** — a fast signal shows up as a fake slow signal, and
once it's aliased you cannot recover it in software. You need an analog low-pass filter *before*
the ADC.

In practice people sample 5–10× the signal frequency, not just 2×, to make filtering easier.

---

## How a SAR ADC works

SAR = Successive Approximation Register. This is what's inside almost every MCU.

It's a binary search. For 12 bits it takes 12 steps:

```
Input = 1.2 V, Vref = 3.3 V

Step 1: is input > 1.65 V (half)?      No  -> bit = 0
Step 2: is input > 0.825 V (quarter)?  Yes -> bit = 1
Step 3: is input > 1.24 V?             No  -> bit = 0
... 12 steps total -> 12-bit result
```

So conversion time = 12 clock cycles plus the acquisition time. N bits needs N cycles — that's the
key property to remember.

Other types exist (delta-sigma for high resolution and slow signals, flash for very fast), but SAR
is the one on your MCU.

---

## Sample and hold — the part people forget

Before converting, the ADC charges a small internal capacitor to the input voltage. That's the
**acquisition time**.

The problem: your source has resistance, the ADC has that capacitor, so you get an RC circuit. If
the source impedance is too high, the cap doesn't fully charge in time and **your reading is low**.

```
Source ---[ R_source ]---+--- ADC
                         |
                        === C_sample (a few pF)
```

Two fixes:
- Increase the sampling time in the ADC config (easy, but slower)
- Put an op-amp buffer in front (proper fix for high-impedance sensors)

This is a very common real bug: readings look right on a low-impedance source and are mysteriously
low on a high-impedance one.

---

## Three ways to read an ADC

| Method | How it works | When to use |
|---|---|---|
| **Polling** | Start conversion, spin until the done flag sets | Simple, occasional reads |
| **Interrupt** | Conversion done fires an ISR | Regular reads, CPU free meanwhile |
| **DMA** | Hardware moves results to a buffer, no CPU at all | High rate, multiple channels, continuous |

```c
/* Polling - simplest */
HAL_ADC_Start(&hadc1);
HAL_ADC_PollForConversion(&hadc1, 10);   // 10 ms timeout
uint32_t raw = HAL_ADC_GetValue(&hadc1);
```

For anything continuous — audio, motor current, sensor streaming — the answer is **DMA**. Polling
at 10 kHz would eat your whole CPU.

---

## How do you improve accuracy?

**Averaging.** Take N samples, average them. Random noise drops by √N. 16 samples → noise down 4×.

**Oversampling.** Sample 4^n times faster than you need and average — that buys you n extra bits.
So 16× oversampling on a 12-bit ADC gives you roughly 14 bits. It only works if there's a little
noise present to dither the signal.

**Hardware side:** stable Vref, separate analog ground, decoupling caps close to the pin, keep
analog traces away from switching noise, don't sample while a motor or radio is switching.

---

## Accuracy errors — the vocabulary

| Error | What it means |
|---|---|
| **Offset** | Whole reading shifted by a constant |
| **Gain** | Slope is wrong — error grows with input |
| **DNL** | Step sizes aren't equal; bad enough and you get missing codes |
| **INL** | Total deviation from the ideal straight line |

Offset and gain you can calibrate out in software. INL and DNL you can't — that's silicon quality.

---

## Common mistakes

| Mistake | What happens |
|---|---|
| Forgetting to enable the ADC clock | Reads always 0 |
| Source impedance too high | Readings consistently low |
| Sampling time too short | Same — cap never charges |
| Sampling below Nyquist | Aliasing, unrecoverable |
| Assuming Vref = exactly 3.3 V | Scale error on every reading |
| Reading a floating pin | Random values, and people call it "noise" |
| No analog ground plane | Noisy, unstable readings |
| Polling at high rates | Wastes the whole CPU; use DMA |
| Sampling during a switching event | Spikes in the data |

---

## Numbers worth memorising

```
12-bit, 3.3 V Vref:
  4096 steps
  1 LSB = 0.806 mV
  code 2048 = ~1.65 V

Nyquist: sample > 2 x signal frequency (use 5-10x in practice)
Averaging N samples: noise drops by sqrt(N)
Oversampling 4^n: gains n bits
SAR: N bits = N clock cycles
```

---

## 30-second summary

An ADC turns a voltage into a number. Resolution decides the step size — 12-bit over 3.3 V gives
about 0.8 mV per step. Everything is measured relative to Vref, so Vref stability is your accuracy
ceiling. You must sample above twice the signal frequency or you get aliasing. Inside an MCU it's
almost always a SAR ADC doing a binary search, one clock per bit. The thing people miss is
acquisition time — the input capacitor needs time to charge, so a high-impedance source gives you
low readings unless you lengthen the sampling time or add a buffer. For continuous sampling use
DMA, and to fight noise use averaging or oversampling.

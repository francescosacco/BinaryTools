# Binary Tools (CLI Toolkit for Binary and Firmware Files)

A collection of command-line utilities written in C for binary file manipulation and firmware-related formats such as Intel HEX.

This project focuses on:
- Simplicity and clarity
- High performance
- Cross-platform compatibility (Linux / Windows)
- Clean architecture (separation between CLI and core logic)

---

## bin2hex

### Description
Converts a binary file into a plain hexadecimal ASCII representation.

### Usage

```bash
bin2hex <input.bin> <output.hex>
```

### Example

```bash
bin2hex firmware.bin firmware.hex
```

---

## hex2bin

### Description
Converts a hexadecimal ASCII file into a binary file.

### Features
- Ignores \n and \r
- Detects invalid characters
- Validates input correctness

### Usage

```bash
hex2bin <input.hex> <output.bin>
```

### Example

```bash
hex2bin firmware.hex firmware.bin
```

---

## bin2IntelHex

### Description
Converts a binary file into Intel HEX format.

### Features
- Extended Linear Address (default)
- Extended Segment Address
- Address offset support

### Usage

```bash
bin2IntelHex <input.bin> <output.hex> [options]
```

| Option           | Description                           |
| :--------------: | :-----------------------------------: |
| --linear         | Use Extended Linear Address (default) |
| --segment        | Use Extended Segment Address          |
| --offset <value> | Add address offset (decimal or hex)   |

### Example

```bash
bin2IntelHex input.bin output.hex
```

```bash
bin2IntelHex input.bin output.hex --offset 0x1000
```

```bash
bin2IntelHex input.bin output.hex --segment
```

---

## intelHex2bin

### Description
Parses an Intel HEX file and reconstructs the corresponding binary image.

### Features
- Checksum validation
- Extended address support (Linear & Segment)
- Gap filling
- Overlap detection
- Strict validation mode

### Usage

```bash
intelHex2bin <input.hex> <output.bin> [options]
```

| Option         | Description                                  |
| :------------: | :------------------------------------------: |
| --fill <value> | Fill value for unused memory (default: 0x00) |
| --strictEnable | strict validation (fail on overlaps)         |

### Example

```bash
intelHex2bin firmware.hex firmware.bin
```

```bash
intelHex2bin firmware.hex firmware.bin --fill 0xFF
```

```bash
intelHex2bin firmware.hex firmware.bin --strict
```

---

## binCompare

### Description
Compares two binary files byte-by-byte.

### Usage

```bash
binCompare <file1.bin> <file2.bin>
```

### Example

```bash
binCompare original.bin generated.bin
```

---

## binSplit

### Description
Extracts a portion of a binary file.

### Usage

```bash
binSplit <input.bin> <output.bin> <start> <size>
```

| Option | Description                      |
| :----: | :------------------------------: |
| start  | Starting offset (decimal or hex) |
| size   | Number of bytes to extract       |

### Example

```bash
binSplit firmware.bin part.bin 1024 512
```

```bash
binSplit firmware.bin part.bin 0x400 0x200
```

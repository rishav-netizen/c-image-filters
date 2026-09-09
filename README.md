<div align="center">

  <h1>🖼️ c-image-filters</h1>

  <p><strong>A low-level, zero-dependency command-line image processing suite written in C99.</strong></p>

  <p>
    <a href="#"><img src="https://img.shields.io/badge/STATUS-UNDER%20PROGRESS-FF6B6B?style=for-the-badge&logo=git&logoColor=white" alt="Status"></a>
    <a href="#"><img src="https://img.shields.io/badge/LANGUAGE-C99-00599C?style=for-the-badge&logo=c&logoColor=white" alt="C Standard"></a>
    <a href="#"><img src="https://img.shields.io/badge/BUILD-MAKEFILE-informational?style=for-the-badge&logo=gnu" alt="Build"></a>
    <a href="LICENSE"><img src="https://img.shields.io/badge/LICENSE-MIT-10B981?style=for-the-badge" alt="License"></a>
  </p>

  <p>
    <a href="#-overview">Overview</a> •
    <a href="#-processing-pipeline">Pipeline</a> •
    <a href="#-filter-gallery">Filter Gallery</a> •
    <a href="#-bmp-specification">BMP Anatomy</a> •
    <a href="#-project-architecture">Architecture</a> •
    <a href="#-getting-started">Getting Started</a> •
    <a href="#-roadmap">Roadmap</a>
  </p>

</div>

---

> [!NOTE]
> ### 🚧 Active Development Notice
> This repository is currently **Under Progress**. The project scaffolding, build system, test fixtures, and architectural blueprints are complete. The low-level binary BMP parser and matrix convolution engine are actively under development.

---

## 🌟 Overview

**`c-image-filters`** is an educational, high-performance CLI utility designed to perform image manipulation directly at the byte and pixel level. Built entirely from scratch with **zero third-party dependencies**, it parses standard 24-bit uncompressed Windows Bitmap (`.bmp`) files, applies custom spatial convolution kernels and color channel transformations, and serializes the processed pixel buffer back into a valid BMP file.

---

## ⚡ Processing Pipeline

```text
  ┌─────────────────┐       ┌───────────────────────────┐       ┌─────────────────────────────┐       ┌──────────────────┐
  │   input.bmp     │ ───►  │     Binary BMP Parser     │ ───►  │    Pixel Transformation     │ ───►  │    output.bmp    │
  │  (24-bit RGB)   │       │  • Header struct decoding │       │  • Kernel convolutions      │       │ (Processed Image)│
  └─────────────────┘       │  • Channel unpack (BGR)   │       │  • Channel remapping        │       └──────────────────┘
                            │  • 4-byte padding removal │       │  • Spatial edge detection   │
                            └───────────────────────────┘       └─────────────────────────────┘
```

---

## 🎨 Filter Gallery

The planned filters range from single-pixel channel operations to multi-channel spatial convolutions:

| Filter | Category | Mathematical Operation / Kernel | Visual Effect | Status |
| :--- | :--- | :--- | :--- | :---: |
| 🌑 **Grayscale** | Point Transform | $Y = 0.299R + 0.587G + 0.114B$ | Converts colors to human-perceived luminance | 🟡 *Planned* |
| 🔲 **Invert** | Point Transform | $C' = 255 - C$ | Produces a classic photographic negative | 🟡 *Planned* |
| 📜 **Sepia** | Color Remap | Weighted warm RGB matrix transform | Warm, antique nostalgic tint | 🟡 *Planned* |
| 🌫️ **Box Blur** | Convolution | $\frac{1}{9} \begin{bmatrix} 1 & 1 & 1 \\ 1 & 1 & 1 \\ 1 & 1 & 1 \end{bmatrix}$ | Smooths details by neighborhood averaging | 🟡 *Planned* |
| 💫 **Gaussian Blur** | Convolution | $\frac{1}{16} \begin{bmatrix} 1 & 2 & 1 \\ 2 & 4 & 2 \\ 1 & 2 & 1 \end{bmatrix}$ | Weighted blur preserving natural edge falloff | 🟡 *Planned* |
| 🔍 **Sobel Edge** | Convolution | $G = \sqrt{G_x^2 + G_y^2}$ | Outlines object boundaries and high-frequency edges | 🟡 *Planned* |
| 🔄 **Flip Horizontal** | Geometric | $(x, y) \mapsto (W - 1 - x, y)$ | Horizontal mirror image reflection | 🟡 *Planned* |
| 🔃 **Flip Vertical** | Geometric | $(x, y) \mapsto (x, H - 1 - y)$ | Vertical upside-down inversion | 🟡 *Planned* |

---

## 🔬 BMP Specification Anatomy

Windows BMP files are structured as tightly packed binary data. Parsing them from scratch requires handling byte alignment and scanline row padding:

```text
  Offset    Size (Bytes)   Structure / Data
  ─────────────────────────────────────────────────────────────────────────────
  0x0000         14        BITMAPFILEHEADER  (Magic 'BM', FileSize, PixelOffset)
  0x000E         40        BITMAPINFOHEADER  (Width, Height, BitCount=24, etc.)
  0x0036          N        Raw Pixel Array:
                           • Row N (Top)    : [B][G][R] ... [Padding 0..3 Bytes]
                           • ...
                           • Row 0 (Bottom) : [B][G][R] ... [Padding 0..3 Bytes]
  ─────────────────────────────────────────────────────────────────────────────
  * Note: Rows are stored bottom-to-top with 4-byte boundary padding:
          padding = (4 - (width * 3) % 4) % 4
```

---

## 📁 Project Architecture

```text
c-image-filters/
├── 📁 include/           # Public headers & declarations
│   ├── filters.h         # Filter function prototypes & convolution math
│   └── image.h           # BMP headers, Pixel struct, and Image memory layout
├── 📁 src/               # Implementation source files
│   ├── main.c            # CLI entry point, argument parsing, execution flow
│   ├── image.c           # Binary file I/O, padding calculation, memory management
│   └── filters.c         # Matrix transformations and image filtering algorithms
├── 📁 examples/          # Sample image fixtures
│   ├── test.bmp          # 4x4 24-bit test BMP with multi-color pattern
│   └── .gitkeep
├── Makefile              # Build automation (flags: -Wall -Wextra -std=c99 -Iinclude)
├── .gitignore            # Ignores build artifacts (*.o, *.dSYM, binaries)
├── LICENSE               # MIT License
└── README.md             # Project documentation
```

---

## 🚀 Getting Started

### Prerequisites
* A C compiler supporting **C99** (`gcc` or `clang`)
* `make` utility

### Compilation

Clone and build the executable using the bundled Makefile:

```bash
# 1. Clone the repository
git clone https://github.com/<your-username>/c-image-filters.git
cd c-image-filters

# 2. Build executable
make

# 3. Clean object files & binaries
make clean
```

### Planned Usage

```bash
./c-image-filters -f <filter_name> <input.bmp> <output.bmp>
```

#### Example Commands
```bash
# Apply grayscale filter
./c-image-filters -f grayscale examples/test.bmp output_gray.bmp

# Apply Sobel edge detection
./c-image-filters -f sobel examples/test.bmp output_edges.bmp
```

---

## 🗺️ Roadmap & Progress

- [x] Initial repository scaffolding & clean directory structure
- [x] Strict Makefile configuration (`-Wall -Wextra -std=c99`)
- [x] Generated sample test asset ([`examples/test.bmp`](examples/test.bmp))
- [ ] Define packed structs (`BITMAPFILEHEADER`, `BITMAPINFOHEADER`, `Pixel`)
- [ ] Implement `image_read` with 4-byte stride padding support
- [ ] Implement `image_write` and memory cleanups
- [ ] Verify image I/O integrity (identity test)
- [ ] Implement CLI parser in `main.c`
- [ ] Point filters: Grayscale, Invert, Sepia
- [ ] Geometric filters: Flip horizontal/vertical
- [ ] Convolution filters: Box Blur, Gaussian Blur, Sobel Edge Detection

---

## 📄 License

Distributed under the **MIT License**. See [`LICENSE`](LICENSE) for details.

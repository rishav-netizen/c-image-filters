<div align="center">

  <img src="assets/logo.png" alt="c-image-filters logo" width="160" height="160" />

  <h1>c-image-filters</h1>

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
    <a href="#-roadmap">Roadmap</a> •
    <a href="#-references--documentation">References</a>
  </p>

</div>

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

The implemented filters range from single-pixel channel operations to multi-channel spatial convolutions:

| Filter | Category | Mathematical Operation / Kernel | Visual Effect | Status |
| :--- | :--- | :--- | :--- | :---: |
| 🌑 **Grayscale** | Point Transform | $Y = 0.299R + 0.587G + 0.114B$ | Converts colors to human-perceived luminance | ✅ *Completed* |
| 🔲 **Invert** | Point Transform | $C' = 255 - C$ | Produces a classic photographic negative | ✅ *Completed* |
| 📜 **Sepia** | Color Remap | Weighted warm RGB matrix transform | Warm, antique nostalgic tint | ✅ *Completed* |
| 🌫️ **Box Blur** | Convolution | $\frac{1}{(2r+1)^2} \sum P(x+dx, y+dy)$ | Smooths details by neighborhood averaging | ✅ *Completed* |
| 💫 **Gaussian Blur** | Convolution | $W = e^{-\frac{dx^2+dy^2}{2\sigma^2}}$ | Weighted blur preserving natural edge falloff | ✅ *Completed* |
| 🔍 **Sobel Edge** | Convolution | $G = \sqrt{G_x^2 + G_y^2}$ | Outlines object boundaries and high-frequency edges | ✅ *Completed* |
| 🔄 **Flip Horizontal** | Geometric | $(x, y) \mapsto (W - 1 - x, y)$ | Horizontal mirror image reflection | ✅ *Completed* |
| 🔃 **Flip Vertical** | Geometric | $(x, y) \mapsto (x, H - 1 - y)$ | Vertical upside-down inversion | ✅ *Completed* |

### 🖼️ Visual Filter Samples

Demonstrated on [`examples/tiger.bmp`](examples/tiger.bmp) ($1280 \times 853$):

<div align="center">

| 📷 **Original** | 🌑 **Grayscale** | 🔲 **Invert** |
| :---: | :---: | :---: |
| <img src="assets/samples/original.jpg" width="260" alt="Original Image" /><br><sub><em>Source Reference</em></sub> | <img src="assets/samples/grayscale.jpg" width="260" alt="Grayscale Filter" /><br><sub><code>-f grayscale</code></sub> | <img src="assets/samples/invert.jpg" width="260" alt="Invert Filter" /><br><sub><code>-f invert</code></sub> |

| 📜 **Sepia** | 🌫️ **Box Blur** | 💫 **Gaussian Blur** |
| :---: | :---: | :---: |
| <img src="assets/samples/sepia.jpg" width="260" alt="Sepia Filter" /><br><sub><code>-f sepia</code></sub> | <img src="assets/samples/box_blur.jpg" width="260" alt="Box Blur Filter" /><br><sub><code>-f boxblur -r 6</code></sub> | <img src="assets/samples/gaussian_blur.jpg" width="260" alt="Gaussian Blur Filter" /><br><sub><code>-f gaussian -r 6</code></sub> |

| 🔍 **Sobel Edge** | 🔄 **Flip Horizontal** | 🔃 **Flip Vertical** |
| :---: | :---: | :---: |
| <img src="assets/samples/sobel_edge.jpg" width="260" alt="Sobel Edge Filter" /><br><sub><code>-f sobel</code></sub> | <img src="assets/samples/flip_horizontal.jpg" width="260" alt="Flip Horizontal Filter" /><br><sub><code>-f fliphorizontal</code></sub> | <img src="assets/samples/flip_vertical.jpg" width="260" alt="Flip Vertical Filter" /><br><sub><code>-f flipvertical</code></sub> |

</div>

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
├── 📁 examples/          # Sample 24-bit BMP image fixtures
│   ├── tiger.bmp         # High-detail wildlife close-up (whiskers, fur)
│   ├── porsche.bmp       # Sleek automotive curves and reflections
│   ├── cyberpunk.bmp     # Neon cityscape with wet reflections
│   ├── galaxy.bmp        # Deep-space Andromeda astronomical capture
│   └── test.bmp          # 4x4 24-bit test BMP with multi-color pattern
├── 📁 assets/            # Project artwork, branding & sample galleries
│   ├── logo.png          # App icon & project branding mark (512x512)
│   ├── logo.svg          # Scalable vector logo
│   └── 📁 samples/       # Filter showcase image previews for README
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

### Usage

```bash
./c-image-filters -f <filter_name> [-r <radius>] <input.bmp> <output.bmp>
```

#### Example Commands
```bash
# Apply grayscale filter
./c-image-filters -f grayscale examples/tiger.bmp results/tiger_gray.bmp

# Apply Gaussian blur with a custom convolution radius
./c-image-filters -f gaussian -r 6 examples/tiger.bmp results/tiger_blur.bmp

# Apply Sobel spatial edge detection
./c-image-filters -f sobel examples/tiger.bmp results/tiger_sobel.bmp

# Apply vintage sepia tone
./c-image-filters -f sepia examples/porsche.bmp results/porsche_sepia.bmp
```

---

## 🗺️ Roadmap & Progress

- [x] Initial repository scaffolding & clean directory structure
- [x] Strict Makefile configuration (`-Wall -Wextra -std=c99`)
- [x] Generated sample test asset ([`examples/test.bmp`](examples/test.bmp))
- [x] Define packed structs (`BMPHeader`, `BMPInfoHeader`, `Pixel`)
- [x] Implement `image_read` with 4-byte stride padding support
- [x] Implement `image_write` and memory cleanups
- [x] Verify image I/O integrity (identity test)
- [x] Implement CLI parser in `main.c`
- [x] Point filters: Grayscale, Invert, Sepia
- [x] Geometric filters: Flip horizontal/vertical
- [x] Convolution filters: Box Blur, Gaussian Blur, Sobel Edge Detection

---

## 📚 References & Documentation

* [University of Alberta: BMP File Format Reference](https://www.ece.ualberta.ca/~elliott/ee552/studentAppNotes/2003_w/misc/bmp_file_format/bmp_file_format.htm) — Architectural guide to BMP headers, pixel arrays, and row padding.
* [Microsoft Open Specifications: Windows Data Types](https://learn.microsoft.com/en-us/openspecs/windows_protocols/ms-dtyp/d7edc080-e499-4219-a837-1bc40b64bb04) — Official specification for standard Windows binary types and structure packing.
* [The Open Group Base Specifications: `<stdint.h>`](https://pubs.opengroup.org/onlinepubs/009695399/basedefs/stdint.h.html) — Standard POSIX / C99 fixed-width integer types reference.
			
---

## 📄 License

Distributed under the **MIT License**. See [`LICENSE`](LICENSE) for details.

# hscan — Hash Scanner

**hscan** (short for **Hash Scanner**) is a command-line tool written in C that scans files and directories and produces a configurable hash-like output. When scanning a directory, it recursively visits subdirectories, processes regular files, and prints progress information so you can see which paths are being scanned.

> **Status:** This project is under development. The current implementation uses a custom, `rand()`-based mixing routine; it is **not a cryptographic hash function** and must not be used for security-sensitive integrity checks, signatures, password storage, or authentication.

## Features

- Written in C.
- Recursively traverses directories and their subdirectories.
- Processes regular files found during a directory scan.
- Configurable output length.
- Displays progress logs while scanning.
- Optional terminal colors and spacing in the output.
- Configurable number of output words per line.

## Requirements

- A C compiler such as `gcc`
- `make`
- A POSIX-like environment (for example, Linux)

On Arch Linux, the build tools can be installed with:

```bash
sudo pacman -S --needed base-devel
```

## Build

Clone the repository and enter its directory:

```bash
git clone https://github.com/UnknownSword/hscan.git
cd hscan
```

Build the program:

```bash
make build
```

The resulting executable is expected at:

```text
bin/main
```

To remove generated build files:

```bash
make clean
```

## Usage

The program accepts an input path and optional output-format settings:

```text
./bin/main -i <path> [options]
```

Options:

| Option | Description |
| --- | --- |
| `-i <path>` | Input path to scan. |
| `-l <number>` | Number of 32-bit output words. Each word is printed as 8 hexadecimal characters. |
| `-w <number>` | Number of output words to print on each line. Use `0` to disable line wrapping. |
| `-s <y/n>` | Show (`y`) or hide (`n`) spaces between output words. |
| `-c <y/n>` | Enable (`y`) or disable (`n`) terminal colors. |

If no options are supplied, the program prints its usage information.

### Examples

Scan a directory recursively using the default output length:

```bash
./bin/main -i ./my_directory
```

Request 32 output words and print 4 words per line:

```bash
./bin/main -i ./my_directory -l 32 -w 4
```

Disable colors and spaces between output words:

```bash
./bin/main -i ./my_directory -l 32 -w 4 -s n -c n
```

The output length supplied to `-l` is currently a **word count**, not a bit count. For example, `-l 16` creates 16 32-bit words, displayed as 128 hexadecimal characters (512 bits of displayed data).

## How it works

1. The program takes an input path and output-format options.
2. For a directory, it walks through entries and recursively enters subdirectories.
3. It reads regular files and combines their per-file results into a single result.
4. It prints the final result, while progress messages identify paths as they are processed.

## Current limitations

- The current top-level scan routine calls the directory scanner directly. As a result, **passing a regular file as the top-level `-i` path is not yet supported by the current implementation**, even though hashing an individual file is part of the intended interface.
- The output routine is custom and uses `rand()`-based mixing. It is not a standard hash such as SHA-256 or BLAKE2 and is not suitable for cryptographic use.
- Output size is specified as a count of 32-bit words, rather than directly in bits.
- Behavior can vary for files that cannot be opened because of permissions or filesystem errors.

## License

This project is distributed under the [BSD 3-Clause License](LICENSE).

## Repository

https://github.com/UnknownSword/hscan

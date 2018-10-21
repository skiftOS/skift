<img src="doc/logo/logo128.png" align="right" />

# skiftOS

> Build infrastructure, kernel, and core pakages of the skift Operating System

**skiftOS** is a simple, handmade, build from scratch, 32bit operating system for intel x86 CPU.

**skiftOS** support virtual memory, multi processing with threads, and filesystem.

**skiftOS** provide a rich syscall interface for user application.

# What **skiftOS** look like

<img src="doc/screenshots/Capture d’écran_2018-10-20_16-37-52.png" align="center" />

# Build-it

```sh
# Build the operating system
./SOSBS.py build-all

# Run it in qemu
./SOSBS.py run
```
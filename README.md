# File access filter

An external Linux kernel module for restricting access to user-specified files

## Driver building

Navigate to the driver directory and run from command line:

```bash
make
```

Also, you can immediately load driver into kernel with following command:

```bash
make load
```

To unload driver use:

```bash
make unload
```

## User application building\

Navigate to the tools directory and run from command line:

```bash
cmake -S . -B build
cmake --build build
```

## Testing

Unit tests are built and executed in user space using **CUnit**.

### Install dependencies

Refer to the official CUnit website for installation instructions:

https://cunit.sourceforge.net/

### Running the tests

Navigate to the driver directory and run from command line:

```bash
make test
```
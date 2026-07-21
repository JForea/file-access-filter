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

## User application

### Building

Navigate to the tools directory and run from command line:

```bash
cmake -S . -B build
cmake --build build
```

### Using

After the build navigate to tools/build directory. There you'll see **faf** executable file. To run commands you need to run it with sudo. To see command list run the following command:

```bash
sudo faf help
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

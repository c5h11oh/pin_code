# PIN Code generator

## Current PIN Codes that are avoided
- PIN codes with differences between adjacent digits are identical (e.g. 0000, 1234, 9753)
- PIN codes with 3 or more identical digits 

## Prerequisites
- g++ that supports C++17 standards
- Make
- Python 3 (if you want to run the simple test)

## Compile
```bash
make
```
## Run
```bash
./pin_codes
```

## Clean up (remove persistent data and executable)
```bash
make clean
```

## Remove persistent data
```bash
make clean_persistent_data
```

## Run simple test
```bash
python3 test.py
```

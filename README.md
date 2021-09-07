# NRL-MSISE00 C++ Wrapper by AndoLab

## Install
0. Clone this repository.
1. Download original NRL-MSISE00 (C verstion) from [Official site](https://www.brodo.de/space/nrlmsise/).
 Then, extract tar `tar zxvf nrlmsise-_____.tar.gz`, and copy *nrlmsise-00.h*, *nrlmsise-00.c*, and *nrlmsise-00_data.c* to the repository directory, for example, `cp nrlmsise____/nrlmsise-00.h nrlmsise____/nrlmsise-00.c nrlmsise____/nrlmsise-00_data.c .`
2. patch
```Tcsh
% patch -p1 < nrlmsise-00.patch
```
3. Edit Makefile if necessary, especially PREFIX, and then type `make ; make install`

4. Test
```Tcsh
% make test_nrlmsise
% ./test_nrlmsise
```

## Example of Usage

```C++:exmple
under construction
```

## TODO
* implement options

## Version
0.0.1 (August 30, 2021): Initial commit

## Resources
[MSISE Official](https://ccmc.gsfc.nasa.gov/models/modelinfo.php?model=MSISE)
[MSISE C version](https://www.brodo.de/space/nrlmsise/)

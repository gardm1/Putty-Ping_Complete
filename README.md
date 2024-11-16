# Putty-Ping

**WINDOWS SPECIFIC** <br />
<sub>nmake Makefile to link .lib's and vs2022 to link .dll's</sub>

Not finished

#### Todos: <br />
- [x] Convert code to c code <br />
- [x] Add libgcrypt encryption <br />
- [x] Simplify <br />
- [ ] Embed the c code in go <br />
- [ ] Add CLI built in go (Cobra/Bonzai) <br />
- [ ] Add go crypto as a option as well <br />
- [ ] Simplify again and clean up <br />

### Libgcrypt (gcrypt.h & gpg-error.h) installation guide using cl.exe

**Step one.** <br />
Install MSYS2

* Enter MSYS2 UCRT64. Make sure it's updated <br />
``` bash
pacman -Syu
```
* Install libgcrypt and libgpg-error
``` bash
pacman -S mingw-w64-x86_64-libgcrypt mingw-w64-x86_64-libgpg-error
```
* Install mingw-w64-tools
``` bash
pacman -S mingw-w64-ucrt-x86_64-tools
```

**Step two.** <br />
Build .lib from .dll using .def

* Create a 'C:\temp' folder, cd to 'C:\temp'
  * Copy libgcrypt-20.dll from ucrt64\bin
  * Copy gcrypt.h from ucrt64\include
* From 'C:\temp' run:
  * Make sure libgcrypt-20.dll isn't set as read-only
  ``` bash
  attrib -r libgcrypt-20.dll
  ```
  * Create the .def information
  ``` bash
  gendef libgcrypt-20.dll -o libgcrypt.def
  ```
  * Create the actual libgcrypt-20.def file
  ``` bash
  touch libgcrypt-20.def
  ```
  Open and check that the file isn't empty
  
  * Create a .lib from .def
  ``` bash
  dlltool -d libgcrypt.def -l libgcrypt.lib
  ```

Repeat step two and do the same for gpg-error.h (libgpg-error-0.dll). <br />
Make sure the compiler knows where they are.

Now you can copy this to wherever and use it in a project.

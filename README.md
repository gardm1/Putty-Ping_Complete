# Putty-Ping

**WINDOWS SPECIFIC**

Not finished

#### Todos: <br />
- [x] Convert code to c code <br />
- [ ] Add openssl or libgcrypt encryption <br />
- [ ] Simplify <br />
- [ ] Add CLI built in go (Cobra/Bonzai) <br />
- [ ] Embed the c code in go <br />
- [ ] Add go crypto as a option as well <br />

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

Repeat and do the same for gpg-error.h (libgpg-error-0.dll). <br />
Make sure the .lib files are in the same directory, and <br />
that the .h files are in the same directory afterwards. 

Now you can copy this to wherever and use it in a project. 

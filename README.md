# SFLog - A library for Simple Formatted Logging to stdout

Some information will be provided on this page soon.

In the mean time, you can use the library as follows:
 - In a project using CMake, use [CPM] to import the library as a dependency:
```cmake
CPMAddPackage(
    NAME sflog
    GITHUB_REPOSITORY gabrieleara/sflog
    GIT_TAG some.version.number
)
```

 - then link it against your project like so:
```cmake
target_link_libraries(target_name PUBLIC sflog::sflog)
```

> **NOTICE**: this library depends on [magic_enum] and [fmt]. To avoid
> dependency issues, follow the guide on [CPM wiki] to setup a
> Package Lock file and handle transitive dependencies.


<!-- Links -->
[magic_enum]: https://github.com/Neargye/magic_enum
[fmt]: https://github.com/fmtlib/fmt
[CPM]: https://github.com/cpm-cmake/CPM.cmake
[CPM wiki]: https://github.com/cpm-cmake/CPM.cmake/wiki/Package-lock

# CPM Package Lock
# This file should be committed to version control

# magic_enum
CPMDeclarePackage(magic_enum
  NAME magic_enum
  GIT_TAG v0.9.3
  GITHUB_REPOSITORY Neargye/magic_enum
  OPTIONS "MAGIC_ENUM_OPT_INSTALL YES"
)
# fmt
CPMDeclarePackage(fmt
  NAME fmt
  GIT_TAG 10.1.1
  GITHUB_REPOSITORY fmtlib/fmt
  OPTIONS "FMT_INSTALL ON"
)

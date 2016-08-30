# runtime-linker-audit

This example program shows how to connect auditing hooks to prevent loading of dynamic library symbols based on user programmable metrics.
Currently it filters out symbols from shared objects that are not part of a Fedora system repository, but it can be easily adjusted to perform other types of filtering.

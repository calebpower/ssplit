# Stream Splitter

Quick program to split incoming streams into discrete chunks that are then sent
to the standard input of some other specified program.

## Building

You can build with `make` or `make all` and you can clean with `make clean`.

## Usage

```sh
ssplit <sz> <prgm> [prefix]
```

- **sz** indicates discrete chunk size in mebibytes
- **prgm** indicates the consumer program
- **prefix** indicates some string to be prepended to each chunk

Let's say you have some file `input.txt` that you want to split and send off to
both standard output and to some file via `tee`. You'd like these files to
avoid conflicting names. You can use `[[ORD]]` in your filename to assist. For
example:

```sh
cat input.txt | ssplit 2 "tee -a output_[[ORD]].txt" START
```

If `input.txt` is 9 mebibytes, this will yield files `output_1.txt` through
`output_5.txt`. The first four output files will be 2,097,157 bytes in length
(that is, 2 mebibytes plus the 5 bytes attributed to the "START" prefix), and
the last output file will be the remaining 1,048,581 bytes plus the 5 bytes
attributed to the "START" prefix, for a total of 1,048,586 bytes.

This was designed in general as a way to throw large files over an SSH tunnel
where connections may time out. One might have done something similar to the
following:

```sh
cat in.file \
  | cat <(echo 'C0600 109951162776 stdin') - \
  | ssh user@server "scp -t out.file"
```

Theoretically, this tool could be used as follows.

```sh
cat in.file \
  | ssplit 2 \
    "ssh user@server \"scp -t out_[[ORD]].file\"" \
    "C0600 109951162776 stdin"
```

## Caveats

This program is largely untested, so use it at your own risk.

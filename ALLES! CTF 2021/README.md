# e?️PF (1 solve)

> TL;DR _Rust Reverse_ _Solana ebpf_

I solve this challenge far after the game ended.

A ebpf ELF file is given, stripped. I used some tools to help the reverse
process.

- [jawilk/solana-poc-debugging-example](https://github.com/jawilk/solana-poc-debugging-example) is a gdb debugging tool, which is very important.
- [Toizi/eBPF-for-Ghidra](https://github.com/Toizi/eBPF-for-Ghidra) is a Ghidra
  extension which recognized the solana syscall, which is quite helpful.

I also manually produces some header file during the reverseing process
[here](./ebpf.h).

The whole process is just reverseing a rust binary, which is not quick plesant.

It turns out that the logic is just shuffles the bits in the input.

```python
data = [0x05, 0x23, 0x23, 0x25, 0x55, 0x84, 0xd7, 0x31, 0x83, 0xa5, 0x80, 0xa5,
        0x80, 0xd5, 0xb1, 0x85, 0xe3, 0xa1, 0xd5, 0x80, 0xe1, 0xe7, 0xd1, 0x80,
        0xa5, 0xd1, 0xd5, 0xc5, 0x83, 0xd1, 0xa5, 0xc5, 0xc7, 0xa5, 0xe3, 0xa1,
        0xb7 ]

tab = [2,6,5,1,4,7,0,3]
lst = []

for c in data:
    res = 0
    for i in range(0,8):
        res |= ((c >> tab[i])&1) << i
    lst += [res]

flag = bytes.fromhex(''.join([hex(i)[2:] for i in lst]))
print(flag)
# ALLES!{The e stands for erschreckend}
```

I cannot find the challenge's binary anywhere after the game ended. The official
[repository](https://github.com/infosecstreams/allesctf2021) only contains a
image to the challenge description, without the binary itself. I dm the ALLES
team and the challenge author on twitter without receiving any response. Then I
dm the only solver of this chall during the game: Synacktiv.
[Synacktiv](https://twitter.com/Synacktiv) is very nice to share me the binary.
I thanks their help very much. The binary file [here](./ebpf_reverse_alles).

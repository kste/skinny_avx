Skinny
=========

Skinny is a lightweight tweakable block cipher which provides strong security guarantees against
common attack vectors. This is an implementation of Skinny for modern high-end platforms like Intel Haswell and Skylake. It utilizes the available SIMD instruction sets to achieve high performance when processing multiple inputs.

For encrypting 64 blocks in parallel the following performance (in cycles per byte) can be achieved:

|                | Haswell  | Skylake  |
| ---------------|:--------:|:--------:|
| Skinny-64/128  | 2.58 cpb | 2.48 cpb |
| Skinny-128/128 | 3.78 cpb | 3.43 cpb |



For more information you can look at the [Skinny Website](https://sites.google.com/site/skinnycipher/) or the [Skinny Paper](https://eprint.iacr.org/2016/660).

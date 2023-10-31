# kaeru
kaeru is an ARMv7 payload that provides arbitrary code execution on MediaTek bootloaders (LK) with full permissions, initiated post-hardware initialization and before the main LK function (app) execution. For more details about it, [visit and read my blog](https://blog.r0rt1z2.com/unsigned-code-execution-in-lk-bootloaders.html).

## Building
### Requirements
* Python 3
* [`liblk`](https://github.com/R0rt1z2/liblk)
* [`gcc-arm-none-eabi`](https://developer.arm.com/downloads/-/gnu-rm)

### Linux
The payload needs to be built before injecting it:
```bash
git clone git@github.com:R0rt1z2/kaeru.git
cd kaeru
make
```
> _Debugging can be enabled by with `export KAERU_DEBUG=1`._

## Injecting
After successfully building the payload, it must be injected into your LK image with the provided script:
```bash
python3 inject_payload bin/lk.bin build/payload.bin <payload_address> <caller_address>
```
> _Both the payload address and the caller address can be found in `common.h`._

## License
This project is licensed under the GPLv3 license - see the [`LICENSE`](https://github.com/R0rt1z2/kaeru/blob/mt8163-aquaris_m8/LICENSE) file for details.

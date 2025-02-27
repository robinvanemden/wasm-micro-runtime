WebAssembly Micro Runtime Attributions
======================================

WAMR project reused some components from other open source project:
- **cJson**: used in the host_tool for remotely managing wasm applications
- **contiki-ng**: for the coap protocol implementation
- **freebsd libm**: used in core/shared/platform/alios/bh_math.c
- **littlevgl**: for the gui samples and wrapped the wasm graphic layer
- **llvm**. for the AOT/JIT compilation
- **wasm-c-api**. to implement the C-APIs of wasm. using headers and sameples
- **wasmtime**: for the wasi libc implementation
- **zephyr**. for several platform specific examples

The WAMR fast interpreter is a clean room development. We would acknowledge the inspirations by [WASM3](https://github.com/wasm3/wasm3) open source project for the approach of pre-calculated oprand stack location.

|  third party components | version number | latest release | vendor pages | CVE details |
| --- | --- | --- | --- | --- |
| cjson | 1.7.10 | 1.7.14 | https://github.com/DaveGamble/cJSON | https://www.cvedetails.com/vendor/19164/Cjson-Project.html |
| contiki-ng (er-coap) | unspecified | 3.0 | https://github.com/contiki-os/contiki | https://www.cvedetails.com/vendor/16528/Contiki-os.html |
| freebsd libm | unspecified | 13.0 | https://www.freebsd.org/ | https://www.cvedetails.com/vendor/6/Freebsd.html |
| littlevgl | 6.0.1 | 7.11.0 | https://lvgl.io/ | |
| llvm | 11.0.1 | 12.0.0 | https://llvm.org | https://www.cvedetails.com/vendor/13260/Llvm.html |
| wasm-c-api | ac9b509f4df86e40e56e9b01f3f49afab0100037 | c9d31284651b975f05ac27cee0bab1377560b87e | https://github.com/WebAssembly/wasm-c-api | |
| wasmtime | unspecified | v0.26.0 | https://github.com/bytecodealliance/wasmtime | |
| zephyr | unspecified | v2.5.0 | https://www.zephyrproject.org/ | https://www.cvedetails.com/vendor/19255/Zephyrproject.html |

## Licenses

### cJson

[LICENSE](./test-tools/host-tool/external/cJSON/LICENSE)

### contiki-ng

[LICENSE](./core/shared/coap/er-coap/LICENSE.md)

### freebsd libm

[COPYRIGHT](./core/shared/platform/common/math/COPYRIGHT)

### littlevgl

[LICENCE](./samples/littlevgl/LICENCE.txt)

[LICENSE](./core/deps/lvgl/LICENCE.txt)

### llvm

[LICENSE](./core/deps/llvm/llvm/LICENCE.txt)

### wasm-c-api

[LICENSE](./samples/wasm-c-api/src/LICENSE)

### wasmtime

[LICENSE](./core/iwasm/libraries/libc-wasi/sandboxed-system-primitives/LICENSE)

[LICENSE](./core/iwasm/libraries/libc-wasi/sandboxed-system-primitives/src/LICENSE)

[LICENSE](./core/iwasm/libraries/libc-wasi/sandboxed-system-primitives/include/LICENSE)

### zephyr

[LICENSE](./samples/gui/wasm-runtime-wgl/src/platform/zephyr/LICENSE)

### wac

[LICENSE](./tests/wamr-test-suites/spec-test-script/LICENSE)

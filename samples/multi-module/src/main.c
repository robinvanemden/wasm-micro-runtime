#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bh_read_file.h"
#include "platform_common.h"
#include "wasm_export.h"

static char *
build_module_path(const char *module_name)
{
    const char *module_search_path = "./wasm-apps";
    const char *format = "%s/%s.wasm";
    int sz = strlen(module_search_path) + strlen("/") + strlen(module_name)
             + strlen(".wasm") + 1;
    char *wasm_file_name = BH_MALLOC(sz);
    if (!wasm_file_name) {
        return NULL;
    }

    snprintf(wasm_file_name, sz, format, module_search_path, module_name);
    return wasm_file_name;
}

static bool
module_reader_cb(const char *module_name, uint8 **p_buffer, uint32 *p_size)
{
    char *wasm_file_path = build_module_path(module_name);
    if (!wasm_file_path) {
        return false;
    }

    printf("- bh_read_file_to_buffer %s\n", wasm_file_path);
    *p_buffer = (uint8_t *)bh_read_file_to_buffer(wasm_file_path, p_size);
    BH_FREE(wasm_file_path);
    return *p_buffer != NULL;
}

static void
module_destroyer_cb(uint8 *buffer, uint32 size)
{
    printf("- release the read file buffer\n");
    if (!buffer) {
        return;
    }

    BH_FREE(buffer);
    buffer = NULL;
}

/* 10M */
static char sandbox_memory_space[10 * 1024 * 1024] = { 0 };
int
main()
{
    bool ret = false;
    /* 16K */
    const uint32 stack_size = 16 * 1024;
    const uint32 heap_size = 16 * 1024;

    RuntimeInitArgs init_args = { 0 };
    char error_buf[128] = { 0 };
    /* parameters and return values */
    char *args[1] = { 0 };

    uint8 *file_buf = NULL;
    uint32 file_buf_size = 0;
    wasm_module_t module = NULL;
    wasm_module_inst_t module_inst = NULL;

    /* all malloc() only from the given buffer */
    init_args.mem_alloc_type = Alloc_With_Pool;
    init_args.mem_alloc_option.pool.heap_buf = sandbox_memory_space;
    init_args.mem_alloc_option.pool.heap_size = sizeof(sandbox_memory_space);

    printf("- wasm_runtime_full_init\n");
    /* initialize runtime environment */
    if (!wasm_runtime_full_init(&init_args)) {
        printf("Init runtime environment failed.\n");
        goto EXIT;
    }

#if WASM_ENABLE_MULTI_MODULE != 0
    printf("- wasm_runtime_set_module_reader\n");
    /* set module reader and destroyer */
    wasm_runtime_set_module_reader(module_reader_cb, module_destroyer_cb);
#endif

    /* load WASM byte buffer from WASM bin file */
    if (!module_reader_cb("mC", &file_buf, &file_buf_size)) {
        goto RELEASE_RUNTIME;
    }

    /* load mC and let WAMR load mA and mB */
    printf("- wasm_runtime_load\n");
    if (!(module = wasm_runtime_load(file_buf, file_buf_size, error_buf,
                                     sizeof(error_buf)))) {
        printf("%s\n", error_buf);
        goto RELEASE_BINARY;
    }

    /* instantiate the module */
    printf("- wasm_runtime_instantiate\n");
    if (!(module_inst = wasm_runtime_instantiate(
              module, stack_size, heap_size, error_buf, sizeof(error_buf)))) {
        printf("%s\n", error_buf);
        goto UNLOAD_MODULE;
    }

    /* call some functions of mC */
    printf("\n----------------------------------------\n");
    printf("call \"C\", it will return 0xc:i32, ===> ");
    wasm_application_execute_func(module_inst, "C", 0, &args[0]);
    printf("call \"call_B\", it will return 0xb:i32, ===> ");
    wasm_application_execute_func(module_inst, "call_B", 0, &args[0]);
    printf("call \"call_A\", it will return 0xa:i32, ===>");
    wasm_application_execute_func(module_inst, "call_A", 0, &args[0]);

    /* call some functions of mB */
    printf("call \"mB.B\", it will return 0xb:i32, ===>");
    wasm_application_execute_func(module_inst, "$mB$B", 0, &args[0]);
    printf("call \"mB.call_A\", it will return 0xa:i32, ===>");
    wasm_application_execute_func(module_inst, "$mB$call_A", 0, &args[0]);

    /* call some functions of mA */
    printf("call \"mA.A\", it will return 0xa:i32, ===>");
    wasm_application_execute_func(module_inst, "$mA$A", 0, &args[0]);
    printf("----------------------------------------\n\n");
    ret = true;

    printf("- wasm_runtime_deinstantiate\n");
    wasm_runtime_deinstantiate(module_inst);
UNLOAD_MODULE:
    printf("- wasm_runtime_unload\n");
    wasm_runtime_unload(module);
RELEASE_BINARY:
    module_destroyer_cb(file_buf, file_buf_size);
RELEASE_RUNTIME:
    printf("- wasm_runtime_destroy\n");
    wasm_runtime_destroy();
EXIT:
    return ret ? 0 : 1;
}

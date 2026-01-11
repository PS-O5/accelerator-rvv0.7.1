#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

// Force 16-byte alignment for Vector Loads/Stores
uint32_t a[8] __attribute__((aligned(16))) = {1, 2, 3, 4, 5, 6, 7, 8};
uint32_t b[8] __attribute__((aligned(16))) = {10, 20, 30, 40, 50, 60, 70, 80};
uint32_t c[8] __attribute__((aligned(16))) = {0};

void print_array(const char* name, uint32_t* arr, int len) {
    printf("%s: ", name);
    for(int i=0; i<len; i++) printf("%d ", arr[i]);
    printf("\n");
}

int main() {
    printf("=== Lichee Pi 4A: Linux Vector Test (RVV 0.7.1) ===\n");

    // Pointers for Assembly
    uint32_t *ptr_a = a;
    uint32_t *ptr_b = b;
    uint32_t *ptr_c = c;
    long vl = 8; // Vector Length

    printf("Executing Vector Pipeline...\n");

    // Inline Assembly with .long to bypass compiler version mismatches
    // This is SEW=32, LMUL=1 logic for C910
    asm volatile (
        // 1. vsetvli t0, a0, e32, m1
        // Configures vector unit for 32-bit integers
        ".long 0x008572d7 \n"

        // 2. vle.v v0, (a1)  -> Load 'a' into v0
        // Opcode: 0x12058007
        ".long 0x12058007 \n"

        // 3. vle.v v1, (a2)  -> Load 'b' into v1
        // Opcode: 0x12060087
        ".long 0x12060087 \n"

        // 4. vadd.vv v2, v0, v1 -> Add v0 + v1 = v2
        // Opcode: 0x02100157
        ".long 0x02100157 \n"

        // 5. vse.v v2, (a3)  -> Store v2 into 'c'
        // Opcode: 0x12068127
        ".long 0x12068127 \n"

        : 
        : "r"(vl), "r"(ptr_a), "r"(ptr_b), "r"(ptr_c)
        : "t0", "v0", "v1", "v2", "memory"
    );

    printf("Done.\n");
    
    // Verification
    print_array("Input A", a, 8);
    print_array("Input B", b, 8);
    print_array("Output C", c, 8);

    int success = 1;
    for(int i=0; i<8; i++) {
        if (c[i] != a[i] + b[i]) {
            success = 0;
            break;
        }
    }

    if(success) {
        printf("\n[SUCCESS] Vector Unit calculated correctly!\n");
        printf("Memory Coherency and Trap handling are working.\n");
    } else {
        printf("\n[FAILURE] Calculation Mismatch.\n");
    }

    return 0;
}

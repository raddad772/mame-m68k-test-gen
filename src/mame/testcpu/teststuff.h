#include "helpers/multisize_memaccess.h"

#define M8 1
#define M16 2
#define M32 4
#define M64 8

#define R8 cR[M8](ptr, 0); ptr += 1
#define R16 cR[M16](ptr, 0); ptr += 2
#define R32 cR[M32](ptr, 0); ptr += 4
#define R64 cR[M64](ptr, 0); ptr += 8
#define W8(v) cW[M8](ptr, 0); ptr += 1
#define W16(v) cW[M16](ptr, 0); ptr += 2
#define W32(v) cW[M32](ptr, 0); ptr += 4
#define W64(v) cW[M64](ptr, 0); ptr += 8

#define MAX_RAM_PAIRS 100
#define MAX_TRANSACTIONS 100

enum transaction_kind {
    tk_idle_cycles,
    tk_read,
    tk_write,
    tk_tas
};

struct transaction {
    enum transaction_kind kind;
    u32 len;
    u32 fc;
    u32 addr_bus;
    u32 sz;
    u32 data_bus;
    u32 UDS, LDS;

    u32 visited;

    u32 start_cycle;
};

struct RAM_pair {
    u32 addr;
    u16 val;
};

struct m68k_test_transactions {
    u32 num_transactions;
    struct transaction items[MAX_TRANSACTIONS];
};

struct m68k_test_state {
    u32 d[8];
    u32 a[7];
    u32 usp, ssp, sr, pc;
    u32 prefetch[2];
    u32 num_RAM;
    struct RAM_pair RAM_pairs[MAX_RAM_PAIRS];
};

// individual test
struct m68k_test {
    char name[100];
    u32 opcode;
    struct m68k_test_state initial;
    struct m68k_test_state final;
    struct m68k_test_transactions transactions;
    u32 num_cycles;

    u32 current_cycle;
};

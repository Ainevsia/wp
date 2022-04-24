#include <stdbool.h>
#define SIZE_PUBKEY 32

typedef signed char int8_t;
typedef unsigned char uint8_t;
typedef signed short int16_t;
typedef unsigned short uint16_t;
typedef signed int int32_t;
typedef unsigned int uint32_t;
typedef signed long long int64_t;
typedef unsigned long long uint64_t;
// typedef int64_t ssize_t;
// typedef uint64_t size_t;

/**
 * Public key
 */
typedef struct {
  uint8_t x[SIZE_PUBKEY];
} SolPubkey;


/**
 * Keyed Account
 */
typedef struct {
  SolPubkey *key;      /** Public key of the account */
  uint64_t *lamports;  /** Number of lamports owned by this account */
  uint64_t data_len;   /** Length of data in bytes */
  uint8_t *data;       /** On-chain data within this account */
  SolPubkey *owner;    /** Program that owns this account */
  uint64_t rent_epoch; /** The epoch at which this account will next owe rent */
  bool is_signer;      /** Transaction was signed by this account's key? */
  bool is_writable;    /** Is the account writable? */
  bool executable;     /** This account's data contains a loaded program (and is now read-only) */
} SolAccountInfo;

/**
 * The Solana runtime provides a memory region that is available to programs at
 * a fixed virtual address and length. The builtin functions `sol_calloc` and
 * `sol_free` call into the Solana runtime to allocate from this memory region
 * for heap operations.  Because the memory region is directly available to
 * programs another option is a program can implement their own heap directly on
 * top of that region.  If a program chooses to implement their own heap they
 * should not call the builtin heap functions because they will conflict.
 * `HEAP_START_ADDRESS` and `HEAP_LENGTH` specify the memory region's start
 * virtual address and length.
 */

// #define HEAP_START_ADDRESS (uint64_t)0x300000000
// #define HEAP_LENGTH (uint64_t)(32 * 1024)

/**
 * Structure that the program's entrypoint input data is deserialized into.
 */
typedef struct {
  SolAccountInfo* ka; /** Pointer to an array of SolAccountInfo, must already
                          point to an array of SolAccountInfos */
  uint64_t ka_num; /** Number of SolAccountInfo entries in `ka` */
  const uint8_t *data; /** pointer to the instruction data */
  uint64_t data_len; /** Length in bytes of the instruction data */
  const SolPubkey *program_id; /** program_id of the currently executing program */
} SolParameters;

// #define SOL_ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))



/**
 * Account Meta
 */
typedef struct {
  SolPubkey *pubkey; /** An account's public key */
  bool is_writable; /** True if the `pubkey` can be loaded as a read-write account */
  bool is_signer; /** True if an Instruction requires a Transaction signature matching `pubkey` */
} SolAccountMeta;

/**
 * Instruction
 */
typedef struct {
  SolPubkey *program_id; /** Pubkey of the instruction processor that executes this instruction */
  SolAccountMeta *accounts; /** Metadata for what accounts should be passed to the instruction processor */
  uint64_t account_len; /** Number of SolAccountMetas */
  uint8_t *data; /** Opaque data passed to the instruction processor */
  uint64_t data_len; /** Length of the data in bytes */
} SolInstruction;


/**
 * Seed used to create a program address or passed to sol_invoke_signed
 */
typedef struct {
  const uint8_t *addr; /** Seed bytes */
  uint64_t len; /** Length of the seed bytes */
} SolSignerSeed;

/**
 * Seeds used by a signer to create a program address or passed to
 * sol_invoke_signed
 */
typedef struct {
  const SolSignerSeed *addr; /** An array of a signer's seeds */
  uint64_t len; /** Number of seeds */
} SolSignerSeeds;

/**
 * Invoke another program and sign for some of the keys
 *
 * @param instruction Instruction to process
 * @param account_infos Accounts used by instruction
 * @param account_infos_len Length of account_infos array
 * @param seeds Seed bytes used to sign program accounts
 * @param seeds_len Length of the seeds array
 */
static uint64_t sol_invoke_signed(
    const SolInstruction *instruction,
    const SolAccountInfo *account_infos,
    int account_infos_len,
    const SolSignerSeeds *signers_seeds,
    int signers_seeds_len
);

/**
 * Internal cross-program invocation function
 */
uint64_t sol_invoke_signed_c(
  const SolInstruction *instruction,
  const SolAccountInfo *account_infos,
  int account_infos_len,
  const SolSignerSeeds *signers_seeds,
  int signers_seeds_len
);

typedef struct {
	uint8_t * ptr;
	uint64_t cap;
	uint64_t len;
} Vec;


typedef struct {
  SolPubkey program_id;
  Vec accounts;
  uint8_t * instruction_data;
} RustTupleDeser;

typedef struct {
  uint64_t u1;
  uint64_t u2;
  uint64_t borrowed;
  uint8_t * data;
} Rcu8ptr;

typedef struct {
  SolPubkey * key;
  uint64_t * lamports;
  Rcu8ptr * data;
  SolPubkey * owner;
  bool is_signer;
  bool is_writable;
  bool executable;
  uint64_t rent_epoch;
} AccountInfo;

typedef struct {
  AccountInfo * ptr;
  uint64_t cap;
  uint64_t len;
} VecAccountInfo;

typedef struct {
  uint8_t * ptr;
  uint64_t len;
} u8array;

typedef struct {
  SolPubkey * program_id;
  VecAccountInfo accounts;
  u8array ix_data;
} Deserialized;



typedef struct {
  uint8_t * start;
  uint8_t * end;
} Range;

typedef struct {
  ulonglong start;
  ulonglong end;
} RangeNum;

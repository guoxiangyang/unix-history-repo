/*
 * Copyright (c) 1993 The Regents of the University of California.
 * Copyright (c) 1993 Jan-Simon Pendry
 * All rights reserved.
 *
 * This code is derived from software contributed to Berkeley by
 * Jan-Simon Pendry.
 *
 * %sccs.include.redist.c%
 *
 *	@(#)procfs_ctl.c	8.2 (Berkeley) %G%
 *
 * From:
 *	$Id: procfs_ctl.c,v 3.2 1993/12/15 09:40:17 jsp Exp $
 */

#include <sys/param.h>
#include <sys/systm.h>
#include <sys/time.h>
#include <sys/kernel.h>
#include <sys/proc.h>
#include <sys/vnode.h>
#include <sys/ioctl.h>
#include <sys/tty.h>
#include <sys/resource.h>
#include <sys/resourcevar.h>
#include <miscfs/procfs/procfs.h>

/*
 * True iff process (p) is in trace wait state
 * relative to process (curp)
 */
#define TRACE_WAIT_P(curp, p) \
	((p)->p_stat == SSTOP && \
	 (p)->p_pptr == (curp) && \
	 ((p)->p_flag & P_TRACED))

#ifdef notdef
#define FIX_SSTEP(p) { \
		procfs_fix_sstep(p); \
	} \
}
#else
#define FIX_SSTEP(p)
#endif

#define PROCFS_CTL_ATTACH	1
#define PROCFS_CTL_DETACH	2
#define PROCFS_CTL_STEP		3
#define PROCFS_CTL_RUN		4
#define PROCFS_CTL_WAIT		5

static vfs_namemap_t ctlnames[] = {
	/* special /proc commands */
	{ "attach",	PROCFS_CTL_ATTACH },
	{ "detach",	PROCFS_CTL_DETACH },
	{ "step",	PROCFS_CTL_STEP },
	{ "run",	PROCFS_CTL_RUN },
	{ "wait",	PROCFS_CTL_WAIT },
	{ 0 },
};

static vfs_namemap_t signames[] = {
	/* regular signal names */
	{ "hup",	SIGHUP },	{ "int",	SIGINT },
	{ "quit",	SIGQUIT },	{ "ill",	SIGILL },
	{ "trap",	SIGTRAP },	{ "abrt",	SIGABRT },
	{ "iot",	SIGIOT },	{ "emt",	SIGEMT },
	{ "fpe",	SIGFPE },	{ "kill",	SIGKILL },
	{ "bus",	SIGBUS },	{ "segv",	SIGSEGV },
	{ "sys",	SIGSYS },	{ "pipe",	SIGPIPE },
	{ "alrm",	SIGALRM },	{ "term",	SIGTERM },
	{ "urg",	SIGURG },	{ "stop",	SIGSTOP },
	{ "tstp",	SIGTSTP },	{ "cont",	SIGCONT },
	{ "chld",	SIGCHLD },	{ "ttin",	SIGTTIN },
	{ "ttou",	SIGTTOU },	{ "io",		SIGIO },
	{ "xcpu",	SIGXCPU },	{ "xfsz",	SIGXFSZ },
	{ "vtalrm",	SIGVTALRM },	{ "prof",	SIGPROF },
	{ "winch",	SIGWINCH },	{ "info",	SIGINFO },
	{ "usr1",	SIGUSR1 },	{ "usr2",	SIGUSR2 },
	{ 0 },
};

static int
procfs_control(curp, p, op)
	struct proc *curp;
	struct proc *p;
	int op;
{
	int error;

	/*
	 * Attach - attaches the target process for debugging
	 * by the calling process.
	 */
	if (op == PROCFS_CTL_ATTACH) {
		/* check whether already being traced */
		if (p->p_flag & P_TRACED)
			return (EBUSY);

		/* can't trace yourself! */
		if (p->p_pid == curp->p_pid)
			return (EINVAL);

		/*
		 * Go ahead and set the trace flag.
		 * Save the old parent (it's reset in
		 *   _DETACH, and also in kern_exit.c:wait4()
		 * Reparent the process so that the tracing
		 *   proc gets to see all the action.
		 * Stop the target.
		 */
		p->p_flag |= P_TRACED;
		p->p_xstat = 0;		/* XXX ? */
		if (p->p_pptr != curp) {
			p->p_oppid = p->p_pptr->p_pid;
			proc_reparent(p, curp);
		}
		psignal(p, SIGSTOP);
		return (0);
	}

	/*
	 * Target process must be stopped, owned by (curp) and
	 * be set up for tracing (P_TRACED flag set).
	 * Allow DETACH to take place at any time for sanity.
	 * Allow WAIT any time, of course.
	 */
	switch (op) {
	case PROCFS_CTL_DETACH:
	case PROCFS_CTL_WAIT:
		break;

	default:
		if (!TRACE_WAIT_P(curp, p))
			return (EBUSY);
	}

	/*
	 * do single-step fixup if needed
	 */
	FIX_SSTEP(p);

	/*
	 * Don't deliver any signal by default.
	 * To continue with a signal, just send
	 * the signal name to the ctl file
	 */
	p->p_xstat = 0;

	switch (op) {
	/*
	 * Detach.  Cleans up the target process, reparent it if possible
	 * and set it running once more.
	 */
	case PROCFS_CTL_DETACH:
		/* if not being traced, then this is a painless no-op */
		if ((p->p_flag & P_TRACED) == 0)
			return (0);

		/* not being traced any more */
		p->p_flag &= ~P_TRACED;

		/* give process back to original parent */
		if (p->p_oppid != p->p_pptr->p_pid) {
			struct proc *pp;

			pp = pfind(p->p_oppid);
			if (pp)
				proc_reparent(p, pp);
		}

		p->p_oppid = 0;
		p->p_flag &= ~P_WAITED;	/* XXX ? */
		wakeup((caddr_t) curp);	/* XXX for CTL_WAIT below ? */

		break;

	/*
	 * Step.  Let the target process execute a single instruction.
	 */
	case PROCFS_CTL_STEP:
		procfs_sstep(p);
		break;

	/*
	 * Run.  Let the target process continue running until a breakpoint
	 * or some other trap.
	 */
	case PROCFS_CTL_RUN:
		break;

	/*
	 * Wait for the target process to stop.
	 * If the target is not being traced then just wait
	 * to enter
	 */
	case PROCFS_CTL_WAIT:
		error = 0;
		if (p->p_flag & P_TRACED) {
			while (error == 0 &&
					(p->p_stat != SSTOP) &&
					(p->p_flag & P_TRACED) &&
					(p->p_pptr == curp)) {
				error = tsleep((caddr_t) p,
						PWAIT|PCATCH, "procfsx", 0);
			}
			if (error == 0 && !TRACE_WAIT_P(curp, p))
				error = EBUSY;
		} else {
			while (error == 0 && p->p_stat != SSTOP) {
				error = tsleep((caddr_t) p,
						PWAIT|PCATCH, "procfs", 0);
			}
		}
		return (error);

	default:
		panic("procfs_control");
	}

	if (p->p_stat == SSTOP)
		setrunnable(p);
	return (0);
}

int
procfs_doctl(curp, p, pfs, uio)
	struct proc *curp;
	struct pfsnode *pfs;
	struct uio *uio;
	struct proc *p;
{
	int xlen;
	int error;
	char msg[PROCFS_CTLLEN+1];
	vfs_namemap_t *nm;

	if (uio->uio_rw != UIO_WRITE)
		return (EOPNOTSUPP);

	xlen = PROCFS_CTLLEN;
	error = vfs_getuserstr(uio, msg, &xlen);
	if (error)
		return (error);

	/*
	 * Map signal names into signal generation
	 * or debug control.  Unknown commands and/or signals
	 * return EOPNOTSUPP.
	 *
	 * Sending a signal while the process is being debugged
	 * also has the side effect of letting the target continue
	 * to run.  There is no way to single-step a signal delivery.
	 */
	error = EOPNOTSUPP;

	nm = vfs_findname(ctlnames, msg, xlen);
	if (nm) {
		error = procfs_control(curp, p, nm->nm_val);
	} else {
		nm = vfs_findname(signames, msg, xlen);
		if (nm) {
			if (TRACE_WAIT_P(curp, p)) {
				p->p_xstat = nm->nm_val;
				FIX_SSTEP(p);
				setrunnable(p);
			} else {
				psignal(p, nm->nm_val);
			}
			error = 0;
		}
	}

	return (error);
}

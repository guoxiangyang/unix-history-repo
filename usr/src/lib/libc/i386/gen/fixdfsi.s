/*-
 * Copyright (c) 1990 The Regents of the University of California.
 * All rights reserved.
 *
 * This code is derived from software contributed to Berkeley by
 * William Jolitz.
 *
 * %sccs.include.redist.c%
 */

#if defined(LIBC_SCCS) && !defined(lint)
	.asciz "@(#)fixdfsi.s	5.1 (Berkeley) 5/15/90"
#endif /* LIBC_SCCS and not lint */

	.globl ___fixdfsi
___fixdfsi:
	fldl	4(%esp)
	fistpl	4(%esp)
	movl	4(%esp),%eax
	ret

addiu	$sp, $sp, 4
sw		$t5, 0($sp)
lw		$t1, 0($sp)
subu	$sp, $sp, 4
syscall

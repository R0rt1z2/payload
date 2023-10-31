#pragma once

// ARM does not have a division instruction.
unsigned int __aeabi_uidiv(unsigned int n, unsigned int d);
#!/bin/bash

gcc -Iinclude main.c crypto/*.c crypto/bn/*.c crypto/ec/*.c crypto/asn1/*.c crypto/stack/*.c 2>&1 -D SIXTY_FOUR_BIT_LONG -D OPENSSL_API_COMPAT=30000 -D OPENSSL_CONFIGURED_API=30000 -D OPENSSL_NO_ENGINE -g

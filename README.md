This repository contains code written for the advent-of-code challenges. https://adventofcode.com

The advent of code poses programming challenges, typically of increasing complexity. 

I'm an old programmer, so I prefer old languages. I use C and for AoC I limit myself to
only using the following libraries 
- <stdlib.h> (because here are malloc, free, memset, memcpy, NULL)
- <stdio.h> (because here are fgets, scanf, printf, sscanf, snprintf, stdin, stdout, EOF)
- <string.h> (because here are strcmp, strcpy, strdup, strtok, strtok_r)

I don't provide a build-script. In most cases there is only one file (main.c), which should
compile with your favorite C compiler. I've also provided the input I received from AoC,
but you probably received something different when you try this yourself.

All programs read input from stdin.

/*----------- file -- MAINRF.C -------------------*/
/*          Standard REFAL-initiator              */
/*       Last modification : 05.04.2005 (BLF)     */
/*------------------------------------------------*/

/* BLF GO -> go */
extern char rgo() asm("rgo");

void main()
{
    rfexec(rgo);
}

/*---------  end of file MAINRF.C  ---------*/

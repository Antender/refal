/*----------- file -- MAINRF.C -------------------*/
/*          Standard REFAL-initiator              */
/*       Last modification : 05.04.2005 (BLF)     */
/*------------------------------------------------*/

/* BLF GO -> go */
extern char rgo() __asm__("rgo");

int main()
{
    rfexec(rgo);
}

/*---------  end of file MAINRF.C  ---------*/

int kolosh;
char tmp[255];

void error(char* s)
{
    pchk();
    pchk_t();
    kolosh++;
    log("***** ");
    log(s);
}

void error_message(char* s)
{
    error(s);
    log("\n");
}

void error_message_label(char* s, char* sid, int lsid, char* s1)
{
    error(s);
    memcpy(tmp, sid, lsid);
    tmp[lsid] = '\0';
    log(tmp);
    log(s1);
    log("\n");
}

void error_message_character(char* s, char c)
{
    error(s);
    tmp[0] = c;
    tmp[1] = '\0';
    log(tmp);
    log('\n');
}
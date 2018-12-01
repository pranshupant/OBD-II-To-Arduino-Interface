long wheelspeed(unsigned long ti)
{
    unsigned long totaltime = 0;
    unsigned long t1 = 0;
    unsigned long t2 = 0;
    unsigned long tf = 0;

    long lspeed = 0;
    double deltime = 0;
    double dspeed = 0;
    int count = 1;
    while(totaltime < 500)
    {
        tf = millis();
        totaltime = tf - ti;

        t1 = t2;

        if (digitalRead(2) == LOW && count != 0)
        {
            t2 = millis();
            deltime = t2 - t1;
            deltime /= 1000.00;
            if (deltime > 0 && deltime < 6000)
            {
                dspeed = SPF / deltime;
            }
            count = 0;
        }
        else if (digitalRead(2) == HIGH)
        {
            count = 1;
        }
    }
    lspeed = dspeed;
    return lspeed;
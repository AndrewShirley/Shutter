#include "Timer.h"



int TimerInstance::n = 0;


TimerInstance::TimerInstance()
{
    Serial.printf("...D");
    //sprintf(this->TimerName,"%d", n++);
    
    snprintf(this->TimerName, MaxTimerNameSize, "Timer_%d", n++);
    Serial.printf("...E");
}



bool TimerInstance::IsExpired()
{
    return (this->FutureTimeExpirey && millis() >= FutureTimeExpirey);
}

void TimerInstance::ProcessExpired()
{
    if(this->ExpiryHandler != nullptr)
    {
        Serial.printf("Calling Handler...\n");
        ExpiryHandler(this);
    }
    if(this->Retrigger)
        this->FutureTimeExpirey += this->Interval;
    else
        this->FutureTimeExpirey = 0;

}

TimerInstance *Timer::AddTimer(char *TimerName, int Interval, bool Repeat, void (*ExpiryHandler)(TimerInstance *Handler))
{
    TimerInstance *T = new TimerInstance();
    strncpy((char *)T->TimerName, TimerName, MaxTimerNameSize);
    T->FutureTimeExpirey = millis() + Interval;
    T->Retrigger = Repeat;
    T->Interval = Interval;
    T->ExpiryHandler = ExpiryHandler;

    AddTimer(T);
    return T;
}



// Add a new timer
void Timer::AddTimer(TimerInstance *T)
{
    Items[T->TimerName] = T;
}



void Timer::ScanForExpired()
{
    for (auto const &I : this->Items)
        if (I.second->IsExpired())
            I.second->ProcessExpired();

    return;

}

void Timer::loop()
{
    ScanForExpired();
}



Timer::Timer()
{

}




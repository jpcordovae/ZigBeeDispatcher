#ifndef CEVENT_H
#define CEVENT_H

#include "common.h"

template<class T = int>
class CEvent
{

public:

    typedef boost::signals2::signal<void (void)> signalTrigger;

    boost::signals2::connection connect(signalTrigger::slot_type &_subscriber)
    {
        return signalTriggerCallBack.connect(_subscriber);
    }

    CEvent();

    CEvent(const T trigger_value, signalTrigger::slot_type &__subscriber){
       __m_trigger_value = trigger_value;
       signalTriggerCallBack.connect(__subscriber);
    }

    CEvent(const T trigger_value){
       __m_trigger_value = trigger_value;
    }

    CEvent(const T trigger_value, const T _value){
        __m_trigger_value = trigger_value;
        __m_value = _value;
    }

    void operator ++(){
        if(++__m_value >=__m_trigger_value) {
            __m_value = 0;
            signalTriggerCallBack();
        }
    }

    void operator ++(int){
        if(__m_value++ >=__m_trigger_value) {
            __m_value = 0;
            signalTriggerCallBack();
        }
    }

private:
    signalTrigger signalTriggerCallBack;
    T __m_trigger_value;
    T __m_value;
};

#endif // CEVENT_H

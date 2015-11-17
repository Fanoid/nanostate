#ifndef _QNANOSTATE_H_
#define _QNANOSTATE_H_

#include "libnanostate/nanostate.h"

#include <QThread>

class QNanostate : public QThread, public Nanostate
{
  Q_OBJECT

public:
    explicit QNanostate(const char *identity, const char *addr);
    ~QNanostate();
protected:
    void run();
signals:
    void state_updated();
private:
    int _interval;
};

#endif  // _QNANOSTATE_H_

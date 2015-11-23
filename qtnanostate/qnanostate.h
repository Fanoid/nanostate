#ifndef _QNANOSTATE_H_
#define _QNANOSTATE_H_

#include "cnanostate/nanostate.h"

#include <string>

#include <QMetaType>
#include <QThread>

Q_DECLARE_METATYPE(std::string)


class QNanostate : public QThread, public Nanostate
{
  Q_OBJECT

public:
    explicit QNanostate(const char *identity, const char *addr);
    ~QNanostate();
protected:
    void run();
signals:
    void state_updated(const std::string &, const std::string &, const std::string &);
private:
    int _interval;
};

#endif  // _QNANOSTATE_H_

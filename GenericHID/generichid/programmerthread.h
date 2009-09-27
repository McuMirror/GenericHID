#ifndef PROGRAMMERTHREAD_H
#define PROGRAMMERTHREAD_H

#include <QThread>
#include "programmer.h"

class ProgrammerThread : public QThread, public Programmer
{
    Q_OBJECT

public:
    ProgrammerThread(QObject *parent);
    ~ProgrammerThread();
    bool StartProgram( const QString &sEeprom, const QString &sFirmwarePath );
    bool success() const { return m_bSuccess; }

signals:
    void onUpdateStatus( int n );
    void onCompletionStatus( int nPercentage );

private:
    virtual void UpdateStatus( ProgramState::ProgramState status );
    virtual void CompletionStatus( int nPercentComplete );
    virtual void run();
    QString m_sEeprom;
    QString m_sFirmwarePath;
    bool m_bSuccess;
};

#endif // PROGRAMMERTHREAD_H

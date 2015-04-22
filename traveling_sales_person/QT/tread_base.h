//************************************************************
// Copyright 2015 Ian Maines
//
// Description:     Some necessities for threading.
// Date:            04/21/2015
//
//************************************************************

#ifndef TSP_TREAD_BASE_H
#define TSP_TREAD_BASE_H

#include <thread>
#include <mutex>

// RAII for mutex
class CMutexLocker
    {
    public:
        CMutexLocker (std::mutex& rMutex)
            : m_rMutex (rMutex)
            {
            m_rMutex.lock ();
            }

        ~CMutexLocker ()
            {
            m_rMutex.unlock ();
            }

    private:
        std::mutex& m_rMutex;
    };

// Base class for threads. Overload ThreadMain to do what you need it to.
class IThread
    {
    public:
        IThread ()
            // Don't construct the thread until we need it.
            : m_cThread ()
            , m_bStop (false)
        {}

        virtual ~IThread ()
            {
            Stop ();
            }

        // Overload for functionality.
        virtual void ThreadMain () =  0;

        void Start()
            {
            m_cThread = std::thread(&IThread::ThreadMain, this);
            }

        void Stop ()
            {
            m_bStop = true;
            // Wait for threadmain to finish its thing.
            // joinable () is a blocking call.
            if (m_cThread.joinable())
                {
                m_cThread.join();
                }
            }

    protected:
        std::thread m_cThread;
        // Implementation should respect bStop by terminating work ASAP and returning from ThreadMain.
        bool m_bStop;
    };
#endif // TSP_TREAD_BASE_H


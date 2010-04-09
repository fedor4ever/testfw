/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies). 
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
* 
* Description: This file contains the header file of the CTestEngine.
*
*/

#ifndef TESTENGINESERVER_H
#define TESTENGINESERVER_H

//  INCLUDES
#include <e32base.h>
#include "TestEngineCommon.h"

// FORWARD DECLARATIONS
class CTestEngineServerAutoShutdown;
class CStifLogger;

// CLASS DECLARATION

// DESCRIPTION
// CTestEngineServer is a server class.
// Contains an object container index which provides subsessions with object
// containers.
class CTestEngineServer
        : public CServer2
    {
    public:     // Enumerations
        enum { ETestEngineServerPriority = CActive::EPriorityHigh };

    private:    // Enumerations

    public:     // Constructors and destructor

        /**
        * NewL is two-phased constructor.
        */
        static CTestEngineServer* NewL();

        /**
        * Destructor of CTestEngineServer.
        */
        ~CTestEngineServer();

    public:     // New functions
        /**
        * Return object container, guaranteed to produce object containers with
        * unique ids within the server.
        */
        CObjectCon* NewContainerL();

        /**
        * Delete container.
        */
        void DeleteContainer( CObjectCon* aContainer );

        /**
        * Inform Server that session is closed.
        */
        void SessionClosed();

        /**
        * PanicServer panics the CTestEngineServer
        */
        static void PanicServer( TTestEnginePanic aPanic );

        void SetLoggerSettings( const TLoggerSettings& aLoggerSettings );
        
        const TLoggerSettings& GetLoggerSettings() const;
                
        /**
        * Return the pointer to iLogger
        */
        CStifLogger* Logger();

    public:     // Functions from base classes

        /**
        * NewSessionL creates new CTestEngineServer session.
        */
        CSession2* NewSessionL( const TVersion &aVersion,
                                const RMessage2& aMessage ) const;

    protected:  // New functions

    protected:  // Functions from base classes

    private:    // New functions

        /** 
        * C++ default constructor.
        */
        CTestEngineServer();

        /**
        * By default Symbian OS constructor is private.
        */
        void ConstructL();


        void InitLoggerSettings();
        
        void InitLoggerL();
    public:     // Data
        
    protected:  // Data

    private:    // Data
        // The server has an object container index which
        // creates an object container for each session
        CObjectConIx*           iContainerIndex; 

        // Container count
        TInt                    iSessionCount;
        
        // Auto shutdown timer used to shutdown TestEngine server after specified 
        // time when last TestEngine session have been closed
        CTestEngineServerAutoShutdown* iAutoShutdownTimer;

        // Logger instance
        CStifLogger*            iLogger;
        
        // Logger overwrite settings
        TLoggerSettings         iLoggerSettings;
        
    public:     // Friend classes

    protected:  // Friend classes

    private:    // Friend classes

    };


// DESCRIPTION
// CTestEngineServerAutoShutdown is timer class used to stop active scheduler after specified time
class CTestEngineServerAutoShutdown : public CTimer
    {
    public:
        /**
        * NewL is two-phased constructor.
        */
        static CTestEngineServerAutoShutdown* NewL();

        /**
         * Starts timer countdown.
         */
        void Start();
    private:
        /** 
        * C++ default constructor.
        */
        CTestEngineServerAutoShutdown();

        /**
        * By default Symbian OS constructor is private.
        */
        void ConstructL();
        
        
        /**
        * See CActive::RunL
        */
        void RunL();
    };        
        
#endif /* TESTENGINESERVER_H */
// End of File

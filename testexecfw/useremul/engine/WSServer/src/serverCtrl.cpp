/*------------------------------------------------------------------
 -
 * Software Name : UserEmulator
 * Version       : v4.2.1309
 * 
 * Copyright (c) 2009 France Telecom. All rights reserved.
 * This software is distributed under the License 
 * "Eclipse Public License - v 1.0" the text of which is available
 * at the URL "http://www.eclipse.org/legal/epl-v10.html".
 *
 * Initial Contributors:
 * France Telecom 
 *
 * Contributors:
 *------------------------------------------------------------------
 -
 * File Name: serverCtrl.cpp
 * 
 * Created: 13/08/2009
 * Author(s): Marcell Kiss
 *   
 * Description:
 * Implementation of Anim dll server side class
 *------------------------------------------------------------------
 -
 *
 */

// System Includes
#include <e32std.h>
#include <e32event.h>
#include <e32base.h>
#include <BAUTILS.H>
// User Includes
#include "serverCtrl.h"
#include "clientcommander.h"

// Local constants
_LIT8(KStartXml,"<!--?xml version='1.0' encoding='utf-16' ?>-->\n<UserEmulator>\n<action>\n<type>screenreset</type>\n</action>");
_LIT8(KEndXml, "\n</UserEmulator>");
_LIT8(KStr,"\n<action>\n<name>key</name>\n<type>keypress</type>\n<params>%d</params>\n<keys> %d </keys>\n</action>");
_LIT8(KUp,"\n<action>\n<name>key</name>\n<type>keypress</type>\n<params>%d</params>\n<keys>UAK</keys>\n</action>");
_LIT8(KDown,"\n<action>\n<name>key</name>\n<type>keypress</type>\n<params>%d</params>\n<keys>DAK</keys>\n</action>");
_LIT8(KLeft,"\n<action>\n<name>key</name>\n<type>keypress</type>\n<params>%d</params>\n<keys>LAK</keys>\n</action>");
_LIT8(KRight,"\n<action>\n<name>key</name>\n<type>keypress</type>\n<params>%d</params>\n<keys>RAK</keys>\n</action>");
_LIT8(KLSK,"\n<action>\n<name>key</name>\n<type>keypress</type>\n<params>%d</params>\n<keys>LSK</keys>\n</action>");
_LIT8(KRSK,"\n<action>\n<name>key</name>\n<type>keypress</type>\n<params>%d</params>\n<keys>RSK</keys>\n</action>");
_LIT8(KMSK,"\n<action>\n<name>key</name>\n<type>keypress</type>\n<params>%d</params>\n<keys>MSK</keys>\n</action>");
_LIT8(KBS,"\n<action>\n<name>key</name>\n<type>keypress</type>\n<params>%d</params>\n<keys>BS</keys>\n</action>");
_LIT8(KAsterisk,"\n<action>\n<name>key</name>\n<type>keypress</type>\n<params>%d</params>\n<keys>*</keys>\n</action>");
_LIT8(KYes,"\n<action>\n<name>key</name>\n<type>keypress</type>\n<params>%d</params>\n<keys>KYES</keys>\n</action>");
_LIT8(KNo,"\n<action>\n<name>key</name>\n<type>keypress</type>\n<params>%d</params>\n<keys>KNO</keys>\n</action>");
_LIT8(KHash,"\n<action>\n<name>key</name>\n<type>keypress</type>\n<params>%d</params>\n<keys>#</keys>\n</action>");
_LIT8(KTasks,"\n<action>\n<name>key</name>\n<type>keypress</type>\n<params>%d</params>\n<keys>MENU</keys>\n</action>");
_LIT8(KScreenShot1,"\n<action>\n<name> Pause </name>\n<type> wait </type>\n<params> 10 </params>\n<screenshot>");
_LIT8(KScreenShot2,"-%LD</screenshot>\n</action>");

_LIT8(KPortrait,"\n<action>\n<name>Orientation</name>\n<type>orientation</type>\n<params>portrait</params>\n</action>");
_LIT8(KLandscape,"\n<action>\n<name>Orientation</name>\n<type>orientation</type>\n<params>landscape</params>\n</action>");

_LIT8(KStrWait,"\n<action>\n<name> Pause </name>\n<type> wait </type>\n<params> %d </params>\n</action>");
_LIT8(KStrPointerDown,"\n<action>\n<name>Pointer</name>\n<type>pointerevent</type>\n<params>pointerdownAt,%d,%d</params>\n</action>");
_LIT8(KStrDrag,"\n<action>\n<name>Pointer</name>\n<type>pointerevent</type>\n<params>moveTo,%d,%d</params>\n</action>");
_LIT8(KStrPointerUp,"\n<action>\n<name>Pointer</name>\n<type>pointerevent</type>\n<params>pointerupAt,%d,%d</params>\n</action>");

_LIT8(KCamera,"Camera");

const TInt KAsteriskKeyScanCode		= 0x2A;
const TInt KWriteBufferSize			= 20;

// ======== MEMBER FUNCTIONS ========

CServerCtrl::CServerCtrl():iMainBuffer(NULL)
    {// No implementation required
    }

CServerCtrl::~CServerCtrl()
    {
    delete iCaptureKeyTimer;
    iCaptureKeyTimer = NULL;
        
    if(iMainBuffer)
        {
        // If buffer's length is not zero then write that data to file.
        if(iMainBuffer->Length()>0)
            iRFile.Write(*iMainBuffer);
        
        delete iMainBuffer;
        iMainBuffer = NULL;
        }
                            
    iRFile.Close();
    iFs.Close();
    }
// ----------------------------------------------------------
// Second phase constructor
// ----------------------------------------------------------
//
void CServerCtrl::ConstructL( TAny* /* aArgs */, TBool /* aHasFocus */ )
    {
    // Initialisation
    iRecord = EFalse;
    iFirstDelay=EFalse;
    iCameraButtonPressed = EFalse;
    iCaptureKeyTimer = CCaptureKeyTimer::NewL(*this);
    
    
    iIsEventEnabled = EFalse;
    }
// ------------------------------------------------------------
// Communication between client and server. Called by client
// ------------------------------------------------------------
//
TInt CServerCtrl::CommandReplyL( TInt aCode, TAny*  aArgs )
    {
    // Execute client command (aCode)
    switch ( aCode )
            {
        // Start recording
        case RClientCommander::EStart:
        	{
        	 TBuf8<KBuffer512> buf((const TUint8 *)aArgs);
             // Name and path of script file defined on Settings page
        	 iKeyFilePath.Copy(buf);
        	 
        	 // Cut long extension names
        	 TInt pos=iKeyFilePath.LocateReverse('.');
        	 if( pos!=KErrNotFound )
        		 {
        		 iKeyFilePath = iKeyFilePath.Left(pos+4);
        		 }
        		 
        	 // Check if device is in landscape or portrait mode
        	 TSize screen=iFunctions->ScreenDevice()->SizeInPixels();
        	 if(screen.iWidth <= screen.iHeight)
        		iOrientation = ETrue;
        	 else
        		iOrientation = EFalse;
        	   
        	 // Create script file
        	 User::LeaveIfError(iFs.Connect());
        	 TInt val=iRFile.Replace(iFs,iKeyFilePath,EFileShareAny|EFileWrite);
        	 if(val!=KErrNone)
        		 return 1;
        	 
        	 // Write xml header to file
        	 buf.Copy(KStartXml);
        	 
        	 
        	 // Write orientation 'action' to file
        	 if(iOrientation)
        		 buf.Append(KPortrait);
        	 else
        		 buf.Append(KLandscape);
        	 iRecord = ETrue;
        	
        	 iRFile.Write(buf);
        	 iIsEventEnabled = EFalse;
        	 
        	 // Enable raw events
        	 iFunctions->GetRawEvents(ETrue);
        	 
			 iWritePointer = 0;
			 // Initialise main buffer (length of it will be allocated dynamically)
			 iMainBuffer = HBufC8::NewL(1);
        	 
			 iNoCount = 0;
			 
        	}
            break;
            // Start wait timing
        case RClientCommander::EStartTiming:
            {
            // Store home time at start
            TTime time;
            time.HomeTime();
             
            iWaitTimer = time;
            }
            break;
        // Stop recording
        case RClientCommander::EStop:
        	{ 
        	// Disable raw events
            iFunctions->GetRawEvents(EFalse);
            if(iRecord)
            	{
            	TBuf8<KBuffer64> buf;
            	// Write last wait period to buffer
            	CheckTimerInsertWait();
            	
            	iIsEventEnabled = EFalse;
            	
            	// Flush buffer
            	if(iMainBuffer)
            		{
            	    if(iMainBuffer->Length()>0)
            	    	iRFile.Write(*iMainBuffer);
            	    	
            	    delete iMainBuffer;
            	    iMainBuffer = NULL;
            	    }
            	
            	// End tag of xml script
            	buf.Copy(KEndXml);
            	iRFile.Write(buf);            	
            	
            	iRecord = EFalse;
            		
            	iRFile.Flush();
            	iRFile.Close();
            	iFs.Close();
            	}
        	}
            break;
        // Poll the status of camera button (pressed or not)
        case RClientCommander::EPoll:
        	{
        	TBuf8<KBuffer256> buf((const TUint8 *)aArgs);
        	
        	if(buf.Length()>0)
        		{
        		const TUint8* a = buf.Left(0).Ptr();
        		const TUint8* b = buf.Mid(1,1).Ptr();
        		TInt length;
        		if(*b!=42) // 42 = '*'
        			length = (*a-48)*10+(*b-48);
        		else
        			length = *a-48;
        		
        		if(length>0 && 2+length<=buf.Length())
        			buf = buf.Mid(2,length);
        		}
        	// Check if camera application is still in foreground or closed already.
        	if(buf.Compare(KCamera)==0 && !iFirstDelay)
        		iFirstDelay = ETrue;
        	else
        		iFirstDelay = EFalse;
        	
        	if(!iFirstDelay)
        		{
				if(buf.Length()>2)
					iAppName.Copy(buf);
        		}
        				
        	if(iCameraButtonPressed)
        		return 1;
        		
             // Check orientation and write orientation 'action' to buffer if that changed
            TSize screen=iFunctions->ScreenDevice()->SizeInPixels();
		    if(screen.iWidth <= screen.iHeight)
			{
				if(!iOrientation)
					{// log portrait
					TBuf8<KBuffer128> buf;
					buf.Copy(KPortrait);    
					WriteFile(buf);
					}
				iOrientation = ETrue;
			}
			else
			{
				if(iOrientation)
					{// log landscape
					TBuf8<KBuffer128> buf;
					buf.Copy(KLandscape);   
					WriteFile(buf);
					}
				iOrientation = EFalse;
			}
		}
        	break;
        // Reset camera button's state flag
        case RClientCommander::ETaskExisted:
        	{
        	iCameraButtonPressed = EFalse;
        	}
        break;	
        default:
        	{}
            break;
            }
    
    return 0;
    }

TInt CServerCtrl::CommandReplyL( TInt /*aCode*/)
    {
    return 0;
    }

void CServerCtrl::Command( TInt /*aCode*/, TAny* /* aArgs */ )
    {
    }
void CServerCtrl::Redraw()
    {
    }
void CServerCtrl::Animate( TDateTime* /* aDT */ )
    {
    }
void CServerCtrl::FocusChanged( TBool /* aState */ )
    {
    }
    
// ----------------------------------------------------------------------------------
// OfferRawEvent is called by Window Server when an event occurs (key press, pointer event)
// ----------------------------------------------------------------------------------
//
TBool CServerCtrl::OfferRawEvent( const TRawEvent& aRawEvent )
   {
    if (iRecord)
    {
    // Pointer down
    if(aRawEvent.Type() == TRawEvent::EButton1Down)
    	{
    	// Convert coordinates if needed
        // Check elapsed time and write it to buffer 
    	CheckTimerInsertWait();
    	// Write button down event to buffer
    	TBuf8<KBuffer128> buf;
    	buf.Format(KStrPointerDown,aRawEvent.Pos().iX,aRawEvent.Pos().iY);	
    	WriteFile(buf);
    	}
    // Drag event occured
    if(aRawEvent.Type() == TRawEvent::EPointerMove)
    	{
    		CheckTimerInsertWait();
    		TBuf8<KBuffer128> buf;
    		buf.Format(KStrDrag,aRawEvent.Pos().iX,aRawEvent.Pos().iY);
    		WriteFile(buf);
    	}
    // Pointer released
    if(aRawEvent.Type() == TRawEvent::EButton1Up)
    	{   	
    	CheckTimerInsertWait();  	
    	TBuf8<KBuffer128> buf;
    	buf.Format(KStrPointerUp,aRawEvent.Pos().iX,aRawEvent.Pos().iY);	
    	WriteFile(buf);
    	}
    // Button pressed
    if(aRawEvent.Type() == TRawEvent::EKeyDown)
       	{
       	// camera button pressed, write screen shot command to buffer
        if(aRawEvent.ScanCode() == 227 || aRawEvent.ScanCode() == 171 )
            {
            TBuf8<KBuffer256> buf;
            TTime Now;
            Now.HomeTime();
            buf.Copy(KScreenShot1);
            buf.Append(iAppName);
            buf.AppendFormat(KScreenShot2, Now.Int64());
        
            WriteFile(buf);
            
            iCameraButtonPressed = ETrue;
            }
        else
            {
       	// Start to count time while button is pressed
       	iSecCounter = 0;
       	iCaptureKeyTimer->After(KWait08); // 0.8 sec
       	    }
       	}
       	
       	
    // Button released
    if(aRawEvent.Type() == TRawEvent::EKeyUp)
    	{
    	iCaptureKeyTimer->Cancel();
    	
    	TBuf8<KBuffer256> buf;
    	// Write pressed key to buffer
    	if(aRawEvent.ScanCode()>47 && aRawEvent.ScanCode()<=57)
    	    	buf.Format(KStr,iSecCounter,aRawEvent.ScanCode()-48);	
    	if(aRawEvent.ScanCode()==EStdKeyUpArrow)
    			buf.Format(KUp,iSecCounter);
    	if(aRawEvent.ScanCode()==EStdKeyDownArrow)
    			buf.Format(KDown,iSecCounter);
    	if(aRawEvent.ScanCode()==EStdKeyRightArrow)
				buf.Format(KRight,iSecCounter);
    	if(aRawEvent.ScanCode()==EStdKeyLeftArrow)
    			buf.Format(KLeft,iSecCounter);
    	if(aRawEvent.ScanCode()==EStdKeyDevice0)
    			buf.Format(KLSK,iSecCounter);
    	if(aRawEvent.ScanCode()==EStdKeyDevice1)
    			buf.Format(KRSK,iSecCounter);
    	if(aRawEvent.ScanCode()==EStdKeyDevice3)
    			buf.Format(KMSK,iSecCounter);
    	if(aRawEvent.ScanCode()==EStdKeyBackspace)
    			buf.Format(KBS,iSecCounter);
    	if(aRawEvent.ScanCode()==KAsteriskKeyScanCode)
    			buf.Format(KAsterisk,iSecCounter);
    	if(aRawEvent.ScanCode()==EStdKeyYes)
    			buf.Format(KYes,iSecCounter);
    	if(aRawEvent.ScanCode()==EStdKeyNo)
    		{// To filter first two 'No' keys (part of screen reset)
    		    if(iNoCount==0)
    		    	{
    		    	iNoCount++;
                    iIsEventEnabled = ETrue;
                    return EFalse;
    		    	}
    		  
    			buf.Format(KNo,iSecCounter);
    		}
    	if(aRawEvent.ScanCode()==EStdKeyHash)
    			buf.Format(KHash,iSecCounter);
    	if(aRawEvent.ScanCode()==EStdKeyApplication0) //MENU
    			buf.Format(KTasks,iSecCounter);
    	if(buf.Length()>0)
    		{
    		
    		CheckTimerInsertWait();
    		WriteFile(buf);
    		}
    	//
    	    				
    	}
    	
    	
    }
    return EFalse;
   }
// -------------------------------------------
// Count time while button is pressed
// -------------------------------------------
//
void CServerCtrl::KeyTimerExpired()
    {
    iSecCounter++;
    iCaptureKeyTimer->After(KWait08); // 0.8 sec
    }
// --------------------------------------------------------------
// Inserts time interval between two events into the script
// --------------------------------------------------------------
//   
void CServerCtrl::CheckTimerInsertWait()
    {
    TTime time;
    time.HomeTime();
    
    TTimeIntervalMicroSeconds interval = time.MicroSecondsFrom(iWaitTimer);
    
    // If time interval is bigger than 100 micosecs then write it to buffer
    if( interval > 100 )
        {
        TBuf8<KBuffer128> buf;
        buf.Format(KStrWait, interval.Int64() );
        WriteFile(buf);
        }
    // Home time will be the previous time at next event
    iWaitTimer = time;
    }

// --------------------------------------------------------------
// Implements a buffer. Writes to file only if buffer is full.
// --------------------------------------------------------------
//    
void CServerCtrl::WriteFile(TDesC8& aBuf)
	{
	// It doesn't record till screen reset (2 NO key presses) is not over
	if(!iIsEventEnabled)
		return;
	
	TInt r=iMainBuffer->Length();
	TInt t=aBuf.Length();
	iMainBuffer = iMainBuffer->ReAlloc(r+t+1);
	TPtr8 bufferPtr( iMainBuffer->Des() );
	bufferPtr.Append(aBuf);

	if(iWritePointer++ > KWriteBufferSize)
		{
		// Buffer is full. Write to file and reset buffer
		iWritePointer = 0;
		iRFile.Write(*iMainBuffer);
		delete iMainBuffer;
		iMainBuffer = NULL;
		TRAPD(err, iMainBuffer = HBufC8::NewL(1));
		}
	}
// End of File

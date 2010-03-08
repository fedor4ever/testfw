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
 * File Name: ImageCapture.cpp
 * 
 * Created: 13/08/2009
 * Author(s): Marcell Kiss, Reshma Sandeep Das
 *   
 * Description:
 * Active object implementation for Image capture
 *------------------------------------------------------------------
 -
 *
 */

//System Includes
#include <icl\ImageData.h>
#include <icl\ImageCodecData.h>
#include <coemain.h>
#include <PathInfo.h>
#include <bautils.h>
#include <apgwgnam.h>// CApaWindowGroupName

//User Include
#include "ImageCapture.h"

// -----------------------------------------------------------------------------
// CImageCapture::NewL
// Creates the instance of class and returns it.
// -----------------------------------------------------------------------------
//
CImageCapture* CImageCapture::NewL(CSettings& aSettings ,MScreenshotObserver& aObserver, CEikonEnv* aEikonEnv)
{
	CImageCapture* self=CImageCapture::NewLC(aSettings, aObserver, aEikonEnv);
	CleanupStack::Pop(); // self;
	return self;
}

// -----------------------------------------------------------------------------
// CImageCapture::NewLC
// Creates the instance of class and pushes it to the CleanupStack and return
// it.
// -----------------------------------------------------------------------------
//
CImageCapture* CImageCapture::NewLC(CSettings& aSettings,MScreenshotObserver& aObserver, CEikonEnv* aEikonEnv)
{
	CImageCapture* self = new (ELeave)CImageCapture(aSettings, aObserver, aEikonEnv);
	CleanupStack::PushL(self);
	self->ConstructL();
	return self;
}
// -----------------------------------------------------------------------------
// CImageCapture::CImageCapture
// -----------------------------------------------------------------------------
//
CImageCapture::CImageCapture(CSettings& aSettings,MScreenshotObserver& aObserver, CEikonEnv* aEikonEnv)
    : CActive(CActive::EPriorityStandard),
      iSettings(aSettings),iBitmap(NULL),
      iImageEncoder(NULL),
      iObserver(aObserver),
      iEEnv(aEikonEnv)
{
}
// -----------------------------------------------------------------------------
// CImageCapture::ConstructL
// -----------------------------------------------------------------------------
//
void CImageCapture::ConstructL()
{
	CActiveScheduler::Add(this);
	User::LeaveIfError(iTimer.CreateLocal());
}
// -----------------------------------------------------------------------------
// CImageCapture::~CImageCapture
// -----------------------------------------------------------------------------
//
CImageCapture::~CImageCapture()
{
    Cancel();
     
    if (iImageEncoder)
    {
       delete iImageEncoder;
       iImageEncoder = 0;
    }
    
    if(iBitmap)
    	delete iBitmap;
   
    iTimer.Close();
    iScreenShotStatus = EIdle;
}
// ------------------------------------------------------------------------------
// CImageCapture::RunL
// Issues request to save Images and notifies the observer after saving the image
// ------------------------------------------------------------------------------
//
void CImageCapture::RunL()
{
	switch (iScreenShotStatus)
	{
		case EIdle:
		break;
			
		case EWaiting:
			{
			RWsSession& wsSession = iEEnv->WsSession();
			TInt id = wsSession.GetFocusWindowGroup();
			CApaWindowGroupName* lApaWGName;
			lApaWGName = CApaWindowGroupName::NewLC(wsSession,id);
			
			TUid uid = lApaWGName->AppUid();
			
			if(uid.operator ==(iAppUid) )
   			   Fire(ECapturing);
			else
				FireCapture(1000000);
			
			CleanupStack::PopAndDestroy(); // lApaWGName
			
			}
			break;
			
		case ECapturing:
			DoCaptureL();
			break;
			
	
		case ESaveComplete:
		default:
			iObserver.PerformNextAction(1);
			delete this;		
			
	}          
}
// -----------------------------------------------------------------------------
// CImageCapture::CaptureL
// -----------------------------------------------------------------------------
//
void CImageCapture::CaptureL(const TDesC& aName,const TDesC& aXmlFileName, TUid aAppUid)
{
	iFileName.Copy(aName);
	iXmlFileName.Copy(aXmlFileName);
	iAppUid = aAppUid;
	
	if(AknLayoutUtils::PenEnabled() && !(aAppUid.iUid ) )
	{
		User::After(KWait1);
	}
	
	if(iAppUid.iUid)
		Fire(EWaiting);
	else
		DoCaptureL();
	
}

// --------------------------------------------------------------------------
// Captures the screenshot and store it to a buffer.
// --------------------------------------------------------------------------
void CImageCapture::DoCaptureL()
{
    const CWsScreenDevice* screenDevice = CCoeEnv::Static()->ScreenDevice();
	
	TPixelsTwipsAndRotation sizeAndRotation;
	screenDevice->GetScreenModeSizeAndRotation(
			screenDevice->CurrentScreenMode(), sizeAndRotation);

	if(iBitmap)
		delete iBitmap;
	iBitmap = NULL;
	iBitmap = new (ELeave) CFbsBitmap;
	iBitmap->SetSizeInTwips( screenDevice);
	User::LeaveIfError( iBitmap->Create( screenDevice->SizeInPixels(),screenDevice->DisplayMode() ) );	
	User::LeaveIfError( screenDevice->CopyScreenToBitmap(iBitmap));

	//save it in a file
	DoSaveL(iFileName);	
	iScreenShotStatus = ESaveComplete;
	SetActive();
}

// --------------------------------------------------------------------------
// CImageCapture::Fire
// Fires an event. It means set this active object to active, but completes
// the request immediately.
// --------------------------------------------------------------------------
void CImageCapture::Fire(TScreenShotStatus aStatus)
{
    iScreenShotStatus = aStatus;
    SetActive();
    TRequestStatus* status = &iStatus;
    User::RequestComplete(status, KErrNone);
}

// --------------------------------------------------------------------------
// CImageCapture::FireCapture
// Fires an event after aTime microseconds.
// --------------------------------------------------------------------------
void CImageCapture::FireCapture(TTimeIntervalMicroSeconds32 aTime)
{
    iScreenShotStatus = EWaiting;
    iTimer.After(iStatus, aTime);
    if(!IsActive())
    	SetActive();
}

// --------------------------------------------------------------------------
// CImageCapture::DoSaveL
// Saves captured screenshot to a file.
// --------------------------------------------------------------------------
void CImageCapture::DoSaveL(const TDesC& aName)
{
    // Fetch the next available file name.

    HBufC* fileName = GetNextFileNameLC(aName);
    CCoeEnv::Static()->FsSession().MkDirAll(*fileName);

    // Set the MIME type
    _LIT8(KMimeTypepng,"image/png");
    TPtrC8 mimeType(KMimeTypepng);

    // Prepares the image data (if applied).
    CFrameImageData* frameImageData = CFrameImageData::NewL();
    CleanupStack::PushL(frameImageData);
    
    TPngEncodeData* encodeData = new (ELeave) TPngEncodeData();
    encodeData->iPaletted     = EFalse;
    encodeData->iColor        = ETrue;
    encodeData->iBitsPerPixel = 24;
    encodeData->iLevel = TPngEncodeData::EBestCompression;
    User::LeaveIfError(frameImageData->AppendFrameData(encodeData));
    
   // Creates a new encoder and then convert the bitmap.
    TRAPD(err, iImageEncoder = CImageEncoder::FileNewL(
    		CCoeEnv::Static()->FsSession(), *fileName, mimeType));
    if(err == KErrNone)
    	iImageEncoder->Convert(&iStatus, *iBitmap, frameImageData);
    
    CleanupStack::PopAndDestroy(frameImageData); // frameImageData
    CleanupStack::PopAndDestroy(fileName);           // fileName
    
}
// --------------------------------------------------------------------------
// CImageCapture::GetNextFileNameLC
// Get the file name
// --------------------------------------------------------------------------
HBufC* CImageCapture::GetNextFileNameLC(const TDesC& aName) const
{
    const TInt KNumberLength = 4; // this is to indicate the file numbering,
                                  // e.g. 0001, 0002, etc.
    const TInt KMaxIndex = 10000;
    const TInt KTimeRecordSize = 512;
    
    TPtrC filePathPtr;
    _LIT(KExtensionpng, ".png");
    _LIT(KSlash,"\\");
    
    // Gets the file extension.
    TPtrC fileExtension;
    fileExtension.Set(KExtensionpng);
   
    filePathPtr.Set(iSettings.iLogPath);
    TInt result = filePathPtr.LocateReverse('\\');
    TPtrC string;
    if(result!=KErrNotFound)
    	string.Set(filePathPtr.Left(result+1));
    
    TBuf8<KTimeRecordSize> fileName;
    fileName.Copy(string);
    
    if(iXmlFileName.Length()>0)
    {
    	TInt pos=iXmlFileName.LocateReverse('.');
    	TPtrC8 ptr;
    	if(pos!=KErrNotFound)
    	{
    		ptr.Set(iXmlFileName.Left(pos));
    		fileName.Append(ptr);
    	}
    	
		fileName.Append(KSlash);
    }
    
    fileName.Append(aName);
    HBufC* newFileName = HBufC::NewLC(fileName.Length()+ KNumberLength + fileExtension.Length() + 1);
    TPtr newFileNamePtr(newFileName->Des());
    newFileNamePtr.Copy(fileName);

    
    // Checks whether aNamexxxx.png already exists on the phone or not.
    // This is to prevent over-riding of any existing images with the same name
    TBool IsFileExist = ETrue;
    TInt index = 1;
    HBufC* buffer = HBufC::NewL(newFileNamePtr.MaxLength());
    TPtr bufferPtr(buffer->Des());
    while ((index < KMaxIndex) && (IsFileExist))
    {
        bufferPtr.Copy(newFileNamePtr);
        bufferPtr.AppendNumFixedWidth(index, EDecimal, KNumberLength);
        bufferPtr.Append(fileExtension);
        if (BaflUtils::FileExists(CCoeEnv::Static()->FsSession(), *buffer))
        {
        	index++;
        }
        else
        {
        	IsFileExist = EFalse;
        }
    }
    delete buffer;

    // If the index exceeds KMaxIndex, then we don't need to format the file name.
    if (index >= KMaxIndex)
    {
    	newFileNamePtr.AppendNum(index);
    }
    else
    {
    	newFileNamePtr.AppendNumFixedWidth(index, EDecimal, KNumberLength);
    }
    newFileNamePtr.Append(fileExtension);

    // If the index greated then KMaxIndex, then rollback to 1
    
    if (index >= KMaxIndex)
    {
    	index = 1;
    }
    
    return newFileName;
}

void CImageCapture::DoCancel()
{
   if (iImageEncoder)
   {
	   iImageEncoder->Cancel();
       delete iImageEncoder;
       iImageEncoder = 0;
   }
   iTimer.Cancel();
}

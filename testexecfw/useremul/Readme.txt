========================================
=             SYMBIAN SIGNED           =
========================================
Document version:	1.00(002), 25th of Nov 2009

Contents
SECTION 1 - QUICK CHECKS
SECTION 2 - RELEASE NOTES
SECTION 3 - USER GUIDE / DOCUMENTATION

SECTION 1 - QUICK CHECKS
========================

This file contains the following files
	- This completed document
	- UserEmulator_v4.2_2509.sis file
	- UserEmulator.pkg
	- ReleaseNotes_v4.2_2509.txt
  - OrangeUserEmulator.doc
	
Symbian signed submission criteria checklist:
CHECK-01: Provided ReleaseNotes_v4.2_2509.txt
CHECK-02: There are no embedded SIS files.
CHECK-03: Yes. The application can be tested via its own UI.
CHECK-04: The submitted SIS file is signed with the Publisher ID of France Telecom R&D UK LTD.
CHECK-05: The SIS file contains correct version information that matches with the application's About box.
CHECK-06: Capabilities: 
		PowerMgmt     : Class TApaTaskList    -> EndTask 		
		SwEvent       : Class RWsSession      -> SimulateRawEvent
		LocalServices : Class CC32Server      -> CustomSecurityCheckL 	
		ReadDeviceData: Publish and Subscribe Property
		ProtServ
		WriteDeviceData: CWsScreenDevice      -> SetScreenMode
CHECK-07: No Backup required
CHECK-08: Target platform UID: 
      Series 60 v 3.1 : 0x102032BE
		  Series 60 v 3.2 : 0x102752AE
		  Series 60 v 5.0 : 0x1028315F
CHECK-09: Package UID: 0x2001C3AF (assuming application UID is referred to here)
CHECK-10: Not Applicable
CHECK-11: Not Applicable
CHECK-12: Not Applicable
CHECK-13: Application UID: 0x2001C3AF
CHECK-14: UserEmulator_0x2001C3AF.exe
CHECK-15: Not Applicable
CHECK-16: Not accessing SIM card.
CHECK-17: Not Applicable

SECTION 2 - RELEASE NOTES
=========================
Application version:	4.2_2509

Phone(s) concerned:	Nokia 6220, Nokia 5800 XpressMusic
Release note:		This version is designed to work on Nokia 6220 and Nokia 5800 phones 
                (And not just restricted to these phones). User Emulator is a tool for automated test creation, 
                execution and management. It is capable of handling any key events (up/down/left/right arrow, 
                selection key, LSK, RSK, Menu key, alpha/numeric keys, etc) and pointer events. This tool helps in 
                writing, reusing and executing tests on the code under development. 
		
Phone(s) concerned:	All
Release note:		This version is designed to work on series60 3.2 and 5.0 phones. 
                User Emulator is a tool for automated test creation, 
                execution and management. It is capable of handling any key events (up/down/left/right arrow, 
                selection key, LSK, RSK, Menu key, alpha/numeric keys, etc) and pointer events. This tool helps in 
                writing, reusing and executing tests on the code under development. 

Please find additional release notes included in this zip file.

SECTION 3 - USER GUIDE
======================
User Emulator is a tool for automated test creation, execution and management. It is capable of handling any 
key events (up/down/left/right arrow, selection key, LSK, RSK, Menu key, alpha/numeric keys, etc) and pointer events.
This tool helps in writing, reusing and executing tests on the code under development.

Menu Options 
------------
After the application launch, any scripts available in the default settings path are displayed to the user. 
This view is refreshed if there is any change in the settings path or addition/deletion of scripts.

1. Run Scripts
Script execution can start by selecting the Run Scripts menu option. Script selection can be made by clicking on a script name. 
Single selection or multiple script selection option is available to the user. After the selection, file(s) are parsed using an XML 
parser and appropriate actions are performed. Visual indications of scripts execution pass/fail/cancellation are displayed using icons on the screen.

Green Icon: pass
Red Icon: Fail
Yellow Icon: Cancellation of script execution

Script run starts on idle screen after the Menu application and idle screen reset.

2. Select
This option can be used for file selection and de-selection respectively. There is an option to select/de-select a single 
file or multiple files at the same time.

3. Record input
This menu option allows users to start recording any key presses or screen taps. User has to enter a name for the recorded script. 
The file name cannot contain any special characters. An error message is displayed to the user and a dialog is prompted to enter 
correct filename. The file is stored in 'Scripts Path ' folder. Screenshot tag can be added dynamically in the script by pushing 
the camera button once during script recording. Recording can be cancelled any time by pressing the power button once.
Script recording starts on idle screen after the Menu application and idle screen reset.

4. Random test run
Starts the Random test on the device and sends random key presses and screen taps to the device. Random test can be stopped by pressing 
the power button once.

5. Settings
This menu option launches a settings dialog that has configuration information for the User Emulator tool.

•	Scripts Path: User can modify the storage path of the test scripts.
•	Log Status: Logging option can be turned ON/OFF using this settings option.      
•	Log File Path: Path for the log file that logs the script execution actions. This file also captures any system wide panics.  
Note: User has to exit the tool in order to check the log files. Log files are overwritten every time the tool starts.
•	Target for Random Test? Can be ON/OFF. 'ON' enables random tests on ID specified application(s). If it is in 'OFF' state then random test 
will perform without starting an application at start time.
•	App IDs of Random Test: If the previous option is 'ON' then 'Random test run' will start application(s) at the beginning of the test using these IDs.

Note: A sample log file generated after a simple script execution is attached at the end of this document.

6. System Info
Option that gives the complete system information like OS-Version, UI-Version, Machine ID,IMEI No, Family, CPU, CPUSpeed, 
Start-up reason, RAM, Free RAM and ROM available on the device.

7. Help
Help file describing the User Emulator tool usage

8. About
A brief about the tool


Middle soft Key Options:
------------------------
1.	Run Scripts
User can call Run Scripts to run a script on focus. 

Please find attached the OrangeUserEmulator.doc for more details.


Installation
============
Install the UserEmulator_v4.2_2509.sis file package on your phone through your favourite installation method 
(e.g. the Nokia Phone Suite or through a Bluetooth file transfer).

IMPORTANT NOTES:
1. There is no backup required of this application

2. UNI-08: Uninstall: The log files and screenshots generated from the User Emulator are not removed after the application uninstallation. 
   This tool is used as a stress test tool and the information gathered is used for further analysis. 

3. If the log path is the Phone memory, and the path set is say any directory other than 
   "C:\\Data\\", log files are not visible using the Nokia native File Browser application. 
    This issue is not considered a fatal error because of it is a system limitation.

4. In order to get the same results during script execution, all the screens on the device should be reset to 
   its initial state before script recording and playback. 
    o	Currently the tool supports basic reset functionality of resetting the Menu application's scroll bar, 
      closing any open applications on the device and resetting the idle screen. 
    o	If there are any changes to the application position in the Menu application or any changes on the 
      HomeScreen, the script when played back may not produce the same results. This is a known limitation of the tool. 

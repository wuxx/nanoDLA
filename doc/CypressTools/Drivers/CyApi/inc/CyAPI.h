//______________________________________________________________________________
//
// Copyright (c) Cypress Semiconductor, 2003
// All rights reserved.
//
//______________________________________________________________________________

#ifndef CyUSBH
#define CyUSBH

#ifndef   __USB200_H__
#define   __USB200_H__
#pragma pack(push,1)
typedef struct _USB_DEVICE_DESCRIPTOR {
    UCHAR bLength;
    UCHAR bDescriptorType;
    USHORT bcdUSB;
    UCHAR bDeviceClass;
    UCHAR bDeviceSubClass;
    UCHAR bDeviceProtocol;
    UCHAR bMaxPacketSize0;
    USHORT idVendor;
    USHORT idProduct;
    USHORT bcdDevice;
    UCHAR iManufacturer;
    UCHAR iProduct;
    UCHAR iSerialNumber;
    UCHAR bNumConfigurations;
} USB_DEVICE_DESCRIPTOR, *PUSB_DEVICE_DESCRIPTOR;

typedef struct _USB_ENDPOINT_DESCRIPTOR {
    UCHAR bLength;
    UCHAR bDescriptorType;
    UCHAR bEndpointAddress;
    UCHAR bmAttributes;
    USHORT wMaxPacketSize;
    UCHAR bInterval;
} USB_ENDPOINT_DESCRIPTOR, *PUSB_ENDPOINT_DESCRIPTOR;

typedef struct _USB_CONFIGURATION_DESCRIPTOR {
    UCHAR bLength;
    UCHAR bDescriptorType;
    USHORT wTotalLength;
    UCHAR bNumInterfaces;
    UCHAR bConfigurationValue;
    UCHAR iConfiguration;
    UCHAR bmAttributes;
    UCHAR MaxPower;
} USB_CONFIGURATION_DESCRIPTOR, *PUSB_CONFIGURATION_DESCRIPTOR;

typedef struct _USB_INTERFACE_DESCRIPTOR {
    UCHAR bLength;
    UCHAR bDescriptorType;
    UCHAR bInterfaceNumber;
    UCHAR bAlternateSetting;
    UCHAR bNumEndpoints;
    UCHAR bInterfaceClass;
    UCHAR bInterfaceSubClass;
    UCHAR bInterfaceProtocol;
    UCHAR iInterface;
} USB_INTERFACE_DESCRIPTOR, *PUSB_INTERFACE_DESCRIPTOR;

typedef struct _USB_STRING_DESCRIPTOR {
    UCHAR bLength;
    UCHAR bDescriptorType;
    WCHAR bString[1];
} USB_STRING_DESCRIPTOR, *PUSB_STRING_DESCRIPTOR;

typedef struct _USB_COMMON_DESCRIPTOR {
    UCHAR bLength;
    UCHAR bDescriptorType;
} USB_COMMON_DESCRIPTOR, *PUSB_COMMON_DESCRIPTOR;
#pragma pack(pop)
#endif
//______________________________________________________________________________

class CCyIsoPktInfo {
public:
	LONG Status;
	LONG Length;
};

//______________________________________________________________________________


// {AE18AA60-7F6A-11d4-97DD-00010229B959}
static GUID CYUSBDRV_GUID = {0xae18aa60, 0x7f6a, 0x11d4, 0x97, 0xdd, 0x0, 0x1, 0x2, 0x29, 0xb9, 0x59};

typedef enum {TGT_DEVICE, TGT_INTFC, TGT_ENDPT, TGT_OTHER } CTL_XFER_TGT_TYPE;
typedef enum {REQ_STD, REQ_CLASS, REQ_VENDOR } CTL_XFER_REQ_TYPE;
typedef enum {DIR_TO_DEVICE, DIR_FROM_DEVICE } CTL_XFER_DIR_TYPE;
typedef enum {XMODE_BUFFERED, XMODE_DIRECT } XFER_MODE_TYPE;

const MAX_ENDPTS = 16;
const MAX_INTERFACES = 8;
const USB_STRING_MAXLEN = 256;


////////////////////////////////////////////////////////////////////////////////
//
// The CCyEndPoint ABSTRACT Class
//
////////////////////////////////////////////////////////////////////////////////
class CCyUSBEndPoint
{
protected:
  bool WaitForIO(OVERLAPPED *ovLapStatus);

  virtual PUCHAR BeginDirectXfer(PUCHAR buf, LONG bufLen, OVERLAPPED *ov);
  virtual PUCHAR BeginBufferedXfer(PUCHAR buf, LONG bufLen, OVERLAPPED *ov);


public:

  CCyUSBEndPoint(void);
  CCyUSBEndPoint(CCyUSBEndPoint& ept);
  CCyUSBEndPoint(HANDLE h, PUSB_ENDPOINT_DESCRIPTOR pEndPtDescriptor);

  HANDLE        hDevice;

  // The fields of an EndPoint Descriptor
  UCHAR  DscLen;
  UCHAR  DscType;
  UCHAR  Address;
  UCHAR  Attributes;
  USHORT MaxPktSize;
  USHORT PktsPerFrame;
  UCHAR  Interval;

  // Other fields
  ULONG  TimeOut;
  ULONG  UsbdStatus;
  ULONG  NtStatus;

  DWORD  bytesWritten;
  DWORD  LastError;
  bool   bIn;

  XFER_MODE_TYPE   XferMode;

  bool    XferData(PUCHAR buf, LONG &len, CCyIsoPktInfo* pktInfos = NULL);
  virtual PUCHAR BeginDataXfer(PUCHAR buf, LONG len, OVERLAPPED *ov) = 0;
  virtual bool FinishDataXfer(PUCHAR buf, LONG &len, OVERLAPPED *ov, PUCHAR pXmitBuf, CCyIsoPktInfo* pktInfos = NULL);
  bool    WaitForXfer(OVERLAPPED *ov, ULONG tOut);
  ULONG   GetXferSize(void);
  void    SetXferSize(ULONG xfer);

  bool Reset(void);
  bool Abort(void);

private:



};


////////////////////////////////////////////////////////////////////////////////
//
// The Control Endpoint Class
//
////////////////////////////////////////////////////////////////////////////////
class CCyControlEndPoint : public CCyUSBEndPoint
{
private:

public:
  CCyControlEndPoint(void);
  CCyControlEndPoint(CCyControlEndPoint& ept);
  CCyControlEndPoint(HANDLE h, PUSB_ENDPOINT_DESCRIPTOR pEndPtDescriptor);

  CTL_XFER_TGT_TYPE Target;
  CTL_XFER_REQ_TYPE ReqType;
  CTL_XFER_DIR_TYPE Direction;

  UCHAR             ReqCode;
  WORD              Value;
  WORD              Index;

  bool Read(PUCHAR buf, LONG &len);
  bool Write(PUCHAR buf, LONG &len);
  PUCHAR BeginDataXfer(PUCHAR buf, LONG len, OVERLAPPED *ov);
};



////////////////////////////////////////////////////////////////////////////////
//
// The Isoc Endpoint Class
//
////////////////////////////////////////////////////////////////////////////////
class CCyIsocEndPoint : public CCyUSBEndPoint
{

protected:
  virtual PUCHAR BeginDirectXfer(PUCHAR buf, LONG bufLen, OVERLAPPED *ov);
  virtual PUCHAR BeginBufferedXfer(PUCHAR buf, LONG bufLen, OVERLAPPED *ov);

public:
  CCyIsocEndPoint(void);
  CCyIsocEndPoint(HANDLE h, PUSB_ENDPOINT_DESCRIPTOR pEndPtDescriptor);

  PUCHAR BeginDataXfer(PUCHAR buf, LONG len, OVERLAPPED *ov);
  CCyIsoPktInfo* CreatePktInfos(LONG bufLen, int &packets);

};



////////////////////////////////////////////////////////////////////////////////
//
// The Bulk Endpoint Class
//
////////////////////////////////////////////////////////////////////////////////
class CCyBulkEndPoint : public CCyUSBEndPoint
{
public:
  CCyBulkEndPoint(void);
  CCyBulkEndPoint(HANDLE h, PUSB_ENDPOINT_DESCRIPTOR pEndPtDescriptor);

  PUCHAR BeginDataXfer(PUCHAR buf, LONG len, OVERLAPPED *ov);

};



////////////////////////////////////////////////////////////////////////////////
//
// The Interrupt Endpoint Class
//
////////////////////////////////////////////////////////////////////////////////
class CCyInterruptEndPoint : public CCyUSBEndPoint
{
public:
  CCyInterruptEndPoint(void);
  CCyInterruptEndPoint(HANDLE h, PUSB_ENDPOINT_DESCRIPTOR pEndPtDescriptor);

  PUCHAR BeginDataXfer(PUCHAR buf, LONG len, OVERLAPPED *ov);
};



////////////////////////////////////////////////////////////////////////////////
//
// The Interface Class
//
////////////////////////////////////////////////////////////////////////////////

class CCyUSBInterface
{
private:
protected:
public:
  CCyUSBEndPoint *EndPoints[MAX_ENDPTS];  // Holds pointers to all the interface's endpoints, plus a pointer to the Control endpoint zero

  UCHAR bLength;
  UCHAR bDescriptorType;
  UCHAR bInterfaceNumber;
  UCHAR bAlternateSetting;
  UCHAR bNumEndpoints;           // Not counting the control endpoint
  UCHAR bInterfaceClass;
  UCHAR bInterfaceSubClass;
  UCHAR bInterfaceProtocol;
  UCHAR iInterface;

  UCHAR bAltSettings;
  USHORT wTotalLength;          // Needed in case Intfc has additional (non-endpt) descriptors


  CCyUSBInterface(HANDLE h, PUSB_INTERFACE_DESCRIPTOR pIntfcDescriptor);
  CCyUSBInterface(CCyUSBInterface& ifc);  // Copy Constructor
  ~CCyUSBInterface(void);

};

////////////////////////////////////////////////////////////////////////////////
//
// The Config Class
//
////////////////////////////////////////////////////////////////////////////////

class CCyUSBConfig
{
private:

protected:
public:
  CCyUSBInterface *Interfaces[MAX_INTERFACES];

  UCHAR bLength;
  UCHAR bDescriptorType;
  USHORT wTotalLength;
  UCHAR bNumInterfaces;
  UCHAR bConfigurationValue;
  UCHAR iConfiguration;
  UCHAR bmAttributes;
  UCHAR MaxPower;

  UCHAR AltInterfaces;


  CCyUSBConfig(void);
  CCyUSBConfig(CCyUSBConfig& cfg);  // Copy Constructor
  CCyUSBConfig(HANDLE h, PUSB_CONFIGURATION_DESCRIPTOR pConfigDescr);
  ~CCyUSBConfig(void);



};

////////////////////////////////////////////////////////////////////////////////
//
// The USB Device Class - This is the main class that contains members of all the
// other classes.
//
// To use the library, create an instance of this Class and call it's Open method
//
////////////////////////////////////////////////////////////////////////////////

class CCyUSBDevice
{
// The public members are accessible (i.e. corruptible) by the user of the library
// Algorithms of the class don't rely on any public members.  Instead, they use the
// private members of the class for their calculations.

public:

  CCyUSBDevice(HANDLE hnd = NULL, GUID guid = CYUSBDRV_GUID, BOOL bOpen = true);
  ~CCyUSBDevice(void);

  CCyUSBEndPoint      **EndPoints;     // Shortcut to USBCfgs[CfgNum]->Interfaces[IntfcIndex]->Endpoints
  CCyUSBEndPoint       *EndPointOf(UCHAR addr);

  CCyControlEndPoint   *ControlEndPt;
  CCyIsocEndPoint      *IsocInEndPt;
  CCyIsocEndPoint      *IsocOutEndPt;
  CCyBulkEndPoint      *BulkInEndPt;
  CCyBulkEndPoint      *BulkOutEndPt;
  CCyInterruptEndPoint *InterruptInEndPt;
  CCyInterruptEndPoint *InterruptOutEndPt;

  USHORT                StrLangID;
  ULONG                 UsbdStatus;
  ULONG                 NtStatus;
  ULONG                 DriverVersion;
  ULONG                 USBDIVersion;
  char                  DeviceName[USB_STRING_MAXLEN];
  char                  FriendlyName[USB_STRING_MAXLEN];
  wchar_t               Manufacturer[USB_STRING_MAXLEN];
  wchar_t               Product[USB_STRING_MAXLEN];
  wchar_t               SerialNumber[USB_STRING_MAXLEN];

  CHAR                  DevPath[USB_STRING_MAXLEN];

  USHORT                BcdUSB;
  USHORT                VendorID;
  USHORT                ProductID;
  UCHAR                 USBAddress;
  UCHAR                 DevClass;
  UCHAR                 DevSubClass;
  UCHAR                 DevProtocol;
  UCHAR                 MaxPacketSize;
  USHORT                BcdDevice;

  UCHAR                 ConfigValue;
  UCHAR                 ConfigAttrib;
  UCHAR                 MaxPower;

  UCHAR                 IntfcClass;
  UCHAR                 IntfcSubClass;
  UCHAR                 IntfcProtocol;
  bool                  bHighSpeed;

  DWORD                 BytesXfered;


  UCHAR                 DeviceCount(void);
  UCHAR                 ConfigCount(void);
  UCHAR                 IntfcCount(void);
  UCHAR                 AltIntfcCount(void);
  UCHAR                 EndPointCount(void);

  UCHAR                 Config(void)     { return CfgNum; }    // Normally 0
  void                  SetConfig(UCHAR cfg);

  UCHAR                 Interface(void)  { return IntfcNum; }  // Usually 0
                        // No SetInterface method since only 1 intfc per device (per Windows)

  UCHAR                 AltIntfc(void);
  bool                  SetAltIntfc(UCHAR alt);

  GUID                  DriverGUID(void) { return DrvGuid; }
  HANDLE                DeviceHandle(void) { return hDevice; }
  void                  UsbdStatusString(ULONG stat, PCHAR s);
  bool                  CreateHandle(UCHAR dev);
  void                  DestroyHandle();


  bool                  Open(UCHAR dev);
  void                  Close(void);
  bool                  Reset(void);
  bool                  ReConnect(void);
  bool                  Suspend(void);
  bool                  Resume(void);
  bool                  IsOpen(void)      { return (hDevice != INVALID_HANDLE_VALUE); }

  UCHAR                 PowerState(void);


  void                  GetDeviceDescriptor(PUSB_DEVICE_DESCRIPTOR descr);
  void                  GetConfigDescriptor(PUSB_CONFIGURATION_DESCRIPTOR descr);
  void                  GetIntfcDescriptor(PUSB_INTERFACE_DESCRIPTOR descr);
  CCyUSBConfig          GetUSBConfig(int index);


private:

  USB_DEVICE_DESCRIPTOR         USBDeviceDescriptor;
  PUSB_CONFIGURATION_DESCRIPTOR USBConfigDescriptors[2];

  CCyUSBConfig                 *USBCfgs[2];

  HANDLE                        hWnd;
  HANDLE                        hDevice;
  HANDLE                        hDevNotification;
  HANDLE                        hHndNotification;

  GUID                          DrvGuid;

  UCHAR                         Devices;
  UCHAR                         Interfaces;
  UCHAR                         AltInterfaces;
  UCHAR                         Configs;

  UCHAR                         DevNum;
  UCHAR                         CfgNum;
  UCHAR                         IntfcNum;     // The current selected interface's bInterfaceNumber
  UCHAR                         IntfcIndex;   // The entry in the Config's interfaces table matching to IntfcNum and AltSetting

  void                          GetDevDescriptor(void);
  void                          GetCfgDescriptor(int descIndex);
  void                          GetString(wchar_t *s, UCHAR sIndex);
  void                          SetStringDescrLanguage(void);
  void                          SetAltIntfcParams(UCHAR alt);
  bool                          IoControl(ULONG cmd, PUCHAR buf, ULONG len);

  void                          SetEndPointPtrs(void);
  void                          GetDeviceName(void);
  void                          GetFriendlyName(void);
  void                          GetDriverVer(void);
  void                          GetUSBDIVer(void);
  void                          GetSpeed(void);
  void                          GetUSBAddress(void);
  //void                          CloseEndPtHandles(void);

  bool                          RegisterForPnpEvents(HANDLE h);
};

//---------------------------------------------------------------------------
#endif



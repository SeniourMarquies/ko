// ZipStorage.h: interface for the CZipStorage class.
//
////////////////////////////////////////////////////////////////////////////////
//  Copyright (C) 2000 Tadeusz Dracz.
//  For conditions of distribution and use, see copyright notice in ZipArchive.h
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "BigFile.h"    // Added by ClassView
#include "AutoBuffer.h" // Added by ClassView

// callback function called when there is a need for a disk change
// calling CZipArchive functions, apart from the static ones, may have unexpected results
// iNumber  - disk number needed
// iCode :
//        -1 - disk needed for reading
// other codes occurs during writting
//        >=0 : number of bytes needed
//         -2 - the file with the archive name already exists on the disk
//        -3 - the disk is probably write - protected
//        -4 - couldn't create a file
//    pData - user defined data
//    return false to abort operation: the proper exception will be thrown
typedef bool (*CHANGEDISKFUNC)(int iNumber, int iCode, void * pData);

class CZipStorage {
  public:
    // return the position in the file, taking into account the bytes in the write buffer
    DWORD GetPosition();

    // flush the data from the read buffer to the disk
    void Flush();

    // only called by CCentralDir when opening an existing archive
    void UpdateSpanMode(WORD uLastDisk);
    // the preset size of the write buffer
    int m_iWriteBufferSize;

    // user data to be passed to the callback function
    void * m_pCallbackData;

    // function used to change disks during writing to the disk spanning archive
    void NextDisk(int iNeeded, LPCTSTR lpszFileName = NULL);

    void Close(bool bAfterException);

    // return the numer of the current disk
    int GetCurrentDisk();

    void SetCurrentDisk(int iNumber);

    // change the disk during extract operations
    void ChangeDisk(int iNumber);

    // Function name    : IsSpanMode
    // Description        : detect span mode
    // Return type        : int
    //        -1 - existing span opened
    //         0 - no span
    //         1 - new span
    int IsSpanMode();

    void     Open(LPCTSTR szPathName, int iMode, int iVolumeSize);
    void     Write(void * pBuf, DWORD iSize, bool bAtOnce);
    DWORD    Read(void * pBuf, DWORD iSize, bool bAtOnce);
    CBigFile m_file;
    CZipStorage();
    virtual ~CZipStorage();
    enum {
        noSpan,
        pkzipSpan,
        tdSpan,
        suggestedAuto,
        suggestedTd
    };
    int            m_iSpanMode;
    CHANGEDISKFUNC m_pChangeDiskFunc;
    static char    m_gszSpanSignature[];

    // open tdspan: last disk number, create tdspan: volume size
    // create pkspan: not used
    int m_iTdSpanData;

  protected:
    // how many bytes left free in the write buffer
    DWORD GetFreeInBuffer();
    friend class CCentralDir;
    // numer of bytes available in the write buffer
    DWORD m_uBytesInWriteBuffer;

    //  tdSpan : the total size of the current volume, pkSpan : free space on the current volume
    DWORD m_uCurrentVolSize;

    // return the number of bytes left on the current volume
    DWORD VolumeLeft();

    // write data to the internal buffer
    void WriteInternalBuffer(char * pBuf, DWORD uSize);

    // number of bytes left free in the write buffer
    DWORD m_uVolumeFreeInBuffer;

    CAutoBuffer m_pWriteBuffer;

    // return the number of free bytes on the current removable disk
    DWORD GetFreeVolumeSpace();

    void CallCallback(int iCode, CString szTemp);

    // only disk spanning creation: tells how many bytes have been written physically to the current volume
    DWORD m_iBytesWritten;

    // construct the name of the volume in tdSpan mode
    CString GetTdVolumeName(bool bLast, LPCTSTR lpszZipName = NULL);

    // change the disk in tdSpan mode
    CString ChangeTdRead();

    // change the disk in pkSpan mode
    CString ChangePkzipRead();

    //  you can only add a new files to the new disk spanning archive and only extract
    //    them from the existing one
    bool m_bNewSpan;

    int  m_iCurrentDisk;
    bool OpenFile(LPCTSTR lpszName, UINT uFlags, bool bThrow = true);
    void ThrowError(int err);
};
